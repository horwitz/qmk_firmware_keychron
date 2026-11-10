#pragma once

// Custom keycodes. SAFE_RANGE (= QK_USER) is safe on the X4: nothing in the X4 tree uses it, and this keymap does not
// include Keychron's common code, whose keycodes (KC_LOPTN, ...) sit in the QK_KB range. The K3 userspace has to use
// NEW_SAFE_RANGE instead because it does include that code.
//
// If VIA/Launcher support is ever added (see TASKS.md), VIA expects custom keycodes in the QK_KB range and declared in its
// JSON, so these would have to move.
enum ctrl_keycodes {
    MODE_WIN = SAFE_RANGE, // select Windows mode (persistent): Fn + \ (the Windows directory separator)
    MODE_MAC,              // select Mac mode (persistent): Fn + / (the macOS directory separator)
};
