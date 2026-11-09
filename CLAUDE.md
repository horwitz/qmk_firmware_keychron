# CLAUDE.md

Personal fork of Keychron's QMK firmware (`origin` = `git@github.com:horwitz/qmk_firmware_keychron.git`). The owner's
keymap is called `horwitz`. Tags below: **(observed)** = happened or was checked in a past session here; **(general)** =
ordinary QMK knowledge, not verified in this repo; **(unverified)** = check before relying on it.

## Working rules

- The owner's global working agreements (loaded from `~/.claude/CLAUDE.md`) govern git/commits, task tracking, code
  style and writing. Task list: `TASKS.md` in the repo root (read it at session start, per the agreements).
- Never commit; the owner does. **Firmware changes are not ready to commit until flashed and smoke tested on the
  physical keyboard(s) they affect.** Until then, end with the next hardware test steps, not commit material.
- Prefer the clean, documented fix over a workaround. Do not edit Keychron/Lemokey vendor code. If the clean option
  costs more, present it and recommend it. Check claims against the repo or by running a command before stating them.

## Upstream and branches

- Remote `keychron` = `git@github.com:Keychron/qmk_firmware.git`. Local branch `wireless_playground` tracks
  `origin/wireless_playground` and sits on top of `keychron/wireless_playground`. (observed)
- `wireless_playground` is the only Keychron branch with both K3 Pro and K3 Max; `bluetooth_playground` (older) lacks
  the Max; `master` has neither. (observed, 2026-08) Whether a `*_playground` branch is "official" or supported:
  unverified.
- The remote's default branch is `2025q3`. Other remote branches: `bluetooth_playground`, `hall_effect_playground`,
  `k8_pro_lightless`, `master`, `old_playground`, `playground`, `wired_wb`, `wireless_playground`, `wls_2025q1`, plus
  `dependabot/*`. (observed, `git ls-remote --heads keychron`, 2026-11)
- Sync: `git fetch keychron`, then merge `keychron/wireless_playground` (the earlier upstream catch-up was a merge; the
  exact commands were not recorded: unverified). After any upstream merge or branch switch, run `qmk git-submodule`.
- Lemokey boards live under `keyboards/lemokey/`. On `wireless_playground` that is `common`, `l1`, `l3`, `p1_pro`.

## Commands

Run from the repo root. Replace the `-kb` path for other boards.

- Build: `qmk compile -kb keychron/k3_pro/ansi/rgb -km horwitz` (also `make keychron/k3_pro/ansi/rgb:horwitz`, which
  is what CI runs). Debug output: add `DEBUG=1`. (observed)
- Flash: `qmk flash -kb <board> -km horwitz`, then put the board in bootloader mode while it waits (K3: unplug, hold
  Esc, replug; other boards differ: unverified). (observed for K3)
- Unit tests: `make test:horwitz` (native build, `tests/horwitz/`; 51 tests as of 2026-10). After switching branches or
  restoring files, `rm -rf .build/test_obj/horwitz` first. (observed)
- Lint/format: `qmk lint -kb <board> -km horwitz`; clang-tidy, cppcheck, `-Wconversion` and clang-format recipes are in
  `users/horwitz/DEVELOPMENT.md`. Those recipes were edited during the userspace refactor and **not re-run**
  (unverified).
- clangd database: compile, then `qmk generate-compilation-database -kb <board> -km horwitz`, then compile again (the
  generate step wipes `.build/`), then Reload Window in VS Code. (observed)
- Reproduce CI locally (Docker): CI uses container `qmkfm/qmk_cli` and runs `pip3 install -r requirements-dev.txt`
  before `make ...`; without that step the build fails with "Cannot run qmk hello". Use
  `docker run --rm --platform linux/amd64 -v "$PWD":/qmk_firmware -w /qmk_firmware qmkfm/qmk_cli bash -c '...'` and
  `git config --global --add safe.directory "*"` inside it. Jobs are in `.github/workflows/horwitz.yml`. (observed)
- Which `qmk` runs: `which -a qmk` showed three (`~/.local/bin/qmk`, `/opt/homebrew/bin/qmk`, `/usr/local/bin/qmk`);
  the first, a pipx install, wins. (observed, 2026-11) The Homebrew copy (1.1.8) is still installed behind it and has
  the same missing-requirements problem. `users/horwitz/DEVELOPMENT.md` step 1 has the fix for each.
- If every `make`/`qmk` command fails with `Cannot run "qmk hello"` or `Could not find module appdirs!`, the pipx
  environment is missing the repo's Python requirements. The fix `qmk` itself prints (run from the repo root):
  `"$HOME/Library/Application Support/pipx/venvs/qmk/bin/python" -m pip install -r requirements.txt`. (observed,
  2026-11: it installed `appdirs`, after which `qmk hello` worked, qmk was 1.2.0, `make test:horwitz` passed 51 tests,
  and both K3 firmware builds compiled without warnings)

## Layout and conventions

- Shared keymap code lives in `users/horwitz/`; each board keeps only a tiny `keyboards/<kb>/keymaps/horwitz/`
  (`keymap.c` plus a `rules.mk` that is one comment). `USER_NAME` defaults to the keymap name, so
  `users/horwitz/rules.mk` is included automatically. (observed)
- `keymaps[]` is in `users/horwitz/horwitz_keymap.c`, pulled in through `INTROSPECTION_KEYMAP_C` in
  `users/horwitz/rules.mk`. The hook exists but is undocumented in QMK's docs; never add that file to `SRC`. (observed)
- Every board's `keymap.c` must define `process_record_keymap` (declared in `users/horwitz/horwitz.h`, which says what
  each board must do). There is deliberately no weak default, so a forgotten definition fails at link time. (observed)
- Layer names (`users/horwitz/layers.h`): `MAC_BASE`, `MAC_FN`, `WIN_BASE`, `WIN_FN`, `GCP`, `CCP`. Fn layers are
  macros (`LAYOUT_macfn`, `LAYOUT_winfn`, ...) in `users/horwitz/keymaps.h`. The Mac base layer uses Keychron's
  `KC_LOPTN`/`KC_LCMMD`/`KC_RCMMD`; the Windows base uses `KC_LGUI`/`KC_LALT`. (observed for K3)
- Custom keycodes start at `NEW_SAFE_RANGE`, never `SAFE_RANGE` (it collides with Keychron's `KC_LOPTN`, etc.); see
  `users/horwitz/ctrlkeycodes.h`. Where `NEW_SAFE_RANGE` comes from differs per board (one `#ifdef` there).
- `users/horwitz/layout.h`, `layout.c`, `fnhi.c`, `ccp.c`, `gcp.c` hard-code the **K3's 84-key ANSI layout** (LED
  indices, `ansi_84_hole_map`). They are not board-agnostic. (observed)
- Adding a board: follow "Supporting another keyboard" in `users/horwitz/DEVELOPMENT.md`. Before porting, compare
  everything the board's own `default` keymap `keymap.c` does beyond `keymaps[]`.
- Keep existing valid comments; American English.

## Pitfalls

- **Submodules must match the branch.** `lib/chibios`, `lib/chibios-contrib`, `lib/pico-sdk` drift after branch changes
  or merges; wrong versions give confusing build and flash behavior. Fix: `qmk git-submodule`. (observed)
- **`dfu-util` can hang** at "Transitioning to dfuMANIFEST state". Intermittent and harmless: the flash has finished.
  Ctrl+C, unplug, replug. Root cause unknown. (observed)
- **Vendor keycodes need vendor handling.** On the K3 Max, `KC_LOPTN`, `KC_LCMMD`, `KC_RCMMD`, `KC_SNAP` silently did
  nothing until the keymap called `process_record_keychron_common()`; there was no build error. The K3 Pro handles them
  in its own `process_record_kb`. Check each board's `default` keymap and keyboard `.c` file for this. (observed)
- **Every keymap directory needs a `keymap.c`**, even if it holds only a comment, or the build fails. (observed)
- **Undefined behavior hides on the host.** `rgb_to_hsv` once cast a negative `double` to `uint8_t`: the Mac test run
  wrapped to the right value, the ARM firmware clamped to 0 (red). Tests now build with
  `-fsanitize=float-cast-overflow`. On Linux/CI the message shows "Lg" instead of the value (QMK's printf replaces the
  standard one); run `make test:horwitz` on macOS to see it. (observed)
- **Tests use QMK's 4x10 matrix** (`tests/test_common/test_common.h`), not the real keyboard's. (observed)
- **Stale test objects.** `make` compares timestamps; a `git stash pop` in the same second left an old `.o`, and a
  test failed against fixed source. `rm -rf .build/test_obj/horwitz`. (observed)
- **USB checks on macOS.** `system_profiler SPUSBDataType` returns nothing on this macOS; use `SPUSBHostDataType` or
  `ioreg -p IOUSB`. Confirm the USB product ID matches the variant being built (`pid` in the board's `info.json`).
  (observed)
- **VIA/Launcher.** Keychron's `via` keymaps set `VIA_ENABLE = yes` in their own `rules.mk`; the `horwitz` keymap does
  not, so the K3 builds have no VIA. (observed in the K3 tree.) A Launcher/VIA definition must agree with the firmware
  on layer count and keycodes (`DYNAMIC_KEYMAP_LAYER_COUNT` is 6 in `users/horwitz/config.h`), and with VIA/dynamic
  keymap enabled, EEPROM-stored keymap data can override the compiled keymap after a flash. (general; not hit here)
- **Default layer / OS toggle.** The K3s pick the base layer from a physical switch in `dip_switch_update_kb`
  (`keyboards/keychron/k3_pro/k3_pro.c`, `k3_max/k3_max.c`). Whether a default layer persists across power cycles
  depends on how it is set. (K3: observed; persistence: general.)

## Where to look, and what to skip

- Matter: `users/horwitz/`, `keyboards/keychron/k3_pro/`, `keyboards/keychron/k3_max/`, `keyboards/lemokey/`,
  `tests/horwitz/`, `.github/workflows/horwitz.yml`, `TASKS.md`. QMK core (`quantum/`, `builddefs/`, `platforms/`) is
  read-only reference.
- Avoid scanning: `lib/` (about 1 GB: `chibios-contrib` 473 MB, `chibios` 363 MB, `lvgl` 95 MB), `.build/`,
  `.build-old/`, `.build-older/`. Use targeted paths instead of repo-wide greps. `keyboards/` holds only `keychron` and
  `lemokey` in this fork. (observed)
- Untracked clutter to leave alone and never stage: `.build-old/`, `.build-older/`, `keymap.json`,
  `230826-diff-w-ecp-first-attempt.diff`, `keymap-*.c` files in `keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/`.
  Stage files by name; never `git add -A`.
