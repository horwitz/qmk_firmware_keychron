#include "gtest/gtest.h"

extern "C" {
#include "gcp_color.h"
}

// color_hue_for_index: round(i * HUE_STEPS / (double)COLOR_PALETTE_SIZE)

TEST(ColorHueForIndex, First) {
    EXPECT_EQ(0, color_hue_for_index(0));
}

TEST(ColorHueForIndex, Second) {
    EXPECT_EQ(5, color_hue_for_index(1));
}

TEST(ColorHueForIndex, Third) {
    EXPECT_EQ(11, color_hue_for_index(2));
}

TEST(ColorHueForIndex, Quarter) {
    EXPECT_EQ(64, color_hue_for_index(12));
}

TEST(ColorHueForIndex, Midpoint) {
    EXPECT_EQ(128, color_hue_for_index(24));
}

TEST(ColorHueForIndex, SecondToLast) {
    EXPECT_EQ(245, color_hue_for_index(46));
}

TEST(ColorHueForIndex, Last) {
    EXPECT_EQ(251, color_hue_for_index(47));
}

// scan_palette_keycodes

TEST(ScanPaletteKeycodes, SkipsHoles) {
    // col 1 is a hole (hole_map == 0); COLOR_BASE+0 -> LED 0, COLOR_BASE+1 -> LED 1 (col 1 skipped)
    const uint16_t COLOR_BASE = 100;
    uint16_t keymap[1][3] = {{COLOR_BASE, 0, COLOR_BASE + 1}};
    uint16_t holes[1][3]  = {{1, 0, 1}};
    uint8_t out_color[2] = {0xff, 0xff};
    uint8_t out_gray[1]  = {0xff};
    scan_palette_keycodes(&keymap[0][0], &holes[0][0], 1, 3,
        COLOR_BASE, 2, out_color, 200, 1, out_gray);
    EXPECT_EQ(0, out_color[0]);
    EXPECT_EQ(1, out_color[1]);
    EXPECT_EQ(0xff, out_gray[0]);  // no gray key -> unchanged
}

TEST(ScanPaletteKeycodes, ColorAndGrayMapped) {
    // 2x2, no holes: COLOR at (0,0) and (1,0), GRAY at (0,1) and (1,1)
    const uint16_t COLOR_BASE = 100, GRAY_BASE = 200;
    uint16_t keymap[2][2] = {
        {COLOR_BASE,     GRAY_BASE    },
        {COLOR_BASE + 1, GRAY_BASE + 1}
    };
    uint16_t holes[2][2] = {{1, 1}, {1, 1}};
    uint8_t out_color[2] = {}, out_gray[2] = {};
    scan_palette_keycodes(&keymap[0][0], &holes[0][0], 2, 2,
        COLOR_BASE, 2, out_color, GRAY_BASE, 2, out_gray);
    EXPECT_EQ(0, out_color[0]);  // LED 0
    EXPECT_EQ(1, out_gray[0]);   // LED 1
    EXPECT_EQ(2, out_color[1]);  // LED 2
    EXPECT_EQ(3, out_gray[1]);   // LED 3
}

TEST(ScanPaletteKeycodes, UnrecognizedKeycodesIgnored) {
    // keycode 99 is outside both ranges; out arrays at those positions should be unchanged
    const uint16_t COLOR_BASE = 100, GRAY_BASE = 200;
    uint16_t keymap[1][3] = {{COLOR_BASE, 99, COLOR_BASE + 1}};
    uint16_t holes[1][3]  = {{1, 1, 1}};
    uint8_t out_color[2] = {0xff, 0xff};
    uint8_t out_gray[1]  = {0xff};
    scan_palette_keycodes(&keymap[0][0], &holes[0][0], 1, 3,
        COLOR_BASE, 2, out_color, GRAY_BASE, 1, out_gray);
    EXPECT_EQ(0, out_color[0]);   // LED 0
    EXPECT_EQ(2, out_color[1]);   // LED 2 (99 skips no LED, just isn't mapped)
    EXPECT_EQ(0xff, out_gray[0]); // unchanged
}

// gray_intensity_for_index: round(i * MAX_COMPONENT / (double)(GRAY_PALETTE_SIZE - 1))

TEST(GrayIntensityForIndex, First) {
    EXPECT_EQ(0,   gray_intensity_for_index(0));
}

TEST(GrayIntensityForIndex, Second) {
    EXPECT_EQ(23,  gray_intensity_for_index(1));
}

TEST(GrayIntensityForIndex, LowerMid) {
    EXPECT_EQ(116, gray_intensity_for_index(5));
}

TEST(GrayIntensityForIndex, UpperMid) {
    EXPECT_EQ(139, gray_intensity_for_index(6));
}

TEST(GrayIntensityForIndex, SecondToLast) {
    EXPECT_EQ(232, gray_intensity_for_index(10));
}

TEST(GrayIntensityForIndex, Last) {
    EXPECT_EQ(255, gray_intensity_for_index(11));
}
