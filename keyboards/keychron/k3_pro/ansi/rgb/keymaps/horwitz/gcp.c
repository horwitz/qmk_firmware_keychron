#include QMK_KEYBOARD_H
#include "layers.h"
#include "gcp.h"    // transitively includes gcp_color.h (color_hue_for_index, gray_intensity_for_index) and colorconst.h
#include "tap_dance.h"
#include "layout.h"

/*
 * `fn+V` presents a 12x4 rainbow grid of keys and 13 (GRAY_PALETTE_SIZE) grayscale keys† on the top row (with the remaining keys‡
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
 * † grayscale mode is on by default; its availability is toggled from the main layer by double tapping X while holding
 *   Fn
 * ‡ End is set to RGB_RED and, when hit, leaves the layer, making no change to the base layer's color
 */

#define MIN_COLOR_KEYCODE COLOR00
#define MAX_COLOR_KEYCODE COLOR47
#define MIN_GRAY_KEYCODE  GRAY00
#define MAX_GRAY_KEYCODE  GRAY12

_Static_assert(MAX_COLOR_KEYCODE - MIN_COLOR_KEYCODE + 1 == COLOR_PALETTE_SIZE,
               "COLOR_PALETTE_SIZE does not match MIN_COLOR_KEYCODE..MAX_COLOR_KEYCODE range");
_Static_assert(MAX_GRAY_KEYCODE - MIN_GRAY_KEYCODE + 1 == GRAY_PALETTE_SIZE,
               "GRAY_PALETTE_SIZE does not match MIN_GRAY_KEYCODE..MAX_GRAY_KEYCODE range");

uint8_t color_picker_color_hues[COLOR_PALETTE_SIZE];
RGB color_picker_color_rgbs[COLOR_PALETTE_SIZE];
uint8_t color_picker_gray_intensities[GRAY_PALETTE_SIZE];
// LED indices for each palette entry; populated at init time by scanning LAYOUT_gcp (see gcp.h for the mapping)
uint8_t color_picker_color_palette_keycodes[COLOR_PALETTE_SIZE];
uint8_t color_picker_gray_palette_keycodes[GRAY_PALETTE_SIZE];

static bool cPickGrayscaleAvailable = true;

#define GCP_ANIM_FRAME_MS 39 // 39ms * 13 frames ≈ 0.5s total

static bool     gcp_anim_active;
static uint8_t  gcp_anim_frame;
static uint16_t gcp_anim_timer;

static void dance_grayscale(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) {
        cPickGrayscaleAvailable = !cPickGrayscaleAvailable;
        gcp_anim_active = true;
        gcp_anim_frame  = 0;
        gcp_anim_timer  = timer_read();
    }
}

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap twice to toggle grayscale availability
    [TD_GRAY] = ACTION_TAP_DANCE_FN(dance_grayscale)
};


static void initialize_gcp_palette_keycodes(const uint16_t keymap[MATRIX_ROWS][MATRIX_COLS]) {
    scan_palette_keycodes(
        &keymap[0][0], &ansi_84_hole_map[0][0], MATRIX_ROWS, MATRIX_COLS,
        MIN_COLOR_KEYCODE, COLOR_PALETTE_SIZE, color_picker_color_palette_keycodes,
        MIN_GRAY_KEYCODE,  GRAY_PALETTE_SIZE,  color_picker_gray_palette_keycodes);
}

void keyboard_post_init_user_gcp(void) {
    initialize_gcp_palette_keycodes(keymaps[GCP]);

    /*
      hues:
      0,   5,  11,  16,  21,  27,  32,  37,  43,  48,  53,  59,
     64,  69,  75,  80,  85,  91,  96, 101, 107, 112, 117, 123,
    128, 133, 139, 144, 149, 155, 160, 165, 171, 176, 181, 187,
    192, 197, 203, 208, 213, 219, 224, 229, 235, 240, 245, 251
    */
    for (int i = 0; i < COLOR_PALETTE_SIZE; ++i) {
        color_picker_color_hues[i] = color_hue_for_index(i);
        HSV hsv = { .h=color_picker_color_hues[i], .s=MAX_COMPONENT, .v=MAX_COMPONENT };
        color_picker_color_rgbs[i] = hsv_to_rgb_nocie(hsv);
    }
    for (int i = 0; i < GRAY_PALETTE_SIZE; ++i) {
        color_picker_gray_intensities[i] = gray_intensity_for_index(i);
    }
}

// returns 0 for MIN_COLOR_KEYCODE, 1 for the next, ..., COLOR_PALETTE_SIZE-1 for MAX_COLOR_KEYCODE
static uint8_t get_color_picker_color_keycode_index(uint16_t keycode) {
    return keycode - MIN_COLOR_KEYCODE;
}

// returns 0 for MIN_GRAY_KEYCODE, 1 for the next, ..., GRAY_PALETTE_SIZE-1 for MAX_GRAY_KEYCODE
static uint8_t get_color_picker_gray_keycode_index(uint16_t keycode) {
    return keycode - MIN_GRAY_KEYCODE;
}

static bool is_gcp_keycode(uint16_t keycode) {
    return (bool)(keycode >= MIN_COLOR_KEYCODE && keycode <= MAX_GRAY_KEYCODE);
}

static enum COLOR_SCHEME get_color_scheme(uint16_t keycode) {
    enum COLOR_SCHEME retval;
    if (keycode >= MIN_COLOR_KEYCODE && keycode <= MAX_COLOR_KEYCODE) {
        retval = RGB_SCHEME;
    } else if (keycode >= MIN_GRAY_KEYCODE && keycode <= MAX_GRAY_KEYCODE) {
        retval = GRAY_SCHEME;
    } else {
        retval = UNKNOWN_SCHEME;
    }

    return retval;
}

bool process_record_user_gcp(uint16_t keycode, const keyrecord_t *record) {
    bool retval = true;
    if (is_gcp_keycode(keycode)) {
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
    rgb_matrix_set_color(LED_INDEX_END, RGB_RED); // set End to red (abort key)

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

// called every render frame from keymap.c; paints a grayscale-toggle animation on the top row
void rgb_matrix_indicators_advanced_user_gcp_anim(void) {
    if (gcp_anim_active) {
        if (timer_elapsed(gcp_anim_timer) >= GCP_ANIM_FRAME_MS) {
            ++gcp_anim_frame;
            gcp_anim_timer = timer_read();
        }

        int lit_count;
        if (gcp_anim_frame >= GRAY_PALETTE_SIZE) {
            gcp_anim_active = false;
            lit_count = 0;
        } else if (cPickGrayscaleAvailable) {
            lit_count = gcp_anim_frame + 1;               // 1..GRAY_PALETTE_SIZE
        } else {
            lit_count = (int)GRAY_PALETTE_SIZE - gcp_anim_frame; // GRAY_PALETTE_SIZE..1
        }

        rgb_matrix_set_color_all(RGB_OFF);
        for (int i = 0; i < lit_count; ++i) {
            uint8_t intensity = color_picker_gray_intensities[i];
            rgb_matrix_set_color(color_picker_gray_palette_keycodes[i], intensity, intensity, intensity);
        }
    }
}
