#!/usr/bin/env python
"""Does: records each heavy step's seconds, rc, commit and the machine's load during it, and summarizes a label (phase) against another.
Exists: the gate keeps layer seconds only in its last-run JSON; other heavy-step times are lost; the 5.4 baseline was scratch scripts.
Delta: durable, attributable, machine-local numbers per step.

Self-contained, standard library only (ctypes for Windows PDH); copy it as is into another project.

Library:
  with timing.step(name, label=None, marker=None, **meta) as rec:   # rec.rc settable; default 0, 1 on exception
      ...                                              # (re-raised), SystemExit records its code; marker=str writes the
                                                       # shared gate marker (docs/ops/gate_marker.md) for the block
  timing.run(name, argv, label=None) -> rc             # subprocess, stdout/stderr inherited, untouched
  timing.free_mb() -> int | None                       # free physical memory now (MB); None off Windows or on failure
CLI:
  timing.py run <step> [--label L] -- <cmd...>         exit = the command's rc
  timing.py summary --label L [--against L2] [--steps a,b]   markdown section, L and L2 side by side
  timing.py add FILE.jsonl                             append pre-built records (keys validated; all or nothing)
  timing.py --self-test [DIR]                          controls in DIR (default a throwaway folder in the record home;
                                                       5.5 T12: an existing non-empty DIR is refused, rc 2)
  summary: n, median, min, max over rc 0 runs; `failed` counts the rest (5.5 T12; was: every run)
  appends hold an OS byte lock on records.jsonl (5.5 T12; was: plain append, rows could tear)
Record (one JSON line in <TIMING_DIR>/records.jsonl): step, label, ts (local start), seconds, rc, commit
(`git rev-parse HEAD` at start, null on failure), host, disk (PDH instance), disk_busy_pct (mean 100 - % Idle Time),
cpu_pct (\\Processor(_Total)), msmpeng_pct (\\Process(MsMpEng), % of one core), min_free_mb (\\Memory\\Available
MBytes), samples (1 s polls), overlap (other projects' live gate markers seen at start, every TIMING_MARKER_POLL s
and at end: sorted [{project, command, start}]; [] none, null when the marker folder could not be read), plus meta keys.
Env: TIMING_DIR (record folder; default %LOCALAPPDATA%/timing-records/<project>, else
~/.local/share/timing-records/<project>, project = basename of the git toplevel or cwd); TIMING_DISK (PhysicalDisk
instance name, default the one holding the cwd's drive letter); TIMING_PLANT=sampler (control: the sampler raises);
TIMING_MARKERS (gate marker folder; default %LOCALAPPDATA%/PA3/gate-markers, else ~/.local/share/PA3/gate-markers;
shared with other projects: controls never use it); TIMING_MARKER_POLL (overlap poll seconds, default 30);
TIMING_MARKER_HOLDER (5.4 T21: `<pid>|<project>`, set by the step that wrote the marker for its block and inherited
by its children: a step under it writes no marker, and overlap skips the holder's project as well as its own).
Safety: a sampler, marker or record failure never changes the step's rc, never raises out of timing, never prints to stdout;
at most one `timing: ...` line on stderr per step. Counter fields are null on non-Windows or any PDH failure.
"""
import argparse
import contextlib
import datetime
import io
import json
import os
import shutil
import socket
import statistics
import subprocess
import sys
import threading
import time
from pathlib import Path

try:
    import msvcrt  # 5.5 T12 (S7): the records byte lock; None off Windows (appends unlocked there)
except ImportError:
    msvcrt = None

CORE =("step", "label", "ts", "seconds", "rc", "commit", "host", "disk", "disk_busy_pct", "cpu_pct", "msmpeng_pct",
        "min_free_mb", "samples")
COUNTERS = ("disk_busy_pct", "cpu_pct", "msmpeng_pct", "min_free_mb")
LOCK_AT = 1 << 40  # 5.5 T12 (S7): records.jsonl's OS byte lock, past any end (match_one.LEDGER_LOCK_AT's pattern)
_NESTED = []  # 5.5 T12: non-empty while self-test case selftest-dir calls self_test in-process (that case skipped)


def _git(*args):
    try:
        p = subprocess.run(["git", *args], capture_output=True, text=True, timeout=20, stdin=subprocess.DEVNULL)
        return p.stdout.strip() if p.returncode == 0 and p.stdout.strip() else None
    except Exception:
        return None


def free_mb():
    """-> free physical memory in MB now (GlobalMemoryStatusEx ullAvailPhys, as delink.ghidra_mem_guard), None on
    non-Windows or failure; never raises (5.4 T5.c2: sizes the gate self-test's workers)."""
    try:
        import ctypes

        class MEMORYSTATUSEX(ctypes.Structure):
            _fields_ = [("dwLength", ctypes.c_ulong), ("dwMemoryLoad", ctypes.c_ulong),
                        ("ullTotalPhys", ctypes.c_ulonglong), ("ullAvailPhys", ctypes.c_ulonglong),
                        ("ullTotalPageFile", ctypes.c_ulonglong), ("ullAvailPageFile", ctypes.c_ulonglong),
                        ("ullTotalVirtual", ctypes.c_ulonglong), ("ullAvailVirtual", ctypes.c_ulonglong),
                        ("ullAvailExtendedVirtual", ctypes.c_ulonglong)]
        st = MEMORYSTATUSEX()
        st.dwLength = ctypes.sizeof(MEMORYSTATUSEX)
        if not ctypes.windll.kernel32.GlobalMemoryStatusEx(ctypes.byref(st)):
            return None
        return st.ullAvailPhys // (1024 * 1024)
    except Exception:
        return None


def project():
    """-> basename of the git toplevel, else of the cwd (the record folder's and the gate marker's project name)."""
    top = _git("rev-parse", "--show-toplevel")
    return Path(top).name if top else Path.cwd().name


def record_dir():
    env = os.environ.get("TIMING_DIR")
    if env:
        return Path(env)
    base = os.environ.get("LOCALAPPDATA")
    home = Path(base) if base else Path.home() / ".local" / "share"
    return home / "timing-records" / project()


# ---- shared gate marker (docs/ops/gate_marker.md, 5.4 T20; a contract with ProjectCombine) ----

MARKER_STALE = 6 * 3600  # spec item 6: a marker started longer ago than this is stale


def marker_dir():
    env = os.environ.get("TIMING_MARKERS")
    if env:
        return Path(env)
    base = os.environ.get("LOCALAPPDATA")
    return (Path(base) if base else Path.home() / ".local" / "share") / "PA3" / "gate-markers"


def pid_alive(pid):
    """True when process pid runs. Windows: OpenProcess + GetExitCodeProcess == STILL_ACTIVE, access denied counts
    alive (as ProjectCombine aio/gate.py:277-288); else os.kill(pid, 0)."""
    pid = int(pid)
    if pid <= 0:
        return False
    if os.name == "nt":
        import ctypes
        k32 = ctypes.windll.kernel32
        h = k32.OpenProcess(0x1000, False, pid)  # PROCESS_QUERY_LIMITED_INFORMATION
        if not h:
            return ctypes.GetLastError() == 5  # ERROR_ACCESS_DENIED: exists, not ours
        try:
            code = ctypes.c_ulong()
            return bool(k32.GetExitCodeProcess(h, ctypes.byref(code))) and code.value == 259
        finally:
            k32.CloseHandle(h)
    try:
        os.kill(pid, 0)
    except PermissionError:
        return True
    except OSError:
        return False
    return True


def _marker_write(proj, command):
    """Write <marker_dir>/<proj>-<pid>.json via <name>.tmp + os.replace (spec items 1-4); -> its path."""
    d = marker_dir()
    d.mkdir(parents=True, exist_ok=True)
    path = d / f"{proj}-{os.getpid()}.json"
    tmp = d / f"{proj}-{os.getpid()}.tmp"
    tmp.write_text(json.dumps({"project": proj, "command": command, "pid": os.getpid(),
                               "start": time.strftime("%Y-%m-%dT%H:%M:%S")}), encoding="utf-8")
    os.replace(tmp, path)
    return path


def _markers_live(own):
    """Other projects' live markers (dicts). Skips project == own (any pid), project == the TIMING_MARKER_HOLDER note's
    project (5.4 T21; a malformed note is ignored), unreadable, malformed and stale (dead pid, or start older than
    MARKER_STALE) files; deletes nothing. Missing folder -> []; an unlistable folder raises."""
    skip = {own}
    note = os.environ.get("TIMING_MARKER_HOLDER") or ""  # 5.4 T21: the marked ancestor's project
    if "|" in note and note.split("|", 1)[1]:
        skip.add(note.split("|", 1)[1])
    d = marker_dir()
    if not d.exists():
        return []
    if not d.is_dir():
        raise NotADirectoryError(f"marker folder {d} is not a folder")
    now = datetime.datetime.now()
    out = []
    for n in sorted(os.listdir(d)):
        if not n.endswith(".json"):
            continue
        try:
            m = json.loads((d / n).read_text(encoding="utf-8"))
            pid, proj, start = int(m["pid"]), m["project"], datetime.datetime.fromisoformat(m["start"])
            if not isinstance(proj, str) or not isinstance(m["command"], str) or not isinstance(m["start"], str):
                continue
            if proj in skip or (now - start).total_seconds() > MARKER_STALE or not pid_alive(pid):
                continue
        except Exception:
            continue
        out.append(m)
    return out


def _append(recs):
    """Append whole lines under the OS byte lock at LOCK_AT (5.5 T12 S7, match_one._append_row's pattern; released by
    the OS if this process dies). Was: plain open("a"), which tears rows under concurrent writers (C0178)."""
    d = record_dir()
    d.mkdir(parents=True, exist_ok=True)
    p = d / "records.jsonl"
    data = "".join(json.dumps(r) + "\n" for r in recs).encode("utf-8")
    p.open("a").close()
    with p.open("r+b") as fh:
        if msvcrt is not None:
            fh.seek(LOCK_AT)
            for tries_left in range(6000, -1, -1):  # held for one write: 60 s means something is wrong (raises,
                try:  # step() reports it as a `timing:` warning)
                    msvcrt.locking(fh.fileno(), msvcrt.LK_NBLCK, 1)
                    break
                except OSError:
                    if not tries_left:
                        raise
                    time.sleep(0.01)
        try:
            fh.seek(0, 2)
            fh.write(data)
            fh.flush()
        finally:
            if msvcrt is not None:
                fh.seek(LOCK_AT)
                msvcrt.locking(fh.fileno(), msvcrt.LK_UNLCK, 1)


class _Sampler:
    """Daemon thread polling PDH once a second; aggregates only. Any failure leaves the fields null."""

    def __init__(self):
        self.err = None
        self.disk = None
        self.vals = {"busy": [], "cpu": [], "av": [], "free": []}
        self.n = 0
        self._stop = threading.Event()
        self._t = threading.Thread(target=self._main, daemon=True)

    def start(self):
        try:
            self._t.start()
        except Exception as e:
            self.err = f"sampler: {e}"

    def stop(self):
        self._stop.set()
        try:
            if self._t.is_alive():
                self._t.join(5)
        except Exception:
            pass

    def _main(self):
        try:
            if os.environ.get("TIMING_PLANT") == "sampler":
                raise RuntimeError("planted sampler failure")
            if os.name != "nt":
                return
            self._poll()
        except Exception as e:
            self.err = f"sampler: {e}"
            self.vals = {k: [] for k in self.vals}
            self.n = 0

    def _poll(self):
        import ctypes
        from ctypes import wintypes as wt

        class FMT(ctypes.Structure):
            _fields_ = [("CStatus", wt.DWORD), ("doubleValue", ctypes.c_double)]

        class ITEM(ctypes.Structure):
            _fields_ = [("szName", wt.LPWSTR), ("FmtValue", FMT)]

        pdh = ctypes.WinDLL("pdh")
        DOUBLE, NOCAP = 0x200, 0x8000
        MORE = 0x800007D2
        q = wt.HANDLE()
        if pdh.PdhOpenQueryW(None, None, ctypes.byref(q)):
            raise RuntimeError("PdhOpenQueryW failed")
        try:
            def add(path):
                h = wt.HANDLE()
                return h if pdh.PdhAddEnglishCounterW(q, path, None, ctypes.byref(h)) == 0 else None
            want = os.environ.get("TIMING_DISK")
            letter = os.path.splitdrive(os.getcwd())[0].lower()
            hs = {"disk": add("\\PhysicalDisk(*)\\% Idle Time"), "cpu": add("\\Processor(_Total)\\% Processor Time"),
                  "av": add("\\Process(MsMpEng)\\% Processor Time"), "free": add("\\Memory\\Available MBytes")}

            def one(h):
                v = FMT()
                if h is None or pdh.PdhGetFormattedCounterValue(h, DOUBLE | NOCAP, None, ctypes.byref(v)) or v.CStatus > 1:
                    return None
                return v.doubleValue

            def idle(h):
                if h is None:
                    return None
                size, count = wt.DWORD(0), wt.DWORD(0)
                rc = pdh.PdhGetFormattedCounterArrayW(h, DOUBLE | NOCAP, ctypes.byref(size), ctypes.byref(count), None)
                if (rc & 0xFFFFFFFF) != MORE or not size.value:
                    return None
                buf = (ctypes.c_byte * size.value)()
                if pdh.PdhGetFormattedCounterArrayW(h, DOUBLE | NOCAP, ctypes.byref(size), ctypes.byref(count), buf):
                    return None
                items = ctypes.cast(buf, ctypes.POINTER(ITEM))
                for i in range(count.value):
                    name = items[i].szName or ""
                    hit = name == want if want else (letter and letter in name.lower() and name != "_Total")
                    if hit and items[i].FmtValue.CStatus <= 1:
                        self.disk = name
                        return items[i].FmtValue.doubleValue
                return None

            pdh.PdhCollectQueryData(q)

            def sample():
                if pdh.PdhCollectQueryData(q):
                    return
                got = {"busy": idle(hs["disk"]), "cpu": one(hs["cpu"]), "av": one(hs["av"]), "free": one(hs["free"])}
                if got["busy"] is not None:
                    got["busy"] = 100.0 - got["busy"]
                if any(v is not None for v in got.values()):
                    self.n += 1
                    for k, v in got.items():
                        if v is not None:
                            self.vals[k].append(v)
            while not self._stop.wait(1.0):
                sample()
            sample()
        finally:
            pdh.PdhCloseQuery(q)

    def fields(self):
        m = lambda xs: round(statistics.fmean(xs), 1) if xs else None
        v = self.vals
        return {"disk": self.disk, "disk_busy_pct": m(v["busy"]), "cpu_pct": m(v["cpu"]), "msmpeng_pct": m(v["av"]),
                "min_free_mb": round(min(v["free"])) if v["free"] else None, "samples": self.n}


class _Overlap:
    """Daemon thread: _markers_live() at start, every TIMING_MARKER_POLL s and at stop(). value() = sorted unique
    [{project, command, start}] keyed by (project, pid, start); None when no look could read the folder."""

    def __init__(self, own):
        self.own, self.found, self.read, self.err = own, {}, False, None
        try:
            self.poll = float(os.environ.get("TIMING_MARKER_POLL") or 30)
        except ValueError:
            self.poll = 30.0
        if self.poll <= 0:
            self.poll = 30.0
        self._lock = threading.Lock()
        self._stop = threading.Event()
        self._t = threading.Thread(target=self._main, daemon=True)

    def look(self):
        try:
            live = _markers_live(self.own)
        except Exception as e:
            with self._lock:
                self.err = self.err or f"overlap: {e}"
            return
        with self._lock:
            self.read = True
            for m in live:
                self.found[(m["project"], m["pid"], m["start"])] = {k: m[k] for k in ("project", "command", "start")}

    def _main(self):
        self.look()
        while not self._stop.wait(self.poll):
            self.look()

    def start(self):
        try:
            self._t.start()
        except Exception as e:
            self.err = f"overlap: {e}"

    def stop(self):
        self._stop.set()
        try:
            if self._t.is_alive():
                self._t.join(5)
        except Exception:
            pass
        self.look()

    def value(self):
        with self._lock:
            if not self.read:
                return None
            return sorted(self.found.values(), key=lambda m: (m["project"], m["start"], m["command"]))


class _Rec:
    def __init__(self):
        self.rc = 0


@contextlib.contextmanager
def step(name, label=None, marker=None, **meta):
    """Time the block; append one record. rc: rec.rc, 1 on exception, SystemExit's code; the exception is re-raised.
    marker (str, the gate command): the shared gate marker exists for the block, removed in finally (5.4 T20).
    5.4 T21: only when TIMING_MARKER_HOLDER is unset; after the write the note `<pid>|<project>` is set for the block
    (children and nested steps inherit it and write none), popped in finally. Under a note: no marker, nothing set."""
    rec = _Rec()
    warn = []
    t0 = time.perf_counter()
    ts = time.strftime("%Y-%m-%dT%H:%M:%S")
    commit = host = None
    sampler = watcher = mpath = None
    held = False  # 5.4 T21: this step set TIMING_MARKER_HOLDER
    try:
        commit = _git("rev-parse", "HEAD")
        host = socket.gethostname()
        sampler = _Sampler()
        sampler.start()
        own = project()
        if marker is not None and not os.environ.get("TIMING_MARKER_HOLDER"):  # 5.4 T21: one marker per tree
            try:
                mpath = _marker_write(own, str(marker))
                os.environ["TIMING_MARKER_HOLDER"] = f"{os.getpid()}|{own}"
                held = True
            except Exception as e:
                warn.append(f"marker write: {e}")
        watcher = _Overlap(own)
        watcher.start()
    except Exception as e:
        warn.append(f"setup: {e}")
    try:
        yield rec
    except SystemExit as e:
        rec.rc = e.code if isinstance(e.code, int) else (0 if e.code is None else 1)
        raise
    except BaseException:
        rec.rc = 1
        raise
    finally:
        if held:
            os.environ.pop("TIMING_MARKER_HOLDER", None)
        if mpath is not None:
            try:
                mpath.unlink(missing_ok=True)
            except Exception as e:
                warn.append(f"marker remove: {e}")
        try:
            secs = time.perf_counter() - t0
            fields = dict.fromkeys(COUNTERS) | {"disk": None, "samples": 0}
            if sampler is not None:
                sampler.stop()
                if sampler.err:
                    warn.append(sampler.err)
                else:
                    fields = sampler.fields()
            overlap = None
            if watcher is not None:
                watcher.stop()
                if watcher.err:
                    warn.append(watcher.err)
                overlap = watcher.value()
            r = {"step": name, "label": label, "ts": ts, "seconds": round(secs, 3), "rc": rec.rc, "commit": commit,
                 "host": host, **fields, "overlap": overlap, **{k: v for k, v in meta.items() if k not in CORE}}
            _append([r])
        except Exception as e:
            warn.append(f"record: {e}")
        if warn:
            try:
                print("timing: " + "; ".join(warn) + " (step result unchanged)", file=sys.stderr, flush=True)
            except Exception:
                pass


def run(name, argv, label=None):
    """Run argv with inherited stdout/stderr inside step(); return its rc."""
    with step(name, label=label, cmd=" ".join(map(str, argv))[:200]) as rec:
        rec.rc = subprocess.run(argv).returncode
    return rec.rc


# ---- summary / add ----

def _load(path=None):
    p = Path(path) if path else record_dir() / "records.jsonl"
    if not p.is_file():
        return []
    out = []
    for ln in p.read_text(encoding="utf-8").splitlines():
        try:
            r = json.loads(ln)
            if isinstance(r, dict):
                out.append(r)
        except ValueError:
            pass
    return out


def stats(recs, label, steps=None):
    """{step: {n, failed, median, min, max, busy, cpu, av, free, overlapped}} for one label, steps in first-seen order;
    5.5 T12 (S7): every value over the rc 0 runs (n), failed = runs with rc != 0 (was: medians over every run);
    median/min/max None when no run passed; overlapped = runs whose overlap is a non-empty list, None when no run has
    the key."""
    by, bad = {}, {}
    for r in recs:
        if r.get("label") == label and (steps is None or r.get("step") in steps):
            by.setdefault(r["step"], [])
            if r.get("rc") == 0:
                by[r["step"]].append(r)
            else:
                bad[r["step"]] = bad.get(r["step"], 0) + 1
    m = lambda xs: round(statistics.fmean(xs), 1) if xs else None
    r2 = lambda f, xs: round(f(xs), 2) if xs else None
    out = {}
    for s, rs in by.items():
        sec = [float(r["seconds"]) for r in rs]
        col = lambda k: [r[k] for r in rs if r.get(k) is not None]
        out[s] = {"n": len(rs), "failed": bad.get(s, 0), "median": r2(statistics.median, sec), "min": r2(min, sec),
                  "max": r2(max, sec), "busy": m(col("disk_busy_pct")), "cpu": m(col("cpu_pct")),
                  "av": m(col("msmpeng_pct")), "free": min(col("min_free_mb")) if col("min_free_mb") else None,
                  "overlapped": sum(1 for r in rs if isinstance(r.get("overlap"), list) and r["overlap"])
                  if any("overlap" in r for r in rs) else None}
    return out


def summary_text(recs, label, against=None, steps=None):
    a = stats(recs, label, steps)
    b = stats(recs, against, steps) if against else {}
    order = steps or list(dict.fromkeys([*a, *b]))
    keys = ("n", "failed", "median", "min", "max", "busy", "cpu", "av", "free", "overlapped")  # 5.5 T12: +failed
    head = ("step", "n (rc 0)", "failed", "median s", "min s", "max s", "disk busy %", "CPU %", "MsMpEng %core",
            "min free MB", "overlapped runs")
    title = f"## timing: {label}" + (f" vs {against} (cells: {label} / {against})" if against else "")
    lines = [title, "", "| " + " | ".join(head) + " |", "|" + "---|" * len(head)]
    fmt = lambda v: "-" if v is None else str(v)
    for s in order:
        if s not in a and s not in b:
            continue
        x, y = a.get(s, {}), b.get(s, {})
        cells = [fmt(x.get(k)) + (" / " + fmt(y.get(k)) if against else "") for k in keys]
        lines.append(f"| {s} | " + " | ".join(cells) + " |")
    return "\n".join(lines)


def add_file(path):
    """Validate every line first; append all or nothing. Returns (count, error)."""
    recs = []
    for i, ln in enumerate(Path(path).read_text(encoding="utf-8").splitlines(), 1):
        if not ln.strip():
            continue
        try:
            r = json.loads(ln)
        except ValueError as e:
            return 0, f"line {i}: {e}"
        if not isinstance(r, dict):
            return 0, f"line {i}: not an object"
        miss = [k for k in CORE if k not in r]
        if miss:
            return 0, f"line {i}: missing {','.join(miss)}"
        if not isinstance(r["step"], str) or not isinstance(r["seconds"], (int, float)) or not isinstance(r["rc"], int) \
                or not (r["label"] is None or isinstance(r["label"], str)):
            return 0, f"line {i}: bad step/label/seconds/rc type"
        recs.append(r)
    _append(recs)
    return len(recs), None


# ---- controls ----

CHILD = "import sys; print('line one'); print('line two \\u00e9'); sys.stdout.flush(); sys.exit(3)"


def _wrapped(env_over):
    env = dict(os.environ, **env_over)
    me = os.path.abspath(__file__)
    plain = subprocess.run([sys.executable, "-c", CHILD], capture_output=True, env=env)
    wrap = subprocess.run([sys.executable, me, "run", "ctl", "--label", "selftest", "--", sys.executable, "-c", CHILD],
                          capture_output=True, env=env)
    tl = [l for l in wrap.stderr.decode("utf-8", "replace").splitlines() if l.startswith("timing:")]
    ok = plain.returncode == wrap.returncode == 3 and plain.stdout == wrap.stdout and len(tl) <= 1
    return ok, plain, wrap, tl


def self_test(arg=None):
    """5.5 T12 (S7): DIR given and existing non-empty -> refused, rc 2, untouched; afterwards only what this run put
    in DIR is removed (DIR itself only when this run made it). Was: DIR created and removed whole in finally."""
    base = Path(arg) if arg else record_dir() / f"selftest-{os.getpid()}"
    if base.exists() and (not base.is_dir() or any(base.iterdir())):
        print(f"timing self-test: REFUSED {base} exists and is not an empty folder (give a new or empty DIR)")
        return 2
    made = not base.exists()
    base.mkdir(parents=True, exist_ok=True)
    fails = 0
    held = os.environ.pop("TIMING_MARKER_HOLDER", None)  # 5.4 T21: controls run as if no marked ancestor
    try:
        # (a) unwritable record folder: TIMING_DIR is an existing file
        blocker = base / "not_a_dir"
        blocker.write_text("x", encoding="utf-8")
        ok, p, w, tl = _wrapped({"TIMING_DIR": str(blocker)})
        print(f"control unwritable-records: {'ok' if ok else 'FAIL'} rc {w.returncode} (plain {p.returncode}), "
              f"stdout {len(w.stdout)} B {'identical' if p.stdout == w.stdout else 'DIFFERS'}, stderr timing lines {len(tl)}")
        fails += not ok
        # (b) planted sampler failure: rc and stdout identical, record written with null counters
        d = base / "sampler"
        ok, p, w, tl = _wrapped({"TIMING_DIR": str(d), "TIMING_PLANT": "sampler"})
        recs = _load(d / "records.jsonl")
        nul = len(recs) == 1 and recs[0]["rc"] == 3 and all(recs[0][k] is None for k in COUNTERS)
        ok = ok and nul and len(tl) == 1
        print(f"control sampler-failure: {'ok' if ok else 'FAIL'} rc {w.returncode} (plain {p.returncode}), "
              f"stdout {'identical' if p.stdout == w.stdout else 'DIFFERS'}, record rc {recs[0]['rc'] if recs else None} "
              f"counters null {nul}, stderr timing lines {len(tl)}")
        fails += not ok
        # (c) summary over planted records
        planted = [{"step": "s", "label": "A", "seconds": v, "rc": 0} for v in (3, 1, 2, 10)] + \
                  [{"step": "s", "label": "B", "seconds": 5, "rc": 0}, {"step": "t", "label": "A", "seconds": 7, "rc": 0}]
        sa, sb = stats(planted, "A"), stats(planted, "B")  # 5.5 T12: rc 0 on every planted row; +failed column
        got = (sa["s"]["n"], sa["s"]["median"], sa["s"]["min"], sa["s"]["max"], sb["s"]["median"], sa["t"]["median"])
        want = (4, 2.5, 1.0, 10.0, 5.0, 7.0)
        text = summary_text(planted, "A", "B")
        ok = got == want and "| s | 4 / 1 | 0 / 0 | 2.5 / 5.0 | 1.0 / 5.0 | 10.0 / 5.0 |" in text
        print(f"control summary: {'ok' if ok else 'FAIL'} n/median/min/max {got[:4]} want {want[:4]}, against {got[4]}, "
              f"second step {got[5]}")
        fails += not ok
        # (c2) 5.5 T12 (S7): medians over rc 0 runs only, the rest counted as failed
        sf = stats([{"step": "s", "label": "F", "seconds": v, "rc": rc} for v, rc in ((10, 0), (10, 0), (0.5, 2),
                                                                                      (0.5, 2))], "F")["s"]
        ok = (sf["n"], sf["median"], sf.get("failed")) == (2, 10.0, 2)
        print(f"control summary-failed: {'ok' if ok else 'FAIL'} n/median/failed {(sf['n'], sf['median'], sf.get('failed'))}"
              f" want (2, 10.0, 2)")
        fails += not ok
        # (c3) 5.5 T12 (S7): an existing non-empty DIR is refused and left as it was (in-process, this case skipped)
        if not _NESTED:
            sd = base / "user_dir"
            sd.mkdir()
            (sd / "sentinel.txt").write_text("x", encoding="utf-8")
            _NESTED.append(1)
            try:
                with contextlib.redirect_stdout(io.StringIO()):
                    rc = self_test(str(sd))
            finally:
                _NESTED.pop()
            kept = (sd / "sentinel.txt").is_file()
            ok = rc == 2 and kept
            print(f"control selftest-dir: {'ok' if ok else 'FAIL'} --self-test on a non-empty DIR rc {rc} (want 2), "
                  f"sentinel {'kept' if kept else 'DELETED'}")
            fails += not ok
        # (c4) 5.5 T12 (S7): an append waits while the records byte lock is held, then writes its whole line
        if msvcrt is not None:
            ld = base / "rec_lock"
            ld.mkdir()
            lf = ld / "records.jsonl"
            lf.write_bytes(b"")
            was = os.environ.get("TIMING_DIR")
            os.environ["TIMING_DIR"] = str(ld)
            try:
                with lf.open("r+b") as fh:
                    fh.seek(LOCK_AT)
                    msvcrt.locking(fh.fileno(), msvcrt.LK_NBLCK, 1)
                    th = threading.Thread(target=_append, args=([{"step": "lk", "seconds": 1}],), daemon=True)
                    th.start()
                    time.sleep(0.5)
                    during = lf.stat().st_size
                    fh.seek(LOCK_AT)
                    msvcrt.locking(fh.fileno(), msvcrt.LK_UNLCK, 1)
                th.join(10)
                after = len(_load(lf))
            finally:
                if was is None:
                    os.environ.pop("TIMING_DIR", None)
                else:
                    os.environ["TIMING_DIR"] = was
            ok = during == 0 and after == 1
            print(f"control append-locked: {'ok' if ok else 'FAIL'} bytes written while held {during} (want 0), "
                  f"records after release {after} (want 1)")
            fails += not ok
        # 5.4 T20: gate markers; every control's TIMING_MARKERS is under base, never the shared folder (another
        # project reads a marker there as a live gate)
        me = os.path.abspath(__file__)
        mk = lambda proj, pid, start, cmd: json.dumps({"project": proj, "command": cmd, "pid": pid, "start": start})
        # (d) a live foreign marker (pid = this process, foreign to the `run` child) recorded; stale, dead-pid,
        # malformed and own-project markers ignored; all five left in place; stdout/rc as plain
        md, rd = base / "markers_d", base / "rec_d"
        md.mkdir()
        now = time.strftime("%Y-%m-%dT%H:%M:%S")
        old = (datetime.datetime.now() - datetime.timedelta(hours=7)).isoformat(timespec="seconds")
        dead = subprocess.Popen([sys.executable, "-c", "pass"])
        dead.wait()
        pid = os.getpid()
        files = {f"OtherProj-{pid}.json": mk("OtherProj", pid, now, "other gate"),
                 f"StaleProj-{pid}.json": mk("StaleProj", pid, old, "stale"),
                 f"DeadProj-{dead.pid}.json": mk("DeadProj", dead.pid, now, "dead"),
                 "BadProj-1.json": "{not json",
                 f"{project()}-{pid}.json": mk(project(), pid, now, "own")}
        for n, t in files.items():
            (md / n).write_text(t, encoding="utf-8")
        ok, p, w, tl = _wrapped({"TIMING_DIR": str(rd), "TIMING_MARKERS": str(md)})
        recs = _load(rd / "records.jsonl")
        got = recs[0].get("overlap", "missing") if len(recs) == 1 else "no record"
        kept = sum((md / n).is_file() for n in files)
        ok = ok and got == [{"project": "OtherProj", "command": "other gate", "start": now}] and kept == 5
        print(f"control overlap-markers: {'ok' if ok else 'FAIL'} rc {w.returncode} (plain {p.returncode}), stdout "
              f"{'identical' if p.stdout == w.stdout else 'DIFFERS'}, overlap projects "
              f"{[m.get('project') for m in got] if isinstance(got, list) else got}, files kept {kept} of 5, "
              f"stderr timing lines {len(tl)}")
        fails += not ok
        # (e) a foreign marker present only mid-run (the child plants and removes it) appears in overlap
        md, rd = base / "markers_e", base / "rec_e"
        mid = ("import json, os, pathlib, time\n"
               "d = pathlib.Path(os.environ['TIMING_MARKERS']); d.mkdir(parents=True, exist_ok=True)\n"
               "time.sleep(0.6); p = d / f'MidProj-{os.getpid()}.json'\n"
               "p.write_text(json.dumps({'project': 'MidProj', 'command': 'mid', 'pid': os.getpid(), "
               "'start': time.strftime('%Y-%m-%dT%H:%M:%S')}), encoding='utf-8')\n"
               "time.sleep(1.0); p.unlink()\n")
        env = dict(os.environ, TIMING_DIR=str(rd), TIMING_MARKERS=str(md), TIMING_MARKER_POLL="0.2")
        w = subprocess.run([sys.executable, me, "run", "ctl_e", "--label", "selftest", "--", sys.executable, "-c", mid],
                           capture_output=True, env=env)
        recs = _load(rd / "records.jsonl")
        got = recs[0].get("overlap", "missing") if len(recs) == 1 else "no record"
        left = len(list(md.glob("*.json"))) if md.is_dir() else 0
        names = [m.get("project") for m in got] if isinstance(got, list) else got
        ok = w.returncode == 0 and names == ["MidProj"] and left == 0
        print(f"control overlap-mid-run: {'ok' if ok else 'FAIL'} rc {w.returncode}, overlap projects {names}, "
              f"markers left after {left}")
        fails += not ok
        # (f) marker folder is an existing file: marker write fails, overlap null, rc/stdout as plain, <= 1 timing line
        rd = base / "rec_f"
        code = (f"import sys; sys.path.insert(0, {os.path.dirname(me)!r}); import timing\n"
                f"with timing.step('ctl_f', label='selftest', marker='ctl f'):\n    exec({CHILD!r})\n")
        env = dict(os.environ, TIMING_DIR=str(rd), TIMING_MARKERS=str(blocker))
        p = subprocess.run([sys.executable, "-c", CHILD], capture_output=True, env=env)
        w = subprocess.run([sys.executable, "-c", code], capture_output=True, env=env)
        tl = [l for l in w.stderr.decode("utf-8", "replace").splitlines() if l.startswith("timing:")]
        recs = _load(rd / "records.jsonl")
        got = recs[0].get("overlap", "missing") if len(recs) == 1 else "no record"
        wfail = len(tl) == 1 and "marker write" in tl[0]
        ok = p.returncode == w.returncode == 3 and p.stdout == w.stdout and len(tl) <= 1 and got is None and wfail
        print(f"control marker-folder-is-file: {'ok' if ok else 'FAIL'} rc {w.returncode} (plain {p.returncode}), stdout "
              f"{'identical' if p.stdout == w.stdout else 'DIFFERS'}, overlap {got}, write failure reported {wfail}, "
              f"stderr timing lines {len(tl)}")
        fails += not ok
        # (g) in-process step(marker=...) raising SystemExit(1), then KeyboardInterrupt: inside, the marker holds the
        # spec's 4 keys and no .tmp is left; after, none of ours remains
        md, rd = base / "markers_g", base / "rec_g"
        saved = {k: os.environ.get(k) for k in ("TIMING_DIR", "TIMING_MARKERS")}
        os.environ.update(TIMING_DIR=str(rd), TIMING_MARKERS=str(md))
        own = f"{project()}-{os.getpid()}"
        seen = []
        try:
            for exc in (SystemExit(1), KeyboardInterrupt()):
                try:
                    with step("ctl_g", label="selftest", marker="ctl g"):
                        f = md / f"{own}.json"
                        m = json.loads(f.read_text(encoding="utf-8")) if f.is_file() else {}
                        seen.append(sorted(m) == ["command", "pid", "project", "start"] and m["pid"] == os.getpid()
                                    and not list(md.glob("*.tmp")))
                        raise exc
                except (SystemExit, KeyboardInterrupt):
                    pass
                seen.append(not (md / f"{own}.json").exists() and not (md / f"{own}.tmp").exists())
        finally:
            for k, v in saved.items():
                if v is None:
                    os.environ.pop(k, None)
                else:
                    os.environ[k] = v
        rcs = [r.get("rc") for r in _load(rd / "records.jsonl")]
        ok = seen == [True] * 4 and rcs == [1, 1]
        print(f"control marker-lifecycle: {'ok' if ok else 'FAIL'} inside/after (SystemExit, KeyboardInterrupt) {seen}, "
              f"record rc {rcs}")
        fails += not ok
        # 5.4 T21 (h) a parent step(marker=) runs a child `timing.py run` in a cwd outside git (another project): during
        # it only the parent's marker exists, the child's overlap is [] (holder's project skipped), none after
        md, rd, cw = base / "markers_h", base / "rec_h", base / "cwd_h"
        cw.mkdir()
        lister = (f"import json, os, sys; sys.path.insert(0, {os.path.dirname(me)!r}); import timing\n"
                  "d = os.environ['TIMING_MARKERS']\n"
                  "print('during', json.dumps(sorted(n for n in os.listdir(d) if n.endswith('.json'))))\n"
                  "print('project', json.dumps(timing.project()))\n")
        parent = (f"import json, os, subprocess, sys; sys.path.insert(0, {os.path.dirname(me)!r}); import timing\n"
                  "print('parent', os.getpid())\n"
                  "with timing.step('ctl_h_parent', label='selftest', marker='ctl h'):\n"
                  f"    r = subprocess.run([sys.executable, {me!r}, 'run', 'ctl_h', '--label', 'selftest', '--', "
                  f"sys.executable, '-c', {lister!r}], cwd={str(cw)!r}, capture_output=True)\n"
                  "    sys.stdout.write(r.stdout.decode('utf-8', 'replace')); sys.stdout.flush()\n"
                  "d = os.environ['TIMING_MARKERS']\n"
                  "print('after', json.dumps(sorted(n for n in os.listdir(d) if n.endswith('.json'))))\n")
        env = {k: v for k, v in os.environ.items() if k != "TIMING_MARKER_HOLDER"}
        env.update(TIMING_DIR=str(rd), TIMING_MARKERS=str(md), GIT_CEILING_DIRECTORIES=str(base))
        w = subprocess.run([sys.executable, "-c", parent], capture_output=True, env=env)
        out = dict(l.split(" ", 1) for l in w.stdout.decode("utf-8", "replace").splitlines() if " " in l)
        try:
            during, after = json.loads(out.get("during", "null")), json.loads(out.get("after", "null"))
            cproj = json.loads(out.get("project", "null"))
        except ValueError:
            during = after = cproj = None
        want = [f"{project()}-{out.get('parent')}.json"]
        recs = {r.get("step"): r for r in _load(rd / "records.jsonl")}
        got = recs["ctl_h"].get("overlap", "missing") if "ctl_h" in recs else "no record"
        ok = w.returncode == 0 and during == want and after == [] and got == [] and cproj not in (None, project())
        print(f"control marker-holder-child: {'ok' if ok else 'FAIL'} rc {w.returncode}, markers during {during} "
              f"(want {want}), child project {cproj}, child overlap {got}, after {after}")
        fails += not ok
        # 5.4 T21 (i) a child of a marked step that itself calls step(marker=...) writes no marker file
        md, rd = base / "markers_i", base / "rec_i"
        child = (f"import json, os, sys; sys.path.insert(0, {os.path.dirname(me)!r}); import timing\n"
                 "with timing.step('ctl_i_child', label='selftest', marker='ctl i child'):\n"
                 "    d = os.environ['TIMING_MARKERS']\n"
                 "    print('during', json.dumps(sorted(n for n in os.listdir(d) if n.endswith('.json'))))\n")
        parent = (f"import os, subprocess, sys; sys.path.insert(0, {os.path.dirname(me)!r}); import timing\n"
                  "print('parent', os.getpid())\n"
                  "with timing.step('ctl_i_parent', label='selftest', marker='ctl i'):\n"
                  f"    r = subprocess.run([sys.executable, '-c', {child!r}], capture_output=True)\n"
                  "    sys.stdout.write(r.stdout.decode('utf-8', 'replace')); sys.stdout.flush()\n")
        env.update(TIMING_DIR=str(rd), TIMING_MARKERS=str(md))
        w = subprocess.run([sys.executable, "-c", parent], capture_output=True, env=env)
        out = dict(l.split(" ", 1) for l in w.stdout.decode("utf-8", "replace").splitlines() if " " in l)
        try:
            during = json.loads(out.get("during", "null"))
        except ValueError:
            during = None
        want = [f"{project()}-{out.get('parent')}.json"]
        left = len(list(md.glob("*.json"))) if md.is_dir() else 0
        ok = w.returncode == 0 and during == want and left == 0
        print(f"control marker-holder-nested: {'ok' if ok else 'FAIL'} rc {w.returncode}, markers during child step "
              f"{during} (want {want}), left after {left}")
        fails += not ok
    finally:
        if made:  # 5.5 T12 (S7): was: shutil.rmtree(base) always (a user's DIR included)
            shutil.rmtree(base, ignore_errors=True)
        else:
            for p in list(base.iterdir()):
                shutil.rmtree(p, ignore_errors=True) if p.is_dir() else p.unlink(missing_ok=True)
        if held is not None:
            os.environ["TIMING_MARKER_HOLDER"] = held
    total = 12 - bool(_NESTED) - (msvcrt is None)  # was: 9 (5.5 T12: +summary-failed, +selftest-dir, +append-locked)
    print(f"timing self-test: {total - fails} of {total} ok")
    return 1 if fails else 0


def main(argv=None):
    argv = list(sys.argv[1:] if argv is None else argv)
    if argv[:1] == ["--self-test"]:
        return self_test(argv[1] if len(argv) > 1 else None)
    if argv[:1] == ["run"]:
        if "--" not in argv:
            print("timing: usage: timing.py run <step> [--label L] -- <cmd...>", file=sys.stderr)
            return 2
        i = argv.index("--")
        ap = argparse.ArgumentParser(prog="timing.py run")
        ap.add_argument("step")
        ap.add_argument("--label")
        a = ap.parse_args(argv[1:i])
        if not argv[i + 1:]:
            print("timing: no command after --", file=sys.stderr)
            return 2
        return run(a.step, argv[i + 1:], a.label)
    ap = argparse.ArgumentParser(prog="timing.py", description=__doc__.splitlines()[0],
                                 epilog="also: run <step> [--label L] -- <cmd...>; --self-test [DIR]")
    sub = ap.add_subparsers(dest="cmd")
    p = sub.add_parser("summary", help="markdown summary of a label, optionally against another")
    p.add_argument("--label", required=True)
    p.add_argument("--against")
    p.add_argument("--steps", help="comma list; also the row order")
    p = sub.add_parser("add", help="append pre-built records from a .jsonl file (validated; all or nothing)")
    p.add_argument("file")
    a = ap.parse_args(argv)
    if a.cmd == "summary":
        steps = a.steps.split(",") if a.steps else None
        print(summary_text(_load(), a.label, a.against, steps))
        print(f"\nrecords: {record_dir() / 'records.jsonl'}")
        return 0
    if a.cmd == "add":
        n, err = add_file(a.file)
        if err:
            print(f"timing: add refused: {err}", file=sys.stderr)
            return 2
        print(f"timing: added {n} records to {record_dir() / 'records.jsonl'}")
        return 0
    ap.print_help()
    return 2


if __name__ == "__main__":
    sys.exit(main())
