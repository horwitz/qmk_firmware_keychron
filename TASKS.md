Agreements: v0.1.1

Items: 14 total — 6 ■ · 8 □
```
 ■  1. 🔧 (1/1) Update fork's playground to Keychron's (4a8143cce9); keep the old one as old_playground (verified identical with gh compare)
 ■  2. 🔧 (2/2) Create x4 from playground; qmk git-submodule; build stock keychron/x4/ansi_encoder/red:default as the baseline (MSYS 1.7.2, qmk 1.1.1, GCC 10.1.0: built with no warnings)
 ■  3. 📄 (1/1) Fresh CLAUDE.md for the X4 instance, using the latest origin/wireless_playground; record the gh/fork lessons from the playground sync (claims checked against this branch; docs only, nothing to build)
 ■ 13. 🧪 (2/2) Flash the unchanged baseline build to the physical X4 and smoke test it (verified on the X4: all keys, encoder, Fn layer including Win lock, backlight and Caps Lock LED, and Esc-hold bootloader re-entry; no driver install needed; official .bin download skipped by choice)
 ■  5. 🔧 (3/3) Update QMK MSYS (1.7.2 → 1.12.0), qmk CLI and toolchain (verified: backup at C:\QMK_MSYS_1.7.2 runs standalone; installer SHA-256 matched, unsigned, run by the owner with drivers unchecked; now qmk 1.2.0, GCC 15.2.0, make 4.4.1, Python 3.12.12; baseline rebuilds with no compiler warnings; GCC 15.2 build flashed and passed the X4 smoke test again, with Esc-hold bootloader re-entry; estimate was raised from 2 to 3 chunks)
 ■  4. ⬆️ (1/1) Update wireless_playground (no-op: verified with gh compare that the fork's wireless_playground is 9 ahead and 0 behind Keychron's, whose tip 666862cb81 is its merge base; moving K3 code to a newer Keychron line, e.g., 2025q3, would be a K3-instance task; estimate was 2 chunks)
 □  6. 🏗️ (0/1) Decide the Mac/Windows selection method (options and recommendation, then the owner's decision)
 □  7. 🏗️ (0/4) MAC_BASE and MAC_FN layers (and the selection method) on the X4
 □  8. 🔧 (0/1) CI build job and docs for the X4
 □  9. 🧪 (0/2) Flash and smoke test on the X4 (Cmd+Space, Option+e then e, Fn layer, Windows layer unchanged)
 □ 10. 🔍 (0/1) Confirm the MCU (info.json says STM32F401; kickoff prompt said F402RC); low priority
 □ 11. 💡 (0/3) Later features: DEBUG, FN-HI on/off variant, SUS-RGB (compare the X4's existing LED_DISABLE_WHEN_USB_SUSPENDED)
 □ 12. 🧹 (0/1) Update the origin URL to qmk_firmware_keychron (cosmetic; the old name redirects)
 □ 14. 🔍 (0/1) Investigate the QMK CLI warnings seen under qmk 1.2.0 / Python 3.12 (SyntaxWarning invalid escape sequences in lib/python/qmk/cli/*.py; UserWarning that cli._subcommand is deprecated in lib/python/qmk/decorators.py): harmless noise from vendor code? Decide whether anything is worth doing (no vendor edits)
```
Key: 🔧 tooling · 📄 docs · ⬆️ catch-up · 🏗️ design/refactor · 🧪 testing · 🔍 investigate · 💡 feature · 🧹 cleanup
