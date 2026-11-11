"""Run named gate self-test controls alone, serially in this process (gate_controls._one; the teeth protocol, C0036, C0298).

usage: control_only.py NAME [NAME ...] [--relink-lines DIR]
Each NAME must be in gate_controls.CONTROLS (CFG and `direct` controls included). On the unfixed tree a new control must print
`DID NOT FAIL as expected`; after the fix, `failed as expected`. --relink-lines writes each relink.gate_check run's
lines to DIR/relink_<k>.txt (k counts runs; the control dirs are removed after each control).
Exit: 0 all named controls failed as expected; 1 otherwise; 2 an unknown name.
Kept at the 5.1 close (T11) from the scratch drivers of 5.1 T1, T2, T3.1, T4, T13, T14; never a match claim. Lives in tests/, not tools/: see the sys.path line.
5.5 T12.c4: gate.controls()/gate.CONTROLS moved to gate_controls (5.4 T5); its controls() runs every control in
worker processes that rebuild the full list, so a subset runs here through gate_controls._one, serially.
"""
import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))  # lives outside tools/: a tools/ module the
# gate process loads must be LISTED (verifier_lock.unlisted -> FAIL lock)
import byte_compare  # noqa: E402
import gate  # noqa: E402
import gate_controls  # noqa: E402
import relink  # noqa: E402


def main(argv):
    ap = argparse.ArgumentParser(prog="control_only.py", description=__doc__.splitlines()[0])
    ap.add_argument("names", nargs="+")
    ap.add_argument("--relink-lines", type=Path)
    a = ap.parse_args(argv)
    known = [n for n, _ in gate_controls.CONTROLS]
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
    gate_controls._bind(gate)
    root = byte_compare._phase_run_dir() / "gate" / "controls-only"
    lst = [c for c in gate_controls.CONTROLS if c[0] in a.names]
    good = sum(gate_controls._one(n, m, root, None) for n, m in lst)  # was: gate.CONTROLS filtered; gate.controls()
    print(f"controls: {good} failed as expected of {len(lst)}")
    return 0 if good == len(lst) else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
