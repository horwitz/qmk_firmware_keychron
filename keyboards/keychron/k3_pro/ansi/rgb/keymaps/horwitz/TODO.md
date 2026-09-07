* `TO()` vs. `TG()`
* what exactly _is_ the color returned by `rgb_matrix_get_hsv()`\? is this some overall color \(as opposed to per-key
  ones\)\?
* `uint16_t` vs. `int` \(etc.\) (all values fit in [0,255]; using uint8_t makes the intent explicit and saves memory; explicit (uint8_t) casts on round() assignments suppress implicit double -> uint8_t conversion warnings)
* make any use of `(RGB|HSV)_(AZURE|BLUE|...)` from `color.h`\?
* `process_record_user` vs. `rgb_matrix_indicators_advanced_user`
* `[cg]cp.h` \(`LAYOUT_[cg]cp`\) \(etc.\): construct as a value instead of a #define
* `gcp.h` \(`TD_GRAY`\): define somewhere aside from `gcp.h`\?
* `([cg]cp|fnhi).c` \(`rgb_matrix_indicators_advanced_user_([cg]cp|fnhi)`\): return `bool` instead of `void`\?
* `ccp.c`: do something fancier than `RGB_WHITE` with keycodes 0–15 \(maybe something state dependent\)\?
* `fnhi.c` \(`initialize_layer_used_indices`\): derive `layout` from `layer`\(\?\)
* `fnhi.c` \(`keyboard_post_init_user_fnhi`\): do the `(mac|win)FnLayer` stuff differently \(at least move some of it to
  a header file\?\)
* `fnhi.c` \(`rgb_matrix_indicators_advanced_user_fnhi`\): if all `r`,`g`,`b` are close to 255/2, the complement will be
  close to \(and hard to discern from\) the original color \(should this be changed\?\)
* `keymap.c`: should `rgb_matrix_indicators_advanced_user` always return `true`\?
* `rgb_to_hsv` and `hsv_to_rgb_nocie` are not inverses of each other
    * rgb 255 0 255 -> hsv 213 255 255 -> rgbBack 255 0 252
    * rgb 255 0 252 -> hsv 214 255 255 -> rgbBack 255 0 246
    * rgb 255 0 246 -> hsv 215 255 255 -> rgbBack 255 0 240
    * rgb 255 0 240 -> hsv 216 255 255 -> rgbBack 255 0 234
    * rgb 255 0 234 -> hsv 217 255 255 -> rgbBack 255 0 228
    * rgb 255 0 228 -> hsv 218 255 255 -> rgbBack 255 0 222
    * rgb 255 0 222 -> hsv 219 255 255 -> rgbBack 255 0 216
    * rgb 255 0 216 -> hsv 220 255 255 -> rgbBack 255 0 210
    * rgb 255 0 210 -> hsv 221 255 255 -> rgbBack 255 0 204
    * rgb 255 0 204 -> hsv 222 255 255 -> rgbBack 255 0 198
    * rgb 255 0 198 -> hsv 223 255 255 -> rgbBack 255 0 192
* for FN_HI, try `f(x) = (x + 128) % 255` instead of `f(x) = 255 - x`\? \(or something else\?\)
* "noeeprom" versions of methods\? \(use\? don't use\? use to replace \(some\) current methods\?\)
    * † `rgb_matrix_mode`                \[CPICK/ECP\] \(when setting color\)
    * † `rgb_matrix_sethsv`              \[CPICK/ECP\] \(when setting color\)
    * ‡ `rgb_matrix_set_color`
    * ‡ `rgb_matrix_set_color_all`
    * \? `rgb_matrix_set_suspend_state`   \[SUS\]
    *
    * † has "_noeeprom" variant
    * ‡ doesn't write to EEPROM
* \(momentarily\) light up keycodes 0 \(Esc\), then 1 \(keeping 0 on\), then 2 \(keeping 1 and 0 on\), ... 11 when Fn-XX
  turns on grayscale; light up all of 0–11 and turn _off_ 11, then 10, ..., 1
* \(\?\)CPICK: switch gray from 0–11 to 0–12
