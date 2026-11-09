Agreements: v0.1.0

Items: 56 total — 3 ✕ · 49 ■ · 1 ⏸ · 3 □
```
 ✕ 37. 🧹 (0/0) [B3] color_picker_color_hues[] → stack-local (dropped: array also used at keypress; savings trivial on this hardware)
 ✕ 40. 🧪 (0/0) [B6] Tests: get_ccp_key (dropped: static fn, QMK keycode deps make native testing impractical; logic simple enough to verify by inspection)
 ✕ 29. 🧹 (0/0) [A5] get_ccp_key default branches (dropped: intent already clear)
 ■  1. 💻 (1/1) Set up VS Code with clangd, qmk setup, compile_commands.json
 ■  2. 🧹 (2/2) Convert //uprintf call sites to #if DEBUG … #endif guards
 ■  3. 🧹 (1/1) Sync CONSOLE_ENABLE with DEBUG (rules.mk)
 ■  4. 🧹 (1/1) MIN/MAX_CCP_CHANGE_KEYCODE → static const uint16_t in ccp.c
 ■  5. 🧹 (1/1) MIN/MAX_COLOR_KEYCODE → static const uint16_t in gcp.c
 ■  6. 🧹 (1/1) Fix get_color_scheme impossible-case return (-1 as enum)
 ■  7. 🧹 (2/2) Fix LAYOUT_ccp TO(MAC_BASE) → dynamic; address remaining TODO.md items
 ■  8. 🧹 (1/1) gcp.c: int → uint8_t for arrays, return types, local intensity
 ■  9. 🧹 (1/1) ccp.c rgb_to_hsv: remove dead if (hsv.h < 0) branch
 ■ 10. 🔍 (2/2) Static analysis: clang-tidy
 ■ 11. 🔍 (1/1) Static analysis: cppcheck staticFunction (add static; trim ccp.h)
 ■ 12. 🔍 (1/1) Static analysis: cppcheck constParameterPointer (const keyrecord_t *)
 ■ 13. 🔍 (2/2) Static analysis: -Wconversion
 ■ 14. 📄 (1/1) DEVELOPMENT.md: rename + add linting & formatting section
 ■ 15. ⬆️ (3/3) Merge upstream keychron/bluetooth_playground
 ■ 16. 🔧 (2/2) QMK CLI + Python deps update (pipx, 1.2.0)
 ■ 17. 🧪 (4/4) Test framework: Apple clang 21 compatibility fixes
 ■ 18. 🏗️ (3/3) Refactor: extract ccp_color.h/ccp_color.c
 ■ 19. 🧪 (4/4) Unit tests: tests/horwitz (22 tests)
 ■ 20. 🧪 (1/1) Flash and test all recent changes on physical keyboard
 ■ 21. 🐛 (4/4) Fix GCP grays (GRAY00–GRAY11): not visible on GCP layer, pressing them doesn't change keyboard color
 ■ 22. 🧹 (1/1) #ifndef include guards → #pragma once in all horwitz headers
 ■ 24. 🔧 (2/2) Fix CCP round-trip drift: store last CCP-set RGB; reseed ccpRgb from HSV only when color changed externally
 ■ 25. 🧹 (1/1) [A1] Make index_in_byte and ccpRgb static in ccp.c; remove extern from ccp.h
 ■ 26. 🧹 (1/1) [A2] Make layers_used_indices and layer_used_indices_size static in fnhi.c
 ■ 27. 🧹 (1/1) [A3] Replace 28 magic LED indices in ccp.c with named constants in layout.h
 ■ 28. 🧹 (1/1) [A4] Resolve/remove TODO comments in ccp.c, fnhi.c, keymap.c
 ■ 30. 🧹 (2/2) [A6] Rewrite fnhi initialize_layer_used_indices_inner to use ansi_84_hole_map
 ■ 31. 🧹 (1/1) [A7] last_layer = -1 → UINT8_MAX in keymap.c
 ■ 32. 🔧 (1/1) [A8] Add firmware build job to CI (horwitz.yml); fix CI path filter and GCC issues
 ■ 33. 🧹 (1/1) [A9] Reformat complement_hsv initializer to multi-line in fnhi.c
 ■ 34. 🧪 (2/2) [A10] Add tests for initialize_gcp_palette_keycodes and CCP drift logic
 ■ 51. 🧪 (1/1) [CI1] Fix: uint16_t undeclared in keycode_util.hpp (add #include <cstdint>)
 ■ 52. 🧪 (1/1) [CI2] Fix: add tests/test_common/** to CI path filter
 ■ 53. 🧪 (1/1) [CI3] Fix: designated initializer order in test_ccp.cpp (make_rgb helper)
 ■ 35. 🧹 (1/1) [B1] Fix two stale comments: "white lights" in layer_state_set_user_ccp; "12 grayscale keys" in gcp.c block comment
 ■ 36. 🧹 (1/1) [B2] Make cPickGrayscaleAvailable static in gcp.c
 ■ 38. 🔧 (1/1) [B4] Move GCP_ANIM_FRAME_MS to config.h
 ■ 39. 🧪 (1/1) [B5] Tests: addBounded/subtractBounded in ccp_color.c
 ■ 41. 🧪 (2/2) [B7] Tests: initialize_layer_used_indices_inner in fnhi.c
 ■ 43. ⬆️ (2/2) [C0] Migrate fork from bluetooth_playground to wireless_playground (adds K3 Max support; wireless_playground is Keychron's active branch)
 ■ 46. 🧪 (1/1) [C3] Flash and verify wireless_playground firmware on physical K3 Pro (re-verified after syncing lib/ submodules)
 ■ 47. 🏗️ (3/3) [C4] Refactor to users/horwitz/ shared keymap: zero duplication between Pro and Max (behavior in horwitz.c, keymaps[] via INTROSPECTION_KEYMAP_C)
 ■ 44. 🏗️ (2/2) [C1] Create K3 Max keymap: keyboards/keychron/k3_max/ansi/rgb/keymaps/horwitz/, verify layout/LED indices match Pro
 ■ 45. 🔧 (1/1) [C2] Add K3 Max firmware build to CI (keychron/k3_max/ansi/rgb:horwitz)
 ■ 48. 🧪 (2/2) [C5] Flash and verify horwitz firmware on physical K3 Max (first flash: Cmd/Option/Print Screen dead on Mac layer → 49; passed after 49)
 ■ 49. 🐛 (1/1) [C6] K3 Max: Keychron keycodes (KC_LOPTN/KC_LCMMD/KC_RCMMD/KC_SNAP) unhandled; add required process_record_keymap hook (horwitz.h) + per-board keymap.c + DEVELOPMENT.md "Supporting another keyboard"
 ■ 50. 🐛 (1/1) [C7] CCP: rgb_to_hsv negative hue → uint8_t is UB; ARM clamps to 0 so magenta–red colors (e.g. #FF00FF) commit as red; wrap hue + -fsanitize=float-cast-overflow in tests + 2 new tests
 ■ 54. 🔧 (1/1) Migrate task tracking to working agreements v0.1.0 (TASKS.md; retire memory task list and users/horwitz/TODO.md)
 ⏸ 23. 🧹 (0/2) clang-format across all horwitz keymap source files (create a .clang-format config first; large diff, so do as a standalone commit) (deferred: tabled for now)
 □ 42. 🧪 (0/3) [B8] Investigate QMK test framework for horwitz keymap; target: process_record_user_ccp (stateful key→RGB logic) and process_record_user_gcp
 □ 55. 📄 (0/1) Repo-root CLAUDE.md for the Lemokey X4 session (cross-keyboard knowledge only)
 □ 56. 📄 (0/1) Kickoff prompt for the Lemokey X4 macOS-support session
```
Key: 💻 dev environment · 🧹 cleanup · 🔍 static analysis · 📄 docs · ⬆️ catch-up · 🔧 tooling · 🧪 testing · 🏗️ refactor · 🐛 bug fix
