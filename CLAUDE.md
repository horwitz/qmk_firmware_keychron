# CLAUDE.md

Personal fork of Keychron's QMK firmware (GitHub: `horwitz/qmk_firmware_keychron`, which is what `origin` points
to since 2026-11-10; the fork's old name, `qmk_firmware`, still redirects). This branch, `x4`, is for the **Lemokey X4** keyboard: Mac and Windows
layers, and a way to choose between them. The owner's keymap name elsewhere in the fork is `horwitz`. Tags: **(observed)** =
checked in this repo or by running a command; **(general)** = ordinary QMK knowledge, not verified here;
**(unverified)** = check before relying on it.

## Working rules

- The owner's global working agreements (loaded from `~/.claude/CLAUDE.md`) govern git/commits, task tracking, code
  style and writing. Task list: `TASKS.md` in the repo root (read it at session start). This branch has its own
  `TASKS.md`; the K3 Pro/Max list lives on `wireless_playground` and belongs to a different Claude Code instance.
- Never commit; the owner does. Firmware changes may be committed before they are flashed and smoke tested on the
  physical X4, but the commit message must say so (a line such as "UNTESTED ON HARDWARE"), and the next hardware test
  steps must be listed. Once they have been tested, say so in `TASKS.md` and in the next commit. (Before 2026-11-09 the
  rule, inherited from the K3 work, was to wait until after the flash test.)
- Prefer the clean, documented fix over a workaround. Do not edit Keychron/Lemokey vendor code. Check claims against the
  repo or by running a command before stating them.
- Scope: Windows and Mac layers plus the selection method (built, and tested on a real Mac). Later (see `TASKS.md`): DEBUG, an on/off variant of
  FN-HI, SUS-RGB, and tooling updates. The K3's per-key RGB features (GCP, CCP) do not apply: the X4's LEDs are single
  color.

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
- `info.json`: `Lemokey X4`, **STM32F401** (the kickoff prompt said F402RC; resolved in task 10, 2026-11-12: the board's DFU bootloader reports 256 KB of flash, 4x16 KB + 64 KB + 128 KB, which is the F401xC size and matches the build's `GENERIC_STM32_F401XC` board; the bootloader does not name the part, so "F402" is probably Keychron's label, since its C2 Pro readme says rev2 is "STM32F402" while that board's `info.json` also says `STM32F401`; observed with `dfu-util -l`, nothing flashed), `stm32-dfu`
  bootloader, VID `0x362D`, PID `0x0240`, `LAYOUT_82_ansi` (75%, 82 keys, one rotary encoder on B14/B15). Wired only.
- Lighting: single-color LED matrix (CKLED2001, 81 LEDs), `LED_DISABLE_WHEN_USB_SUSPENDED` already set. Caps Lock LED
  index 44, Win-lock LED index 72.
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

## Mac/Windows design (decided 2026-11-09; layers and mode keys implemented and tested on the board 2026-11-09, LED display 2026-11-10; the final build passed full tests on a real Mac and on Windows, 2026-11-10)

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
  `quantum/bootmagic/magic.c`, so the mode survives unplugging (tested). Auto-detect with an override is a later task (see `TASKS.md`).
- **Blink (task 16, built and tested on the board 2026-11-12):** pressing a mode key pulses its LED dark three times
  (`BLINK_PULSES`), the same for both modes: 120 ms steps (`BLINK_STEP_MS`) lit, dark, lit, dark, lit, dark, lit, so it
  starts and ends lit and every pulse is a visible change from either starting state (the first version, once for Windows
  and twice for Mac, started dark, which showed nothing when switching to the mode whose key was already dark; the owner
  also saw no need for two behaviors, since the mode display already shows the chosen mode). It is
  drawn after the mode display and overrides it, so it runs to the end even if Fn is released, in any backlight state
  (the dark step shows against a lit backlight, the lit step against a dark one). Pressing a mode key again, even the
  active one, restarts it. Code: `start_mode_blink` and `apply_blink` in `horwitz.c`. Build: 67,756 bytes, no compiler warnings, sha256 `EE8932B549637F52…` (the tested image; an earlier version that kept
  the mode as a `bool` instead of the base layer was 67,764 bytes and passed the same tests). Tested on the X4 (owner, 2026-11-12): noticeable with
  the backlight on; 3 pulses at 120 ms judged right (fewer felt wrong, more was expected to); also passed with the backlight off,
  with Fn released mid-blink and on a repeat press of the active mode; the Win-lock release on entering Mac mode still works
  (checked by pressing the Win key afterward, not by the LED, which is hard to read with a bright backlight).
- **Auto-detect (task 15, in progress; the mechanism is verified on the Windows PC, not yet on a Mac):** `OS_DETECTION_ENABLE`
  guesses the host's OS from its USB setup requests. Each connection (the USB state going to INIT: a boot, a bus reset)
  clears QMK's detection counters (nothing in this base ever does) and arms a one-shot; `housekeeping_task_user` polls
  `detected_host_os()` (this base has no callback) and, for Windows or macOS, applies that mode with `default_layer_set()`
  **without saving it**, so the EEPROM keeps the owner's last manual choice, which is also the fallback when detection says
  unsure, Linux or iOS. A mode key pressed meanwhile or later wins until the next connection. Observed on the Windows PC:
  Windows is recognized 133-205 ms after the connection (18 requests, 14 of length 0xFF and 2 of length 4; the USB state
  goes INIT, SUSPEND, INIT, INIT, CONFIGURED, so the counters are cleared several times, which is correct), and a saved Mac
  mode (EEPROM default layer `4`) is left alone. Reading the Fn display: with the backlight on, the inactive mode's key is
  the one *dark* key, and the active mode's key looks like all the others, which is easy to misread. Still to do: the
  blink at every connection and a timeout for the unsure case (15-2), and a test on a Mac (15-3). If Mac detection
  misbehaves, a typed-out diagnostic (3 s after a connection it types the counters, the detected OS, the live and saved
  layers) is a quick way to see inside; the diagnostic version was kept outside the repo and can be recreated from this
  description: `send_string` of an `snprintf` of `detected_host_os()`, a local copy of `struct setups_data_t` from
  `quantum/os_detection.c` and its `setups_data`, `default_layer_state`, `layer_state` and `eeconfig_read_default_layer()`.
- **LED feedback:** while a Fn layer is active, the active mode's key LED stays lit and the other mode key's goes dark.
  Facts observed on the board: indicator LEDs are drawn even when the backlight is off, and one key's LED can be driven
  above the background (the Win-lock LED looks brighter when the backlight is dimmed), **and one key's LED can be
  forced dark while its neighbors stay lit** (tested 2026-11-10). The Caps Lock key shows purple with the backlight on
  and blue with it off, which is probably a separate blue indicator LED (`indicators.caps_lock` on C9) plus red light
  bleeding from neighbors.
  Implemented in `led_matrix_indicators_user` (`horwitz.c`): the two LEDs are found from the keymap (scan the Fn layer for
  `MODE_WIN`/`MODE_MAC`, then `g_led_config.matrix_co`), not hard-coded; the replay of that lookup gives LED 42 for `\` and
  68 for `/`. Pitfall found on the board: with the backlight off (`x4.c`'s `BL_TOGG` sets the flags to none and clears
  every LED once), nothing repaints the LEDs, so a value set while Fn is held stays lit after Fn is released. The code
  therefore clears both keys when `!led_matrix_get_flags()`, as `x4.c` does for the Win-lock LED; this assumes that
  "flags == none" means the backlight is off.
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
- `ctrlkeycodes.h`: `MODE_WIN` and `MODE_MAC`.
- `horwitz.c`: the behavior: `process_record_user` (mode selection and the GUI-lock release) and
  `led_matrix_indicators_user` (the mode display) with the blink helpers `start_mode_blink` and `apply_blink`.
- `rules.mk`: `ENCODER_MAP_ENABLE = yes` and `SRC += horwitz.c`.

License headers: none of these files carries one for now. The Keychron block was removed from `keymap.c` and `horwitz.c`
because they are not purely Keychron's code (this repo is private). As a result `qmk lint` reports its license-header
check as failed for the three `.h` files (it does not flag the `.c` files); that is its only finding. What the headers
should say is task 18. The lint only runs in CI on pull requests that touch `keyboards/**` (`lint.yml`), not on pushes.

Naming hazard: the keymap is named `horwitz`, and `USER_NAME` defaults to the keymap name, so QMK would also include
`users/horwitz/rules.mk` and `config.h` if that directory existed (`builddefs/build_keyboard.mk`). It does not on this
branch. If the K3 branch were ever merged in, rename this keymap or set `USER_NAME` in its `rules.mk`.

## Environment: Windows + QMK MSYS

- `qmk`, `make`, `python` and `arm-none-eabi-gcc` are not on the PATH of PowerShell or Git Bash. They live in
  `C:\QMK_MSYS` (QMK MSYS 1.12.0 since 2026-11-09: qmk 1.2.0, GCC 15.2.0 in `/opt/qmk/bin`, make 4.4.1, Python 3.12.12,
  `dfu-util` 0.11-dev; observed). `qmk.exe` refuses to run outside the MINGW64 shell. Run it from PowerShell like this
  (observed to work):

  ```powershell
  $env:MSYSTEM='MINGW64'; $env:CHERE_INVOKING='1'
  & 'C:\QMK_MSYS\usr\bin\bash.exe' -lc 'cd /c/Users/jahor/Documents/GitHub/qmk_firmware && qmk compile -kb keychron/x4/ansi_encoder/red -km default'
  ```

- Docker Desktop (4.99.0) is installed too. It is used only to reproduce CI locally (see "Versions and CI"), not for daily
  builds (see "Why QMK MSYS").
- The old MSYS 1.7.2 (qmk 1.1.1, GCC 10.1.0) was moved to the Windows Recycle Bin on 2026-11-12 (task 22). It stays
  recoverable until the bin is emptied (Restore puts it back at `C:\QMK_MSYS_1.7.2`; it ran standalone). Its result is
  recorded here instead: building the unchanged stock `default` keymap with it from a clean `.build` (source commit
  `4a36d7efd4`, no compiler warnings) gave a 68,208-byte `.bin`, SHA-256
  `4ea835af9f74df71033806ce6525761c7c0d573c39b2357d834123102362fa88`, with 35,902 bytes of `text` + `data`. That is the
  same size as the image flashed to the X4 and tested on 2026-11-08 (the flashed file itself was not kept, so byte
  identity with it is not confirmed). The 1.12.0 installer was run by the owner with all optional boxes unchecked (no
  USB drivers installed). It is unsigned and has no GitHub attestation, but its SHA-256 matched the release checksum and
  the assets were uploaded by `github-actions[bot]`. (observed)
- Baseline: `qmk compile -kb keychron/x4/ansi_encoder/red -km default` builds with no compiler warnings on both
  toolchains (observed: GCC 10.1 gave a 68,208-byte `.bin`, GCC 15.2 a 67,244-byte one). The `.bin` is copied to the repo
  root, where it is gitignored along with `.build/`. QMK's "Size after" line prints `text` + `data` from the ELF (35,902
  with GCC 10.1, 34,940 with GCC 15.2); the `.bin` is larger because the image is contiguous from `0x08000000` and
  `.text` starts at `0x08008000`, leaving about 32 KB of padding after the vector table. It fits the 256 KB flash. Why
  Keychron's layout leaves that gap: unverified. The build banner says `QMK Firmware 0.14.29`; how that string is derived
  in this fork is unverified.
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
  completed). Bootloader entry: the board's `readme.md` says hold K00 (Esc) while plugging
  in, or press the RESET button under the spacebar. Lemokey's pages (read through a summarizer; verify) say hold Esc
  while plugging in, or remove the spacebar keycap and hold RESET (left of the spacebar switch). Holding Esc while plugging in
  worked (observed); the RESET-button route is untried. No driver install was needed: Windows shows
  `STM32  BOOTLOADER` (`0483:DF11`) and `dfu-util -l` lists the board.
- Recovery (Lemokey pages, not yet downloaded or checked): factory reset of settings is Fn + J + Z for 4 seconds
  (backlight flashes red). Official firmware: `x4_ansi_encoder_red_v1.0.2_2404191652.bin` from
  `https://www.lemokey.com/pages/firmware-and-json-files-of-the-lemokey-qmk-keyboards`, flashed with the web Launcher
  (`launcher.lemokey.com`, cable mode) or QMK Toolbox. Put any download in its own new directory; it is untrusted data.
- The `default` build has no VIA, so Launcher/VIA cannot edit its keymap, but the Launcher's firmware update works at
  bootloader level and can restore the official image, as could `dfu-util` or QMK Toolbox with the official `.bin`. (general)

## Versions and CI

Audit of 2026-11-10 (task 19). Policy: pin known-good versions and upgrade deliberately (rebuild, retest on the board,
record it), as in the 2026-11-09 toolchain upgrade; never rely on a floating "latest" in CI. The QMK base of the tree stays
on Keychron's fork, because the X4 exists only there and moving to upstream QMK (0.34.7) would mean porting vendor code.

| Component | In use | Latest at the audit |
|---|---|---|
| QMK MSYS | 1.12.0 | 1.12.0 (stable; a beta exists, not used) |
| `qmk` CLI | 1.2.0 | 1.2.0 |
| `arm-none-eabi-gcc` | 15.2.0 (`qmk_toolchains` v15.2.0-1) | v15.2.0-1 |
| `qmk_flashutils` (`dfu-util`) | 20251130, bundled in MSYS (in `/opt/qmk`, outside `pacman`) | 20260711. Left alone: flashing works, and the 14 commits since (no release notes) are mostly packaging for other platforms and tools (`hidapi`, `picotool`, Linux/macOS), dependency bumps and CI; nothing names `dfu-util` or Windows STM32 flashing. Update only for a named fix or a flashing problem, and try it on a copy first (task 23) |
| Git for Windows | 2.56.0.windows.4 | 2.56.1.windows.1, a security release (the owner updates it) |
| `git` inside QMK MSYS | 2.52.0 (package `git 2.52.0-2`) | MSYS has `pacman` (6.1.0), but a full `pacman -Syu` breaks QMK MSYS (task 23, below); a git-only update is untested; left alone |
| `gh` | 2.103.3 | 2.103.3 |
| Docker Desktop | 4.99.0 | 4.99.0 |
| `actions/checkout` | v7.0.1 | v7.0.1 |
| CI container | `qmkfm/qmk_cli:1.2.0` (`sha256:b7d7fa8f…`) | the same image as `latest` |
| Keychron `playground` | `4a8143cce9` | `4a8143cce9` |

CI is `.github/workflows/horwitz_x4.yml`: it builds `keychron/x4/ansi_encoder/red:horwitz` in the pinned container on every
push to `x4`, on pull requests and on demand. It omits `qmk lint` until task 18. The container's GCC (15.2.0, the same
`qmk_toolchains` build) and `qmk` (1.2.0) match MSYS, and its firmware is byte-for-byte identical to MSYS's (67,636 bytes,
sha256 `67AFA083E17F6AA1…`, observed 2026-11-10). Its first run on GitHub (2026-11-11, commit `1ed1b1633b`, runner image `ubuntu-24.04`)
succeeded in about 1 minute 42 seconds in total. The workflow now pins `runs-on: ubuntu-24.04` (the image that run used)
instead of the floating `ubuntu-latest` label, which GitHub is moving to Ubuntu 26; the build runs inside the pinned
container, so the effect would have been small, but a pinned runner keeps CI from changing under us. The first run with
the pin (2026-11-11, commit `0fdfbd50a0`) succeeded in about 1 minute 47 seconds. To reproduce CI locally (about 12.5 minutes, against 1.7 for
MSYS, because of the Windows bind mount):

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
  2026-10-19 exists; not examined).

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
2026-10-19 beta) appears, which could carry a newer `git`; the bundled `git` (2.52.0) becomes a real exposure; MSYS and CI binaries ever stop matching; or a fast Docker or WSL2 workflow becomes attractive.
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
  `qmk lint`, and compare the firmware hash with the known good one (`67AFA083E17F6AA1…`). It would not remove the
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
