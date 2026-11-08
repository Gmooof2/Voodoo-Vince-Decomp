"""Run named gate self-test controls alone, as gate.controls() does (the teeth protocol, C0036, C0298).

usage: control_only.py NAME [NAME ...] [--relink-lines DIR]
Each NAME must be in gate.CONTROLS (CFG and `direct` controls included). On the unfixed tree a new control must print
`DID NOT FAIL as expected`; after the fix, `failed as expected`. --relink-lines writes each relink.gate_check run's
lines to DIR/relink_<k>.txt (k counts runs; the control dirs are removed after each control).
Exit: 0 all named controls failed as expected; 1 otherwise; 2 an unknown name.
Kept at the 5.1 close (T11) from the scratch drivers of 5.1 T1, T2, T3.1, T4, T13, T14; never a match claim. Lives in tests/, not tools/: see the sys.path line.
"""
import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))  # lives outside tools/: a tools/ module the
# gate process loads must be LISTED (verifier_lock.unlisted -> FAIL lock)
import gate  # noqa: E402
import relink  # noqa: E402


def main(argv):
    ap = argparse.ArgumentParser(prog="control_only.py", description=__doc__.splitlines()[0])
    ap.add_argument("names", nargs="+")
    ap.add_argument("--relink-lines", type=Path)
    a = ap.parse_args(argv)
    known = [n for n, _ in gate.CONTROLS]
    bad = [n for n in a.names if n not in known]
    if bad:
        print(f"control_only: unknown control(s): {', '.join(bad)}", file=sys.stderr)
        return 2
    if a.relink_lines:
        a.relink_lines.mkdir(parents=True, exist_ok=True)
        orig, runs = relink.gate_check, [0]

        def gc(*p, **k):
            rc, lines = orig(*p, **k)
            runs[0] += 1
            (a.relink_lines / f"relink_{runs[0]}.txt").write_text("\n".join(lines), encoding="utf-8")
            return rc, lines
        relink.gate_check = gc
    gate.CONTROLS = tuple(c for c in gate.CONTROLS if c[0] in a.names)
    good, k = gate.controls()
    return 0 if good == k else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
