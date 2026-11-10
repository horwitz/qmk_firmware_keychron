# CLAUDE.md

Personal fork of Keychron's QMK firmware (GitHub: `horwitz/qmk_firmware_keychron`; the `…/qmk_firmware.git` URL in
`git remote -v` is an old name that redirects). This branch, `x4`, is for the **Lemokey X4** keyboard: Mac and Windows
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
- Scope for now: Windows and Mac layers plus the selection method. Later (see `TASKS.md`): DEBUG, an on/off variant of
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
- `info.json`: `Lemokey X4`, **STM32F401** (the kickoff prompt said F402RC: unresolved, low priority), `stm32-dfu`
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
  **`NEW_SAFE_RANGE` does not exist on this branch** (the K3 userspace uses it); how to add custom keycodes here:
  unverified. Only the `keychron` keymap calls into the Keychron handlers; `default` has no `process_record_user` at all.
- Mac modifiers (general, unverified here): plain `KC_LGUI` and `KC_LALT` with swapped bottom-row positions should be
  enough; the Keychron vendor keycodes may be unnecessary on the X4. Test on hardware.

## Mac/Windows design (decided 2026-11-09; layers and mode keys implemented and tested on the board 2026-11-09, LED display 2026-11-10)

- **Four layers in stock order:** `WIN_BASE` 0, `WIN_FN` 1, `MAC_BASE` 2, `MAC_FN` 3 (taking the two empty `WIN_L2`/`WIN_L3`
  slots). Windows is layer 0, the boot layer of a fresh or erased board, and the stock Windows layers keep their indices.
  The K3 spells every layer out in full (only its two Fn layers are named `LAYOUT_*` macros in `users/horwitz/keymaps.h`;
  its Mac and Windows bases are duplicated rows in `keymaps[]`). How much to share between layers here is an open design
  point in task 7.
- **Selection:** two explicit Fn-layer keys that set the persistent default layer: **Fn + `\` for Windows, Fn + `/` for
  Mac** (the directory separators). Both are free on the stock Fn layer, are at the same positions on both Fn layers,
  and avoid Lemokey's Fn + J + Z factory-reset combo. They need a small custom keycode calling
  `set_single_persistent_default_layer()` (documented in `docs/feature_layers.md`); a persistent `PDF` keycode was not
  found on this base. The keycodes are `MODE_WIN`/`MODE_MAC` (`ctrlkeycodes.h`, from `SAFE_RANGE`, which nothing in the X4
  tree uses), handled in `horwitz.c`; the saved layer is restored at every boot by `magic()` in
  `quantum/bootmagic/magic.c`, so the mode survives unplugging (tested). Auto-detect with an override, and a blink on
  switching, are later tasks (see `TASKS.md`).
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

## Environment: Windows + QMK MSYS

- `qmk`, `make`, `python` and `arm-none-eabi-gcc` are not on the PATH of PowerShell or Git Bash. They live in
  `C:\QMK_MSYS` (QMK MSYS 1.12.0 since 2026-11-09: qmk 1.2.0, GCC 15.2.0 in `/opt/qmk/bin`, make 4.4.1, Python 3.12.12,
  `dfu-util` 0.11-dev; observed). `qmk.exe` refuses to run outside the MINGW64 shell. Run it from PowerShell like this
  (observed to work):

  ```powershell
  $env:MSYSTEM='MINGW64'; $env:CHERE_INVOKING='1'
  & 'C:\QMK_MSYS\usr\bin\bash.exe' -lc 'cd /c/Users/jahor/Documents/GitHub/qmk_firmware && qmk compile -kb keychron/x4/ansi_encoder/red -km default'
  ```

- Docker Desktop is installed too (the `qmkfm/qmk_cli` container is a fallback; unverified for this branch).
- The old MSYS 1.7.2 (qmk 1.1.1, GCC 10.1.0) is kept as a working copy at `C:\QMK_MSYS_1.7.2` (runs standalone; observed),
  as a fallback or to compare builds. The 1.12.0 installer was run by the owner with all optional boxes unchecked (no
  USB drivers installed). It is unsigned and has no GitHub attestation, but its SHA-256 matched the release checksum and
  the assets were uploaded by `github-actions[bot]`. (observed)
- Baseline: `qmk compile -kb keychron/x4/ansi_encoder/red -km default` builds with no compiler warnings on both
  toolchains (observed: GCC 10.1 gave a 68,208-byte `.bin`, GCC 15.2 a 67,244-byte one). The `.bin` is copied to the repo
  root, where it is gitignored along with `.build/`. QMK's "Size after" line prints `text` + `data` from the ELF (35,902
  with GCC 10.1, 34,940 with GCC 15.2); the `.bin` is larger because the image is contiguous from `0x08000000` and
  `.text` starts at `0x08008000`, leaving about 32 KB of padding after the vector table. It fits the 256 KB flash. Why
  Keychron's layout leaves that gap: unverified. The build banner says `QMK Firmware 0.14.29`; how that string is derived
  in this fork is unverified.
- With qmk 1.2.0 and Python 3.12, every `qmk` command prints noise from this repo's old CLI code (`SyntaxWarning: invalid
  escape sequence` in `lib/python/qmk/cli/*.py`, and a `UserWarning` that `cli._subcommand` is deprecated in
  `lib/python/qmk/decorators.py`). It is harmless and is vendor code; investigating is on `TASKS.md`. A separate,
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

## Pitfalls carried over from the K3 work (observed there; re-check here)

- **Submodules must match the branch.** `lib/chibios`, `lib/chibios-contrib`, `lib/pico-sdk` and others drift after
  branch changes. Fix: `qmk git-submodule` (runs `git submodule sync --recursive` and `update --init --recursive`;
  **never use `-f/--force`**, which also deletes unexpected directories under `lib/`). `lib/` is about 1 GB per the K3
  notes (not measured here); the first run on this branch fetched the NXP `mcux-sdk` submodule (about 240 MB).
- **Vendor keycodes need vendor handling.** On the K3 Max, `KC_LOPTN`/`KC_LCMMD`/`KC_RCMMD` silently did nothing until
  the keymap called the Keychron handler; there was no build error. Check how this board's keymap and `x4.c` handle them
  before using any vendor keycode.
- **Every keymap directory needs a `keymap.c`**, even if it holds only a comment.
- **VIA/Launcher and EEPROM.** With VIA or dynamic keymap enabled, EEPROM-stored keymap data can override the compiled
  keymap after a flash, and the VIA JSON must agree with the firmware on layer count and keycodes. (general)
- **`dfu-util` can hang** at "Transitioning to dfuMANIFEST state" (seen on the K3 on macOS); the flash has usually
  finished. Ctrl+C, unplug, replug.
- **USB checks:** confirm the USB product ID matches the variant being built (`pid` in `info.json`).

## Where to look, and what to skip

- Matter: `keyboards/keychron/x4/`, `keyboards/keychron/common/`, `TASKS.md`; QMK core (`quantum/`, `builddefs/`,
  `platforms/`) is read-only reference.
- Avoid: `lib/` (about 1 GB), `.build/`, and listing or searching all of `keyboards/` (over a thousand board
  directories; use `keyboards/keychron/` and `keyboards/lemokey/`). The workflows in `.github/workflows/` are stock QMK
  CI; this branch has no `horwitz.yml`, so an X4 CI job is an open task.
- Ignored clutter: `.build/` and the `keychron_x4_*.bin` in the repo root are gitignored, so nothing needs to be
  excluded when staging. `git add -A` is fine here as long as `git status` shows nothing else untracked (unlike the K3
  clone, which has untracked scratch files).
