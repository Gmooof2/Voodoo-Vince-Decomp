"""XBDM read-only probe (T4, phase 1.7, D75): check the running game's memory on the box against the pinned retail XBE.

CLI: xbdm_probe.py --identify
     xbdm_probe.py --known-true | --sections | --verify SYM [--va 0xX] [--module NAME] [--rehearse]
     xbdm_probe.py --self-test
--identify: TCP 731 to each configured xbox_ip candidate, read one line, close; sends nothing.
--known-true: getmem baseAddr len 4 == XBEH; getmem baseAddr+0x124 len 4 == file initFlags; then build identity
  (T2, phase 1.8): header TimeDate (baseAddr+0x114), certificate titleId (certificateAddr+0x08) and version
  (certificateAddr+0xAC), each == the pinned file's (xbe_info.parse_xbe); a difference FAILs as a wrong build
  (prototype/preview pass magic + initFlags, C0050).
--sections: game module listed once in `modules` (base/size printed, not compared); `modsections` == XBE sections 1:1.
--verify SYM: known-true + sections, then box sha1(va..va+size) == file sha1 == body_sha1 of the config/matched_set.toml
  row (SYM mangled or `Scope::Name`); a control read at the first va+k*size in the same section whose file bytes
  differ must mismatch body_sha1; then dmversion. --va overrides the row va.
--rehearse (with --verify): a loopback mock serving the pinned retail XBE image in memory; allowed host 127.0.0.1.
--self-test: synthetic XBE (xbe_info._synthetic, no game bytes), mock on 127.0.0.1, refusal cases.
Read-only: one send path (Probe.send) guards every command before any connect or byte: verb in ALLOWED, no CR/LF/NUL,
host == the one configured xbox_ip. Output: sha1, length, VA and PASS/FAIL lines only; never bytes or a box address
(D46). Wire: R1.7-003, R1.7-004.
Exit: 0 pass; 1 a check failed or a socket/parse error; 2 refusal (stderr `xbdm_probe: REFUSED <why>`).
"""
import argparse
import contextlib
import hashlib
import io
import re
import socket
import socketserver
import struct
import sys
import threading
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import vv_inputs  # noqa: E402
import xbe_info  # noqa: E402

REPO = vv_inputs.REPO
MATCHED = REPO / "config" / "matched_set.toml"
PORT = 731
TIMEOUT, ID_TIMEOUT = 5.0, 2.0
CHUNK = 0x400  # getmem request size cap (R1.7-004)
LOOPBACK = "127.0.0.1"
# Wire, T5 2026-10-26 (retail on the box): the title is listed as `default.exe`; its `modules` base/size are not
# baseAddr/sizeOfImage (box: base baseAddr+0xC00, size < sizeOfImage), so they are printed only.
GAME_MODULE = "default.exe"  # was "default.xbe" (T4 assumption)
# Wire, T5: modsections base/size == XBE section virtualAddr/virtualSize, 16 of 16 (confirmed).
ALLOWED = ("getmem", "modules", "modsections", "dmversion")
DENY = ("setmem writefile sendfile mkdir delete rename setfileattributes fmtfat flash reboot magicboot stop go halt "
        "break continue suspend resume setsystime setconfig adminpw lockmode setuserpriv dedicate funccall notify "
        "setcontext sysfileupd keyxchg signcontent screenshot getsum walkmem title stopon nostopon").split()
GREETING = re.compile(r"^20[01]-")
REPLY = re.compile(r"^(\d{3})-\s?(.*)$")
PAIR = re.compile(r'(\w+)=("[^"]*"|\S+)')
HEXRUN = re.compile(r"[0-9A-Fa-f]{41,}")
IPV4 = re.compile(r"\b\d{1,3}(?:\.\d{1,3}){3}\b")
MANGLED = re.compile(r"^\?([A-Za-z_]\w*)@([A-Za-z_]\w*)@@")


class Refusal(Exception):
    pass


class ProbeError(Exception):
    pass


def _clean(s):
    return IPV4.sub("<addr>", HEXRUN.sub("<hex>", str(s)))


def say(s):
    print(_clean(s))


def ck(ok, label):
    say(f"{'PASS' if ok else 'FAIL'} {label}")
    return bool(ok)


def check_host(host, allowed):
    """The host check in front of every connect."""
    if host not in tuple(allowed):
        raise Refusal("host is not a configured xbox_ip")


def guard(cmd, host, allowed):
    """Every command passes here before any connect or byte."""
    check_host(host, allowed)
    if len(tuple(allowed)) != 1:
        raise Refusal("run --identify, set xbox_ip to the confirmed address")
    if any(c in cmd for c in "\r\n\0") or not cmd.isascii():
        raise Refusal("command contains CR, LF, NUL or non-ASCII")
    verb = cmd.split(" ", 1)[0].lower()
    if verb not in ALLOWED:
        raise Refusal(f"verb {verb[:20]!r} is not in the read-only allow list")


def _recv_line(sock, buf):
    while b"\n" not in buf:
        chunk = sock.recv(4096)
        if not chunk:
            raise ProbeError("connection closed mid-reply")
        buf += chunk
        if len(buf) > 1 << 20:
            raise ProbeError("reply line over 1 MiB")
    line, _, buf = buf.partition(b"\n")
    return line.rstrip(b"\r").decode("ascii", "replace"), buf


class Probe:
    """Read-only XBDM client; `send` is the one path to the socket."""

    def __init__(self, host, allowed, port=PORT, factory=None, timeout=TIMEOUT):
        self.host, self.allowed, self.port, self.timeout = host, tuple(allowed), port, timeout
        self.factory = factory or socket.create_connection
        self.sock, self.buf = None, b""

    def close(self):
        if self.sock is not None:
            self.sock.close()
            self.sock = None

    def _line(self):
        line, self.buf = _recv_line(self.sock, self.buf)
        return line

    def send(self, cmd):
        """-> (code, text, lines). Guard first; connect lazily behind the same guard."""
        guard(cmd, self.host, self.allowed)
        if self.sock is None:
            self.sock = self.factory((self.host, self.port), self.timeout)
            if not GREETING.match(self._line()):
                raise ProbeError("greeting is not 201-/200-")
        self.sock.sendall(cmd.encode("ascii") + b"\r\n")
        m = REPLY.match(self._line())
        if not m:
            raise ProbeError(f"{cmd.split()[0]}: reply is not <code>-<text>")
        code, text, lines = int(m.group(1)), m.group(2), []
        if code == 202:
            while (ln := self._line()) != ".":
                lines.append(ln)
        return code, text, lines

    def getmem(self, addr, length):
        out = bytearray()
        for a in range(addr, addr + length, CHUNK):
            n = min(CHUNK, addr + length - a)
            code, _, lines = self.send(f"getmem addr=0x{a:08X} length=0x{n:X}")
            if code != 202:
                raise ProbeError(f"getmem VA 0x{a:08X} length 0x{n:X}: reply {code}"
                                 + (" memory not mapped" if code == 404 else ""))
            body = "".join(ln.strip() for ln in lines)
            if "??" in body:
                raise ProbeError(f"getmem VA 0x{a:08X} length 0x{n:X}: unreadable byte (??)")
            if len(body) != 2 * n or not re.fullmatch(r"[0-9A-Fa-f]*", body):
                raise ProbeError(f"getmem VA 0x{a:08X}: body length {len(body) // 2} != 0x{n:X} or not hex")
            out += bytes.fromhex(body)
        return bytes(out)

    def _listing(self, cmd):
        code, _, lines = self.send(cmd)
        if code != 202:
            raise ProbeError(f"{cmd.split()[0]}: reply {code}" + (" no such module" if code in (402, 403) else ""))
        return [{k: v.strip("\"") for k, v in PAIR.findall(ln)} for ln in lines]

    def modules(self):
        return self._listing("modules")

    def modsections(self, module):
        return self._listing(f'modsections name="{module}"')

    def dmversion(self):
        code, text, _ = self.send("dmversion")
        if code == 407:
            say("dmversion: 407 unsupported")
        elif code == 200:
            say(f"dmversion: {text}")
        else:
            raise ProbeError(f"dmversion: reply {code}")


def identify(cands, port=PORT, factory=None):
    """Connect to each candidate, read one line, close; sends nothing. -> exit code."""
    factory = factory or socket.create_connection
    answered = 0
    for i, host in enumerate(cands, 1):
        check_host(host, cands)
        code = "no answer"
        try:
            s = factory((host, port), ID_TIMEOUT)
            try:
                s.settimeout(ID_TIMEOUT)
                line, _ = _recv_line(s, b"")
                if re.match(r"^\d{3}", line):
                    code, answered = line[:3], answered + 1
            finally:
                s.close()
        except (OSError, ProbeError):
            pass
        say(f"candidate {i} of {len(cands)}: {code}")
    return 0 if answered else 1


def _u32(b):
    return struct.unpack("<I", b)[0]


def _hexint(v):
    return int(v, 16) if isinstance(v, str) and v.lower().startswith("0x") else int(v)


def _section_of(x, va):
    hit = [s for s in x["sections"] if s["virtualAddr"] <= va < s["virtualAddr"] + s["virtualSize"]]
    return hit[0] if len(hit) == 1 else None


def _file_sha(xb, x, va, size):
    off = xbe_info.va_to_off(x, va)
    if off is None or xbe_info.va_to_off(x, va + size - 1) != off + size - 1:
        return None
    return hashlib.sha1(xb[off:off + size]).hexdigest()


def known_true(p, x):
    base, flags = x["header"]["baseAddr"], x["header"]["initFlags"]
    a = ck(p.getmem(base, 4) == xbe_info.MAGIC, f"known-true: magic at VA 0x{base:08X}")
    b = ck(_u32(p.getmem(base + 0x124, 4)) == flags, f"known-true: initFlags at VA 0x{base + 0x124:08X}")
    return a and b and build_identity(p, x)


def build_identity(p, x):
    """TimeDate, certificate titleId and version on the box == the pinned file's; names the field, never values."""
    h, c = x["header"], x.get("certificate")
    if c is None:
        return ck(False, "build identity: pinned file has no certificate")
    ok = True
    for label, va, want in (("header TimeDate", h["baseAddr"] + 0x114, h["timeDate"]),
                            ("certificate titleId", h["certificateAddr"] + 0x08, c["titleId"]),
                            ("certificate version", h["certificateAddr"] + 0xAC, c["version"])):
        same = _u32(p.getmem(va, 4)) == want
        ok &= ck(same, f"build identity: {label} at VA 0x{va:08X} == pinned retail file")
        if not same:
            say(f"FAIL wrong build: {label} differs from the pinned retail default.xbe "
                "(not Voodoo Vince USA retail: July 2003 prototype or preview running?)")
    return ok


def sections(p, x, module, va=None):
    h = x["header"]
    mods = [m for m in p.modules() if m.get("name", "").lower() == module.lower()]
    if len(mods) != 1:
        return ck(False, f"sections: module {module} listed {len(mods)} times")
    try:
        ok = ck(True, f"sections: module {module} listed once")
        say(f"sections: module {module} base 0x{_hexint(mods[0]['base']):08X} size 0x{_hexint(mods[0]['size']):X} "
            f"(XBE baseAddr 0x{h['baseAddr']:08X} sizeOfImage 0x{h['sizeOfImage']:X}; info, not compared)")
        box = Counter((s["name"], _hexint(s["base"]), _hexint(s["size"])) for s in p.modsections(module))
    except (KeyError, ValueError) as e:
        raise ProbeError(f"modules/modsections line missing or bad field: {e}")
    xbe = Counter((s["name"], s["virtualAddr"], s["virtualSize"]) for s in x["sections"])
    ok &= ck(box == xbe, f"sections: {sum(box.values())} box sections == {sum(xbe.values())} XBE sections one-to-one")
    if va is not None:
        s = _section_of(x, va)
        ok &= ck(s is not None and (s["name"], s["virtualAddr"], s["virtualSize"]) in box,
                 f"sections: section {s['name'] if s else '(none)'} holding VA 0x{va:08X} present")
    return ok


def verify(p, x, xb, row, va, module):
    size, body = row["size"], row["body_sha1"]
    ok = known_true(p, x)
    ok &= sections(p, x, module, va)
    fsha = _file_sha(xb, x, va, size)
    bsha = hashlib.sha1(p.getmem(va, size)).hexdigest()
    say(f"sha1 box  {bsha}")
    say(f"sha1 file {fsha}")
    say(f"sha1 body {body}")
    ok &= ck(fsha is not None and bsha == fsha == body,
             f"verify: VA 0x{va:08X} length 0x{size:X} box sha1 == file sha1 == body_sha1")
    s, ctl = _section_of(x, va), None
    if s is not None:
        end = s["virtualAddr"] + min(s["virtualSize"], s["rawSize"])
        k = 1
        while va + (k + 1) * size <= end:
            cf = _file_sha(xb, x, va + k * size, size)
            if cf is not None and cf != body:
                ctl = (va + k * size, cf)
                break
            k += 1
    if ctl is None:
        ok &= ck(False, f"control: no differing block after VA 0x{va:08X} in its section")
    else:
        cb = hashlib.sha1(p.getmem(ctl[0], size)).hexdigest()
        say(f"sha1 control box {cb}")
        ok &= ck(cb == ctl[1] and cb != body,
                 f"control: VA 0x{ctl[0]:08X} length 0x{size:X} box sha1 == its file sha1, != body_sha1")
    p.dmversion()
    return ok


def find_row(rows, sym):
    for r in rows:
        if r["kind"] != "function":
            continue
        m = MANGLED.match(r["symbol"])
        if sym == r["symbol"] or (m and sym == f"{m.group(2)}::{m.group(1)}"):
            return r
    raise Refusal(f"{sym} is not a [[function]] row of {MATCHED.name}")


def live(mode, p, x, xb=None, row=None, va=None, module=GAME_MODULE):
    """-> exit code; every socket/parse error is a failure (1)."""
    try:
        if mode == "known_true":
            ok = known_true(p, x)
        elif mode == "sections":
            ok = sections(p, x, module)
        else:
            ok = verify(p, x, xb, row, row["va"] if va is None else va, module)
            say(f"{'PASS' if ok else 'FAIL'} {row['symbol']}")
        return 0 if ok else 1
    except (ProbeError, OSError, ValueError) as e:
        say(f"FAIL error: {type(e).__name__}: {e}")
        return 1
    finally:
        p.close()


class Mock:
    """In-memory XBDM image of an XBE: header at baseAddr, sections at virtualAddr (raw data, zero-padded)."""

    def __init__(self, xb, x, module=GAME_MODULE):
        h = x["header"]
        self.module, self.x = module, x
        self.regions = [(h["baseAddr"], xb[:h["sizeOfHeaders"]])]
        for s in x["sections"]:
            raw = xb[s["rawAddr"]:s["rawAddr"] + min(s["rawSize"], s["virtualSize"])]
            self.regions.append((s["virtualAddr"], raw + bytes(s["virtualSize"] - len(raw))))
        self.patch, self.qq, self.unmapped, self.sec_override, self.got = {}, set(), set(), None, []

    def byte(self, va):
        if va in self.unmapped:
            return None
        if va in self.patch:
            return self.patch[va]
        for start, data in self.regions:
            if start <= va < start + len(data):
                return data[va - start]
        return None

    def reply(self, cmd):
        self.got.append(cmd)
        verb = cmd.split(" ", 1)[0].lower()
        h = self.x["header"]
        if verb == "getmem":
            m = re.fullmatch(r"getmem addr=0x([0-9A-Fa-f]+) length=0x([0-9A-Fa-f]+)", cmd)
            if not m:
                return "400- bad parameters\r\n"
            a, n = int(m.group(1), 16), int(m.group(2), 16)
            if self.byte(a) is None and a not in self.qq:
                return "404- memory not mapped\r\n"
            hx = "".join("??" if (a + i) in self.qq or self.byte(a + i) is None else f"{self.byte(a + i):02x}"
                         for i in range(n))
            return "202- multiline response follows\r\n" + "".join(
                hx[i:i + 64] + "\r\n" for i in range(0, len(hx), 64)) + ".\r\n"
        if verb == "modules":
            return ("202- multiline response follows\r\n"
                    f'name="xboxkrnl.exe" base=0x80010000 size=0x00100000 check=0x0 timestamp=0x0\r\n'
                    f'name="{self.module}" base=0x{h["baseAddr"]:08x} size=0x{h["sizeOfImage"]:08x} '
                    "check=0x0 timestamp=0x0\r\n.\r\n")
        if verb == "modsections":
            if cmd != f'modsections name="{self.module}"':
                return "402- no such module\r\n"
            secs = self.sec_override or [(s["name"], s["virtualAddr"], s["virtualSize"]) for s in self.x["sections"]]
            return "202- multiline response follows\r\n" + "".join(
                f'name="{n}" base=0x{b:08x} size=0x{z:08x} index={i} flags=0\r\n'
                for i, (n, b, z) in enumerate(secs)) + ".\r\n"
        return "407- unknown command\r\n"


@contextlib.contextmanager
def serve(mock):
    """Run `mock` on 127.0.0.1 (ephemeral port); yields the port."""
    class H(socketserver.StreamRequestHandler):
        def handle(self):
            self.wfile.write(b"201- connected\r\n")
            while line := self.rfile.readline():
                self.wfile.write(mock.reply(line.rstrip(b"\r\n").decode("ascii", "replace")).encode("ascii"))
    srv = socketserver.ThreadingTCPServer((LOOPBACK, 0), H)
    srv.daemon_threads = True
    t = threading.Thread(target=srv.serve_forever, daemon=True)
    t.start()
    try:
        yield srv.server_address[1]
    finally:
        srv.shutdown()
        srv.server_close()


def _configured_host():
    v = vv_inputs.load_config()["xbox_ip"]
    cands = [v] if isinstance(v, str) else list(v or [])
    return cands


def _retail():
    import matched_set
    try:
        pin, xb, x = matched_set._xbe()
        rows = matched_set.load(MATCHED)
    except matched_set.Refuse as e:
        raise Refusal(str(e))
    return pin, xb, x, rows


class _Rec:
    """Recording socket for the self-test: greets, answers every command with 200, records bytes written."""

    def __init__(self):
        self.written, self.out = b"", b"201- connected\r\n"

    def sendall(self, b):
        self.written += b
        self.out += b"200- ok\r\n"

    def recv(self, n):
        b, self.out = self.out[:n], self.out[n:]
        return b

    def settimeout(self, t):
        pass

    def close(self):
        pass


def self_test():
    d, plant, _ = xbe_info._synthetic()
    d = bytearray(d)
    x = xbe_info.parse_xbe(bytes(d))
    for s in x["sections"]:  # deterministic non-zero section bytes (synthetic, no game bytes)
        for i in range(s["rawAddr"], min(len(d), s["rawAddr"] + s["rawSize"])):
            d[i] = (i * 7 + 3) & 0xFF
    xb = bytes(d)
    x = xbe_info.parse_xbe(xb)
    base, va, size = plant["baseAddr"], plant["baseAddr"] + 0x1010, 0x10
    row = {"kind": "function", "symbol": "?Synth@Test@@QAEXXZ", "va": va, "size": size,
           "body_sha1": hashlib.sha1(xb[0x1010:0x1020]).hexdigest()}
    passed, failed, captured = 0, 0, io.StringIO()

    def want(label, got, exp):
        nonlocal passed, failed
        if got == exp:
            passed += 1
        else:
            failed += 1
            print(f"FAIL self-test {label}: got {got!r}, want {exp!r}")

    def run(mode, setup=None, **kw):
        m = Mock(xb, x)
        if setup:
            setup(m)
        with serve(m) as port, contextlib.redirect_stdout(captured):
            return live(mode, Probe(LOOPBACK, (LOOPBACK,), port), x, xb=xb, row=row, **kw)

    want("known-true pass", run("known_true"), 0)
    want("planted header mismatch", run("known_true", lambda m: m.patch.update({base + 0x124: 0xFF})), 1)
    xw = bytearray(xb)  # synthetic XBE with a changed TimeDate: a wrong build
    struct.pack_into("<I", xw, 0x114, plant["timeDate"] ^ 0x1)
    xw = bytes(xw)
    with serve(Mock(xw, xbe_info.parse_xbe(xw))) as port, contextlib.redirect_stdout(captured):
        n0 = len(captured.getvalue())
        rc = live("known_true", Probe(LOOPBACK, (LOOPBACK,), port), x)
    want("changed TimeDate fails known-true", rc, 1)
    want("changed TimeDate named wrong build",
         "FAIL wrong build: header TimeDate" in captured.getvalue()[n0:], True)
    cert = x["header"]["certificateAddr"]
    want("changed titleId fails known-true", run("known_true", lambda m: m.patch.update({cert + 0x08: 0xFE})), 1)
    want("changed cert version fails known-true",
         run("known_true", lambda m: m.patch.update({cert + 0xAC: 0xFE})), 1)
    want("changed TimeDate fails verify", run("verify", lambda m: m.patch.update({base + 0x114: 0xFE})), 1)
    want("sections pass", run("sections"), 0)
    want("planted section mismatch", run("sections", lambda m: setattr(
        m, "sec_override", [(s["name"], s["virtualAddr"], s["virtualSize"] + (i == 0)) for i, s in
                            enumerate(x["sections"])])), 1)
    want("verify pass", run("verify"), 0)
    want("verify wrong --va", run("verify", va=va + 4), 1)
    want("control equals target fails", run("verify", lambda m: m.patch.update(
        {va + size + i: xb[0x1010 + i] for i in range(size)})), 1)
    want("unmapped read (404)", run("verify", lambda m: m.unmapped.update(range(va, va + size))), 1)
    want("?? read", run("verify", lambda m: m.qq.add(va + 3)), 1)
    want("dmversion 407 printed as unsupported", "dmversion: 407 unsupported" in captured.getvalue(), True)

    rec = _Rec()
    p = Probe(LOOPBACK, (LOOPBACK,), 0, factory=lambda a, t: rec)
    p.send("dmversion")
    n0 = len(rec.written)
    for verb in DENY + ["getmem addr=0x0 length=0x4\r\nsetmem addr=0x0 data=00",
                        "getmem addr=0x0 length=0x4\0", "getmem addr=0x0 length=0x4\n"]:
        cmd = verb if " " in verb else f"{verb} addr=0x0"
        try:
            p.send(cmd)
            want(f"refuse {verb[:12]!r}", "sent", "Refusal")
        except Refusal:
            want(f"refuse {verb[:12]!r} zero bytes", len(rec.written), n0)
    calls = []
    try:
        Probe("example.invalid", (LOOPBACK,), 0, factory=lambda a, t: calls.append(a)).send("dmversion")
        want("non-configured host refused", "sent", "Refusal")
    except Refusal:
        want("non-configured host: factory never called", calls, [])
    try:
        Probe(LOOPBACK, (LOOPBACK, "example.invalid"), 0, factory=lambda a, t: calls.append(a)).send("dmversion")
        want("two candidates refused", "sent", "Refusal")
    except Refusal:
        want("two candidates: factory never called", calls, [])
    with serve(Mock(xb, x)) as port, contextlib.redirect_stdout(captured):
        rc = identify([LOOPBACK], port)
    want("identify loopback", rc, 0)
    want("identify line", "candidate 1 of 1: 201" in captured.getvalue(), True)
    want("no hex run > 40 in output", HEXRUN.search(captured.getvalue()), None)
    print(f"xbdm_probe self-test: {passed} passed, {failed} failed")
    return 0 if failed == 0 else 1


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--identify", action="store_true")
    g.add_argument("--known-true", action="store_true")
    g.add_argument("--sections", action="store_true")
    g.add_argument("--verify", metavar="SYM")
    g.add_argument("--self-test", action="store_true")
    ap.add_argument("--va", type=lambda s: int(s, 16), help="override the row va (hex)")
    ap.add_argument("--module", default=GAME_MODULE, help=f"game module name (default {GAME_MODULE})")
    ap.add_argument("--rehearse", action="store_true", help="with --verify: loopback mock of the pinned retail XBE")
    a = ap.parse_args(argv)
    try:
        if a.self_test:
            return self_test()
        if a.rehearse and not a.verify:
            raise Refusal("--rehearse needs --verify")
        if a.va is not None and not a.verify:
            raise Refusal("--va needs --verify")
        if a.identify:
            cands = _configured_host()
            if not cands:
                raise Refusal("xbox_ip is null in config/local_paths.json")
            return identify(cands)
        pin, xb, x, rows = _retail()
        row = None
        if a.verify:
            row = find_row(rows, a.verify)
            if row["xbe_sha1"] != pin:
                raise Refusal(f"{row['symbol']} xbe_sha1 != the retail pin")
        mode = "verify" if a.verify else "known_true" if a.known_true else "sections"
        if a.rehearse:
            with serve(Mock(xb, x, a.module)) as port:
                return live(mode, Probe(LOOPBACK, (LOOPBACK,), port), x, xb, row, a.va, a.module)
        cands = _configured_host()
        if len(cands) != 1:
            raise Refusal("run --identify, set xbox_ip to the confirmed address")
        return live(mode, Probe(cands[0], cands), x, xb, row, a.va, a.module)
    except Refusal as e:
        print(f"xbdm_probe: REFUSED {_clean(e)}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
