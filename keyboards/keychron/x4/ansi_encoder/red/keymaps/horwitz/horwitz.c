/* Copyright 2023 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Behavior of the horwitz keymap for the Lemokey X4 (named as on the K3, where the shared behavior is horwitz.c):
// the keys that choose between Windows mode and Mac mode.

#include QMK_KEYBOARD_H
#include "layers.h"
#include "ctrlkeycodes.h"

// x4.c keeps the state of its GUI ("Win") lock in a private variable, which it also saves to the EEPROM user datablock
// (eeconfig_update_user_datablock) on every toggle and reloads at boot. We rely on that to read the lock state.
_Static_assert(EECONFIG_USER_DATA_SIZE == 1, "x4.c's lock state is expected to be the only byte of the user datablock");

// The lock disables the GUI keys, which are Command on the Mac layer, and it would stay on after switching modes: Command
// would be dead, with no lock key on the Mac layer to turn it off. So selecting Mac mode first releases it.
//
// Two states have to be cleared: x4.c's private one (which drives the lock LED) and QMK's keymap_config.no_gui (which
// actually drops the GUI keycodes). A GU_TOGG press normally changes both, but they are checked separately here, so that
// neither is toggled the wrong way if they ever disagree.
static void release_gui_lock(void) {
    uint8_t x4_lock[EECONFIG_USER_DATA_SIZE] = {0};
    eeconfig_read_user_datablock(x4_lock);
    if (x4_lock[0]) {
        // x4.c's process_record_kb toggles its own state, the LED and the saved copy on GU_TOGG. It does not toggle
        // keymap_config.no_gui (QMK's own handling of GU_TOGG, which we bypass here, does that).
        keyrecord_t record = {.event = MAKE_KEYEVENT(0, 0, true)};
        process_record_kb(GU_TOGG, &record);
    }
    if (keymap_config.no_gui) {
        keymap_config.no_gui = false;
        eeconfig_update_keymap(keymap_config.raw);
    }
}

// Sets the base layer and saves it to the EEPROM; keyboard_init() restores it at every boot (quantum/bootmagic/magic.c).
static void select_mode(uint8_t base_layer) {
    if (base_layer == MAC_BASE) {
        release_gui_lock();
    }
    set_single_persistent_default_layer(base_layer);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MODE_WIN:
            if (record->event.pressed) {
                select_mode(WIN_BASE);
            }
            return false;
        case MODE_MAC:
            if (record->event.pressed) {
                select_mode(MAC_BASE);
            }
            return false;
        default:
            return true;
    }
}
