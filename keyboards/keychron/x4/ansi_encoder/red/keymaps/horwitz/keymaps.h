#pragma once

#include "ctrlkeycodes.h" // MODE_WIN, MODE_MAC

// Shortcuts used on the Windows Fn layer (the same definitions as the stock X4 keymaps).
#define KC_TASK LGUI(KC_TAB) // task view
#define KC_FLXP LGUI(KC_E)   // File Explorer

// The Fn layers are macros, named as on the K3 (see users/horwitz/keymaps.h on wireless_playground).

#define LAYOUT_winfn LAYOUT_82_ansi( \
    _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  BL_DOWN,  BL_UP,    KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  _______,            BL_TOGG, \
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______, \
    BL_TOGG,  BL_STEP,  BL_UP,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  MODE_WIN,           _______, \
    _______,  _______,  BL_DOWN,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______, \
    _______,            _______,  _______,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  MODE_MAC,             _______,  _______, \
    _______,  GU_TOGG,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______ \
)

// Same as LAYOUT_winfn except F3/F4 (Mission Control and Launchpad, as on the K3's LAYOUT_macfn) and the Win-lock
// position (Fn + Left Win), which is left transparent: there is no GUI lock on the Mac layer, and selecting Mac mode
// releases any lock that is on (see CLAUDE.md, "Mac/Windows design").
#define LAYOUT_macfn LAYOUT_82_ansi( \
    _______,  KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  BL_DOWN,  BL_UP,    KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  _______,            BL_TOGG, \
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______, \
    BL_TOGG,  BL_STEP,  BL_UP,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  MODE_WIN,           _______, \
    _______,  _______,  BL_DOWN,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______, \
    _______,            _______,  _______,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  MODE_MAC,             _______,  _______, \
    _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______ \
)
