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
