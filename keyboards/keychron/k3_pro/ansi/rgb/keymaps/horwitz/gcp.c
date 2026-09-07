#include QMK_KEYBOARD_H
#include <math.h> // for round()
//#include "layers.h"
#include "gcp.h"
#include "layout.h"
#include "colorconst.h"

/*
 * `fn+V` presents a 12x4 rainbow grid of keys and 12 grayscale keys† on the top row (with the remaining keys‡
 * dark)--pressing any one of these keys sets the base layer (e.g., layer 0 when `fn` goes to layer 1) to a solid
 * pattern of that color. The top row (of the four color rows) are primary, secondary, and tertiary colors--in standard
 * order: red (1), orange (3), yellow (2), chartreuse (3), green (1), spring green (3), cyan (2), azure (3), blue (1),
 * violet (3), magenta (2), rose (3) (colors, going down then across, increase in hue by approximately 1/48 (in a range
 * of [0,1)) (w/(HSV) saturation and value both of 1 (in [0,1])). The grayscale keys, left to right, produce an HSV
 * value from 0 to 1 (in a range of [0,1]) (w/(HSV) hue and saturation both of 0 (in [0,1) and [0,1], respectively)).
 *
 * NOTE: The feature creates a fifth layer (layer 4) for the rainbow grid (meant only to be accessed for the purposes
 *       mentioned above). Actual hue values are encoded as integers in [0,255] (scaled from [0,1)).
 *
 * † grayscale mode if off by default--its availability is toggled from the main layer by double tapping X while holding
 *   Fn
 * ‡ End is set to RGB_RED and, when hit, leaves the layer, making no change to the base layer's color
 */

static const uint16_t MIN_COLOR_KEYCODE = COLOR00;
static const uint16_t MAX_COLOR_KEYCODE = GRAY11;

uint8_t color_picker_color_hues[COLOR_PALETTE_SIZE];
RGB color_picker_color_rgbs[COLOR_PALETTE_SIZE];
uint8_t color_picker_gray_intensities[GRAY_PALETTE_SIZE];

bool cPickGrayscaleAvailable = false;

static void dance_grayscale(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) {
        cPickGrayscaleAvailable = !cPickGrayscaleAvailable;
    }
}

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap twice to toggle grayscale availability
    [TD_GRAY] = ACTION_TAP_DANCE_FN(dance_grayscale)
};

void keyboard_post_init_user_gcp(void) {
    /*
      hues:
      0,   5,  11,  16,  21,  27,  32,  37,  43,  48,  53,  59,
     64,  69,  75,  80,  85,  91,  96, 101, 107, 112, 117, 123,
    128, 133, 139, 144, 149, 155, 160, 165, 171, 176, 181, 187,
    192, 197, 203, 208, 213, 219, 224, 229, 235, 240, 245, 251
    */
    for (int i = 0; i < COLOR_PALETTE_SIZE; ++i) {
        // NB: requires COLOR_PALETTE_SIZE < 512 or rvalue for largest i will be 256 (and hues must fall in [0,255])
        color_picker_color_hues[i] = (uint8_t)round(i * HUE_STEPS / (double)COLOR_PALETTE_SIZE);
        HSV hsv = { .h=color_picker_color_hues[i], .s=MAX_COMPONENT, .v=MAX_COMPONENT };
        color_picker_color_rgbs[i] = hsv_to_rgb_nocie(hsv);
    }
    for (int i = 0; i < GRAY_PALETTE_SIZE; ++i) {
        color_picker_gray_intensities[i] = (uint8_t)round(i * MAX_COMPONENT / (double)(GRAY_PALETTE_SIZE - 1));
    }
}

uint8_t color_picker_color_palette_keycodes[COLOR_PALETTE_SIZE] = {
    16, // `    (COLOR00: (1) red)
    31, // TAB  (COLOR01: (5) scarlet)
    46, // CAPS (COLOR02: (4) vermilion)
    60, // LSFT (COLOR03: (5) persimmon)
    17, // 1    (COLOR04: (3) orange)
    32, // Q    (COLOR05: (5) orange peel)
    47, // A    (COLOR06: (4) amber)
    61, // Z    (COLOR07: (5) golden yellow)
    18, // 2    (COLOR08: (2) yellow)
    33, // W    (COLOR09: (5) lemon)
    48, // S    (COLOR10: (4) lime)
    62, // X    (COLOR11: (5) spring bud)
    19, // 3    (COLOR12: (3) chartreuse)
    34, // E    (COLOR13: (5) bright green)
    49, // D    (COLOR14: (4) harlequin)
    63, // C    (COLOR15: (5) neon green)
    20, // 4    (COLOR16: (1) green)
    35, // R    (COLOR17: (5) jade)
    50, // F    (COLOR18: (4) erin)
    64, // V    (COLOR19: (5) emerald)
    21, // 5    (COLOR20: (3) spring green)
    36, // T    (COLOR21: (5) mint)
    51, // G    (COLOR22: (4) aquamarine)
    65, // B    (COLOR23: (5) turquoise)
    22, // 6    (COLOR24: (2) cyan)
    37, // Y    (COLOR25: (5) sky blue)
    52, // H    (COLOR26: (4) capri)
    66, // N    (COLOR27: (5) cornflower)
    23, // 7    (COLOR28: (3) azure)
    38, // U    (COLOR29: (5) cobalt)
    53, // J    (COLOR30: (4) cerulean)
    67, // M    (COLOR31: (5) sapphire)
    24, // 8    (COLOR32: (1) blue)
    39, // I    (COLOR33: (5) iris)
    54, // K    (COLOR34: (4) indigo)
    68, // ,    (COLOR35: (5) veronica)
    25, // 9    (COLOR36: (3) violet)
    40, // O    (COLOR37: (5) amethyst)
    55, // L    (COLOR38: (4) purple)
    69, // .    (COLOR39: (5) phlox)
    26, // 0    (COLOR40: (2) magenta)
    41, // P    (COLOR41: (5) fuchsia)
    56, // ;    (COLOR42: (4) cerise)
    70, // /    (COLOR43: (5) deep pink)
    27, // -    (COLOR44: (3) rose)
    42, // []   (COLOR45: (5) raspberry)
    57, // '    (COLOR46: (4) crimson)
    71  // RSFT (COLOR47: (5) amaranth)
};
// too many elements -> compile error; too few -> silent trailing zeros, so assert the count explicitly
_Static_assert(sizeof(color_picker_color_palette_keycodes) / sizeof(color_picker_color_palette_keycodes[0]) == COLOR_PALETTE_SIZE,
               "color_picker_color_palette_keycodes must have exactly COLOR_PALETTE_SIZE elements");
uint8_t color_picker_gray_palette_keycodes[GRAY_PALETTE_SIZE] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

// returns 0 for COLOR00, 1 for COLOR01, ..., 47 for COLOR47
static uint8_t get_color_picker_color_keycode_index(uint16_t keycode) {
    return keycode - COLOR00;
}

// returns 0 for GRAY00, 1 for GRAY01, ..., 11 for GRAY11
static uint8_t get_color_picker_gray_keycode_index(uint16_t keycode) {
    return keycode - GRAY00;
}

static bool is_color_picker_color_keycode(uint16_t keycode) {
    return (bool)(keycode >= MIN_COLOR_KEYCODE && keycode <= MAX_COLOR_KEYCODE);
}

static enum COLOR_SCHEME get_color_scheme(uint16_t keycode) {
    enum COLOR_SCHEME retval;
    if (keycode >= COLOR00 && keycode <= COLOR47) {
        retval = RGB_SCHEME;
    } else if (keycode >= GRAY00 && keycode <= GRAY11) {
        retval = GRAY_SCHEME;
    } else {
        retval = UNKNOWN_SCHEME;
    }

    return retval;
}

bool process_record_user_gcp(uint16_t keycode, keyrecord_t *record) {
    bool retval = true;
    if (is_color_picker_color_keycode(keycode)) {
        if (record -> event.pressed) {
            rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
            switch (get_color_scheme(keycode)) {
                case RGB_SCHEME:
                    rgb_matrix_sethsv(color_picker_color_hues[get_color_picker_color_keycode_index(keycode)], MAX_COMPONENT, MAX_COMPONENT);
                    break;
                case GRAY_SCHEME:
                    if (cPickGrayscaleAvailable) {
                        rgb_matrix_sethsv(0, 0, color_picker_gray_intensities[get_color_picker_gray_keycode_index(keycode)]);
                    }
                    break;
                case UNKNOWN_SCHEME:
#if DEBUG
                    uprintf("get_color_scheme: unexpected UNKNOWN_SCHEME for keycode %u\n", keycode);
#endif
                    break;
            }
        }
        retval = false;
    }

    return retval;
}

void rgb_matrix_indicators_advanced_user_gcp(void) {
    rgb_matrix_set_color_all(RGB_OFF); // RGB_OFF == RGB_BLACK // set keys not changed below to black
    rgb_matrix_set_color(LED_INDEX_END, RGB_RED); // set End to red // TODO? different color here

    for (int i = 0; i < COLOR_PALETTE_SIZE; ++i) {
        RGB rgb = color_picker_color_rgbs[i];
        rgb_matrix_set_color(color_picker_color_palette_keycodes[i], rgb.r, rgb.g, rgb.b);
    }
    if (cPickGrayscaleAvailable) {
        for (int i = 0; i < GRAY_PALETTE_SIZE; ++i) {
            uint8_t intensity = color_picker_gray_intensities[i];
            rgb_matrix_set_color(color_picker_gray_palette_keycodes[i], intensity, intensity, intensity);
        }
    }
}
