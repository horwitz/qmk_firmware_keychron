// The horwitz keymap is shared across keyboards and lives in users/horwitz/:
//   keymaps[] array: users/horwitz/horwitz_keymap.c (pulled in via INTROSPECTION_KEYMAP_C in users/horwitz/rules.mk)
//   behavior:        users/horwitz/horwitz.c
// This file holds only what is specific to the K3 Max (see "Supporting another keyboard" in
// users/horwitz/DEVELOPMENT.md).

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "horwitz.h"

// Required by users/horwitz (see horwitz.h). On the K3 Max this MUST call process_record_keychron_common(): without
// it, Keychron's keycodes silently do nothing (e.g., the Mac layer's Option and Cmd keys, KC_LOPTN/KC_LCMMD/KC_RCMMD,
// and Print Screen, KC_SNAP). Unlike the K3 Pro, whose process_record_kb (k3_pro.c) handles these itself, the K3 Max
// handles them only when the keymap calls process_record_keychron_common() (common/keychron_common.c), as Keychron's
// own K3 Max keymaps do.
bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    return process_record_keychron_common(keycode, record);
}
