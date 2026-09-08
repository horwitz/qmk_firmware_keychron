* make any use of `(RGB|HSV)_(AZURE|BLUE|...)` from `color.h`\?
* `[cg]cp.h` \(`LAYOUT_[cg]cp`\) \(etc.\): construct as a value instead of a #define
* `gcp.h` \(`TD_GRAY`\): define somewhere aside from `gcp.h`\?
* `([cg]cp|fnhi).c` \(`rgb_matrix_indicators_advanced_user_([cg]cp|fnhi)`\): return `bool` instead of `void`\?
* `ccp.c`: do something fancier than `RGB_WHITE` with keycodes 0–15 \(maybe something state dependent\)\?
* `fnhi.c` \(`initialize_layer_used_indices`\): derive `layout` from `layer`\(\?\)
* `fnhi.c` \(`keyboard_post_init_user_fnhi`\): do the `(mac|win)FnLayer` stuff differently \(at least move some of it to
  a header file\?\)
* `fnhi.c` \(`rgb_matrix_indicators_advanced_user_fnhi`\): if all `r`,`g`,`b` are close to 255/2, the complement will be
  close to \(and hard to discern from\) the original color \(should this be changed\?\)
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
* \(\?\)CPICK: switch gray from 1–12 to 0–12
