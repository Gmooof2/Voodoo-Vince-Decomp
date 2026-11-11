# How this project works

A short guide for visitors: what counts as a match, how the checks are trusted, what the rule ids
in the code mean, and how this public repository is produced.

## The gate decides

`tools/gate.py` is the only judge of what is matched. A function is matched when objdiff scores it
100% against the object cut from the retail executable, and its compiled bytes equal the retail
XBE's bytes with zero unresolved relocations, register allocation included. Expected bytes come
only from retail (rule `D123`). The gate re-checks every matched function on every run, relinks
the whole executable and compares it with retail, and runs the provenance, annotation and audit
checks. Green is the exit code, never a printed line (rule `D26`).

Code outside the matched set compiles only behind `NON_MATCHING` (rule `D127`). Names, fields and
types come from recorded evidence (the prototype's debug files, retail disassembly, retail strings),
never from a guess (rule `D78`).

## Controls first shown failing

A check is trusted only after it has been seen to fail. Each control in `tools/gate.py --self-test`
plants one fault (a changed byte, a missing marker, a planted secret, an unlisted name) and must
fail at the check named for it; a control that does not fail is a failure of the self-test. New
checks are added with their control, run first against the unfixed gate where it can be, to show
it would have caught the problem. The checking tools are locked by fingerprint
(`tools/verifier_lock.py`); changing one needs the maintainer's review.

## Rule ids

Ids such as `D123` or `M28` in code and docs are rules. Each has its own file, `rules/<id>.md`, with
its text, origin and status; `rules/INDEX.md` lists them all, one line each, and can be searched by
id or tag. A rule is never deleted: when it changes, the old one is marked superseded and points to
its successor. Letter groups: `D` decisions about this project, `M` verification and method, `P`, `H`
and `X` the general process. Ids such as `C0036` are techniques in `cookbook/`. Ids such as `T4`
or `R183` point to the private journal (tasks, records, research), and nothing here needs them.

## How the work is organized

The work runs in generations (a new capability) split into phases. Each phase has two human
checkpoints: the maintainer approves its plan, and confirms its milestone, a machine-checkable
outcome (the gate, a fresh-clone walk, a progress figure). Between them the work runs as tasks.

Roles, in one paragraph: AI agents (Claude Code) do the work. One agent talks with the maintainer
and drafts each phase's plan; for each task an expert agent reads the plan, decides the approach
and briefs coders, which run the edit-build-test loops and commit green runs, and retrievers, which
look things up. Lessons are kept as rules (`rules/`), techniques (`cookbook/`), and observed results
(`docs/results.md`). The maintainer sets scope, approves every change to the checking tools, and
runs the hardware test.

## The public cut

This repository is not the development repository. Each public cut is exported from a named commit
by an allowlist (anything not listed stays out), with a small overlay of public-only files such as
`SETUP.md` and the CI workflow. Before a cut is published:

- the firewall audit (`tools/audit_public.py`) runs over the exported tree and its history and must
  report `OK (0 hits)`: no game bytes (rule `D46`), no machine-local values, no private paths;
- the cut is cloned fresh and set up from `SETUP.md` alone, every step run, and the gate and its
  self-test must pass in that clone;
- every number in the README is checked against its source.

Each cut is one new commit on top of the previous one; the private history is not included.
`docs/results.md` lists the observed results per phase.
