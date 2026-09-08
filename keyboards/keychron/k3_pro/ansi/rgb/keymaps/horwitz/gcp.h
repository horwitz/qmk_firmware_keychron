#pragma once

#include "ctrlkeycodes.h"
#include "gcp_color.h"

enum COLOR_SCHEME { RGB_SCHEME, GRAY_SCHEME, UNKNOWN_SCHEME, };

// keycode 73 (End) is lit RED, though pressing any XXXXXXX aborts color choosing
//
// LED index mapping as derived from this layout (for reference; populated at runtime in gcp.c):
//
// color palette (COLOR00–COLOR47): hex values from hsv_to_rgb_nocie(h, s=255, v=255); QMK's
// integer >> 8 arithmetic causes slight imprecision at section boundaries (e.g., yellow is
// #FEFF00 rather than #FFFF00, green is #03FF00 rather than #00FF00):
//   COLOR00 -> LED 16  (`   )  (1) red             #FF0000
//   COLOR01 -> LED 31  (TAB )  (5) scarlet         #FF1E00
//   COLOR02 -> LED 46  (CAPS)  (4) vermilion       #FF4200
//   COLOR03 -> LED 60  (LSFT)  (5) persimmon       #FF6000
//   COLOR04 -> LED 17  (1   )  (3) orange          #FF7E00
//   COLOR05 -> LED 32  (Q   )  (5) orange peel     #FFA200
//   COLOR06 -> LED 47  (A   )  (4) amber           #FFC000
//   COLOR07 -> LED 61  (Z   )  (5) golden yellow   #FFDE00
//   COLOR08 -> LED 18  (2   )  (2) yellow          #FEFF00
//   COLOR09 -> LED 33  (W   )  (5) lemon           #E1FF00
//   COLOR10 -> LED 48  (S   )  (4) lime            #C3FF00
//   COLOR11 -> LED 62  (X   )  (5) spring bud      #9FFF00
//   COLOR12 -> LED 19  (3   )  (3) chartreuse      #81FF00
//   COLOR13 -> LED 34  (E   )  (5) bright green    #63FF00
//   COLOR14 -> LED 49  (D   )  (4) harlequin       #3FFF00
//   COLOR15 -> LED 63  (C   )  (5) neon green      #21FF00
//   COLOR16 -> LED 20  (4   )  (1) green           #03FF00
//   COLOR17 -> LED 35  (R   )  (5) jade            #00FF1E
//   COLOR18 -> LED 50  (F   )  (4) erin            #00FF3C
//   COLOR19 -> LED 64  (V   )  (5) emerald         #00FF5A
//   COLOR20 -> LED 21  (5   )  (3) spring green    #00FF7E
//   COLOR21 -> LED 36  (T   )  (5) mint            #00FF9C
//   COLOR22 -> LED 51  (G   )  (4) aquamarine      #00FFBA
//   COLOR23 -> LED 65  (B   )  (5) turquoise       #00FFDE
//   COLOR24 -> LED 22  (6   )  (2) cyan            #00FFFC
//   COLOR25 -> LED 37  (Y   )  (5) sky blue        #00E7FF
//   COLOR26 -> LED 52  (H   )  (4) capri           #00C3FF
//   COLOR27 -> LED 66  (N   )  (5) cornflower      #00A5FF
//   COLOR28 -> LED 23  (7   )  (3) azure           #0087FF
//   COLOR29 -> LED 38  (U   )  (5) cobalt          #0063FF
//   COLOR30 -> LED 53  (J   )  (4) cerulean        #0045FF
//   COLOR31 -> LED 67  (M   )  (5) sapphire        #0027FF
//   COLOR32 -> LED 24  (8   )  (1) blue            #0003FF
//   COLOR33 -> LED 39  (I   )  (5) iris            #1800FF
//   COLOR34 -> LED 54  (K   )  (4) indigo          #3600FF
//   COLOR35 -> LED 68  (,   )  (5) veronica        #5A00FF
//   COLOR36 -> LED 25  (9   )  (3) violet          #7800FF
//   COLOR37 -> LED 40  (O   )  (5) amethyst        #9600FF
//   COLOR38 -> LED 55  (L   )  (4) purple          #BA00FF
//   COLOR39 -> LED 69  (.   )  (5) phlox           #D800FF
//   COLOR40 -> LED 26  (0   )  (2) magenta         #F600FF
//   COLOR41 -> LED 41  (P   )  (5) fuchsia         #FF00E7
//   COLOR42 -> LED 56  (;   )  (4) cerise          #FF00C9
//   COLOR43 -> LED 70  (/   )  (5) deep pink       #FF00AB
//   COLOR44 -> LED 27  (-   )  (3) rose            #FF0087
//   COLOR45 -> LED 42  ([   )  (5) raspberry       #FF0069
//   COLOR46 -> LED 57  ('   )  (4) crimson         #FF004B
//   COLOR47 -> LED 71  (RSFT)  (5) amaranth        #FF0027
//
// gray palette (GRAY00–GRAY11 -> F1–F12): exact values, round(i * 255 / 11) per channel:
//   GRAY00 -> LED  1  (F1 )  #000000
//   GRAY01 -> LED  2  (F2 )  #171717
//   GRAY02 -> LED  3  (F3 )  #2E2E2E
//   GRAY03 -> LED  4  (F4 )  #464646
//   GRAY04 -> LED  5  (F5 )  #5D5D5D
//   GRAY05 -> LED  6  (F6 )  #747474
//   GRAY06 -> LED  7  (F7 )  #8B8B8B
//   GRAY07 -> LED  8  (F8 )  #A2A2A2
//   GRAY08 -> LED  9  (F9 )  #B9B9B9
//   GRAY09 -> LED 10  (F10)  #D1D1D1
//   GRAY10 -> LED 11  (F11)  #E8E8E8
//   GRAY11 -> LED 12  (F12)  #FFFFFF
#define LAYOUT_gcp LAYOUT_ansi_84( \
    XXXXXXX,  GRAY00,   GRAY01,   GRAY02,   GRAY03,   GRAY04,   GRAY05,   GRAY06,   GRAY07,   GRAY08,   GRAY09,   GRAY10,   GRAY11,   XXXXXXX,  XXXXXXX,  XXXXXXX, \
    COLOR00,  COLOR04,  COLOR08,  COLOR12,  COLOR16,  COLOR20,  COLOR24,  COLOR28,  COLOR32,  COLOR36,  COLOR40,  COLOR44,  XXXXXXX,  XXXXXXX,            XXXXXXX, \
    COLOR01,  COLOR05,  COLOR09,  COLOR13,  COLOR17,  COLOR21,  COLOR25,  COLOR29,  COLOR33,  COLOR37,  COLOR41,  COLOR45,  XXXXXXX,  XXXXXXX,            XXXXXXX, \
    COLOR02,  COLOR06,  COLOR10,  COLOR14,  COLOR18,  COLOR22,  COLOR26,  COLOR30,  COLOR34,  COLOR38,  COLOR42,  COLOR46,            XXXXXXX,            XXXXXXX, \
    COLOR03,            COLOR07,  COLOR11,  COLOR15,  COLOR19,  COLOR23,  COLOR27,  COLOR31,  COLOR35,  COLOR39,  COLOR43,            COLOR47,  XXXXXXX,  XXXXXXX, \
    XXXXXXX,  XXXXXXX,  XXXXXXX,                                XXXXXXX,                                XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX \
)

void keyboard_post_init_user_gcp(void);

bool process_record_user_gcp(uint16_t keycode, const keyrecord_t *record);

void rgb_matrix_indicators_advanced_user_gcp(void);
