#pragma once

// Custom keycodes. SAFE_RANGE (= QK_USER) is safe on the X4: nothing in the X4 tree uses it, and this keymap does not
// include Keychron's common code, whose keycodes (KC_LOPTN, ...) sit in the QK_KB range. The K3 userspace has to use
// NEW_SAFE_RANGE instead because it does include that code.
//
// VIA/Launcher support was decided against (CLAUDE.md, task 17). If it is ever added, VIA expects custom keycodes in the
// QK_KB range (QK_KB_0 to QK_KB_10 are taken by Keychron's), declared in its JSON, so these would have to move.
enum ctrl_keycodes {
    MODE_WIN = SAFE_RANGE, // select Windows mode (persistent): Fn + \ (the Windows directory separator)
    MODE_MAC,              // select Mac mode (persistent): Fn + / (the macOS directory separator)
};
