/* Copyright 202 @ Keychron (https://www.keychron.com)
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

// Modifications Copyright 2026 Jeremy Horwitz

#pragma once

#include "ctrlkeycodes.h" // MODE_WIN, MODE_MAC

// Shortcuts used on the Windows Fn layer (the same definitions as the stock X4 keymaps).
#define KC_TASK LGUI(KC_TAB) // task view
#define KC_FLXP LGUI(KC_E)   // File Explorer

// The two Fn layers are macros, which keymap.c puts into its list of layers.
//
// Navigation (both Fn layers, plain keycodes): the X4 has no End key, so Fn + the arrow keys give Home (Left),
// PgUp (Up), PgDn (Down) and End (Right), as on a laptop, and Fn + Del is a second End. Home, PgUp and PgDn also have
// their own keys.

#define LAYOUT_winfn LAYOUT_82_ansi( \
    _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  BL_DOWN,  BL_UP,    KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_END,             BL_TOGG, \
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______, \
    BL_TOGG,  BL_STEP,  BL_UP,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  MODE_WIN,           _______, \
    _______,  _______,  BL_DOWN,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______, \
    _______,            _______,  _______,  _______,  SHOW_VER,  _______,  NK_TOGG,  _______,  _______,  _______,  MODE_MAC,             _______,  KC_PGUP, \
    _______,  GU_TOGG,  _______,                                _______,                                _______,  _______,    _______,  KC_HOME,  KC_PGDN,  KC_END  \
)

// Same as LAYOUT_winfn except F3/F4 (Mission Control and Launchpad, as on the K3's LAYOUT_macfn) and the Win-lock
// position (Fn + Left Win), which is left transparent: there is no GUI lock on the Mac layer, and selecting Mac mode
// releases any lock that is on.
#define LAYOUT_macfn LAYOUT_82_ansi( \
    _______,  KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  BL_DOWN,  BL_UP,    KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_END,             BL_TOGG, \
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______, \
    BL_TOGG,  BL_STEP,  BL_UP,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  MODE_WIN,           _______, \
    _______,  _______,  BL_DOWN,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______, \
    _______,            _______,  _______,  _______,  SHOW_VER,  _______,  NK_TOGG,  _______,  _______,  _______,  MODE_MAC,             _______,  KC_PGUP, \
    _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  KC_HOME,  KC_PGDN,  KC_END  \
)
