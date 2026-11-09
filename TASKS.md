Agreements: v0.1.0

Items: 13 total — 3 ■ · 10 □
```
 ■  1. 🔧 (1/1) Update fork's playground to Keychron's (4a8143cce9); keep the old one as old_playground (verified identical with gh compare)
 ■  2. 🔧 (2/2) Create x4 from playground; qmk git-submodule; build stock keychron/x4/ansi_encoder/red:default as the baseline (MSYS 1.7.2, qmk 1.1.1, GCC 10.1.0: built with no warnings)
 ■  3. 📄 (1/1) Fresh CLAUDE.md for the X4 instance, using the latest origin/wireless_playground; record the gh/fork lessons from the playground sync (claims checked against this branch; docs only, nothing to build)
 □ 13. 🧪 (0/2) Flash the unchanged baseline build to the physical X4 and smoke test it (proves the Windows flash path, bootloader entry and drivers; do before any layout work)
 □  4. ⬆️ (0/2) Update wireless_playground (your fork vs keychron/qmk_firmware; how far behind is unverified)
 □  5. 🔧 (0/2) Update QMK MSYS (1.7.2 → 1.12.0), qmk CLI and toolchain, after the baseline builds and before any layout work
 □  6. 🏗️ (0/1) Decide the Mac/Windows selection method (options and recommendation, then the owner's decision)
 □  7. 🏗️ (0/4) MAC_BASE and MAC_FN layers (and the selection method) on the X4
 □  8. 🔧 (0/1) CI build job and docs for the X4
 □  9. 🧪 (0/2) Flash and smoke test on the X4 (Cmd+Space, Option+e then e, Fn layer, Windows layer unchanged)
 □ 10. 🔍 (0/1) Confirm the MCU (info.json says STM32F401; kickoff prompt said F402RC); low priority
 □ 11. 💡 (0/3) Later features: DEBUG, FN-HI on/off variant, SUS-RGB (compare the X4's existing LED_DISABLE_WHEN_USB_SUSPENDED)
 □ 12. 🧹 (0/1) Update the origin URL to qmk_firmware_keychron (cosmetic; the old name redirects)
```
Key: 🔧 tooling · 📄 docs · ⬆️ catch-up · 🏗️ design/refactor · 🧪 testing · 🔍 investigate · 💡 feature · 🧹 cleanup
