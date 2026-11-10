#pragma once

// Layer order is the stock X4 order (WIN_BASE 0, WIN_FN 1), so the Windows layers keep their stock indices and a fresh
// or erased board boots into Windows (layer 0). The K3's horwitz keymap puts the Mac layers first; this is a
// deliberate difference.
enum layers {
    WIN_BASE,
    WIN_FN,
    MAC_BASE,
    MAC_FN,
};
