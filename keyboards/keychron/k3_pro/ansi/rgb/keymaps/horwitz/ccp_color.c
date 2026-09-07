#include "ccp_color.h"
#include <math.h>

HSV rgb_to_hsv(RGB rgb) {
    HSV hsv;

    uint8_t rgbMin = min(rgb.r, min(rgb.g, rgb.b));
    uint8_t rgbMax = max(rgb.r, max(rgb.g, rgb.b));

    hsv.v = rgbMax;
    uint8_t chroma = rgbMax - rgbMin;
    if (chroma == 0) {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    // hsv.v != 0 since chroma != 0
    hsv.s = (uint8_t)round(MAX_COMPONENT * chroma / (double)hsv.v);

    double unscaledSextupledH;
    if (rgbMax == rgb.r) {
        unscaledSextupledH = 1.0 * (rgb.g - rgb.b) / chroma;
    } else if (rgbMax == rgb.g) {
        unscaledSextupledH = (1.0 * (rgb.b - rgb.r) / chroma) + 2;
    } else if (rgbMax == rgb.b) {
        unscaledSextupledH = (1.0 * (rgb.r - rgb.g) / chroma) + 4;
    } else { // impossible case
        unscaledSextupledH = 0;
    }
    hsv.h = (uint8_t)round(unscaledSextupledH * MAX_COMPONENT / HUE_SEXTANTS);

    return hsv;
}
