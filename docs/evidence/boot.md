# Boot loop evidence (phase 2.4)

Deploy, launch and readback of our relinked XBE on the developer's box, then restore. Labels only: `boot folder`
(the developer's test copy under `F:\Games`), `original install` (never a target). Values live in gitignored
`config/local_paths.json`. Sources: tasks/T4.md, T5.md, T5.1.md, T5.1.1.md, T6.md; R2.4-006; run.sh logs under
`.run/2.4/logs/` (gitignored; PASS/FAIL lines only, exit codes from the task logs).

## What the loop proves
The relinked XBE (`relink.py check`, `link/all=ours/relinked.xbe`) is byte-identical to retail, so the deployed file
cannot be told from retail by its bytes. The loop proves the write, launch and readback path, not a behaviour change:
(a) the boot folder's `default.xbe` after the write is the file sent (getfile sha1 == source sha1, change time later);
(b) the running title came from the boot folder (`xbeinfo running` name == boot folder `default.xbe`, TimeDate == pin);
(c) `GameOptions::BackupState` (VA 0x00125F88, 0x10 B) in the running image == our object's `body_sha1`.
A behaviour difference needs a non-identical build (outside gen 2).

## Loop, in order
1. Preflight at the dashboard (control, retail bytes in place): `PY tools/xbdm_deploy.py preflight` -> exit 0
   (t4-preflight-live, t4-preflight-live2, t51-preflight). Boot folder sha1 vs pin: equal.
   Preflight in-title (T4) -> exit 1: XBDM hides drive F while a title runs (C0082). Not a valid gate.
2. Deploy (T5.1): `PY tools/xbdm_deploy.py deploy <relink root>/link/all=ours/relinked.xbe` -> exit 0 (t51-deploy).
   Order: attributes before, `delete`, `sendfile`, attributes after, `getfile`, sha1 compare.
   - relink output sha1 vs retail pin: equal.
   - getfile readback sha1 vs relink output sha1: equal; size vs source: equal.
   - change time: after-write later than before-write.
3. Launch by hand from the dashboard (T5.1.1): the developer started the boot folder's `default.xbe`; reached
   in-game (developer's observation, not a tool check).
4. In-title checks (T5.1.1): `PY tools/xbdm_deploy.py status` -> exit 0 (t511-status; running title == boot folder
   `default.xbe`, TimeDate == pin); `PY tools/xbdm_probe.py --verify GameOptions::BackupState` -> exit 0 (t511-verify;
   box sha1 == file sha1 == `body_sha1`: equal; control VA 0x00125F98 box == file, != `body_sha1`);
   `--known-true` -> exit 0 (t4-probe-knowntrue).
5. Developer returned the box to the dashboard (T6 stop 1; T6 state preflight exit 0, drive F listed, t6-state-preflight).
6. Restore (T6): `PY tools/xbdm_deploy.py restore` -> exit 0 (t6-restore) = deploy of the retail pin:
   delete, sendfile, size == source, getfile sha1 vs source: equal, change time later than before, sha1 vs pin: equal.
7. Preflight after restore (T6): `PY tools/xbdm_deploy.py preflight` -> exit 0 (t6-preflight); sha1 vs pin: equal.
   The boot folder is back to retail bytes, written fresh by the restore.

## Why the launch is by hand
`magicboot` cannot launch the boot folder's `default.xbe` on this Cerbios box (T4, C0082):
- `F:\` form, from the dashboard: `magicboot title="F:\...\default.xbe" debug` -> 200, no reboot; the box stays on
  the dashboard (developer watched); running title unchanged after 180 s; `boot` exit 1 (t4-boot-live).
- device form (`\Device\Harddisk0\Partition6\...`): refused, non-200, code not captured; `boot` exit 1 (t4-boot-live2).
Developer decision (2026-10-27): launch by hand; the client writes and checks; automated launch deferred (INBOX, gen 3).
The `boot` verb and `magicboot` were removed from the client in T7 (D83); it has no launch verb.

## Wire seen live (C0083)
- `delete name="<path>"` -> 200.
- `sendfile name="<path>" length=0x<hex>` -> 204, raw payload, final 200 (checked; text not logged). The 0x-hex length
  form is accepted.
- `410` on sendfile onto an existing file: mock only, never seen live (deploy always deletes first).

## Repeat run (phase 2.5 T15, 2026-10-27)
Same loop on the 2.5 relinked build (45 substituted objects). Sources: tasks/T15.md, the T15 review
(discussions/review-20261027-145508.md), run.sh logs under `.run/2.5/logs/` (gitignored; recorded here at 2.5 T16).
1. `relink.py check` -> exit 0 (t15-relink-check): 16/16 section digests OK; relinked.xbe 2453504 B, sha1 vs pin: equal.
2. Preflight at the dashboard -> exit 0 (t15-preflight): boot folder sha1 vs pin: equal.
3. Deploy of `<relink root>/link/all=ours/relinked.xbe` -> exit 0 (t15-deploy): delete, sendfile 204/payload/final,
   size == source, getfile sha1 vs source: equal, change time later than before.
4. Launch by hand from the dashboard; in-game (developer's observation).
5. `status` -> exit 0 (t15-status): running title == boot folder `default.xbe`, TimeDate == pin.
   `xbdm_probe.py --verify GameOptions::BackupState` -> exit 0 (t15-probe): known-true magic and initFlags, build
   identity (TimeDate, titleId, version) == pinned file, 16 box sections == 16 XBE sections; VA 0x00125F88 box sha1 ==
   file sha1 == `body_sha1`: equal; control VA 0x00125F98 box == file, != `body_sha1`; dmversion 1.00.5849.1.
6. Restore -> exit 0 (t15-restore): deploy of the retail pin, then sha1 vs pin: equal.
7. Preflight after restore -> exit 0 (t15-preflight2): drive F listed, sha1 vs pin: equal. Developer accepted T15.

## Repeat run (phase 2.6)
Same loop, run from the clean clone (clone-2.6b) by setup.md step 9 and runtime.md only; the clone's `xbox_ip`, boot
folder and original install keys equal the working copy's (compared by label, values never printed). Every command
through the C0099 redacting wrapper; run.sh logs under `.run/2.6/logs/` (gitignored). validation.md M2.6-4.
1. `relink.py check` -> exit 0 (t8-relink-check): 16/16 section digests OK; relinked.xbe 2453504 B, sha1 vs pin: equal.
2. `xbdm_probe.py --identify` -> exit 0 (t8-identify): candidate 1 of 1 answered 201.
3. Preflight at the dashboard -> exit 0 (t8-preflight): drive F listed, boot folder sha1 vs pin: equal.
4. Deploy of `<relink root>/link/all=ours/relinked.xbe` -> exit 0 (t8-deploy): delete, sendfile, size == source, getfile sha1 == source sha1, change time later.
5. Launch by hand from the dashboard; in-game -> in-game (developer's observation, 2026-10-27).
6. `status`, `xbdm_probe.py --verify GameOptions::BackupState` -> exit 0 each (t8-status: running title == boot folder default.xbe, timestamp == pin; t8-verify: build identity == retail, BackupState box sha1 == file == body, control != body).
7. Back to the dashboard by hand; restore, preflight after -> exit 0 each (t8-restore: boot folder sha1 == pin; t8-preflight2: 5 PASS).

## Repeat run (phase 3.5)
Same loop by setup.md step 9 and runtime.md "Deploy" only, run from the clean clone `<game_dir>/clone-3.5` (HEAD
55b3e35a); the clone's `xbox_ip`, boot folder and original install keys equal the working copy's (labels only). Every
command through the C0099 redacting wrapper (`.run/3.5/t10/inclone.py`); run.sh logs under `.run/3.5/logs/` (gitignored).
Symbol chosen beforehand: `DoorStorageRoomMain::Load` (`?Load@DoorStorageRoomMain@@UAEXABVBeepTDF@@@Z`, VA 0x00079539,
0x73 B; matched in 3.4 T12, 1a5491e3); wrong address = row VA + 0x10 = 0x00079549.
1. `relink.py check` -> exit 0 (t10-relink-check): `check all: digests 16/16 OK`; relinked.xbe 2453504 B, sha1 vs pin: equal.
2. `xbdm_probe.py --identify` -> exit 0 (t10-identify): candidate 1 of 1 answered 201.
3. Rehearsals (loopback mock): `--verify DoorStorageRoomMain::Load --rehearse` -> exit 0 (t10-verify-rehearse: box ==
   file == body_sha1, control != body); `--va 0x00079549 --rehearse` -> exit 1 (t10-wrongva-rehearse: box == file !=
   body); `xbdm_deploy.py preflight|status|restore|deploy <relinked.xbe> --rehearse` -> exit 0 each (t10-*-rehearse).
4. Attended loop, 2026-10-30, the developer at the box, commands run live by Claude through the wrapper (exits as
   Claude reported them; each log's last line agrees, PASS = 0, FAIL = 1):
   a. Preflight at the dashboard -> exit 0 (t10-preflight): drive F listed, boot folder default.xbe exists, sha1 vs pin: equal, xbeinfo answers.
   b. Control: retail launched by hand to the title; `status` -> exit 0 (t10-control-status): running title == boot folder default.xbe, timestamp == pin.
   c. Dashboard by hand; deploy of `<relink root>/link/all=ours/relinked.xbe` -> exit 0 (t10-deploy): delete, sendfile, size == source, getfile sha1 == source sha1, change time later.
   d. Launch by hand from the dashboard -> in-game, controlling Vince, no crash (developer's observation, 2026-10-30).
   e. `status` -> exit 0 (t10-status): running title == boot folder default.xbe, timestamp == pin (relinked == retail bytes).
   f. `xbdm_probe.py --verify DoorStorageRoomMain::Load` -> exit 0 (t10-verify): build identity == retail, 16 == 16 sections, VA 0x00079539 len 0x73 box sha1 == file == body 9e839183..., control 0x000795AC != body, dmversion 1.00.5849.1.
   g. `--verify DoorStorageRoomMain::Load --va 0x00079549` -> exit 1 (t10-wrongva): box == file fb878e7b... != body 9e839183..., `FAIL verify`, as required.
   h. Dashboard by hand; `restore` -> exit 0 (t10-restore): readback sha1 == source, boot folder sha1 vs retail pin: equal.
   i. Preflight after -> exit 0 (t10-preflight2): 5 PASS. Box left as found; no failed step.
