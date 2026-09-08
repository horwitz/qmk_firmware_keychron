#pragma once

enum ctrl_keycodes {
// [GCP]
    // if COLOR00 were set to SAFE_RANGE (instead of NEW_SAFE_RANGE), collisions would occur (e.g.,
    // KC_LOPTN == COLOR00, ...)
    COLOR00 = NEW_SAFE_RANGE, COLOR01, COLOR02, COLOR03,
        COLOR04, COLOR05, COLOR06, COLOR07,
        COLOR08, COLOR09, COLOR10, COLOR11,
        COLOR12, COLOR13, COLOR14, COLOR15,
        COLOR16, COLOR17, COLOR18, COLOR19,
        COLOR20, COLOR21, COLOR22, COLOR23,
        COLOR24, COLOR25, COLOR26, COLOR27,
        COLOR28, COLOR29, COLOR30, COLOR31,
        COLOR32, COLOR33, COLOR34, COLOR35,
        COLOR36, COLOR37, COLOR38, COLOR39,
        COLOR40, COLOR41, COLOR42, COLOR43,
        COLOR44, COLOR45, COLOR46, COLOR47,
        GRAY00, GRAY01, GRAY02, GRAY03,
        GRAY04, GRAY05, GRAY06, GRAY07,
        GRAY08, GRAY09, GRAY10, GRAY11,
        GRAY12,

// [CCP] // meta CCP
        TOCCP,
        CCPABORT, // abort CCP without applying color; returns to whichever base layer was active
// [CCP]
        RHI, RLI, GHI, GLI, BHI, BLI,
        RHD, RLD, GHD, GLD, BHD, BLD,
        CCPSET,
};