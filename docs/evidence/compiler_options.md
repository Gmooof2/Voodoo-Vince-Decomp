# Compiler acquisition options (for the 1.2 close)

Written 2026-10-25 for the close question (GENERATION_PLAN.md "Developer decides", compiler acquisition). Nothing
has been acquired, downloaded or installed. Every fact cites its report; "unknown" means no report sourced it.

## What the evidence says (T11, `docs/evidence/toolchain.md`)
- The prototype exes were compiled by cl **13.10.3077** and linked by LINK 7.10.3077. Status: `observed`, from two
  independent records (Rich header comp.ids and PDB compile records). The same records rule out 13.00.9466 (VC 7.0)
  and the other 13.10 builds (3052, 4035, 6030).
- Retail: cl 13.10.3077 is `inferred`, not observed. Retail is an XBE with no compiler record. It shares the
  prototype's build tree and the XDK 5558 library table, and about half its functions are byte-identical to the
  prototype's after masking.
- /GL (whole-program optimisation): `inferred` off. So no REPLAN is raised, and this decision is not waiting on it.
- Tagged family: **13.10.x (VC 7.1, the Visual Studio .NET 2003 compiler)**. The target build is 13.10.3077.

In plain terms: the compiler we need is the one in Visual Studio .NET 2003, build 3077. An option only counts as
exact if its cl reports exactly that build.

## Options that fit (13.10.x)

1. **Visual C++ Toolkit 2003 v1.01** (free command-line compiler from Microsoft).
   - Build: 13.10.3077. An archived Microsoft text says v1.01 "exactly match[es]" VS .NET 2003's compiler (R0-005:25),
     and a `cl /?` capture shows 13.10.3077 (R0-005:26). These are secondary sources, not a binary we checked.
     Watch out: v1.0 shipped 13.10.3052, which is not exact (R0-005:14,:25).
   - Exact build match: **yes**, if the copy is v1.01.
   - Cost: free when Microsoft released it (R0-003:13). Whether Microsoft still distributes it officially: unknown,
     not researched.
   - Licence risk: low if it comes from Microsoft under its own licence. It rises if the only copy available is an
     unofficial mirror, so check the installer's origin and keep its sha256.
   - Windows 11 install risk: high, unverified. One author reported the installer refused Windows 8/10 and ran it on
     XP x64 instead (R0-003:14). Fallback: install it in an XP or Windows 7 virtual machine and copy the compiler out.
   - Caveat: the Toolkit is the optimising compiler (R0-003:25). The .NET SDK 1.1 also carries a 13.10.3077 cl that
     does not optimise, and the version number alone cannot tell the two apart (R0-005:28). Do not take the SDK copy.
2. **Windows DDKs carrying a 13.10 cl.**
   - Builds: Server 2003 SP1 DDK (3790.1830) has 13.10.4035, from two forum reports (R0-005:15,:27). A "2003DDK" with
     13.10.2179 is a single unconfirmed post (R0-005:18).
   - Exact build match: **no**. Neither is 3077. Useful only as a nearest-build fallback.
   - Cost: was a Microsoft download or media. Current availability and price: unknown.
   - Licence risk: low from Microsoft media, otherwise unknown.
   - Windows 11 install risk: unknown, no report.
3. **Retail Visual Studio .NET 2003** (Professional, Enterprise or Academic; bought used).
   - Build: 13.10.3077 (R0-005:13; R1.2-002 (c)). Original Xbox development was done with VS .NET 2003 on XP
     (RXDK README, R0-003:24; R0-005:33).
   - Exact build match: **yes**, if it is the base release. Avoid any later update or "SP1" compiler: 13.10.6030 was
     reported as an SP1 build of unclear product (R0-005:16,:25).
   - Cost: unknown. Used-market prices were not researched.
   - Licence risk: low with a genuine boxed licence. Check that the copy is genuine and comes with its key.
   - Windows 11 install risk: unknown, no report. A 2003 installer could fail like the Toolkit's, so plan on the same
     XP/Windows 7 virtual machine fallback.

## Options for 13.00.x (VC 7.0): ruled out by the evidence
Listed only to close the question. T11's records refute 13.00.9466 for the prototype, so none of these are exact.
- Retail VS .NET 2002: 13.00.9466 (R0-005:12). Not exact. The Standard edition reportedly lacked the optimiser, an
  anecdotal 2004 report (GENERATION_PLAN.md "Developer decides"). Cost unknown; licence risk low with a genuine copy;
  Windows 11 install risk unknown.
- Windows XP DDK: 13.00.9176, from one Python list post (R0-005:17,:27). Not exact. Cost, licence and Windows 11 risk
  unknown.
- The XDK (5558, the build named by the retail library table): its own cl build is unknown, because no source maps
  XDK builds to cl builds (R0-005:19; R0-003:12,:33). Exact build match: unknown. It would be the highest-fidelity
  ground truth if it shipped cl 13.10.3077 (R0-005:34), but you have no XDK, and copies found online are of unclear
  licence. That licence risk is yours to accept or refuse. Cost: unknown. Windows 11 install risk: unknown (RXDK
  pairs XDK 5849 with VS .NET 2003, R0-005:33).

## Cross-check copy only: archaic-msvc/msvc710
A stripped repackage of VS .NET 2003 on GitHub. Its README states product 7.10.3077 and c1xx.dll 13.10.3077. It has
no licence, no stated origin and no hashes, and a single commit dated 2025-05-17 (R0-023:11,:17). The README's clone
command points at a different account, ifarbod/msvc710 (R0-023:17). It is **never the pinned source**. Once a licensed
13.10.3077 is installed, 1.4 may compare msvc710's file hashes and compiler output against that install as a sanity
check. Licence risk if used as more than a check: high (unknown origin).

## decomp.me: a per-function human aid, never a route
The external audit says decomp.me hosts `msvc7.0` and `msvc7.1` presets under its win32 platform. That is
**unverified**: the preset files returned 404 on 2026-10-25 (R0-030:12,:22,:33; R1.2-002 (d)). Even if the presets
exist, decomp.me has no Xbox platform and no XDK headers or libraries, so a scratch has to stub every Xbox type. It
works one function at a time. A person may use it to ask for help on a single function. It is never a gate, never
match evidence and never an acquisition route.

## The generation plan's recommendation, against this evidence
The generation plan says: take a free or retail route only when its build equals the evidence exactly; take the XDK
only with your explicit acceptance of its licence risk; if no exact build can be obtained, 1.4 records the nearest
build's match rate before anything else is decided; whatever is chosen stays outside git, with only its sha256 pins in
the repo.

Against the evidence, that becomes:
- **First choice: Visual C++ Toolkit 2003 v1.01** (13.10.3077, exact, free, legal). Expect the installer to fail on
  Windows 11 and use an XP or Windows 7 virtual machine to unpack it. Confirm `cl` prints 13.10.3077, not 3052.
- **Second choice: a genuine used VS .NET 2003** (Professional, Enterprise or Academic), base release, if no
  official Toolkit v1.01 installer can be found. Exact; the cost is a used-market purchase.
- **Not recommended:** VS .NET 2002, the XP DDK and the 13.10.4035 DDK (none is exact; keep them only as
  nearest-build fallbacks under the 1.4 rule). The XDK only with your explicit acceptance of the licence risk, and it
  is not needed for the build the evidence names. msvc710 is a cross-check only. decomp.me is a human aid only.
- Whatever is chosen: verify the installed `cl` reports 13.10.3077, record its sha256 in the repo, and keep the
  compiler outside git. The first compile should be the one-fixture `/GL` check in `toolchain.md` "For the close",
  which would turn the /GL answer from `inferred` into `observed`.
