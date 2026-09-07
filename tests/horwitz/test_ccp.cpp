#include "gtest/gtest.h"

extern "C" {
#include "ccp_color.h"
}

// rgb_to_hsv

TEST(RgbToHsv, Black) {
    RGB rgb = {.r = 0, .g = 0, .b = 0};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(hsv.h, 0);
    EXPECT_EQ(hsv.s, 0);
    EXPECT_EQ(hsv.v, 0);
}

TEST(RgbToHsv, White) {
    RGB rgb = {.r = 255, .g = 255, .b = 255};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(hsv.h, 0);
    EXPECT_EQ(hsv.s, 0);
    EXPECT_EQ(hsv.v, 255);
}

TEST(RgbToHsv, Red) {
    RGB rgb = {.r = 255, .g = 0, .b = 0};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(hsv.h, 0);
    EXPECT_EQ(hsv.s, 255);
    EXPECT_EQ(hsv.v, 255);
}

TEST(RgbToHsv, Green) {
    RGB rgb = {.r = 0, .g = 255, .b = 0};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(hsv.h, 85);
    EXPECT_EQ(hsv.s, 255);
    EXPECT_EQ(hsv.v, 255);
}

TEST(RgbToHsv, Blue) {
    RGB rgb = {.r = 0, .g = 0, .b = 255};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(hsv.h, 170);
    EXPECT_EQ(hsv.s, 255);
    EXPECT_EQ(hsv.v, 255);
}

TEST(RgbToHsv, Yellow) {
    RGB rgb = {.r = 255, .g = 255, .b = 0};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(hsv.h, 43);
    EXPECT_EQ(hsv.s, 255);
    EXPECT_EQ(hsv.v, 255);
}

TEST(RgbToHsv, Cyan) {
    RGB rgb = {.r = 0, .g = 255, .b = 255};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(hsv.h, 128);
    EXPECT_EQ(hsv.s, 255);
    EXPECT_EQ(hsv.v, 255);
}

TEST(RgbToHsv, Magenta) {
    RGB rgb = {.r = 255, .g = 0, .b = 255};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(hsv.h, 213);
    EXPECT_EQ(hsv.s, 255);
    EXPECT_EQ(hsv.v, 255);
}

TEST(RgbToHsv, DarkRed) {
    RGB rgb = {.r = 128, .g = 0, .b = 0};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(hsv.h, 0);
    EXPECT_EQ(hsv.s, 255);
    EXPECT_EQ(hsv.v, 128);
}

TEST(RgbToHsv, Orange) {
    RGB rgb = {.r = 255, .g = 128, .b = 0};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(hsv.h, 21);
    EXPECT_EQ(hsv.s, 255);
    EXPECT_EQ(hsv.v, 255);
}

// addBounded

TEST(AddBounded, NoOverflow) {
    EXPECT_EQ(addBounded(100, 50), 150);
}

TEST(AddBounded, ExactMax) {
    EXPECT_EQ(addBounded(200, 55), 255);
}

TEST(AddBounded, Overflow) {
    EXPECT_EQ(addBounded(250, 10), 255);
}

TEST(AddBounded, ZeroPlusZero) {
    EXPECT_EQ(addBounded(0, 0), 0);
}

// subtractBounded

TEST(SubtractBounded, NoUnderflow) {
    EXPECT_EQ(subtractBounded(100, 50), 50);
}

TEST(SubtractBounded, ExactZero) {
    EXPECT_EQ(subtractBounded(50, 50), 0);
}

TEST(SubtractBounded, Underflow) {
    EXPECT_EQ(subtractBounded(10, 20), 0);
}

TEST(SubtractBounded, ZeroMinusZero) {
    EXPECT_EQ(subtractBounded(0, 0), 0);
}

// min / max

TEST(Min, ReturnsSmaller) {
    EXPECT_EQ(min(3, 7), 3);
    EXPECT_EQ(min(7, 3), 3);
}

TEST(Min, Equal) {
    EXPECT_EQ(min(5, 5), 5);
}

TEST(Max, ReturnsLarger) {
    EXPECT_EQ(max(3, 7), 7);
    EXPECT_EQ(max(7, 3), 7);
}

TEST(Max, Equal) {
    EXPECT_EQ(max(5, 5), 5);
}
