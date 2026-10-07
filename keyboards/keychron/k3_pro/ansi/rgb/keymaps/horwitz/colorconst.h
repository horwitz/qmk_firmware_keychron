#pragma once

#define HUE_STEPS     256 // number of distinct hue values; hues are integers in [0, HUE_STEPS)
#define MAX_COMPONENT 255 // maximum value for any RGB or HSV component
#define HUE_SEXTANTS    6 // number of sextants in the HSV color wheel
// NB: HUE_STEPS and MAX_COMPONENT are plain integer literals; cast the divisor to double when used in
//     floating-point expressions (e.g., round(i * HUE_STEPS / (double)n)) to avoid integer division