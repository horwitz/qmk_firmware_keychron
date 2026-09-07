#include QMK_KEYBOARD_H
#include "layers.h"
#include "fnhi.h"
#include "keymaps.h"

/*
 * When the `fn` key is pressed, the keys with new keycodes (i.e., not `_______`) will light up in the color
 * complementary to the color† of layer 0‡; the remaining keys will retain the lighting behavior from before the `fn`
 * key was pressed.
 *
 * † main(?) color
 * ‡ or is it the layer from which one came (e.g., starting at a base layer of 2, will these be complementary to layer
 *   2's color†)?
*/

// for any given layer, layers_used_indices[layer] should not exceed RGB_MATRIX_LED_COUNT in size, since each index
// found in the array should represent a key with a distinct light (see also: doc for
// initialize_layer_used_indices_inner)
// keycode indices (0–83) are hardcoded throughout this file based on the K3 Pro's 84-key layout
_Static_assert(RGB_MATRIX_LED_COUNT == 84, "fnhi.c assumes RGB_MATRIX_LED_COUNT == 84 (K3 Pro ANSI layout)");
char layers_used_indices[DYNAMIC_KEYMAP_LAYER_COUNT][RGB_MATRIX_LED_COUNT];
int layer_used_indices_size[DYNAMIC_KEYMAP_LAYER_COUNT];

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
int initialize_layer_used_indices_inner(int layer, char* layer_used_indices, const uint16_t keymap[MATRIX_ROWS][MATRIX_COLS]) {
    int lui_i = 0;
    int offset = 0;
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

void initialize_layer_used_indices(int layer, const uint16_t keymap[MATRIX_ROWS][MATRIX_COLS]) {
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

// when fn is held down, set lights yellow on keys whose behavior changed from base layer; inspired by
// https://www.reddit.com/r/olkb/comments/kpro3p/comment/h3nb56h
void rgb_matrix_indicators_advanced_user_fnhi(uint8_t layer) {
//    rgb_matrix_set_color_all(RGB_BLUE); // uncomment to have transparent keys appear solid blue
    RGB rgb = hsv_to_rgb_nocie(rgb_matrix_get_hsv());
    // TODO what exactly _is_ the color returned by rgb_matrix_get_hsv()? is this some overall color (as opposed
    //      to per-key ones)?
    // TODO? if all r,g,b are close to 255/2, the complement will be close to (and hard to discern from) the
    //       original color (should this be changed?)
    RGB complement_rgb = { .r = 255 - rgb.r, .g = 255 - rgb.g, .b = 255 - rgb.b };
    for (int i = 0; i < layer_used_indices_size[layer]; ++i) {
        rgb_matrix_set_color(
            layers_used_indices[layer][i],
            complement_rgb.r,
            complement_rgb.g,
            complement_rgb.b
        );
    }
}
