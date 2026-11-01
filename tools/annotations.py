"""annotations (2.2 T2): check the D77 annotation lines in src against config/matched_set.toml.

CLI: annotations.py --check [--src DIR (default src)]
Grammar (docs/matching/CONVENTIONS.md §1): `// FUNCTION: USA 0x%08X` (optional trailing ` FOLDED`), then optional
`// PROTOTYPE: PREVIEW 0x%08X identical|near-identical|ambiguous`, then `// NAME: <source>`.
Checks: every matched_set [[function]] row has one FUNCTION line with its va in its unit's source, or more than one if
every such line is FOLDED (4.1 T4, CONVENTIONS.md:34-38; was: exactly one)
(build.unit_path, matched case-insensitively under DIR); every `// FUNCTION:` line in a .c/.cpp/.h/.hpp under DIR has
the form above and is followed by the optional PROTOTYPE line and the NAME line; a VA with more than one FUNCTION line
in a file, row or no row, passes only if every line is FOLDED (4.1 T4). Each failure is printed `FAIL <file>:<line>:
<why>`. VA order is advisory (4.1 T4; was: a failure): a line whose VA does not ascend (equal is ascending only when
both lines are FOLDED) prints `ORDER <file>:<line>: FUNCTION 0x.. not ascending after 0x..`, counted on the summary
line, never affecting the exit.
Library: check(src, list_path) -> (fails, nrows, nfiles, orders); fails and orders are lists of those lines.
Exit: 0 no failure (order lines allowed); 1 any failure; 2 refusal (missing src dir, unreadable matched set, row whose unit is not in units.toml).
"""
import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import build  # noqa: E402
import matched_set  # noqa: E402

REPO = Path(__file__).resolve().parent.parent
MATCHED = REPO / "config" / "matched_set.toml"
EXTS = (".c", ".cpp", ".h", ".hpp")
FUNC = re.compile(r"^\s*// FUNCTION: USA 0x([0-9A-F]{8})( FOLDED)?\s*$")
PROTO = re.compile(r"^\s*// PROTOTYPE: PREVIEW 0x[0-9A-F]{8} (identical|near-identical|ambiguous)\s*$")
NAME = re.compile(r"^\s*// NAME: \S")


def scan(path, rel, fails, orders):
    """-> [(va, line no, folded)] of the file's FUNCTION lines; grammar failures appended to `fails`, order lines to
    `orders` (4.1 T4: order is advisory; was: order failures in `fails`)."""
    lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
    got, prev = [], None
    for i, line in enumerate(lines):
        if "// FUNCTION:" not in line:
            continue
        m = FUNC.match(line)
        if not m:
            fails.append(f"FAIL {rel}:{i + 1}: malformed FUNCTION line")
            continue
        va, folded = int(m.group(1), 16), bool(m.group(2))
        j = i + 1
        if j < len(lines) and PROTO.match(lines[j]):
            j += 1
        if not (j < len(lines) and NAME.match(lines[j])):
            fails.append(f"FAIL {rel}:{j + 1}: FUNCTION 0x{va:08X} not followed by [PROTOTYPE line then] NAME line")
        if prev is not None and (va < prev[0] or (va == prev[0] and not (folded and prev[1]))):
            orders.append(f"ORDER {rel}:{i + 1}: FUNCTION 0x{va:08X} not ascending after 0x{prev[0]:08X}")
        prev = (va, folded)
        got.append((va, i + 1, folded))
    return got


def check(src, list_path=MATCHED):  # 4.1 T4: list_path for a scratch list; was: always MATCHED
    src = Path(src).resolve()
    if not src.is_dir():
        raise matched_set.Refuse(f"missing src dir {src}")
    rows = [e for e in matched_set.load(list_path) if e["kind"] == "function"]
    units = {u.get("name"): u for u in matched_set._units()}
    files = sorted((p for p in src.rglob("*") if p.is_file() and p.suffix.lower() in EXTS),
                   key=lambda p: p.relative_to(src).as_posix())
    fails, orders, found, rowed = [], [], {}, set()
    for p in files:
        rel = p.relative_to(src).as_posix()
        found[rel.lower()] = (rel, scan(p, rel, fails, orders))
    for r in rows:
        u = units.get(r["unit"])
        if u is None:
            raise matched_set.Refuse(f"{r['symbol']}: unit {r['unit']} not in {build.UNITS}")
        rel = build.unit_path(u)
        if rel is None or rel not in found:
            fails.append(f"FAIL {r['symbol']}: no source {rel} for unit {r['unit']} under {src}")
            continue
        name, vas = found[rel]
        rowed.add((rel, r["va"]))
        at = [folded for va, _ln, folded in vas if va == r["va"]]
        n = len(at)
        if n != 1 and not (n > 1 and all(at)):  # 4.1 T4: all FOLDED is legal; was: n != 1
            fails.append(f"FAIL {name}: {n} `// FUNCTION: USA 0x{r['va']:08X}` lines for {r['symbol']} (need 1, or every"
                         f" line FOLDED)")
    for rel, (name, vas) in found.items():  # 4.1 T4: a duplicate with no row here; was: seen only by the order rule
        dup = {}
        for va, ln, folded in vas:
            dup.setdefault(va, []).append((ln, folded))
        for va, at in dup.items():
            if len(at) > 1 and (rel, va) not in rowed and not all(f for _ln, f in at):
                fails.append(f"FAIL {name}:{at[1][0]}: {len(at)} `// FUNCTION: USA 0x{va:08X}` lines, no row here (need 1,"
                             f" or every line FOLDED)")
    return fails, len(rows), len(files), orders  # 4.1 T4: orders; was: order lines inside fails


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--check", action="store_true", required=True)
    ap.add_argument("--src", default=str(REPO / "src"))
    a = ap.parse_args(argv)
    try:
        fails, nrows, nfiles, orders = check(a.src)
    except matched_set.Refuse as e:
        print(f"annotations: REFUSED {e}", file=sys.stderr)
        return 2
    for f in orders + fails:
        print(f)
    print(f"annotations: {'FAIL' if fails else 'PASS'} {len(fails)} failure(s); {len(orders)} order line(s); "
          f"{nrows} matched row(s), {nfiles} source file(s)")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
