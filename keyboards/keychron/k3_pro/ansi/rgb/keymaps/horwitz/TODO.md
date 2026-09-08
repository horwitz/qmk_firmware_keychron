* make any use of `(RGB|HSV)_(AZURE|BLUE|...)` from `color.h`\?
* `ccp.c`: do something fancier than `RGB_WHITE` with keycodes 0–15 \(maybe something state dependent\)\?
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
