#include QMK_KEYBOARD_H
#include "layers.h"
#include "fnhi.h"
#include "keymaps.h"

/*
 * When the `fn` key is pressed, the keys with new keycodes (i.e., not `_______`) will light up at the complementary
 * hue of the keyboard's current global solid color (rgb_matrix_get_hsv()), always at full saturation and brightness;
 * the remaining keys will retain the lighting behavior from before the `fn` key was pressed.
 *
 * See the comment on complement_hsv in rgb_matrix_indicators_advanced_user_fnhi for the rationale.
*/

// for any given layer, layers_used_indices[layer] should not exceed RGB_MATRIX_LED_COUNT in size, since each index
// found in the array should represent a key with a distinct light (see also: doc for
// initialize_layer_used_indices_inner)
// keycode indices (0–83) are hardcoded throughout this file based on the K3 Pro's 84-key layout
_Static_assert(RGB_MATRIX_LED_COUNT == 84, "fnhi.c assumes RGB_MATRIX_LED_COUNT == 84 (K3 Pro ANSI layout)");
uint8_t layers_used_indices[DYNAMIC_KEYMAP_LAYER_COUNT][RGB_MATRIX_LED_COUNT];
uint8_t layer_used_indices_size[DYNAMIC_KEYMAP_LAYER_COUNT];

/*
 * sets the values in [layer_used_indices] to the indices (in increasing order; a subset of 0-83) that are used in the
 * given [layer] ("used" = not _______) and returns how many values were set. (e.g., if keys at index 0, 5, 83 are the
 * only ones set, layer_used_indices will start { 0, 5, 83, ... } (followed by 0s) and
 * [initialize_layer_used_indices_inner]
 * will return 3.)
 *
 * [offset] is used below because [rgb_matrix_set_color]'s first argument appears to be based on an index of keys _not_
 * including KC_NO (== XXXXXXX)--there are 96 (=[MATRIX_ROWS] * [MATRIX_COLS]) keycodes in each keymaps[layer], but
 * (apparently) the non-KC_NO keys are indexed 0-83 (and there are 96-84=12 instances of KC_NO (per layer))... see
 * LAYOUT_ansi_84's definition in obj_keychron_k3_pro_ansi_rgb/src/default_keyboard.h
 */
static uint8_t initialize_layer_used_indices_inner(uint8_t layer, uint8_t* layer_used_indices, const uint16_t keymap[MATRIX_ROWS][MATRIX_COLS]) {
    uint8_t lui_i = 0;
    uint8_t offset = 0;
    for (int i = 0; i < MATRIX_ROWS * MATRIX_COLS; ++i) {
        int col = i % MATRIX_COLS;
        int row = i / MATRIX_COLS;
        switch (keymap[row][col]) {
            case _______:
                break;
            case KC_NO: // == XXXXXXX
                ++offset;
                break;
            default:
                layer_used_indices[lui_i++] = i - offset;
                break;
        }
    }

    return lui_i;
}

static void initialize_layer_used_indices(uint8_t layer, const uint16_t keymap[MATRIX_ROWS][MATRIX_COLS]) {
    layer_used_indices_size[layer] = initialize_layer_used_indices_inner(layer, layers_used_indices[layer], keymap);
}

// TODO? derive layout from layer (e.g., LAYOUT_macfn from MAC_FN)
void keyboard_post_init_user_fnhi(void) {
    // TODO(?) do this differently
    // TODO(?) put these in a header file (e.g., keymaps.h)
    const uint16_t macFnLayer[MATRIX_ROWS][MATRIX_COLS] = LAYOUT_macfn;
    const uint16_t winFnLayer[MATRIX_ROWS][MATRIX_COLS] = LAYOUT_winfn;

    initialize_layer_used_indices(MAC_FN, macFnLayer);
    initialize_layer_used_indices(WIN_FN, winFnLayer);
}

// v used for the complement highlight when the base color is bright (v > MAX_COMPONENT/2);
// dim but non-zero so the highlight is visible as a colored light rather than appearing off
static const uint8_t COMPLEMENT_DIM_V = 64;

// when fn is held down, highlight keys whose behavior changed from the base layer; inspired by
// https://www.reddit.com/r/olkb/comments/kpro3p/comment/h3nb56h
void rgb_matrix_indicators_advanced_user_fnhi(uint8_t layer) {
//    rgb_matrix_set_color_all(RGB_BLUE); // uncomment to have transparent keys appear solid blue
    HSV hsv = rgb_matrix_get_hsv();

    // goal: highlight FN keys in a color that is maximally distinguishable from the base color.
    //
    // the naive approach of RGB componentwise inversion, (255-r, 255-g, 255-b), fails for
    // near-gray base colors: e.g., (128,128,128) inverts to (127,127,127)--nearly identical.
    //
    // a strict HSV complement—shifting only the hue by 128, keeping s and v the same—avoids that
    // specific failure but introduces another: for dark base colors (low v), the highlight is
    // equally dark and may not stand out against the keyboard background.
    //
    // instead, we use:
    //   h' = (h + HUE_STEPS/2) % HUE_STEPS  — opposite hue on the color wheel
    //   s' = MAX_COMPONENT                  — fully saturated, regardless of base saturation
    //   v' = (v > MAX_COMPONENT/2) ? COMPLEMENT_DIM_V : MAX_COMPONENT  — value flip: dim for bright base, bright for dark
    //
    // the value flip ensures contrast in both directions: a bright base (e.g., white at v=255) gets
    // a dim but visible colored highlight; a dark base gets a bright highlight. always fully saturated
    // means the highlight is never a washed-out near-gray.
    //
    // for achromatic bases (s=0: gray, white, black), hue is geometrically undefined in HSV, so h'
    // is the complement of whatever h happens to be stored. when the color was set via CCP or GCP,
    // that stored h is 0, so h'=128 (cyan)--a predictable, vivid result. when set via QMK's
    // built-in RGB controls, the stored h may differ.
    HSV complement_hsv = { .h = (uint8_t)((hsv.h + HUE_STEPS / 2) % HUE_STEPS), .s = MAX_COMPONENT, .v = (uint8_t)(hsv.v > MAX_COMPONENT / 2 ? COMPLEMENT_DIM_V : MAX_COMPONENT) };
    RGB complement_rgb = hsv_to_rgb_nocie(complement_hsv);

    for (int i = 0; i < layer_used_indices_size[layer]; ++i) {
        rgb_matrix_set_color(
            layers_used_indices[layer][i],
            complement_rgb.r,
            complement_rgb.g,
            complement_rgb.b
        );
    }
}
