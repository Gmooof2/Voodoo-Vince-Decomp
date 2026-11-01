<!-- Cookbook index. One line per entry, appended by tools/cookbook_add.sh. Grep this file by tag or title; never read it whole; never read an entry you did not find here. -->
<!-- C<nnnn> | <title> | <tags> | <date> | <phase>/<task> | origin: <source section or report id> -->
C0001 | Identify and extract an Xbox disc image (XDVDFS, redump offset) | xbox,xdvdfs,iso,extraction,inputs | 2026-10-25 | 1.1/T12 | T12 probe + extraction
C0002 | XBE section digest is SHA1 of u32le(rawSize)+data | xbe,digest,sha1,pristine | 2026-10-25 | 1.1/T7 | T7.c1 retail check
C0003 | Self-test a git audit in a scratch repo (hook-safe, history-only proof) | git,audit,self-test,hooks,firewall | 2026-10-25 | 1.1/T8 | T8.c1 audit_public.py
C0004 | Detect disassembly/hex-dump-shaped text runs without false positives | audit,firewall,regex,disassembly,calibration | 2026-10-25 | 1.1/T8 | T8.c1 calibration
C0005 | pre-push hook: audit pushed blobs per ref, empty tree as OLD for new refs | git,hooks,pre-push,audit,firewall | 2026-10-25 | 1.1/T9 | T9 githooks/pre-push
C0006 | Scratch clones on Windows: core.autocrlf=false when tools hash tracked files | git,windows,crlf,scratch,hooks | 2026-10-25 | 1.1/T9 | T9 rehearsal
C0007 | Rich header decode; /GL records C and C++ under one LTCG_C prodid | rich,pe,msvc,ltcg,toolchain | 2026-10-25 | 1.2/T5 | T5 fixtures cl 19.51.36260; R1.2-005
C0008 | Read PDB 7.00 (MSF/info/DBI/modules) in Python; check vs llvm-pdbutil | pdb,msf,dbi,codeview,ltcg,llvm-pdbutil | 2026-10-25 | 1.2/T6 | T6.c1 pdb_info.py
C0009 | Tie an XBE to its source PE (peTimeDate) and order builds by stamps | xbe,pe,pdb,timestamp,timeline,rsds | 2026-10-25 | 1.2/T7 | T7 timeline
C0010 | Map PE procs into an XBE by masked bytes (iced-x86 offsets; imagebld rebases) | x86,decoder,iced,mask,xbe,pe,imagebld,proto_map | 2026-10-25 | 1.2/T8 | T8.c1 proto_map.py
C0011 | Classify proto procs in a later build: LIS anchors, gap windows, token difflib, decoy | proto_map,similarity,difflib,lis,anchor,decoy,reloc,jump-table | 2026-10-25 | 1.2/T9 | T9.c1 proto_map.py
C0012 | MSVC 7.1 toolchain from Rich counts vs PDB compile records; /GL signals | rich,pdb,msvc,ltcg,toolchain,llvm-pdbutil | 2026-10-25 | 1.2/T11 | T11 prototype default/Vince_qa; R1.2-006
C0013 | Ghidra analyzeHeadless on Windows: loader class name, no dot paths, pause hang | ghidra,headless,windows,toolchain | 2026-10-25 | 1.3/T4 | T4 headless probe
C0014 | ghidra-xbe XbSymbolDatabase/XTLID labels: mechanism and flag rule | ghidra,xbe,xbsymdb,xtlid,symbols | 2026-10-25 | 1.3/T5 | T5.c1 xbsymdb flag
C0015 | Ghidra export determinism: sort rows, primary label unstable at shared switch addresses | ghidra,determinism,export,switch | 2026-10-25 | 1.3/T5 | T5.c1 rerun diff
C0016 | Delink one function headless; prove x86 COFF relocs vs XBE (reloc identity) | delink,ghidra,coff,reloc,objdiff,x86 | 2026-10-25 | 1.3/T6 | T6.c1 delink one + coff_info
C0017 | Byte-compare obj function vs XBE: resolve relocs, mask unknowns, negative controls | byte-compare,reloc,coff,verifier,negative-control,x86 | 2026-10-25 | 1.3/T7 | T7.c1 byte_compare.py
C0018 | Read DBI section contributions; procs vs contributions under ICF; llvm-pdbutil traps | pdb,dbi,section-contributions,icf,llvm-pdbutil | 2026-10-25 | 1.3/T8 | T8.c1 pdb_info.py
C0019 | Classify retail XBE code: game / XDK / unidentified (module rule, byte denominator) | xdk,xbe,pdb,sections,coverage | 2026-10-25 | 1.3/T9 | T9.c1 xdk_report
C0020 | Unit extents in an MSVC 7.1 /Gy image: SC runs, main run, grouped tail sub-sections | units,pdb,section-contributions,link-order,anchor,delink | 2026-10-25 | 1.3/T10 | T10.c1 units_seed.py
C0021 | Delink many units headless byte-exact: thunk redirect off, dup names, rc/cmd traps | delink,ghidra,coff,thunk,headless,windows,determinism | 2026-10-25 | 1.3/T11 | T11.c1 delink --fresh/--repro
C0022 | One output per pipeline stage; route readers by artifact provenance | pipeline,export,ghidra,derived,ordering | 2026-10-25 | 1.3/T12.1 | delink --fresh overwrote T5 export
C0023 | Read VC 7.1 PDB types (TPI 19990903) when llvm-pdbutil refuses | pdb,tpi,types,llvm-pdbutil,vc71,_ST | 2026-10-25 | 1.4/T4 | T4.c1 scratch tpi.py
C0024 | Extract old MSVC without setup: InstallShield stream decode, IS-scripted MSI admin, VS2003 discs | msvc,toolchain,installshield,msi,msiexec,install | 2026-10-25 | 1.4/T11 | T11 VCToolkitSetup.exe + VS .NET 2003 Pro ISOs
C0025 | cl 13.10 /GL marks: S_COMPILE2 flags 0x400 fLTCG 1, Rich 0x64 for C and C++ | msvc,ltcg,pdb,rich,toolchain | 2026-10-25 | 1.4/T7 | T7 fixtures cl 13.10.3077 + LINK 7.10.3077
C0026 | cl 13.10 exits 0 on unknown options; catch D4002 by code | msvc,cl,d4002,negative-control,flags | 2026-10-25 | 1.4/T8 | T8.c1 compiler_pin --self-test
C0027 | Git Bash turns cl /X flags into paths; MSYS_NO_PATHCONV=1 | msvc,cl,git-bash,msys,windows | 2026-10-25 | 1.4/T8 | T8.c1 hand cl runs
C0028 | cl 13.10 /O1 lays blocks in source order; shape source to retail branch order | msvc,cl,codegen,matching,branch-order,gs | 2026-10-25 | 1.4/T9 | T9.c1 LocString_GetStringID
C0029 | cl 13.10 dtor without vptr store = no declared dtor; force emit; control by /Ob0 | msvc,cl,dtor,vptr,matching,inline,negative-control | 2026-10-25 | 1.4/T9 | T9.c2 FlourSackMain dtor
C0030 | cl 13.10 EH: /EHa keeps try-exit store; name __ehhandler$/__EH_prolog/__except_list relocs | msvc,cl,eh,ehprolog,reloc,byte-compare,matching | 2026-10-25 | 1.4/T9 | T9.c3 _Uninit_fill_n
C0031 | cl 13.10 switch fn: inline accessor tell, jump-table statics, string names, /EH per obj | msvc,cl,switch,jump-table,reloc,byte-compare,matching,eh | 2026-10-25 | 1.4/T9 | T9.c4 SetState
C0032 | cl 13.10 COMDAT Selection tells inline copies (2) from real functions (1) | msvc,cl,coff,comdat,gate,matched-set,inline | 2026-10-25 | 1.5/T3 | T3.c1 matched_set extra check
C0033 | objdiff 0% on a matched switch fn: delinked target exports Ghidra caseD labels | objdiff,delink,ghidra,switch,coff | 2026-10-25 | 1.5/T4 | T4.c1 self-test SetState
C0034 | objdiff 99.99% on a byte-matched fn: delinked target has no code relocs in it | objdiff,delink,ghidra,relocation,switch,coff | 2026-10-25 | 1.5/T4.1 | T4.1 self-test SetState
C0036 | Gate negative controls: re-run the real gate core on scratch inputs | gate,negative-control,verifier,self-test,d51 | 2026-10-25 | 1.5/T5 | T5.c1 gate.py --self-test
C0037 | Verifier lock: fingerprint the gate's files and its loaded tools/ imports | gate,verifier,fingerprint,lock,d76,self-test | 2026-10-25 | 1.5/T6 | T6.c1 verifier_lock.py
C0038 | Progress denominators: applied population, entry-byte class, doubtful buckets follow section | progress,denominator,xdk,coverage,applied | 2026-10-25 | 1.5/T7 | T7.c1 progress.py
C0039 | VC 7.1 PDB proc signature: TPI type index, leaf 0x1008/0x1009 (not 16t) | pdb,tpi,signature,vc71,procedure | 2026-10-25 | 1.6/T2 | T2.c1 candidates.py sig
C0040 | units.toml VA-to-unit lookup: 163 units have no text_start; skip them | units,units.toml,lookup,xdk | 2026-10-25 | 1.6/T2 | T2.c1 candidates.py pool
C0041 | Name-only MSVC undecoration via pinned undname.exe 0x1000 | msvc,symbols,demangle,tools | 2026-10-25 | 1.6/T3 | T3.c1 match_one symbol resolve
C0042 | cl 13.10 size assert: negative-array typedef, C2118, zero symbols | msvc,cl,static-assert,struct,pdb,coff,decomp.h | 2026-10-25 | 1.6/T4 | T4.c1 decomp.h control
C0043 | VC 7.1 TPI proc signature decode: LF_PROCEDURE/LF_MFUNCTION/LF_ARGLIST layouts | pdb,tpi,signature,vc71,procedure,arglist,calltype | 2026-10-25 | 1.6/T5 | T5 scratch sig.py
C0044 | default.pdb class/member TPI records are non-_ST forms (C0023 fix) | pdb,tpi,types,vc71,class,member,layout | 2026-10-25 | 1.6/T6 | T6.c1
C0045 | Dump obj code bytes with pinned link -dump -rawdata; no -disasm | msvc,link,dumpbin,bytes,disasm,git-bash | 2026-10-25 | 1.6/T6 | T6.c1
C0046 | /O1 whole-struct assignment = push N/pop ecx/rep movsd | msvc,cl,o1,struct,copy,rep-movsd,idiom | 2026-10-25 | 1.6/T6 | T6.c1
C0047 | git rev-list --objects names each blob once (first path); history fixtures need distinct bytes | git,history,audit,self-test | 2026-10-26 | 1.7/T7 | audit_public journal exemption
C0048 | Bash tool heredoc collapses a doubled backslash; build escapes from bytes | harness,bash,heredoc,escape | 2026-10-26 | 1.7/T7 | audit_public fixture edit
C0049 | Read-only XBDM client: guarded send path, candidate identify, loopback mock | xbox,xbdm,runtime,read-only,mock | 2026-10-26 | 1.7/T4 | T4 xbdm_probe
C0050 | XBDM wire on the box: default.exe module, base/size not header, known-true not build-proof | xbox,xbdm,runtime,wire,modules | 2026-10-26 | 1.7/T5 | T5 live readback
C0051 | Live XBE build identity: TimeDate + cert titleId/version via getmem | xbox,xbdm,runtime,build-identity,xbe | 2026-10-26 | 1.8/T2 | T2 xbdm_probe identity
C0052 | Hash pins taken in an autocrlf=true working copy fail on LF clones | git,windows,crlf,verifier,lock,pin | 2026-10-26 | 1.8/T5 | T5 clone gate FAIL lock
C0053 | Ghidra fresh analysis nondeterministic at low rate: extern callee names | ghidra,delink,determinism,fid,repro | 2026-10-26 | 1.8/T5 | T5 clone vs WC hashes.txt
C0054 | Ghidra FID names vary between fresh runs: reset them to DEFAULT before applying symbols | ghidra,delink,determinism,fid,repro,headless | 2026-10-26 | 1.8/T11 | T11 34 fresh runs, fix B g1-g3 + repro
C0055 | Scratch object compare: .debug$S size follows source name length; use equal-length names | coff,compile,debug,scratch,controls | 2026-10-26 | 1.8/T6 | T6.c1 C1.6-3 .debug$S 0x79 vs 0x78
C0056 | Hook plant controls: non-ignored plant name, assert staged before the hook | hooks,audit,plant,controls,gitignore | 2026-10-26 | 1.8/T6 | T6.c2 C1.1-4 .bin plant ignored
C0057 | Completeness marker: delete first, write last over a digest; readers refuse | delink,cut,marker,refusal,generated | 2026-10-26 | 2.1/T1 | T1 2.1 cut.complete
C0058 | delinker trims intra-unit rel32 relocs (Trim superfluous PC-relative); trim off over-relocates | delink,relocation,objdiff,coff,rel32,ghidra | 2026-10-26 | 2.1/T3 | T3.c1 trim-off trial
C0059 | Name an unmapped retail callee: near_identical row + caller rel32 xref | symbols,pdb,proto_map,pin,xref,d78 | 2026-10-26 | 2.1/T4 | T4 GameOptions::Set pin
C0060 | Static check reads delinked target relocs; refuse stale cut; one-reloc-removed control | candidates,c0035,delink,relocation,rel32,negative-control | 2026-10-26 | 2.1/T5 | T5.c1 c0035 reads cut
C0061 | Accept a leaked value in history: blob-id list plus line carry for growing files | git,audit,firewall,allowlist,history,journal | 2026-10-26 | 2.1/T7 | T7.c1 audit_public address class
C0062 | A /Oy- control cannot discriminate a frameless leaf function | msvc,opt,control,oy,leaf,match_one | 2026-10-26 | 2.2/T2 | T2.c1 BackupState control
C0063 | Read retail body + reloc targets with llvm-objdump on the delinked cut | llvm-objdump,delink,disassembly,relocation,tail-jmp,callee,opt,leaf | 2026-10-26 | 2.2/T3 | T3.c1 RestorePreviousState Set callee
C0064 | cl 13.10 /O1 x87 idioms: fldz/fld1 literals, fst+fstp chain, double fstp | msvc,cl,x87,float,fpu,fld1,fldz,fstp,codegen,matching | 2026-10-26 | 2.2/T4 | T4.c1 four float rows
C0065 | opt control: /O2 discriminates float-literal/float-arg functions; /Oy- only with esp arg reads | msvc,opt,control,o2,oy,negative-control,match_one | 2026-10-26 | 2.2/T4 | T4.c1 controls
C0066 | cl 13.10 /O1 vcall forms: this, member ptr, struct return; slots from PDB intro vbaseoff | msvc,cl,o1,vcall,virtual,vtable,pdb,tpi,codegen,matching | 2026-10-26 | 2.2/T5 | T5.c1 four vcall rows
C0067 | cl 13.10 /O1 bit-test forms: test byte imm from plain-int flags + LF_ENUM masks; no bt | msvc,cl,o1,bittest,test,flags,enum,bitfield,pdb,tpi,codegen,matching | 2026-10-26 | 2.2/T6 | T6.c1 six bittest rows
C0068 | Local variable types from PDB module S_BPREL32 records fix stack layout and store order | pdb,locals,bprel32,module,stack,codegen,matching,lever | 2026-10-26 | 2.2/T6 | T6.c1 SwampInput::GetRecord
C0069 | cl 13.10 inlined callee: retail-byte tells, PDB callee, /Ob0 control proof | msvc,cl,inline,matching,negative-control,pdb,refcount | 2026-10-26 | 2.2/T7 | T7 controls FlourSack dtor, MoviePlayer::GetRenderTexture
C0070 | Switch class: jump table past Ghidra bodyMax; bodymax passes a reloc-field-covered tail | switch,jump-table,candidates,bodymax,delink,relocation,negative-control,matching | 2026-10-26 | 2.2/T8 | T8.c1 SetState
C0071 | Ghidra body holes: data-reached code (SEH filter/handler) gets no cut relocs; ApplySymbols hole fill | ghidra,delink,relocation,objdiff,seh,eh,body,hole,applysymbols | 2026-10-26 | 2.2/T9 | T9.c1 main hole fill
C0072 | cl 13.10 EH class in retail: SEH prolog/scope table, __ehhandler$ stub + FuncInfo, __EH_prolog; pins and controls | msvc,cl,eh,seh,ehprolog,scope-table,funcinfo,reloc,pin,control,matching | 2026-10-26 | 2.2/T9 | T9.c1 main SEH match
C0073 | Embedded zlib 1.1.x (BeepLib) matches verbatim at /O1 /Oy; /O2 is the control | zlib,beeplib,library-source,c,opt,o2,control | 2026-10-26 | 2.2/T10 | T10.c1 _adler32
C0074 | Write an XBE from a LINK 7.10 PE like imagebld (layout, rebase, TLS, thunk) | xbe,pe,imagebld,writer,tls,rebase,relink | 2026-10-26 | 2.3/T2 | T2.c1 xbe_write.py
C0075 | Link a lone retail section: filler objects not absolute stubs; /SUBSYSTEM:XBOX ignores /BASE /ALIGN | link,relink,coff,filler,stub,subsystem,xbe,pe,reloc | 2026-10-26 | 2.3/T3 | T3.c1 relink XGRPH
C0076 | Relink a section from range-cut originals as ordered COMDATs: /ORDER underscore, REF+INCLUDE, dup-name export | link,relink,order,comdat,opt-ref,include,delink,thunk,ghidra | 2026-10-26 | 2.3/T4 | T4.c1 .text+.rdata ordered link
C0077 | Substitute our objects into a relinked section: split at matched VA, /ORDER slot, data split, REF, .sxdata pin | relink,substitution,order,comdat,data,rdata,opt-ref,sxdata,seh,link | 2026-10-26 | 2.3/T4 | T4.c2 + expert: check --units Globals,Main
C0078 | Whole-image relink from originals: bss cover, REL32-outside LNK2016, pinned header region | link,relink,xbe,rel32,lnk2016,bss,absolute,header,coff | 2026-10-26 | 2.3/T5 | T5.c1 check --originals
C0079 | Whole-image substitution: skip unreferenced data COMDATs, rename unreachable inline dups, pin CRT names | relink,substitution,comdat,inline,opt-ref,crt,fltused,verifier | 2026-10-26 | 2.3/T6 | T6.c1 check whole image
C0080 | Relink a C++-EH function: stub + .xdata$x VA by reloc site, $-group sorts last, rename next piece <group>~ | relink,eh,funcinfo,ehhandler,xdata,comdat,link,group,substitution,reloc | 2026-10-26 | 2.3/T7 | T7.c2 fixture _Uninit_fill_n
C0081 | Attended XBDM write client: exact-form guard, pinned path, binary mock | xbox,xbdm,runtime,write,mock,guard | 2026-10-27 | 2.4/T2 | T2 xbdm_deploy
C0082 | XBDM on Cerbios: file verbs need the dashboard; running title in device form | xbox,xbdm,runtime,cerbios,paths | 2026-10-27 | 2.4/T4 | T4 live control boot
C0083 | XBDM file write: delete then sendfile (204, payload, final 200), prove by change time | xbox,xbdm,runtime,write,wire | 2026-10-27 | 2.4/T6 | T5.1/T6 live deploy and restore
C0084 | __asm-only retail body (rdtsc) is a D45 wall; pin unnamed callees for masked bytes | msvc,asm,d45,wall,symbols_pin,callee,masked,matching | 2026-10-27 | 2.5/T4 | T4.c1 GetTSC, ~BeepState
C0085 | objdiff fails with bytes equal: cut symbol spans an unsymboled tail-jump target | delink,cut,objdiff,bodymax,candidates,dtor,llvm-objdump,matching | 2026-10-27 | 2.5/T4 | T4.c1 ~LaundryCarMain
C0086 | Multi-function unit: add all its rows, then gate once; C names keep the underscore | gate,matched_set,unit,d41,match_one,control,msys | 2026-10-27 | 2.5/T5 | T5.c1 inflate.obj
C0087 | Inlined memcpy (rep movsd+movsb) in /O1 body: add /Oi; /O2 /Os is equivalent | msvc,cl,opt,oi,memcpy,intrinsic,rep-movsd,o1,os,zlib | 2026-10-27 | 2.5/T5 | T5.c2 inflate_flush
C0088 | cl 13.10 /O1 x87 compare (fcompp/fnstsw/test ah,5), fst/fstp double-float chain, literal between int args | msvc,cl,x87,float,fpu,fcompp,fnstsw,fldz,fld1,codegen,matching | 2026-10-27 | 2.5/T6 | T6.c1 seven float rows
C0089 | cl 13.10 /O1 x87 arithmetic: chained fst/fstp, inline fsin/fcos needs /Oi, lerp faddp, fcomp loop | msvc,cl,x87,float,fpu,fsin,fcos,oi,intrinsic,codegen,matching | 2026-10-27 | 2.5/T6 | T6.c2 eight float rows
C0090 | Gate-only fail from <string>: extra __ehhandler$ object, memcpy 0 VAs; opaque basic_string fix | msvc,stl,basic_string,eh,ehhandler,relink,gate,objects,matching | 2026-10-27 | 2.5/T6 | T6.c2 SwampLoader::Bundle
C0091 | cl 13.10 /O1: delete on virtual-dtor member = vecDelDtor(1); single-case switch = dec eax/jne | msvc,cl,o1,vcall,virtual,dtor,delete,switch,codegen,matching | 2026-10-27 | 2.5/T7 | T7.c1 FFlyEmitter Set*Ctrl
C0092 | cl 13.10 /O1 Ptr<T>/SubRef dtor and opaque basic_string ~ via external _Tidy; pdb_types LF_ARRAY gap | msvc,cl,o1,dtor,refcount,smartptr,basic_string,stl,pdb,tpi,array,matching | 2026-10-27 | 2.5/T7 | T7.c2 ~Bundle, KillRope
C0093 | Screen pool rows for the C0085 cut-extent tell before drafting | candidates,delink,cut,objdiff,c0085,screening,matching | 2026-10-27 | 2.5/T10.1 | T10.1.c1 GetBounds, SetToLoading
C0094 | C2118 size assert on a char-array-only class: alignment from a PDB vfptr or pointer | msvc,cl,static-assert,struct,pdb,alignment,decomp.h | 2026-10-27 | 2.5/T10.1 | T10.1.c1 VinceGiblets
C0095 | Probe a candidate's relink on a scratch matched list (gate._core), C++ EH case | relink,eh,probe,matched-set,c0080,gate | 2026-10-27 | 2.5/T10.1 | T10.1.c2 _Uninit_fill_n<Vector3>
C0096 | Declaration from mangled name: access letter, explicit template instantiation, D3D wrappers | msvc,mangling,access,template,instantiation,d3d,match_one,matching | 2026-10-27 | 2.5/T12 | T12.c1 DinoBoss, SwampRenderer
C0097 | Pick batch input from pool_buckets.md; library filter splits into stl and stdsig | candidates,pool,buckets,d29,screening,c0093,library,std,matching | 2026-10-27 | 2.5/T14 | T14 pool_buckets
C0098 | Patch a tool's module attribute via a private namespace, not the shared module | python,controls,monkeypatch | 2026-10-27 | 2.6/T6 | T6.c1 C2.1-1 driver
C0099 | Clone commands through a config-value redacting wrapper (D46) | clone,d46,redact,audit,scratch,xbdm | 2026-10-27 | 2.6/T7 | T7.c1 inclone.py
C0100 | Slow gate self-test: time each layer, scope costly globals to their own control | gate,self-test,wall-time,audit,relink,negative-control | 2026-10-27 | 2.6/T11 | T11 gate.py --self-test
gen2 | Generation 2 entries | see cookbook/gen2/INDEX.md
C0035 | VC 7.1 TPI LF_ARRAY element count; same-name forward refs of different sizes | pdb,tpi,array,lf_array,forward-ref,vc71,types | 2026-10-27 | 3.1/T3 | pdb_types tsize
C0101 | Name pool reloc targets by prototype site correspondence (name_sites.py); status = byte check | naming,pin,symbols,pdb,publics,proto_map,reloc,correspondence,xbe,constants | 2026-10-27 | 3.1/T5 | T5.c1 name_sites.py
C0102 | Constant vs address at abs sites via proto .reloc; caller-xref second pass limits | naming,constants,reloc,proto_map,pool_buckets,site-corr,xref | 2026-10-27 | 3.1/T6 | T6.c1 name_sites constant rule
C0103 | Fix a cut-extent wall with a func pin at row end: naming, FUN_ trap, folded targets | delink,cut,c0085,c0093,applysymbols,ghidra,pins,d78,folded | 2026-10-27 | 3.1/T7 | T7.c1 45 cut boundary pins
C0104 | match_finish.sh: one unguarded row per finish; cleanup after a failed finish | match_finish,matched_set,gate,non_matching,ledger,matching | 2026-10-27 | 3.1/T7 | T7.c2 four former cut walls
C0105 | Grow a short Ghidra body to the pin size at ingest; refuse on overlap | ghidra,applysymbols,bodymax,body,delink,pins,candidates,d78 | 2026-10-27 | 3.1/T8 | T8.c1 grow pass
C0106 | Unit edges by byte chain (chain, chain-neighbour); outside procs as moved COMDATs | units,units.toml,delink,edge,chain,comdat,anchor,lis,proto_map,inconsistent | 2026-10-27 | 3.2/T1 | T1.c1 Init#7, Switch#331
C0107 | Split a merged Ghidra body at a pinned func; absorb a FUN_ fragment on grow | ghidra,applysymbols,split,body,grow,absorb,delink,pins,c0105,c0103 | 2026-10-27 | 3.2/T2 | T2.c1 split+absorb
C0108 | Ghidra no ref for pointer imm32/disp32: add USER_DEFINED DATA refs so the delinker relocates | ghidra,delink,relocation,applysymbols | 2026-10-27 | 3.2/T4 | T4.c1 9b742cd9
C0109 | Ghidra folds a tail-jmp target below the jumper's entry: splitBelow pin | ghidra,applysymbols,delink,tail-jmp,split,pins,c0085 | 2026-10-28 | 3.2/T4 | T4.c3 62cf63a0
C0110 | Re-make a past delink state from a git archive to diff targets per unit | delink,review,before-after,coff,reloc,repro | 2026-10-28 | 3.2/T5 | T5 target before/after review
C0111 | run.sh prints exit= first; a piped tail hides the exit code | harness,run.sh,gate,exit-code,d26 | 2026-10-28 | 3.2/T6 | T6.c1 gate runs
C0112 | MSVC typedef names are S_UDT records, not TPI names | pdb,tpi,s_udt,typedef,codeview,lint | 2026-10-28 | 3.2/T6 | T6.c1 cast census
C0113 | Function locals from our own /Z7 obj CodeView (C13), coverage limits | codeview,c13,z7,locals,pdb,lint,s_register,secrel | 2026-10-28 | 3.2/T6 | T6.c2 locals check
C0114 | Route a seeded hand-edited TOML by in-place per-block edits | units,toml,seed,route,generated-doc | 2026-10-28 | 3.2/T8 | T8.c1 units_seed --route
C0115 | Range a no-anchor unit from disasm-verified pins; release moves the block into ranged order | units,units.toml,route,no-range,pins,anchor,chain,c0106 | 2026-10-28 | 3.2/T9 | T9.c1 Particle#451
C0116 | Body holes in created/split fns: fillHoles after createFn and splitAt | ghidra,delink,relocation,applysymbols,hole,body,name_sites | 2026-10-28 | 3.2/T10 | T10.c1 94b2eb11
C0117 | PDB compiler-generated member with no user in its unit stays explicit under !FAKE | fake,copy-ctor,implicit,compiler-generated,__that,d78,pdb | 2026-10-28 | 3.3/T2 | Bundle copy ctor
C0118 | Proto symbol inside another's retail body: second entry into a shared tail | inside,applysymbols,pins,shared-tail,llvm-objdump,size | 2026-10-28 | 3.3/T2 | OptionsMenuGuts/PropellerMain dtors
C0119 | cl 13.10 /O1 small-body tells: ternary vs if, setne bool, esi local copy, mutable | msvc,cl,codegen,matching,o1,setne,sbb,mutable | 2026-10-28 | 3.3/T3 | T3.c1 32 ready rows
C0120 | Name params/locals from pdb_types.py locals before finishing; provenance locals check | provenance,locals,pdb,match_finish,gate,naming | 2026-10-28 | 3.3/T3 | T3.c1 first finish failed on anim vs id
C0121 | Retail calls a helper /O1 inlines: /Ob1 unit control (BeepAlloc) | msvc,cl,inline,opt,ob1,control,matching | 2026-10-28 | 3.3/T3 | T3.c1 BeepAlloc 4 rows
C0122 | Implicit dtors via [[emitter]] row; /O1 call-argument branch forms | msvc,cl,o1,dtor,implicit,emitter,matched_set,stl,codegen,matching | 2026-10-28 | 3.3/T4 | T4.c1 27 ready rows
C0123 | Walls after match_one passes: vptr vtables, SEH $T renumber, Ghidra-only callees, functions.jsonl | relink,vtable,seh,eh,pins,provisional,progress,gate,matching | 2026-10-28 | 3.3/T4 | T4.c1 left rows
C0124 | Delinked obj disasm by address range; implicit copy ctor; Ptr by-value merged call | llvm-objdump,delink,copy-ctor,ptr,friend,o1,codegen,matching | 2026-10-28 | 3.3/T4 | T4.c2 5 rows
C0125 | Large /O1 bodies: __forceinline wrappers, ternary return temps, literal-block relink wall | msvc,cl,o1,forceinline,inline,ternary,temp,rdata,relink,strings,matching | 2026-10-28 | 3.3/T4 | T4.c3 >512 B rows
C0126 | creationFunc debug-new form; scattered .rdata relink wall; float literal-value tell | msvc,cl,o1,creationfunc,operator-new,file-string,rdata,relink,float,literal,switch,provenance,matching | 2026-10-28 | 3.3/T5 | T5.c1 28 rows
C0127 | /O1 bool sete ternary; call-first reference local; std::list shell; copy-ctor emitters | msvc,cl,o1,sete,bool,ternary,eval-order,stl,list,padding,copy-ctor,emitter,placement-new,match_finish,matching | 2026-10-28 | 3.3/T5 | T5.c2 15 rows
C0128 | Relink-wall .rdata screen per unit; PDB vs retail layout field_<off>; same-TU edx wall | relink,rdata,screening,c0126,pdb,layout,field,register,o1,matching | 2026-10-28 | 3.3/T6 | T6.c1 31 rows
C0129 | Promote site-corr data pins by retail bytes; PDB data records are undecorated | naming,pin,site-corr,data,pdb,s_gdata32,tpi,reloc,vtable,bss | 2026-10-28 | 3.3/T8 | T8.c1 name_sites --promote-data
C0130 | Name a folded multi-public callee VA with alias pins (one line per public) | pins,folded,d44,alias,relink,name_sites,objdiff | 2026-10-28 | 3.3/T9 | T9.c1 probe 0x1819A
C0131 | Pin-only func rows need intra-unit REL32: delink _func_rows reads sized pins | delink,rel32,c0035,pins,check-cut,relocation | 2026-10-28 | 3.3/T9 | T9.c3 fold-alias pins
C0132 | STL rows by member explicit instantiation from VC 7.1 bodies; implicit-callee relink FAIL | msvc,stl,template,instantiation,vector,relink,pin,fold,matching | 2026-10-28 | 3.3/T10 | T10.c1 EvilVince, Blender
C0133 | std-sig rows: opaque basic_string VC7.1 layout, adjacent-literal relink wall, bool/return tells | msvc,cl,o1,stl,basic_string,opaque,rdata,relink,literal,strings,bool,ternary,provenance,typedef,matching | 2026-10-28 | 3.3/T11 | T11.c1 21 rows
C0134 | Relink: .rdata contributions placed by retail VA via .rdata$<va8> names; rdata-unsorted control | relink,rdata,link,order,section-name,literal,c0126,c0133,control | 2026-10-28 | 3.3/T12 | T12.c1 3 lifted rows
C0135 | creationFunc rows in bulk by generator; /O2 87.69 control; duplicate literal COMDAT relink wall | msvc,cl,o1,creationfunc,generator,opt,control,o2,relink,comdat,literal,ledger,match_one,matching | 2026-10-29 | 3.3/T13 | T13.c1 66+9 rows
C0136 | Folded ResourceHandle members: relink PE-entry wall, null-temp dtor elision, pin-only finish wall | relink,comdat,folded,alias,pins,resourcehandle,match_finish,matched_set,msvc,cl,o1,dtor,codegen | 2026-10-29 | 3.3/T14 | T14.c1 rows 1,25,27
C0137 | Unit opt from a one-flag --control; re-ledger rows plainly after | msvc,cl,opt,control,ob1,oi,units,ledger,match_one,matching | 2026-10-29 | 3.3/T19 | T19.c1 BeepAlloc/BeepFile/SwampInput
C0138 | M18 callee-name-conflict on XDK thunk: pin outgoing target first | m18,callee,pin,xdk,xbsymdb,name_sites,dsound,d3d,naming | 2026-10-29 | 3.3/T19 | T19.c2 IDirectSoundBuffer pins
C0140 | creationFunc in existing units: merge generated shells; cut-extent screen; tail form no opt evidence | creationfunc,generator,c0093,cut-extent,shell,size,opt,control,matching | 2026-10-29 | 3.3/T20 | T20.c1 22 rows
C0141 | cl 13.10 /O1 sibling x87 add order tie-break and cvttss2si dest register: levers tried | msvc,cl,o1,x87,fadd,operand-order,regalloc,sse,cvttss2si,matching | 2026-10-29 | 3.3/T21 | T21.c1 Sprite::Render thinker round
C0142 | Name a near-identical row: anchor window + similarity + accepted-caller xref (near-corr) | near-identical,naming,proto_map,xref,c0059,anchor,decoy,pin,near-corr,control | 2026-10-29 | 3.4/T2 | T2.c1 name_sites --near
C0143 | Near-lane change class: debug new(file, __LINE__) line constant shifted | near-identical,near-lane,retail-delta,operator-new,line,immediate,d78,matching | 2026-10-29 | 3.4/T3 | T3.c1 BeepFile Open x4
C0144 | std-sig: inline basic_string ctor beside out-of-line one; unresolved data inside pinned global = member | msvc,stl,basic_string,inline,ctor,tidy,c0133,reloc,data,global,member,candidates,matching | 2026-10-29 | 3.4/T3 | T3.c1 BeepFile::Open(string)
C0145 | Inline-elsewhere emitters (LNK2005), ??_G by delete, vptr-store order, string operator=, listing ledger trap | msvc,cl,o1,emitter,comdat,lnk2005,relink,dtor,vptr,ctor,stl,string,ledger,match_one,matching | 2026-10-29 | 3.4/T4 | T4.c1 25 ready rows
C0146 | /O1 levers: vector operator[] via begin()+pos, opaque set shell, block order, chained stores, PDB volatile | msvc,cl,o1,stl,vector,set,opaque,block-order,codegen,vptr,ctor,volatile,provenance,matching | 2026-10-29 | 3.4/T4 | T4.c2 25 ready rows
C0147 | Near-lane change class: member offset shift, retail class grew, body unchanged | near-identical,near-lane,retail-delta,member,offset,layout,field,d78,matching | 2026-10-29 | 3.4/T5 | T5.c1 HUDUI/Vince/Critter 13 rows
C0148 | Near-lane finish traps: pin-only-not-alias wall, one unwrapped draft, global_ placeholder | near-lane,match_finish,matched_set,pin,provenance,placeholder,non_matching,verdict,matching | 2026-10-29 | 3.4/T5 | T5.c1 Diary, GameOptions::Set, GetBeadDest
C0149 | cl 13.10 /O1 load-order source forms: list front via begin, PDB accessor, copy-init, result local | msvc,cl,o1,codegen,load-order,stl,list,copy-init,temporary,string,matching | 2026-10-29 | 3.4/T5 | T5.c2 near-lane drafts
C0150 | opt-unknown batch: one unit at a time; implicit __that members via emitters; member-pointer local | msvc,cl,o1,opt,units,control,emitter,copy-ctor,implicit,__that,provenance,pdb,matching | 2026-10-29 | 3.4/T8 | T8.c1 ten opt-unknown units
C0151 | Implicit-member cascade order; provenance NAME lookahead; shell alignment; /O1 bool return | msvc,cl,o1,emitter,copy-ctor,implicit,provenance,alignment,stl,codegen,matching | 2026-10-29 | 3.4/T8 | T8.c2 SignTrolley 16 rows
C0152 | Near-lane 129-256 B: ctor mem-init order, else-last, incomplete ret type; parallel match_one trap | near-lane,msvc,cl,o1,ctor,initializer,block-order,c2027,match_one,parallel,workdir,provenance,field,matching | 2026-10-29 | 3.4/T9 | T9.c1 12 near rows
C0153 | Near-lane drafts: callee-saved !FAKE local, intrinsic memset order, nested-call arg order, new-unit opt | near-lane,msvc,cl,o1,fake,register,memset,intrinsic,c4164,arg-order,opt,units,build,matching | 2026-10-29 | 3.4/T9 | T9.c2 12 near rows
C0154 | Fold-VA relink: one template arg per fold VA; decl-only explicit specialization | relink,comdat,folded,fold-va,template,specialization,resourcehandle,match_finish,gate,msvc,matching | 2026-10-30 | 3.4/T10 | T10.c2 SetEmptyTexture, HealthUI, ClearStateCache
C0155 | /O1 operand tells: const T& setter temp, inline operator== side; base-disp DIR32 delink wall | msvc,cl,o1,codegen,setter,const-ref,operator==,delink,dir32,reloc,objdiff,pin-only,matching | 2026-10-30 | 3.4/T12 | T12.c1 28 rows
C0156 | /O1 tells: accessor-as-arg, bool cmp, float <= vs >, eax by-value temp; byte_compare --obj; vtable overload order | msvc,cl,o1,codegen,accessor,bool,float,x87,inline,const-ref,byte_compare,vtable,overload,matching | 2026-10-30 | 3.4/T12 | T12.c2 23 rows
C0157 | x87 fild+faddp via (double) cast; RELEASE<T> folded copy via explicit-specialization decl; neutral locals | msvc,cl,o1,x87,fiadd,template,specialization,fold,provenance,locals,match_finish,wall,matching | 2026-10-30 | 3.4/T12 | T12.c3 8 rows
C0158 | A source-hash lock does not cover the bytecode Python runs (planted pyc) | python,pyc,pycache,verifier,lock,d76,gate | 2026-10-30 | 3.5/T3 | R3.5-008 F1, probed on 3.15
C0159 | Scratch git repos on Windows: rmtree(ignore_errors) leaves read-only objects | windows,git,rmtree,scratch,self-test | 2026-10-30 | 3.5/T4 | T4.c4 prov-fake-head
C0160 | Clone vs WC delink hash diff: stale WC cut after a pin commit, not nondeterminism | delink,determinism,repro,clone,stale,pins | 2026-10-30 | 3.5/T7 | T7.c1 SoundStreamer.obj diff
C0161 | A history scan on a CI host must refuse a shallow clone | git,audit,ci,history,self-test | 2026-10-30 | 3.6/T3 | audit_public --public-ci
C0162 | Testing git hooks from Python on Windows: bytes stdin, keep git on PATH | git,windows,hooks,crlf,scratch,control | 2026-10-30 | 3.6/T4 | T4 hook control false pass
C0163 | Reuse a verifier-locked tool's internals: repoint its scratch constant at import, never edit | verifier_lock,provenance_lint,scratch,tools | 2026-10-30 | 3.6/T5 | T5.c1 readability census
C0165 | Prove a source-wide comment edit is comment-only; git archive applies autocrlf | comments,verify,git,archive,autocrlf,crlf,baseline | 2026-10-30 | 3.6/T6 | T6.c1 unit_header
C0166 | Sweep comments across a tree: decisions as a frozen table, code-part proof, dry run on a copy | comments,sweep,verify,readability,provenance,batches | 2026-10-30 | 3.6/T7 | T7 expert, c1 batches
C0167 | Trial removals by rounds: one construct per file per gate; a green row is signal only if a matched body depends on it | fake,trial,gate,rounds,non_matching,readability,driver | 2026-10-30 | 3.6/T8 | -
C0168 | Cast removal by compile trial: obj same means no code from the line; rerun under /DNON_MATCHING | cast,trial,compile,non_matching,readability,msvc,census | 2026-10-30 | 3.6/T9 | T9 scratch trial
C0169 | Comment-only source edits the provenance lint still refuses | provenance,fake-diff,annotations,gate | 2026-10-31 | 3.6/T10.1 | T10.1 gate red on comments
C0170 | Python subprocess bash on Windows can start WSL, not Git bash | windows,bash,wsl,subprocess,scratch | 2026-10-31 | 3.6/T12 | T12.c1 controls
C0171 | A redacting wrapper over short config values also rewrites repo paths | d46,redact,audit,scratch | 2026-10-31 | 3.6/T12 | T12.c1 audit
C0172 | A private value repeated in an agent's reasoning blocks the next commit | audit,journal,private-values,thinking | 2026-10-31 | 3.6/T12 | partner, plan_edit commit refused
C0173 | Null audit values turn two gate self-test controls into false alarms | gate,audit,self-test,public,config | 2026-10-31 | 3.6/T14 | T14 public clone walk
gen3 | Generation 3 entries | see cookbook/gen3/INDEX.md
C0139 | Time gate layers from outside the lock with checkpoint marks | gate,timing,verifier_lock,monkeypatch,scaling | 2026-10-31 | 4.1/T1 | T1.c1 gate_timed
C0164 | Speed a per-line regex history scan without stored state: line memo, literal prefilter | audit,git,history,regex,timing,scratch | 2026-10-31 | 4.1/T1 | T1.c2 routes
C0174 | Reordering function bodies in a matched unit: what keeps the bytes | msvc,mangling,access,order,annotations,scratch,gate,matching | 2026-10-31 | 4.1/T2 | T2.c1 62 body moves
C0175 | Land a speed route in a locked scanner: pin its assumptions from code objects, re-prove on the landed file | audit,verifier,lock,regex,python,plant,crlf,control | 2026-11-01 | 4.1/T3.1 | T3.1.c1 audit_public route
C0176 | PROTOTYPE lines come from the proto map, never from an earlier plant | annotations,prototype,proto-map,verify,controls | 2026-11-01 | 4.1/T4 | T4.c1 wrong protoVA in T2 plant
C0177 | Scratch repo under the project root: repoint the harness tools, or they act on the real repo | git,hooks,scratch,control,commit_task,plan_edit | 2026-11-01 | 4.1/T5 | T5 control committed ddad41a3 in the real repo
C0178 | Concurrent JSONL append on Windows: open('a') tears rows; byte lock past EOF; race control staggers | windows,python,append,jsonl,ledger,lock,msvcrt,concurrency,match_one,parallel,workdir,control | 2026-11-01 | 4.1/T6 | T6 ledger stress
C0179 | A row leaving a batch needs its source guarded; wrong bytes also fail relink | match_finish,batch,gate,matched_set,non_matching,control | 2026-11-01 | 4.1/T7 | T7 clone controls
C0180 | Array loop: index form gives retail's strength-reduced induction variable under /O1 | msvc,o1,loop,induction,regalloc,matching | 2026-11-01 | 4.1/T8 | T8.c1 LowLevelRender
C0181 | Per-file pragma intrinsic for one row; opaque class alignment shifts its stack temp | msvc,o1,intrinsic,strlen,basic_string,alignment,stack,matching | 2026-11-01 | 4.1/T8 | T8.c2 NotifyMatchingRegions, Activate
C0182 | Tell a call from a jump at a rel site: the opcode byte before the field | rel,call,jmp,opcode,sites,census,candidates | 2026-11-01 | 4.1/T9 | T9.c1 family census
C0183 | Retail VA low nibble refutes /O2 /Ot: /Ot section align 16, /Os align 1 | msvc,opt,control,o2,os,ot,coff,section-align | 2026-11-01 | 4.1/T10 | T10 opt controls
C0184 | Before a public cut: refresh the name scan and the overlay figures a batch left stale | public,export,name_scan,readme,re-cut | 2026-11-01 | 4.1/T12 | public_walk rehearsal cut
