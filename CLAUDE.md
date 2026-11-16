# CLAUDE.md

Personal fork of Keychron's QMK firmware (GitHub: `horwitz/qmk_firmware_keychron`, which is what `origin` points
to since 2026-11-10; the fork's old name, `qmk_firmware`, still redirects). **The fork is public on GitHub** (`gh repo view`
reports visibility PUBLIC, 2026-11-15; earlier versions of these notes wrongly called it private; what that means for these
notes is task 38). This branch, `x4`, is for the **Lemokey X4** keyboard: Mac and Windows layers, and a way to choose
between them. The owner's keymap name elsewhere in the fork is `horwitz`. Tags: **(observed)** = checked in this repo or by
running a command; **(general)** = ordinary QMK knowledge, not verified here; **(unverified)** = check before relying on it.

## Working rules

- The owner's global working agreements (loaded from `~/.claude/CLAUDE.md`) govern git/commits, task tracking, code
  style and writing. Task list: `TASKS.md` in the repo root (read it at session start). This branch has its own
  `TASKS.md`; the K3 Pro/Max list lives on `wireless_playground` and belongs to a different Claude Code instance.
- Never commit; the owner does. Firmware changes may be committed before they are flashed and smoke tested on the
  physical X4, but the commit message must say so (a line such as "UNTESTED ON HARDWARE"), and the next hardware test
  steps must be listed. Once they have been tested, say so in `TASKS.md` and in the next commit. (Before 2026-11-09 the
  rule, inherited from the K3 work, was to wait until after the flash test.)
- **Public-facing text** (everything except `CLAUDE.md` and `TASKS.md`: code comments, the keymap `README.md`, the
  workflow) must stand on its own: no task numbers, no pointers to `CLAUDE.md` or `TASKS.md`, no personal wording such as
  "the owner's Mac". Refer to the K3 or its branch only with the detail that makes the sentence stand alone: "Fn + Tab turns
  the backlight on and off, as on the K3" is fine, "works like the K3" is not (owner's rule, task 26, 2026-11-15). CI
  enforces the first part (task 30; the mechanics are under "Versions and CI"). The K3 rule is not machine-checkable and
  stays a review matter.
- Prefer the clean, documented fix over a workaround. Do not edit Keychron/Lemokey vendor code. Check claims against the
  repo or by running a command before stating them.
- Scope: Windows and Mac layers plus the selection method, and what was built around them (see "Mac/Windows design");
  open work is in `TASKS.md`. The K3's per-key RGB features (GCP, CCP) do not apply: the X4's LEDs are single color.

## Branches and upstream

- `x4` was created from `playground`, which on the fork now equals Keychron's `playground` (`4a8143cce9`, 2025-08-20).
  (observed) It has the **full QMK `keyboards/` tree** (over a thousand entries), unlike `wireless_playground`, which
  has only `keychron` and `lemokey`.
- The fork's old `playground` (the April 2023 line, `f9a4104574`) is preserved as `old_playground`, matching Keychron's
  `old_playground`. (observed) The X4 exists only on Keychron's current `playground`: no X4 in `wireless_playground`,
  `2025q3`, `master`, `old_playground`, `bluetooth_playground`, `hall_effect_playground`, `wired_wb` or `wls_2025q1`.
  (observed with `gh api` tree listings, 2026-11)
- The K3 work, including the shared userspace pattern (`users/horwitz/layers.h`, `keymaps.h`, `horwitz_keymap.c`), lives
  on the fork's `wireless_playground`, not here. Read files from it without switching branches:
  `git show origin/wireless_playground:users/horwitz/layers.h`. Do not merge it into `x4` without asking (e.g.,
  `TASKS.md` and `CLAUDE.md` would conflict).
- This clone has no `keychron` remote. `gh` can read Keychron's repo. To put a Keychron-only commit on the fork, fetch by
  URL and push from here (`git fetch git@github.com:Keychron/qmk_firmware.git <branch>`, then a `--force-with-lease`
  push pinned to the old SHA). `gh repo sync --force` and a `gh api` ref update both returned 404, because the fork's API
  could not see a commit that exists only in the parent. (observed, 2026-11)

## The Lemokey X4 (all observed from files on this branch)

- Path: `keyboards/keychron/x4/` (Keychron's name for the board; Lemokey is a sub-brand and the box says "Keychron X4").
  Build target: `keychron/x4/ansi_encoder/red`. Only the red-backlight ANSI variant is in the tree; the owner's board
  is red-backlight. An RGB `.bin` is on Lemokey's site, but no RGB source is in this tree.
- `info.json`: `Lemokey X4`, **STM32F401**, `stm32-dfu` bootloader, VID `0x362D`, PID `0x0240`, `LAYOUT_82_ansi` (75%, 82 keys,
  one rotary encoder on B14/B15). Wired only. **The MCU** (task 10, 2026-11-12, observed with `dfu-util -l`, nothing
  flashed): the kickoff prompt said F402RC, but the board's DFU bootloader reports 256 KB of flash (4x16 KB + 64 KB + 128 KB),
  the F401xC size, which matches the build's `GENERIC_STM32_F401XC` board. The bootloader does not name the part; "F402" is
  probably Keychron's label, since its C2 Pro readme says rev2 is "STM32F402" while that board's `info.json` also says
  `STM32F401`.
- Lighting: single-color LED matrix (CKLED2001, 81 LEDs), `LED_DISABLE_WHEN_USB_SUSPENDED` already set. Caps Lock LED index
  44, Win-lock LED index 72. The lighting already turns off when the computer sleeps (the K3's "SUS-RGB"): `quantum.c` calls
  `led_matrix_set_suspend_state()` from its suspend and wake hooks, and that macro makes it turn the LEDs off. Verified by the
  owner on the board (2026-11-14, task 11-1): the LEDs go off on sleep and come back on wake. No keymap code is needed.
- `x4.c`: `process_record_kb` calls `process_record_user` first, then handles `BL_TOGG` and `GU_TOGG` (Win lock,
  persisted in EEPROM user data). **There is no Mac/Windows dip switch and no wireless code.**
- Stock keymaps (`ansi_encoder/red/keymaps/`): `default` (Windows only; only `ENCODER_MAP_ENABLE`), `keychron`
  (VIA, Keychron common code, factory-test code), `via` (VIA only). All four layers are `WIN_BASE`, `WIN_FN`, `WIN_L2`,
  `WIN_L3`; **`WIN_L2` and `WIN_L3` are entirely transparent**, so they are free slots for `MAC_BASE`/`MAC_FN` without
  changing the layer count of 4. Whether the VIA JSON (`via_json/x4_ansi_encoder_red.json`) assumes 4 layers:
  unverified.
- The X4 uses `keyboards/keychron/common/`, not `keyboards/lemokey/common/` (which here holds only `common` and `p1`).
  `keychron_common.h` defines `KC_LOPTN`, `KC_LCMMD` etc. starting at `QK_KB_2`, and `keychron_common.c` handles them.
  **`NEW_SAFE_RANGE` does not exist on this branch** (the K3 userspace uses it); our custom keycodes use `SAFE_RANGE`,
  which nothing in the X4 tree uses. Only the `keychron` keymap calls into the Keychron handlers; `default` has no
  `process_record_user` at all.
- Mac modifiers: the Mac layer uses plain `KC_LALT` (Option) and `KC_LGUI`/`KC_RGUI` (Command) in the Mac positions, not
  Keychron's vendor keycodes. Tested on a real Mac (2026-11-10): Cmd+Space, Option+e then e (é), other Command shortcuts,
  the right Command, the Mac Fn row (brightness, Mission Control, Launchpad, backlight, media, volume), the encoder,
  persistence across replugging, and switching back to Windows mode all work, so plain QMK keycodes are enough on the X4.

## Mac/Windows design

Decided 2026-11-09. Everything below was built and tested on the board, on a Windows PC and a real Mac, between 2026-11-09
and 2026-11-15. The sizes and hashes of earlier images are in `TASKS.md` and git history. Since task 32 the firmware
contains the git commit it was built from (Fn + V types it), so its exact bytes and SHA-256 differ with every commit: identify
a build by that hash, and get a commit's exact image from the CI artifact, whose run summary shows its SHA-256. The last build
before the identity was added was 68,388 bytes, SHA-256 `1A2D93C3F90C94048C9676C61E32617F60E5A7D1F5A7A99566DC99E1E4CBCE6E`
(the CI artifact and the local QMK MSYS build were byte-identical); the identity adds about 0.7 KB, and a DEBUG build is
about 6 KB larger.

- **Four layers in stock order:** `WIN_BASE` 0, `WIN_FN` 1, `MAC_BASE` 2, `MAC_FN` 3 (taking the two empty `WIN_L2`/`WIN_L3`
  slots). Windows is layer 0, the boot layer of a fresh or erased board, and the stock Windows layers keep their indices.
  The K3 spells every layer out in full (only its two Fn layers are named `LAYOUT_*` macros in `users/horwitz/keymaps.h`;
  its Mac and Windows bases are duplicated rows in `keymaps[]`). The X4 does the same: all four layers are spelled out in
  full (decided 2026-11-09; the Mac layers differ from the Windows ones only in the bottom row and three Fn keys).
- **Selection:** two explicit Fn-layer keys that set the persistent default layer: **Fn + `\` for Windows, Fn + `/` for
  Mac** (the directory separators). Both are free on the stock Fn layer, are at the same positions on both Fn layers,
  and avoid Lemokey's Fn + J + Z factory-reset combo. They need a small custom keycode calling
  `set_single_persistent_default_layer()` (documented in `docs/feature_layers.md`); a persistent `PDF` keycode was not
  found on this base. The keycodes are `MODE_WIN`/`MODE_MAC` (`ctrlkeycodes.h`, from `SAFE_RANGE`, which nothing in the X4
  tree uses), handled in `horwitz.c`; the saved layer is restored at every boot by `magic()` in
  `quantum/bootmagic/magic.c`, so the mode survives unplugging (tested). Auto-detect on top of this is described below.
- **Blink (task 16):** pressing a mode key pulses its LED dark three times (`BLINK_PULSES`), the same for both modes: 120 ms
  steps (`BLINK_STEP_MS`) lit, dark, lit, dark, lit, dark, lit, so it starts and ends lit and every pulse is a visible
  change from either starting state (the first version, once for Windows and twice for Mac, started dark, which showed
  nothing when switching to the mode whose key was already dark; the owner also saw no need for two behaviors, since the
  mode display already shows the chosen mode). It is drawn after the mode display and overrides it, so it runs to the end
  even if Fn is released, in any backlight state (the dark step shows against a lit backlight, the lit step against a dark
  one). Pressing a mode key again, even the active one, restarts it. Its clock starts at the first frame actually drawn
  (`blink_clock_started`, see Sleep and wake). Code: `start_mode_blink` and `apply_blink` in `horwitz.c`. Tested on the X4
  (owner, 2026-11-12): noticeable with the backlight on; 3 pulses at 120 ms judged right (fewer felt wrong, more was expected
  to); also passed with the backlight off, with Fn released mid-blink and on a repeat press of the active mode; the Win-lock
  release on entering Mac mode still works (checked by pressing the Win key afterward, not by the LED, which is hard to read
  with a bright backlight).
- **Auto-detect: design (task 15):** `OS_DETECTION_ENABLE` guesses the host's OS from its USB setup requests. Each
  connection (the USB state going to INIT: a boot, a bus reset) clears QMK's detection counters (nothing in this base ever
  does) and arms a one-shot; `housekeeping_task_user` polls `detected_host_os()` (this base has no callback) and, for
  Windows, macOS or iOS, applies that mode with `default_layer_set()` **without saving it**, so the EEPROM keeps the owner's
  last manual choice, which is also the fallback when detection says unsure or Linux. A mode key pressed meanwhile or later
  wins until the next power cycle or connection to a different kind of host (see Sleep and wake). Each connection ends with
  a **settle**: when detection says Windows, macOS or iOS (that mode is applied), or after `AUTODETECT_TIMEOUT_MS` (2000 ms)
  if it says nothing useful (the saved mode stays), the active mode's key blinks, so every connection shows the mode it
  ended in. `OS_IOS` is treated as Mac, because QMK reports the owner's Mac as iOS (next bullet).
- **Auto-detect: what the hosts do (observed):** on the **Windows PC**, Windows is recognized 133-205 ms after the
  connection (18 requests, 14 of length 0xFF and 2 of length 4; the USB state goes INIT, SUSPEND, INIT, INIT, CONFIGURED, so
  the counters are cleared several times, which is correct), and a saved Mac mode (EEPROM default layer `4`) is left alone.
  **QMK reports a Mac as iOS, not macOS** (observed 2026-11-13 on the owner's Mac: the setup requests have lengths 2, 0x16, 2,
  0x12, 0x101, 0xFF, 0xFF, 0xFF; QMK's iOS rule, exactly 4 requests with two of length 2 and none of 0xFF, matches after the
  first four, its macOS rule wants exactly 5 requests ending in 0xFF and never matches these 8, and QMK never revises a
  guess within a connection), so `OS_IOS` is treated as Mac: every Apple host wants the Mac layout. Windows sends no
  requests of length 2, so the iOS rule cannot fire for it. A **power-only phone charger** (which never enumerates): the
  saved mode's key blinks after about 2 s, with Windows and with Mac saved. Verified (2026-11-13 and 14): on the Windows PC
  the blink comes soon after plug-in; on the Mac, with Windows saved, plug-in gives Mac mode with the blink, the Win lock is
  released, and replugging and a manual Fn + `\` work; with Mac saved, the Windows PC still ends in Windows mode.
- **Sleep and wake (task 28, 2026-11-15):** a manual mode choice survives a computer's sleep and wake. What was found: on
  the **Windows PC** a wake resets the bus (the USB state goes to INIT, which re-arms detection) but Windows does not repeat
  its descriptor requests, so detection answers nothing; after the 2 s timeout the live mode is kept and its key blinks, so
  the choice already survived. On the **Mac** a wake can reset the bus several times (five resets in 1.2 s were seen) and
  the Mac then repeats the whole enumeration, so detection answers again (`os 4`); before the fix that re-applied Mac mode
  and undid a manual Windows choice. A wake cannot be told from a host change by the USB states (a KVM or USB switch also
  looks like suspend, then reset), so the fix compares what the host *is*: `last_detected_layer` remembers the layer
  detection last decided, a re-detection that decides the **same** layer leaves the live mode alone, and one that decides a
  **different** layer applies it. A power cycle remembers nothing, which is what ends a manual choice ("until the next
  connection"). The blink after a same-host wake shows the live mode (`live_mode()`), not the detected one. Also found: when
  the settle step runs after a bus-reset wake the LED matrix is still held off (it comes back when the USB `CONFIGURED`
  event is processed, about 13-21 ms later, `usb_main.c`), so the blink's clock now starts at the first frame actually drawn
  (`blink_clock_started`); at 21 ms that is a robustness measure, and it was NOT shown to be the cause of anything. **Not
  proven:** some Mac wakes showed no blink; the working theory is a wake that only resumes the bus without a reset, which
  runs no settle step and so no blink, by design, but no trace of such a wake was captured. Tested (owner): a manual
  Windows mode on the Mac and a manual Mac mode on the Windows PC both survive a sleep and wake, and a fresh plug-in follows
  the host.
- **Diagnosing host behavior (two methods that worked; both were temporary and removed afterward):** (1) A **typed-out
  diagnostic**: 3 s after a connection the keyboard types the counters, the detected OS and the live and saved layers into
  the focused text editor, with `send_string` of an `snprintf` of `detected_host_os()`, a local copy of `struct
  setups_data_t` from `quantum/os_detection.c` and its `setups_data`, `default_layer_state`, `layer_state` and
  `eeconfig_read_default_layer()`; with `OS_DETECTION_DEBUG_ENABLE = yes` in `rules.mk`, `extern uint16_t usb_setups[]` gives
  every request length in order. (2) A **DEBUG-only USB trace** that recorded each `notify_usb_device_state_change_user`
  state with `timer_read32()`, the moment the settle step ran, `detected_host_os()`, whether it applied, and
  `led_matrix_get_suspend_state()`, printed with `uprintf` four seconds after the last event so the console reader (which
  reconnects about a second late) did not miss it. Timestamps that start near 0 mean the keyboard rebooted (power was cut),
  large ones mean it did not.
- **Debug output (task 11-2):** a build made with `-e DEBUG=1` has the console on (`CONSOLE_ENABLE`) and `DEBUG` defined;
  `horwitz.c` then turns `debug_enable` on at boot and prints with `uprintf` at every mode key press (`mode key: base layer
  N`, 0 = Windows, 2 = Mac) and every auto-detect settle (the detected OS, the layer, whether it was applied, and how many ms
  after the connection started). A normal build is byte-identical to one without any of this. `DEBUG` is deliberately not
  given a default in `rules.mk`: the build system also uses it as the compiler's debug-info level (`-g$(DEBUG)`,
  `builddefs/common_rules.mk`). Build and flash with `qmk compile`/`qmk flash -kb keychron/x4/ansi_encoder/red -km horwitz -e
  DEBUG=1`, and read the output with `qmk console -t` (run through the MSYS command above, with `PYTHONUNBUFFERED=1` if its
  output goes to a file, or Python holds it back); `qmk console --list` shows the board as `362D:0240:1`. **QMK Toolbox works
  too, but on Windows it shows console output in a separate "HID Console" window** (open it from Toolbox's menus), not in
  the main log pane, so look there (verified with 0.3.3 and again with 0.3.4 on 2026-11-15: with a DEBUG build on the board,
  its HID Console window lists the X4 and shows the `mode key:` lines; with a normal build the window shows nothing, because
  a normal build has no console interface). The `settled:` line is usually missed, because it is printed about 0.2 s after a
  connection and the console tool reconnects about 1 s after the board appears; the mode key lines are reliable. Flash the
  normal build back afterward: the DEBUG image stays on the board until you do. **Decided (owner, 2026-11-15): DEBUG stays
  compile-time; no keyboard toggle.** QMK has a keycode for it (`DB_TOGG`, `quantum.c`, which flips `debug_enable`), but it
  can only switch *printing*: the console is a USB interface that exists only when compiled in, so a runtime toggle would
  put the console, about 6 KB and an extra interface, into every build. That would end the guarantee that a normal build is
  byte-identical with or without the DEBUG code, would need the auto-detect results rechecked on the Mac and the Windows PC
  (the extra interface might change what the host asks during enumeration, which is what detection reads; it was tested only
  without it), and would need the `uprintf` calls changed to `dprintf` so they respect `debug_enable`. **Revisit if**
  debugging a host-specific problem (as with the Mac's iOS result, which needed a diagnostic build flashed from the Windows
  PC) becomes frequent.
- **Build identity (task 32):** Fn + V (`SHOW_VER`, free on both Fn layers) types `horwitz x4 ` and the git commit the
  firmware was built from, with `SEND_STRING("horwitz x4 " QMK_GIT_HASH)`; a `*` after the hash means the build had
  uncommitted changes, and a build made by CI from a commit has none. The build already writes `version.h` for every build
  (`qmk generate-version-h`, from `builddefs/build_keyboard.mk`) with `QMK_VERSION` (a `git describe`: the nearest tag, here
  QMK's `0.14.29`, plus the distance and hash, not meaningful for this project until we tag our own releases, task 40),
  `QMK_BUILDDATE` (a timestamp) and `QMK_GIT_HASH`; before this key nothing referenced them, so the firmware contained none.
  **The build date is deliberately not used:** it would make every build differ (verified 2026-11-15: two builds of the same
  source, with different build dates, are byte-identical, and the date string is not in the image). **The cost:** the image
  now changes with every commit, because the hash is inside it, so "a comments-only commit gives a byte-identical binary"
  can no longer be checked by comparing two plain builds. To compare builds of different commits, or to prove that a change
  altered no code, build both with `-e SKIP_VERSION=yes` (QMK's switch: `version.h` then holds constant placeholders, `NA` and
  a 1970 date; verified: the image is 69,108 bytes against 69,120 and the identity reads `horwitz x4 NA`). A committed build
  in CI and a local build of the same commit from a clean tree are byte-identical only if the hash has the same length: git
  (`git rev-parse --short`, what the generator uses) picks the shortest unique prefix, which depends on the clone (observed
  2026-11-15: 8 characters in CI's shallow clone, 10 in the full local one), so the workflow sets `git config --global
  core.abbrev 10`, which is what the full local clone gives today. A local setting is not needed: git's automatic length
  grows by about one character per fourfold growth in the number of objects, so the local length is not expected to change,
  and the length never matters for what Fn + V does, only for this byte comparison. Verified on 2026-11-15, commit
  `ba59dbbb97`: the CI artifact (69,116 bytes, SHA-256 `0905346D0FBFD746D7E4BF64024FE514EF2EE51F38573B7677F514877584084F`)
  holds `horwitz x4 ba59dbbb97` with no `*` and is byte-identical to a local QMK MSYS build of that clean commit. Typed text uses US-layout key codes,
  so on another input source the `*` or `-` could come out differently (hex digits are safe). Tested on the X4 (owner,
  2026-11-15): Fn + V in a text field on the Windows PC and on the Mac, the key lit under Fn, and nothing else changed; the
  results were not itemized.
- **Navigation keys (task 24, 2026-11-15):** the X4 has no End key (observed: the right-hand column is the encoder, Home,
  PgUp and PgDn, and `KC_END` was on no layer). On **both** Fn layers, with plain keycodes (the owner's decision: plain End on
  the Mac layer too, not Cmd+Right): Fn + Left = Home, Fn + Up = PgUp, Fn + Down = PgDn, Fn + Right = End, and Fn + Del = End
  as a second way. The arrows duplicate Home, PgUp and PgDn, which keep their own keys. Shifted combos work (Fn + Shift +
  Right selects to the end of the line). On a Mac, plain End scrolls to the end of a document and does not move the text
  cursor. The FN-HI display lights these five keys on its own, because it reads the keymap. When editing the Fn layers, keep
  `README.md`'s table in step.
- **LED feedback: FN-HI and the mode display (task 11-3, 2026-11-10 to 2026-11-15):** while a Fn layer is active (Fn held),
  every key that does something on that layer is drawn steady at full brightness and every other key is dark: the K3's
  "FN-HI" (highlight the used Fn keys) in an on/off variant for single-color LEDs, working with the backlight on, dimmed or
  off. That is 26 keys in Windows mode and 25 in Mac mode: F1-F12, Tab, Q, W, S, N, V, `\` and `/`, the four arrows and Del
  (navigation, task 24), plus Left Win on Windows (the Win-lock key, transparent on the Mac Fn layer). Which keys are "used"
  is **read from the keymap**, not hard-coded: `find_used_keys` (`horwitz.c`) scans both Fn layers once, at first use, taking
  each key's LED from `g_led_config.matrix_co` and its keycode from `keymap_key_to_keycode()`, and counts anything but
  `KC_TRNS` and `KC_NO`, so a key added to an Fn layer lights up with no further code. The two mode keys are both used, so
  both are lit; the **active** mode's key **breathes** (a triangle wave between `FNHI_BREATHE_MIN`, 64, about 25%, and full,
  over `FNHI_BREATHE_PERIOD_MS`, 2000) and the inactive one stays steady, so the key that moves is the current mode. Both
  constants are for tuning. The three-pulse confirmation blink (see Blink) is drawn after the display and overrides it, so
  after a mode key press the breathing moves to the new key only once the blink, about 0.85 s, is over. The first version
  drove only the two mode keys (active lit, inactive dark), which was easy to misread: with the backlight on, the inactive
  mode's key was the one *dark* key and the active one looked like all the others. Tested on the X4 (owner, 2026-11-15):
  backlight on and off, Mac mode, the Caps Lock and Win-lock LEDs, both blinks, and switching modes while holding Fn.
  Facts observed on the board: indicator LEDs are drawn even when the backlight is off, and one key's LED can be driven
  above the background (the Win-lock LED looks brighter when the backlight is dimmed), **and one key's LED can be forced dark
  while its neighbors stay lit** (tested 2026-11-10). The Caps Lock key shows purple with the backlight on and blue with it
  off, which is probably a separate blue indicator LED (`indicators.caps_lock` on C9) plus red light bleeding from
  neighbors. The LEDs of the mode keys are 42 for `\` and 68 for `/`. Pitfall found on the board: with the backlight off
  (`x4.c`'s `BL_TOGG` sets the flags to none and clears every LED once), nothing repaints the LEDs, so a value set while Fn
  is held stays lit after Fn is released. The code therefore clears every LED used on an Fn layer when Fn is not held and
  `!led_matrix_get_flags()`, as `x4.c` does for the Win-lock LED; this assumes that "flags == none" means the backlight is
  off. `x4.c` draws its own indicators (Caps Lock, the Win lock) after this code runs, so they win for their own LEDs.
- **Win lock:** `GU_TOGG` (Fn + Left Win) stays on the Windows Fn layer only. That position is transparent on the Mac Fn
  layer, because the lock is one global flag (`keymap_config.no_gui`, saved in EEPROM) that would otherwise stay on after
  switching modes and leave Command dead. `horwitz.c` therefore releases it when Mac mode is selected: it reads
  `x4.c`'s saved lock byte (`eeconfig_read_user_datablock`), has `process_record_kb` handle a synthesized `GU_TOGG` press
  if that byte is set (so `x4.c` clears its private state and LED), and clears `no_gui` separately. Tested on the board.
- The Fn layer you are holding stays active after you press a mode key, until you release Fn (normal QMK layer
  behavior); re-press Fn to get the new mode's Fn layer.

## VIA/Launcher: decided not to support (owner, 2026-11-12, task 17; do not repeat this research)

**Decision:** the `horwitz` keymap does not enable VIA. The keymap in git stays the only source of truth, and `qmk flash`
is the way to change the board. Nothing is lost for the Mac/Windows work: it does not need VIA.

Findings behind it (observed in this tree, 2026-11-12):

- *What "yes" would take.* `VIA_ENABLE = yes` in the keymap's `rules.mk` (the stock `via` keymap does this); the layer
  count already matches (`DYNAMIC_KEYMAP_LAYER_COUNT` defaults to 4, `quantum/action_layer.h:27`). `MODE_WIN` and
  `MODE_MAC` sit at `SAFE_RANGE` (= `QK_USER_0`); VIA's custom keycodes start at `QK_KB_0`, and Keychron's JSON already
  uses `QK_KB_0` to `QK_KB_10` for its 11 custom keys, so ours would move to `QK_KB_11` and `QK_KB_12`. It would also need a
  VIA JSON of our own, because the stock one does not know our keys (whether Lemokey's Launcher loads a sideloaded JSON is
  unverified). The mode-key LED lookup reads the live keymap and the saved default layer is a separate EEPROM area, so
  the existing code should keep working.
- *The cost that decided it.* `via_init()` (`quantum/via.c:93-112`) rewrites the EEPROM keymap from the compiled one only
  when the VIA EEPROM magic bytes are invalid. A normal reflash leaves them valid, so edits to `keymap.c` would silently
  stop taking effect on the board, and the git keymap would no longer be what is on the board after a Launcher edit. The
  reset path (Fn + J + Z, or a VIA EEPROM clear) runs `x4.c`'s `eeconfig_init_kb`, which also clears the saved Win-lock
  byte. It would also add a second file (the JSON) to keep in sync with `keymap.c`.
- *What it would buy:* remapping keys in a browser without reflashing; not wanted for now.

**Revisit if:** live remapping is wanted, or someone else will use the keyboard without `qmk flash`. Then add an
implementation task (rules, the keycode move to `QK_KB_11`/`QK_KB_12`, the JSON, an EEPROM-reset test on the board).

## File layout of the X4 keymap

`keyboards/keychron/x4/ansi_encoder/red/keymaps/horwitz/` is self-contained: there is no `users/` directory, and file and
macro names match the K3's where the roles match. Build it with `-km horwitz`.

- `keymap.c`: `keymaps[]` (the base layers spelled out; the Fn layers are the macros below) and `encoder_map`.
- `keymaps.h`: `LAYOUT_winfn` and `LAYOUT_macfn`, and the `KC_TASK`/`KC_FLXP` shortcuts.
- `layers.h`: the layer enum (`WIN_BASE`, `WIN_FN`, `MAC_BASE`, `MAC_FN`, in stock order).
- `ctrlkeycodes.h`: `MODE_WIN`, `MODE_MAC` and `SHOW_VER`.
- `horwitz.c`: the behavior. Mode selection and the Win-lock release (`process_record_user`, `select_mode`, `set_mode`);
  the version key (`SHOW_VER` in `process_record_user`); auto-detect (`notify_usb_device_state_change_user`, `housekeeping_task_user`, `last_detected_layer`, `live_mode`); the Fn
  display (`find_used_keys`, `breathe_level`, `led_matrix_indicators_user`); the blink (`start_mode_blink`, `apply_blink`);
  and the prints that exist only in a DEBUG build.
- `README.md`: the user-facing description: the modes, how the mode is chosen, the Fn layer's keys, and how to build, flash
  and debug. **Keep it in step with `keymaps.h` and `horwitz.c`** when the Fn layer or the behavior changes.
- `rules.mk`: `ENCODER_MAP_ENABLE = yes`, `OS_DETECTION_ENABLE = yes`, `SRC += horwitz.c`, and the `DEBUG=1` switch (see
  "Debug output").

License headers (decided with the owner and done in task 18, 2026-11-15): everything is GPL-2.0-or-later, like the tree
(Keychron's files say "version 2 or, at your option, any later version", and the firmware links their code, so the built
firmware is a GPL work whatever our files say). `keymap.c` and `keymaps.h` are **derived from Keychron's stock `default`
keymap** (the Windows layers and the Windows Fn layer unchanged or edited, the shortcut definitions copied), so they carry
**Keychron's notice verbatim** (copied from the stock file, including its "Copyright 202" typo, which is not ours to alter)
followed by `// Modifications Copyright 2026 Jeremy Horwitz`. The original files, `horwitz.c`, `layers.h` and
`ctrlkeycodes.h`, carry the short form used by much of this tree (137 files): `// Copyright 2026 Jeremy Horwitz` and `//
SPDX-License-Identifier: GPL-2.0-or-later`. `rules.mk` and `README.md` carry none (lint reads only code files, and
`rules.mk` is one stock line plus ours). An earlier commit had removed the Keychron block from `keymap.c` on the grounds that
the file was "not purely Keychron's"; that argues for adding our notice next to theirs, not for dropping theirs. **How `qmk
lint` tests this:** it only checks that a code file's first line starts with `/*` or `//` (`lib/python/qmk/cli/lint.py`,
`_has_license`), so it cannot tell a license from any other comment, and a new file must open with its header. QMK's own
`lint.yml` runs only on pull requests that touch `keyboards/**`; our workflow runs it on every push (see "Versions and
CI"). Verified: `qmk lint -kb keychron/x4/ansi_encoder/red -km horwitz` passes, and the firmware was byte-identical before
and after the headers were added.

Naming hazard: the keymap is named `horwitz`, and `USER_NAME` defaults to the keymap name, so QMK would also include
`users/horwitz/rules.mk` and `config.h` if that directory existed (`builddefs/build_keyboard.mk`). It does not on this
branch. If the K3 branch were ever merged in, rename this keymap or set `USER_NAME` in its `rules.mk`.

## Environment: Windows + QMK MSYS

- `qmk`, `make`, `python` and `arm-none-eabi-gcc` are not on the PATH of PowerShell or Git Bash. They live in
  `C:\QMK_MSYS` (QMK MSYS 1.12.0 since 2026-11-09: qmk 1.2.0, GCC 15.2.0 in `/opt/qmk/bin`, make 4.4.1, Python 3.12.12,
  `dfu-util` 0.11-dev; observed). `qmk.exe` refuses to run outside the MINGW64 shell. Run it from PowerShell like this
  (observed to work; `-km default` builds the stock keymap instead):

  ```powershell
  $env:MSYSTEM='MINGW64'; $env:CHERE_INVOKING='1'
  & 'C:\QMK_MSYS\usr\bin\bash.exe' -lc 'cd /c/Users/jahor/Documents/GitHub/qmk_firmware && qmk compile -kb keychron/x4/ansi_encoder/red -km horwitz'
  ```

- Docker Desktop (4.99.0) is installed too. It is used only to reproduce CI locally (see "Versions and CI"), not for daily
  builds (see "Why QMK MSYS").
- **Baseline and the old MSYS** (observed): building the unchanged stock `default` keymap (`qmk compile -kb
  keychron/x4/ansi_encoder/red -km default`) gives no compiler warnings on either toolchain. With GCC 10.1 (the old QMK MSYS
  1.7.2, qmk 1.1.1; source commit `4a36d7efd4`, from a clean `.build`) it gave a 68,208-byte `.bin`, SHA-256
  `4ea835af9f74df71033806ce6525761c7c0d573c39b2357d834123102362fa88`, with 35,902 bytes of `text` + `data`: the same size as
  the image flashed to the X4 and tested on 2026-11-08 (the flashed file itself was not kept, so byte identity with it is not
  confirmed). With GCC 15.2 it gave a 67,244-byte one (34,940 bytes of `text` + `data`). The `.bin` is copied to the repo
  root, where it is gitignored along with `.build/`. QMK's "Size after" line prints `text` + `data` from the ELF; the `.bin`
  is larger because the image is contiguous from `0x08000000` and `.text` starts at `0x08008000`, leaving about 32 KB of
  padding after the vector table. It fits the 256 KB flash. Why Keychron's layout leaves that gap: unverified. The build
  banner says `QMK Firmware 0.14.29`; how that string is derived in this fork is unverified. The old MSYS 1.7.2 was moved to
  the Windows Recycle Bin on 2026-11-12 (task 22); it stays recoverable until the bin is emptied (Restore puts it back at
  `C:\QMK_MSYS_1.7.2`; it ran standalone). The 1.12.0 installer was run by the owner with all optional boxes unchecked (no
  USB drivers installed). It is unsigned and has no GitHub attestation, but its SHA-256 matched the release checksum and the
  assets were uploaded by `github-actions[bot]`.
- With qmk 1.2.0 and Python 3.12, this repo's old CLI code prints harmless Python warnings. The decision (owner,
  2026-11-11) is to leave them visible and filter nothing; the full findings are in "QMK CLI warnings" below. A separate,
  unrelated old QMK checkout exists at `C:\Users\jahor\qmk_firmware` (2023, a `moonlander-j` fork); `qmk` falls back to
  it outside a repo, so always run `qmk` from this repo. (observed)
- Flashing works on this machine (observed 2026-11-08: the baseline `default` build on the physical X4, using
  `qmk flash -kb keychron/x4/ansi_encoder/red -km default` through the MSYS command above, with the board already in
  bootloader mode; it ended at "Transitioning to dfuMANIFEST state" and exited normally, with no hang). After flashing,
  all keys, the encoder, the Fn layer (including the Win lock), the backlight and the Caps Lock LED worked, and Esc-hold
  re-entered the bootloader. The same checks passed again on 2026-11-09 with the GCC 15.2 build, flashed by the upgraded
  MSYS (its first `dfu-util` status was `dfuERROR … firmware is corrupt`, which `dfu-util` cleared before the flash
  completed).
- **Bootloader entry:** the board's `readme.md` says hold K00 (Esc) while plugging in, or press the RESET button under the
  spacebar. Lemokey's pages (read through a summarizer; verify) say hold Esc while plugging in, or remove the spacebar keycap
  and hold RESET (left of the spacebar switch). Holding Esc while plugging in worked (observed); the RESET-button route is
  untried. No driver install was needed: Windows shows `STM32  BOOTLOADER` (`0483:DF11`) and `dfu-util -l` lists the board.
- **Recovery (task 29, 2026-11-15).** Factory reset of settings is Fn + J + Z for 4 seconds (backlight flashes red; from
  Lemokey's pages, not tried here). **A copy of the official red-backlight firmware is kept outside the repository, never
  flashed:** `C:\Users\jahor\Documents\lemokey_x4_recovery\x4_ansi_encoder_red_v1.0.2_2404191652\x4_ansi_encoder_red_v1.0.2_2404191652.bin`
  (its own new directory; treat it as untrusted data), **74,348 bytes, SHA-256
  `BD321181EFCDD147AEA8F0AA55AB82CF79A9D0EABBE6C00CA0A66D7CAB2B80E5`**, downloaded on 2026-11-15 from the link on
  `https://www.lemokey.com/pages/firmware-and-json-files-of-the-lemokey-qmk-keyboards` (the CDN URL
  `https://cdn.shopify.com/s/files/1/0059/0630/1017/files/x4_ansi_encoder_red_v1.0.2_2404191652.bin?v=1716533868`). That
  page also lists an older red `.zip` (v1.00, 2023) and the RGB image, which is for another variant. **The page publishes no
  checksum, so the hash above is of what was downloaded and was not compared with a vendor value.** Read-only checks
  (Python in isolated mode): it fits the 256 KB flash, its initial stack pointer (`0x20000400`) and reset vector
  (`0x0800811D`) and the words at offset `0x8000` are identical to our own build's (the same Keychron startup code, so it
  flashes at the same address, `0x08000000`, as `qmk flash` does), and its USB strings include "Keychron" and "Lemokey X4".
  That is consistent with a genuine image but is not proof of authenticity. To restore it, put the board in bootloader mode
  and flash that file with the web Launcher (`launcher.lemokey.com`, cable mode), QMK Toolbox or `dfu-util` at `0x08000000`
  (not tried; our own images were flashed with `qmk flash`, which uses `dfu-util` at that address). The Launcher's firmware
  update works at bootloader level, so it can restore the official image even though the stock `default` build has no VIA
  and the Launcher cannot edit its keymap (general). Expect to need the factory reset afterward, because our firmware keeps
  its mode and Win-lock state in the keyboard's EEPROM.

## Versions and CI

Audit of 2026-11-10 (task 19), refreshed on 2026-11-15 (task 33). Policy: pin known-good versions and upgrade deliberately
(rebuild, retest on the board, record it), as in the 2026-11-09 toolchain upgrade; never rely on a floating "latest" in CI.
The QMK base of the tree stays on Keychron's fork, because the X4 exists only there and moving to upstream QMK (0.34.7, from
the 2026-11-10 audit; not re-checked on 2026-11-15, because upstream publishes no GitHub releases) would mean porting vendor
code. **Result of the 2026-11-15 refresh: nothing needs upgrading.** Every row is at the latest version except
`qmk_flashutils` (deliberately left, see its row) and `git` inside QMK MSYS (bundled, see task 23); the only watch item is
the MSYS beta's switch to `ucrt64`. Checked with: `gh release list`, PyPI, Docker Hub's tag list, `winget show`,
`git --version`, `gh --version` and the installed `qmk`, GCC and `dfu-util` in QMK MSYS.

| Component | In use | Latest at the audit (2026-11-15) |
|---|---|---|
| QMK MSYS | 1.12.0 | 1.12.0 (stable, 2025-12-06). A beta dated 2026-11-11 exists, not used; its notes say it swaps `mingw64` for `ucrt64` and ConEmu for Microsoft Terminal, so when it becomes stable the launch command in this file (`MSYSTEM='MINGW64'`) will need changing: try it on a copy first, as in task 23 |
| `qmk` CLI | 1.2.0 | 1.2.0 (PyPI, 2025-11-27) |
| `arm-none-eabi-gcc` | 15.2.0 (`qmk_toolchains` v15.2.0-1) | v15.2.0-1 |
| `qmk_flashutils` (`dfu-util`) | 20251130, bundled in MSYS (in `/opt/qmk`, outside `pacman`) | 20260711 (still the latest on 2026-11-15). Left alone: flashing works, and the 14 commits since (no release notes) are mostly packaging for other platforms and tools (`hidapi`, `picotool`, Linux/macOS), dependency bumps and CI; nothing names `dfu-util` or Windows STM32 flashing. Update only for a named fix or a flashing problem, and try it on a copy first (task 23) |
| Git for Windows | 2.56.1.windows.1 (updated by the owner since the last audit) | 2.56.1.windows.1 |
| `git` inside QMK MSYS | 2.52.0 (package `git 2.52.0-2`) | MSYS has `pacman` (6.1.0), but a full `pacman -Syu` breaks QMK MSYS (task 23, below); a git-only update is untested; left alone |
| `gh` | 2.104.0 (updated since the last audit) | 2.104.0 |
| Docker Desktop | 4.99.0 | 4.99.0 (as `winget` reports it) |
| `actions/checkout` | v7.0.1 | v7.0.1 |
| `actions/upload-artifact` | v7.0.2 (pinned 2026-11-15, task 31) | v7.0.2 (released 2026-10-07) |
| QMK Toolbox | 0.3.4 (updated by the owner, 2026-11-15) | 0.3.4 (2026-08-19; a beta from 2026-08-20 exists, not used) |
| CI container | `qmkfm/qmk_cli:1.2.0` (`sha256:b7d7fa8f…`) | the same image as `latest` (Docker Hub shows both rebuilt on 2026-03-28 with that same digest) |
| Keychron `playground` | `4a8143cce9` | `4a8143cce9` |

CI is `.github/workflows/horwitz_x4.yml`. It runs on every push to `x4`, on pull requests and on demand, in the pinned
container, and takes about 1 minute 45 seconds in total (observed 2026-11-11).

- **Steps, in order:** disable git's `safe.directory` check; check out (with submodules); check the public-facing files for
  internal identifiers (task 30, below); install dependencies; build `keychron/x4/ansi_encoder/red:horwitz`; record the
  firmware's size and SHA-256 (job log and the run's summary page); upload it as the artifact `horwitz-x4-firmware` (task 31:
  `keychron_x4_ansi_encoder_red_horwitz.bin`, kept 30 days; the repository is public, so anyone signed in to GitHub can
  download it, which is consistent with the GPL because the source for it is this repository at that commit); `qmk lint` on
  the keymap (task 18).
- **The public-text check (task 30):** a `grep -rniE` over the keymap folder and the workflow itself that fails the run on a
  match: `task [0-9]`, `CLAUDE.md`, `TASKS.md`, the whole word `owner`, `wireless_playground`, `users/horwitz`, case-
  insensitively. Each pattern has one letter in square brackets (e.g., `[t]ask`) so that the workflow does not match
  itself; keep that when editing it, and never write those words in the workflow's own comments. `ownership` and `owned` are
  fine. Tested on the real tree (clean), on a deliberate violation (all caught, exit 1) and on harmless near-misses (not
  flagged); it ran green on GitHub (2026-11-15).
- **Pins:** the container `qmkfm/qmk_cli:1.2.0`; the runner `ubuntu-24.04`, the image the first run used, instead of the
  floating `ubuntu-latest` label, which GitHub is moving to Ubuntu 26 (the build runs inside the pinned container, so the
  effect would have been small, but a pinned runner keeps CI from changing under us); actions at exact tags.
- **CI and QMK MSYS build the same bytes.** The container's GCC (15.2.0, the same `qmk_toolchains` build) and `qmk` (1.2.0)
  match MSYS. On 2026-11-15 the artifact CI built (68,388 bytes, SHA-256 `1A2D93C3…`) was byte-identical to the local QMK
  MSYS build; an identical result was first observed on 2026-11-10 with an earlier image. Since task 32 the image contains
  the git commit hash, so this holds only for the same commit built from a clean tree with the same hash length (see
  "Build identity"; with `-e SKIP_VERSION=yes` on both sides, any commit can be compared).
- **To reproduce CI locally** (about 12.5 minutes, against 1.7 for MSYS, because of the Windows bind mount):

```powershell
Remove-Item .build -Recurse -Force -ErrorAction SilentlyContinue
docker run --rm --platform linux/amd64 -v "C:/Users/jahor/Documents/GitHub/qmk_firmware:/qmk_firmware" -w /qmk_firmware qmkfm/qmk_cli:1.2.0 bash -c 'git config --global --add safe.directory "*" && pip3 install -q -r requirements-dev.txt && make keychron/x4/ansi_encoder/red:horwitz'
```

Pitfall (observed): do not run the container against a `.build` that MSYS made, or the reverse. MSYS-built dependency
files (`.d`) contain Windows paths (21 of 159 did), and the container's `make` exited with status 2 on such a tree, while
the same command from a clean `.build` succeeded (the failure's exact message was not captured, so the cause is a
well-supported explanation, not proven). Delete `.build` (gitignored) before switching; CI starts clean.

## Why QMK MSYS (decision of 2026-11-12, task 20; do not repeat this research)

**Decision (owner): do not switch.** Keep QMK MSYS for building and flashing, and keep Docker only as the clean-checkout
check of CI (see "Versions and CI"). Nothing has gone wrong with MSYS in this project: it built every time, `qmk flash`
reached the X4 directly with no driver installation, and its firmware is byte-for-byte identical to CI's.

What QMK MSYS is made of (observed): ordinary MSYS2 (242 `pacman` packages, including `bash`, `git`, `make`, Python and
the `qmk` CLI as the package `mingw-w64-x86_64-python-qmk`, which depends only on Python, `python-hid` and `milc`) plus
`/opt/qmk`, a folder outside `pacman` that holds the ARM and AVR compilers (from `qmk_toolchains`) and the flashers (from
`qmk_flashutils`). So its parts can already be updated individually:

- *`/opt/qmk`* (compilers, flashers): replace the folder with a newer release. The compilers are current (GCC 15.2.0 from
  `v15.2.0-1`, the latest). The flashers are four releases behind; left alone (see the audit table).
- *The `pacman` packages* (`git`, `make`, Python, the `qmk` CLI, `milc`): **do not run `pacman -Syu` on the live install.**
  Tested on a copy (task 23, 2026-11-12; the copy was deleted afterwards): the full upgrade (249 packages; `git` 2.56.1,
  `bash` 5.3, Python 3.12 to 3.14) finished without errors but left `qmk` unusable, for three reasons. (1) `python-qmk`
  1.2.0 and `python-milc` 1.9.1 stay installed under `python3.12/site-packages`, so Python 3.14 reports
  `ModuleNotFoundError: No module named 'qmk_cli'` and the build exits 1. (2) `pacman -Qm` lists 19 foreign packages
  (`python-qmk`, `python-milc`, `python-hid`, `hidapi`, `libusb`, `jq`, `python-hjson`, `python-jsonschema`, and more) that
  are no longer in the MSYS2 repositories (the live install has 0), so `pacman` cannot supply newer versions. (3) The new
  `/etc/profile` sources `/etc/post-install/*.post` only when `MAYBE_FIRST_START=true` (the old one always did), so QMK's
  `99-qmk.post` line 31, `export PATH=/opt/qmk/bin:/opt/uv/tools/bin:$PATH`, no longer runs in a plain `bash -l`, and
  `arm-none-eabi-gcc` and `dfu-util` were not found. The live install gets `/opt/qmk/bin` on `PATH` from that line.
  Not tried: updating only `git` (a partial upgrade, which MSYS2 does not support; it also pulled in new Perl
  dependencies). The bundled `git` therefore stays at 2.52.0 until QMK publishes a newer MSYS bundle (a beta dated
  2026-11-11 exists; its notes show a switch to `ucrt64`, see the audit table; not tried).

The comparison that decided it:

| | QMK MSYS (kept) | Docker for builds, plus a separate flasher |
|---|---|---|
| Clean build on this PC | 1.7 minutes | about 12.5 minutes (the same container takes under 2 minutes on GitHub's Linux runner; the likely cause is the Windows bind mount, untested) |
| Flashing | `qmk flash`, one command | needs QMK Toolbox, the Launcher or a standalone `dfu-util` |
| Match with CI | already byte-identical | identical by construction |
| Extra software | none | Docker Desktop must be running (its engine was not, on 2026-11-12) |

WSL2 was also considered: it needs a new Linux distribution (only Docker's internal one exists here) and USB passthrough
for flashing, and the repo would have to live inside WSL for speed. Installing plain MSYS2 and the parts by hand (the
route this tree's `docs/newbs_getting_started.md` calls "advanced users", one `pacman` command for `git` and the `qmk`
package, plus the compilers and flashers from their release archives) would give the same tools plus a newer `git` and
flashers, at the cost of QMK's tested combination.

**Revisit if:** QMK MSYS stops being updated or lacks a toolchain a future change needs; a new QMK MSYS release (or the
2026-11-11 beta) appears, which could carry a newer `git`; the bundled `git` (2.52.0) becomes a real exposure; MSYS and CI binaries ever stop matching; or a fast Docker or WSL2 workflow becomes attractive.
Related tasks: 22 (delete the old MSYS 1.7.2 backup) and 23 (test `pacman` updates on a copy; done, see above).

## QMK CLI warnings (investigated 2026-11-11, task 14; do not repeat this research)

**Decision (owner):** do nothing and filter nothing. The warnings are harmless, and even a message-specific filter could
hide a new warning the owner would want to see. Pinning an older `milc` was ruled out (`qmk` 1.2.0 needs the newer one).
Nothing in vendor code is edited.

The two kinds of warning (both come from QMK upstream's Python in `lib/python/`, which Keychron's fork inherited):

1. **`UserWarning: cli._subcommand has been deprecated, please use cli.subcommand_name…`** from
   `lib/python/qmk/decorators.py`, lines 20 and 40. It appears on **every** command that takes `-kb`/`-km` (`compile`,
   `flash`, `info`, `lint`), and also in `make` runs, which call `qmk` internally (seen in the CI container's `make`).
   Cause: `milc` 1.9.1 (installed) deprecated the attribute `cli._subcommand`, but the repo's code uses it in six lines of
   `decorators.py` (20, 24, 25, 40, 44, 45; it is the only file that does), and the repo's `requirements.txt` only says
   `milc>=1.4.2`, which allows the newer, deprecating version.
2. **`SyntaxWarning: invalid escape sequence`**, from `lib/python/qmk/cli/mass_compile.py` (lines 53-54: `'\[ERRORS\]'`
   and `\e[1;31m` inside the text of a Makefile that the code generates, where the shell needs literal backslashes) and
   `bux.py` (line 33: a stray `\B` inside the `qmk bux` ASCII-art easter egg). Python 3.12 (October 2023) made invalid
   escape sequences in non-raw strings a visible `SyntaxWarning` (a hidden `DeprecationWarning` since 3.6), but it is
   emitted only when Python **compiles** the file. So it appears only when the bytecode cache (`__pycache__`) is missing
   or stale (a fresh clone, the CI container, after the source changes) and not on later runs; `qmk hello` is silent here
   once the cache exists. The strings still come out as the authors intended; Python's documentation says invalid escapes
   will eventually become a `SyntaxError` (general knowledge, no date seen), so keep Python pinned (it comes with MSYS and
   the CI container, both pinned). The authors, per `git log`, are QMK contributors (Nick Brassel for `mass_compile.py`,
   Erovia for `bux.py`).

Warnings that name `multibuild.py` do not come from this repo (the file does not exist here); they come from the old
checkout in the home folder, which `qmk` falls back to when started outside a repo.

**Options considered, and why none was adopted:**

- *A Python warning filter.* It works and was tested: `PYTHONWARNINGS='ignore:invalid escape sequence:SyntaxWarning,
  ignore:cli._subcommand has been deprecated:UserWarning'` silences exactly those messages, while a different
  `SyntaxWarning` (`"is" with 'int' literal`) still shows. The broad form `ignore::SyntaxWarning` hides every
  `SyntaxWarning` and must not be used. Not adopted, because even the narrow form would hide a new occurrence of the same
  message in another file.
- *Pinning an older `milc`.* Ruled out by the owner.
- *The real fix, in vendor code.* Upstream QMK's current `decorators.py` has a helper that tries the public property and
  falls back for old `milc`, and uses it at every former `cli._subcommand.__name__` (the new property returns the same
  name; it returns `None` instead of failing when no subcommand runs):

  ```python
  def _get_subcommand_name():
      """Handle missing cli.subcommand_name on older versions of milc
      """
      try:
          return cli.subcommand_name
      except AttributeError:
          return cli._subcommand.__name__
  ```

  Not applied, because of the no-vendor-edits rule. If it is ever wanted: apply it, run `qmk info`, `qmk compile` and
  `qmk lint`, and compare the firmware hash with the current build's (the CI run's summary page shows it). It would not remove the
  `SyntaxWarning`s, which would need the backslashes doubled in `mass_compile.py` (carefully: its Makefile text already
  uses `\\` and `\\n` correctly, so a raw string would break it) and in `bux.py`.
- *Hiding the "Welcome to QMK MSYS!" banner* (`/etc/post-install/99-qmk.post`; `qmk config user.hide_welcome=True`). Not
  done: the commands used here drop the banner's first 8 output lines (`Select-Object -Skip 8`), which would then cut
  real output.

**How to notice a NEW warning.** The expected baseline for `qmk info -kb keychron/x4/ansi_encoder/red` is exactly the two
`UserWarning`s at `decorators.py:20` and `decorators.py:40`; anything else is new. (A `qmk compile` log shows the same
messages three times, line 20 twice and line 40 once, because `make` calls `qmk` again; observed in a clean MSYS build log.) List the distinct warning lines of a run
and compare them with this baseline instead of filtering by category (`grep -v "Warning"` hides new ones too). To pick
out the **compiler's** diagnostics, match GCC's format with the colon (`grep -E ': (warning|error):'`, case-sensitive),
which does not match Python's `UserWarning:`. When `qmk`, `milc` or Python change, re-run `qmk info` once and compare.

## Pitfalls carried over from the K3 work (observed there; re-check here)

- **Submodules must match the branch.** `lib/chibios`, `lib/chibios-contrib`, `lib/pico-sdk` and others drift after
  branch changes. Fix: `qmk git-submodule` (runs `git submodule sync --recursive` and `update --init --recursive`;
  **never use `-f/--force`**, which also deletes unexpected directories under `lib/`). `lib/` is about 1 GB per the K3
  notes (not measured here); the first run on this branch fetched the NXP `mcux-sdk` submodule (about 240 MB).
- **Vendor keycodes need vendor handling.** On the K3 Max, `KC_LOPTN`/`KC_LCMMD`/`KC_RCMMD` silently did nothing until
  the keymap called the Keychron handler; there was no build error. Check how this board's keymap and `x4.c` handle them
  before using any vendor keycode.
- **Every keymap directory needs a `keymap.c`**, even if it holds only a comment.
- **VIA/Launcher and EEPROM** (not used on the X4: see "VIA/Launcher: decided not to support"). With VIA or dynamic keymap enabled, EEPROM-stored keymap data can override the compiled
  keymap after a flash, and the VIA JSON must agree with the firmware on layer count and keycodes. (general)
- **`dfu-util` can hang** at "Transitioning to dfuMANIFEST state" (seen on the K3 on macOS); the flash has usually
  finished. Ctrl+C, unplug, replug.
- **USB checks:** confirm the USB product ID matches the variant being built (`pid` in `info.json`).

## Where to look, and what to skip

- Matter: `keyboards/keychron/x4/`, `keyboards/keychron/common/`, `TASKS.md`; QMK core (`quantum/`, `builddefs/`,
  `platforms/`) is read-only reference.
- Avoid: `lib/` (about 1 GB), `.build/`, and listing or searching all of `keyboards/` (over a thousand board
  directories; use `keyboards/keychron/` and `keyboards/lemokey/`). The workflows in `.github/workflows/` are stock QMK
  CI, except `horwitz_x4.yml`, which builds the X4 keymap (see "Versions and CI").
- Ignored clutter: `.build/` and the `keychron_x4_*.bin` in the repo root are gitignored, so nothing needs to be
  excluded when staging. `git add -A` is fine here as long as `git status` shows nothing else untracked (unlike the K3
  clone, which has untracked scratch files).
