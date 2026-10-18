#pragma once

#include QMK_KEYBOARD_H

/*
 * Keyboard-specific key handling, adapted from QMK's userspace "_keymap" pattern (docs/feature_userspace.md,
 * "Consolidated Macros"). process_record_user (horwitz.c) calls this last, for every key event that the shared
 * features (GCP, CCP) didn't consume; return false to stop further processing of the key, true to continue.
 *
 * EVERY keyboard's keymaps/horwitz/keymap.c MUST define this. Unlike QMK's documented pattern, there is deliberately
 * no weak default, so a keyboard that forgets fails to link ("undefined reference to process_record_keymap") rather
 * than silently losing keys.
 *
 * Keychron boards differ in what this must do:
 *   - K3 Max: MUST call process_record_keychron_common() (common/keychron_common.c), as Keychron's own K3 Max keymaps
 *             do; otherwise Keychron's keycodes (KC_LOPTN, KC_LCMMD, KC_RCMMD, KC_SNAP, ...) silently do nothing.
 *   - K3 Pro: just returns true; k3_pro.c's process_record_kb already handles those keycodes (and keychron_common.c
 *             isn't compiled for it, so calling process_record_keychron_common() would fail to link).
 * See "Supporting another keyboard" in DEVELOPMENT.md.
 */
bool process_record_keymap(uint16_t keycode, keyrecord_t *record);
