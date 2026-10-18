// The horwitz keymap is shared across keyboards and lives in users/horwitz/:
//   keymaps[] array: users/horwitz/horwitz_keymap.c (pulled in via INTROSPECTION_KEYMAP_C in users/horwitz/rules.mk)
//   behavior:        users/horwitz/horwitz.c
// This file holds only what is specific to the K3 Pro (see "Supporting another keyboard" in
// users/horwitz/DEVELOPMENT.md).

#include QMK_KEYBOARD_H
#include "horwitz.h"

// Required by users/horwitz (see horwitz.h). The K3 Pro needs no extra handling: k3_pro.c's process_record_kb already
// handles Keychron's keycodes (KC_LCMMD, KC_SNAP, etc.). Do NOT call process_record_keychron_common() here (as the
// K3 Max does): keychron_common.c isn't compiled for the K3 Pro, so it would fail to link.
bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    return true;
}
