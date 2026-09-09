#include QMK_KEYBOARD_H
#include "layers.h"
#include "ccp.h"
//#include "ctrlkeycodes.h"

/*
 * Used to set the base layer to a solid pattern of any 24-bit color. The picker's current color is shown on each key of
 * the 3x3 grid  IOP KL; ,./  . Hitting Enter (which will be white) accepts the picker's current color, setting the base
 * layer to a solid pattern of that color. Hitting End aborts, making no change in the base layer's color. A & S show
 * the red contribution (the two highest bytes in the 6-byte RGB representation; note that A and S's colors are the
 * same), D & F show the green contribution, and G & H show the blue contribution. Q increases the red contribution by
 * 16 and W increases it by 1--in both cases stopping at the max of 255. Similarly Z and X respectively lower the red
 * contribution by 16 and 1, stopping at the min of 0. Similarly E,R,C,V raise/lower green by 16 or 1; T,Y,B,N
 * raise/lower blue by 16 or 1. Q,E,T show what would happen if red, green, or blue (respectively) were maxed out with
 * the current settings kept for the other components; Z,C,B show what would happen if red, green, or blue
 * (respectively) were set to 0 with the current settings kept for the other components; W,R,Y show what would happen if
 * red, green, or blue (respectively) were brought up to the nearest multiple of 16 above the current red, green, or
 * blue (as appropriate) value (unless that's 256, in which case 255 is used); X,V,N show what would happen if red,
 * green, or blue (respectively) were brought down to the nearest multiple of 16 below the current red, green, or blue
 * (as appropriate) value. When any of QWERTYZXCVBN is pressed, the 16 top-row characters (Esc, F1, F2, ...) light up to
 * show the current modifier's value, in { 0, 1, ..., 15 }: e.g., if Q is pressed and red's high byte is at 5, it will
 * go up to 6 and the first 7 keys in the top row (Esc, F1, ..., F6) will light up. The representation is 0-based--i.e.,
 * 1 light represents a 0, 2 lights represent a 1, ..., all 16 lights represent a 15.
 *
 * NOTE: The feature creates a fourth layer (layer 3) (meant only to be accessed for the purposes mentioned above).
 */

static const uint16_t MIN_CCP_CHANGE_KEYCODE = RHI;
static const uint16_t MAX_CCP_CHANGE_KEYCODE = CCPSET;

// NB: does NOT include TOCCP (which is not on the CCP layer)
static bool is_ccp_change_keycode(uint16_t keycode) {
    return (bool)(keycode >= MIN_CCP_CHANGE_KEYCODE && keycode <= MAX_CCP_CHANGE_KEYCODE);
}

static int8_t index_in_byte = -1; // 0-15 value equal to the last hex value edited (one of RH, RL, GH, GL, BH, BL)
static enum RGB_COLOR last_color_edited = RED; // only read when index_in_byte >= 0; initial value of RED is (arbitray and) ignored

static RGB ccpRgb;

// Used to avoid round-trip drift on re-entry into CCP. Set by CCPSET; see TOCCP for usage.
static RGB  lastCcpSetRgb;
static HSV  lastCcpSetHsv;
static bool hasCcpSetRgb = false;

static ccp_key_t get_ccp_key(uint16_t keycode) {
    ccp_key_t ccp_key;

    switch (keycode) {
        case RHI:
        case RHD:
        case RLI:
        case RLD:
            ccp_key.color = RED;
            break;
        case GHI:
        case GHD:
        case GLI:
        case GLD:
            ccp_key.color = GREEN;
            break;
        case BHI:
        case BHD:
        case BLI:
        case BLD:
            ccp_key.color = BLUE;
            break;
    }

    switch (keycode) {
        case RHI:
        case RHD:
        case GHI:
        case GHD:
        case BHI:
        case BHD:
            ccp_key.nibbleLevel = HIGH;
            break;
        case RLI:
        case RLD:
        case GLI:
        case GLD:
        case BLI:
        case BLD:
            ccp_key.nibbleLevel = LOW;
            break;
    }

    switch (keycode) {
        case RHI:
        case RLI:
        case GHI:
        case GLI:
        case BHI:
        case BLI:
            ccp_key.deltaDir = INC;
            break;
        case RHD:
        case RLD:
        case GHD:
        case GLD:
        case BHD:
        case BLD:
            ccp_key.deltaDir = DEC;
            break;
    }

    return ccp_key;
}

layer_state_t layer_state_set_user_ccp(layer_state_t state) {
    if (get_highest_layer(state) == CCP) {
        index_in_byte = -1; // clear the white lights from the top row
    }

    return state;
}

bool process_record_user_ccp(uint16_t keycode, const keyrecord_t *record) {
    bool retval;

    if (is_ccp_change_keycode(keycode)) {
        if (record -> event.pressed) {
            if (keycode == CCPSET) {
                rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
                HSV hsv = rgb_to_hsv(ccpRgb);
#if DEBUG
                uprintf("CCPSET: ccpRgb=(%d,%d,%d) -> hsv=(%d,%d,%d)\n", ccpRgb.r, ccpRgb.g, ccpRgb.b, hsv.h, hsv.s, hsv.v);
#endif
                rgb_matrix_sethsv(hsv.h, hsv.s, hsv.v);
                lastCcpSetRgb = ccpRgb;
                lastCcpSetHsv = hsv;
                hasCcpSetRgb  = true;
                layer_off(CCP);
            } else {
#if DEBUG
                uprintf("keycode-RHI: %2u\n", keycode - RHI);
                uprintf(">> ccpRgb: (%2u,%2u,%2u)\n", ccpRgb.r, ccpRgb.g, ccpRgb.b);
#endif
                // if RHI, then ccpRgb.r += 16, index_in_byte = ccpRgb.r/16
                // if RHD, then ccpRgb.r -= 16, index_in_byte = ccpRgb.r/16
                // if RLI, then ++(ccpRgb.r), index_in_byte = ccpRgb.r % 16
                // ...
                // if BLD, then --(ccpRgb.b), index_in_byte = ccpRgb.b % 16
                uint8_t component; // whichever of ccpRgb.r, .g, or .b is going to change

                ccp_key_t ccp_key = get_ccp_key(keycode);

                switch (ccp_key.color) {
                    case RED:
                        component = ccpRgb.r;
                        break;
                    case GREEN:
                        component = ccpRgb.g;
                        break;
                    case BLUE:
                        component = ccpRgb.b;
                        break;
                    default:
                        component = 0; // impossible case
                }

                uint8_t absDelta;
                switch (ccp_key.nibbleLevel) {
                    case HIGH:
                        absDelta = HIGH_NIBBLE_DELTA;
                        break;
                    case LOW:
                        absDelta = 1;
                        break;
                }
                switch (ccp_key.deltaDir) {
                    case INC:
                        component = addBounded(component, absDelta);
                        break;
                    case DEC:
                        component = subtractBounded(component, absDelta);
                        break;
                }

                switch (ccp_key.nibbleLevel) {
                    case HIGH:
                        index_in_byte = (int8_t)(component / HIGH_NIBBLE_DELTA);
                        break;
                    case LOW:
                        index_in_byte = (int8_t)(component % HIGH_NIBBLE_DELTA);
                        break;
                }
                last_color_edited = ccp_key.color;

#if DEBUG
                uprintf("before: (%2u,%2u,%2u)\n", ccpRgb.r, ccpRgb.g, ccpRgb.b);
#endif
                switch (ccp_key.color) {
                    case RED:
                        ccpRgb.r = component;
                        break;
                    case GREEN:
                        ccpRgb.g = component;
                        break;
                    case BLUE:
                        ccpRgb.b = component;
                        break;
                }
#if DEBUG
                uprintf("after: (%2u,%2u,%2u)\n", ccpRgb.r, ccpRgb.g, ccpRgb.b);
#endif

            }
#if DEBUG
            uprintf("<< ccpRgb: (%2u,%2u,%2u)\n", ccpRgb.r, ccpRgb.g, ccpRgb.b);
            uprintf("iib: %2u\n", index_in_byte);
#endif
        }
        retval = false;
    // [CCP]
    } else if (keycode == TOCCP) {
        // CCP works in RGB space because that is the familiar model for users (e.g., HTML hex codes).
        // The hardware API, however, stores and applies color in HSV (rgb_matrix_sethsv writes to
        // EEPROM). This requires two conversions:
        //   - on CCP entry (here): HSV -> RGB via hsv_to_rgb_nocie, to seed ccpRgb from the stored color
        //   - on CCP commit (CCPSET, above): RGB -> HSV via rgb_to_hsv, to write back to the hardware
        //
        // These two functions are not exact inverses. hsv_to_rgb_nocie uses integer arithmetic
        // (quantization loss) and rgb_to_hsv uses floating-point with round(). A round-trip
        // HSV -> RGB -> HSV can introduce a small error (typically ±1 per component) that accumulates
        // across repeated CCP sessions. e.g.:
        //   rgb (255,0,255) -> hsv (213,255,255) -> rgb (255,0,252)
        //   rgb (255,0,252) -> hsv (214,255,255) -> rgb (255,0,246)
        //   ... (blue channel drifts ~6 units per round-trip)
        //
        // Fix: if the current hardware HSV matches the HSV we committed in the last CCPSET, reseed
        // from lastCcpSetRgb (the exact RGB we committed) rather than converting from HSV. This
        // eliminates drift for re-entry after a CCP session. If the hardware HSV differs (color was
        // changed externally, e.g. via GCP), we fall back to hsv_to_rgb_nocie -- unavoidably lossy
        // since the hardware only stores HSV, but it only happens once (no accumulation).
        HSV currentHsv = rgb_matrix_get_hsv();
        if (hasCcpSetRgb &&
                currentHsv.h == lastCcpSetHsv.h &&
                currentHsv.s == lastCcpSetHsv.s &&
                currentHsv.v == lastCcpSetHsv.v) {
            ccpRgb = lastCcpSetRgb;
        } else {
            ccpRgb = hsv_to_rgb_nocie(currentHsv);
        }
#if DEBUG
        uprintf("setting ccpRgb: (%d,%d,%d)\n", ccpRgb.r, ccpRgb.g, ccpRgb.b);
#endif
        layer_on(CCP);
        retval = false;
    // [CCP]
    } else if (keycode == CCPABORT) {
        if (record->event.pressed) {
            layer_off(CCP);
        }
        retval = false;
    } else {
        retval = true; // process all other keycodes normally
    }

    return retval;
}

void rgb_matrix_indicators_advanced_user_ccp(void) {
        // set A,S to R level; D,F to G level; G,H to B level; set ENTER white; set some of top row to white (see
        // below); set QWERTY ZXCVBN as described above; set all else black
        rgb_matrix_set_color_all(RGB_OFF); // RGB_OFF == RGB_BLACK // set keys not changed below to black
        // ESC currently used for top-row 0-15 readout, so we couldn't use it as the abort key (at least we couldn't
        // color it to _signify_ that it's the abort key), so we use the End key as the abort key
        rgb_matrix_set_color(LED_INDEX_END, RGB_RED); // set End to red (abort key)
#if DEBUG
        uprintf("R (AS): %2u / G (DF): %2u / B (GH): %2u\n", ccpRgb.r, ccpRgb.g, ccpRgb.b);
#endif

        rgb_matrix_set_color(LED_INDEX_Q, 255, ccpRgb.g, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_W, (ccpRgb.r / HIGH_NIBBLE_DELTA) * HIGH_NIBBLE_DELTA + (HIGH_NIBBLE_DELTA - 1), ccpRgb.g, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_E, ccpRgb.r, 255, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_R, ccpRgb.r, (ccpRgb.g / HIGH_NIBBLE_DELTA) * HIGH_NIBBLE_DELTA + (HIGH_NIBBLE_DELTA - 1), ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_T, ccpRgb.r, ccpRgb.g, 255);
        rgb_matrix_set_color(LED_INDEX_Y, ccpRgb.r, ccpRgb.g, (ccpRgb.b / HIGH_NIBBLE_DELTA) * HIGH_NIBBLE_DELTA + (HIGH_NIBBLE_DELTA - 1));

        rgb_matrix_set_color(LED_INDEX_I,    ccpRgb.r, ccpRgb.g, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_O,    ccpRgb.r, ccpRgb.g, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_P,    ccpRgb.r, ccpRgb.g, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_K,    ccpRgb.r, ccpRgb.g, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_L,    ccpRgb.r, ccpRgb.g, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_SCLN, ccpRgb.r, ccpRgb.g, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_COMM, ccpRgb.r, ccpRgb.g, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_DOT,  ccpRgb.r, ccpRgb.g, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_SLSH, ccpRgb.r, ccpRgb.g, ccpRgb.b);

        rgb_matrix_set_color(LED_INDEX_A, ccpRgb.r, 0, 0);
        rgb_matrix_set_color(LED_INDEX_S, ccpRgb.r, 0, 0);
        rgb_matrix_set_color(LED_INDEX_D, 0, ccpRgb.g, 0);
        rgb_matrix_set_color(LED_INDEX_F, 0, ccpRgb.g, 0);
        rgb_matrix_set_color(LED_INDEX_G, 0, 0, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_H, 0, 0, ccpRgb.b);

        rgb_matrix_set_color(LED_INDEX_Z, 0, ccpRgb.g, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_X, (ccpRgb.r / HIGH_NIBBLE_DELTA) * HIGH_NIBBLE_DELTA, ccpRgb.g, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_C, ccpRgb.r, 0, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_V, ccpRgb.r, (ccpRgb.g / HIGH_NIBBLE_DELTA) * HIGH_NIBBLE_DELTA, ccpRgb.b);
        rgb_matrix_set_color(LED_INDEX_B, ccpRgb.r, ccpRgb.g, 0);
        rgb_matrix_set_color(LED_INDEX_N, ccpRgb.r, ccpRgb.g, (ccpRgb.b / HIGH_NIBBLE_DELTA) * HIGH_NIBBLE_DELTA);

        rgb_matrix_set_color(LED_INDEX_ENT, RGB_WHITE);

#if DEBUG
        if (index_in_byte >= 0) {
            uprintf("setting 0-%2u to white\n", index_in_byte);
        }
#endif
        // if index_in_byte >= 0, color ESC (in last color edited; 0*17 brightness)
        // and if index_in_byte >= 1, color F1 (1*17 brightness)
        // ...
        // and if index_in_byte >= 15, color RGB_MOD (15*17=255 brightness)
        for (int top_row_keycode = 0; top_row_keycode <= index_in_byte; ++top_row_keycode) {
            uint8_t v = (uint8_t)(top_row_keycode * 17); // top_row_keycode/15 * 255 (exact since 15*17=255)
            switch (last_color_edited) {
                case RED:   rgb_matrix_set_color(top_row_keycode, v, 0, 0); break;
                case GREEN: rgb_matrix_set_color(top_row_keycode, 0, v, 0); break;
                case BLUE:  rgb_matrix_set_color(top_row_keycode, 0, 0, v); break;
            }
        }
}
