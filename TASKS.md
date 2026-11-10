Agreements: v0.1.1

Items: 17 total — 7 ■ · 9 □ · 1 ◧
```
 ■  1. 🔧 (1/1) Update fork's playground to Keychron's (4a8143cce9); keep the old one as old_playground (verified identical with gh compare)
 ■  2. 🔧 (2/2) Create x4 from playground; qmk git-submodule; build stock keychron/x4/ansi_encoder/red:default as the baseline (MSYS 1.7.2, qmk 1.1.1, GCC 10.1.0: built with no warnings)
 ■  3. 📄 (1/1) Fresh CLAUDE.md for the X4 instance, using the latest origin/wireless_playground; record the gh/fork lessons from the playground sync (claims checked against this branch; docs only, nothing to build)
 ■ 13. 🧪 (2/2) Flash the unchanged baseline build to the physical X4 and smoke test it (verified on the X4: all keys, encoder, Fn layer including Win lock, backlight and Caps Lock LED, and Esc-hold bootloader re-entry; no driver install needed; official .bin download skipped by choice)
 ■  5. 🔧 (3/3) Update QMK MSYS (1.7.2 → 1.12.0), qmk CLI and toolchain (verified: backup at C:\QMK_MSYS_1.7.2 runs standalone; installer SHA-256 matched, unsigned, run by the owner with drivers unchecked; now qmk 1.2.0, GCC 15.2.0, make 4.4.1, Python 3.12.12; baseline rebuilds with no compiler warnings; GCC 15.2 build flashed and passed the X4 smoke test again, with Esc-hold bootloader re-entry; estimate was raised from 2 to 3 chunks)
 ■  4. ⬆️ (1/1) Update wireless_playground (no-op: verified with gh compare that the fork's wireless_playground is 9 ahead and 0 behind Keychron's, whose tip 666862cb81 is its merge base; moving K3 code to a newer Keychron line, e.g., 2025q3, would be a K3-instance task; estimate was 2 chunks)
 ■  6. 🏗️ (1/1) Decide the Mac/Windows selection method (decided: four layers in stock order WIN_BASE 0, WIN_FN 1, MAC_BASE 2, MAC_FN 3, Windows is the boot layer; two explicit Fn-layer keys, Fn+\ for Windows and Fn+/ for Mac, calling set_single_persistent_default_layer; LED feedback while a Fn layer is active: the active mode's key stays lit and the other goes dark; auto-detect plus override later as task 15, blink later as task 16; recorded in CLAUDE.md)
 □  8. 🔧 (0/1) CI build job and docs for the X4
 □  9. 🧪 (0/2) Flash and smoke test on the X4 (Cmd+Space, Option+e then e, Fn layer, Windows layer unchanged)
 □ 10. 🔍 (0/1) Confirm the MCU (info.json says STM32F401; kickoff prompt said F402RC); low priority
 □ 11. 💡 (0/3) Later features: DEBUG, FN-HI on/off variant, SUS-RGB (compare the X4's existing LED_DISABLE_WHEN_USB_SUSPENDED)
 □ 12. 🧹 (0/1) Update the origin URL to qmk_firmware_keychron (cosmetic; the old name redirects)
 □ 14. 🔍 (0/1) Investigate the QMK CLI warnings seen under qmk 1.2.0 / Python 3.12 (SyntaxWarning invalid escape sequences in lib/python/qmk/cli/*.py; UserWarning that cli._subcommand is deprecated in lib/python/qmk/decorators.py): harmless noise from vendor code? Decide whether anything is worth doing (no vendor edits)
 □ 15. 💡 (0/3) Add OS auto-detection with a manual override (option C): OS_DETECTION_ENABLE picks the layer at boot and the Fn-layer keys override it (this base has no detection callback, so polling is needed; QMK calls detection a best guess; decide the override semantics)
 □ 16. 💡 (0/1) Add the blink (LED option 4): briefly flash the pressed mode key's LED (once for Windows, twice for Mac) when the mode changes; needs a timer; works in any backlight state
 □ 17. 🔍 (0/1) Decide whether the X4 firmware should support VIA/Launcher (VIA_ENABLE, DYNAMIC_KEYMAP_LAYER_COUNT, a VIA JSON with our four layers and custom keycodes in the QK_KB_* range), weighing the EEPROM-overrides-compiled-keymap pitfall; implement only if the answer is yes
▾◧  7. 🏗️ (0/5) MAC_BASE and MAC_FN layers (and the selection method) on the X4 (estimate raised from 4 to 5 chunks when the subtasks were created)
```
Key: 🔧 tooling · 📄 docs · ⬆️ catch-up · 🏗️ design/refactor · 🧪 testing · 🔍 investigate · 💡 feature · 🧹 cleanup

Subtasks of 7:

Items: 5 total — 1 ■ · 4 □
```
▾◧ 7.   🏗️ (1/5) MAC_BASE and MAC_FN layers (and the selection method) on the X4
   ■ 7-1. 🏗️ (1/1) Skeleton: keymaps/horwitz under keyboards/keychron/x4/ansi_encoder/red/ with layers.h (4-layer enum in stock order), keymaps.h (the Fn layers only, as on the K3: LAYOUT_winfn plus a transparent LAYOUT_macfn placeholder), keymap.c (base layers inline: stock WIN_BASE, transparent MAC_BASE placeholder; stock encoder map) and rules.mk (ENCODER_MAP_ENABLE = yes) (verified: builds with no warnings, and the .bin is byte-identical to a fresh stock default build; not yet flashed)
   □ 7-2. 🏗️ (0/1) Mac layers: MAC_BASE (bottom row Ctrl, Option, Command; Fn key to MO(MAC_FN)) and MAC_FN (Mac F-row: Mission Control, Launchpad, ...; decide what Fn+Left-Win does on the Mac layer); builds
   □ 7-3. 🏗️ (0/1) Mode keys: Fn+\ (Windows) and Fn+/ (Mac) as custom keycodes calling set_single_persistent_default_layer (confirm a safe keycode range next to Keychron's QK_KB_* keycodes); builds
   □ 7-4. 💡 (0/1) LED display: while a Fn layer is active, the active mode's key LED stays lit and the other goes dark (first test that one LED can be forced below the background); builds
   □ 7-5. 🔍 (0/1) Lint (qmk lint), check the build for warnings, and update CLAUDE.md with the final file layout
```
Key: 🏗️ design/refactor · 💡 feature · 🔍 investigate
