#include "gtest/gtest.h"

extern "C" {
#include "ccp_color.h"
}

// rgb_to_hsv

TEST(RgbToHsv, Black) {
    RGB rgb = {.r = 0, .g = 0, .b = 0};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(0, hsv.h);
    EXPECT_EQ(0, hsv.s);
    EXPECT_EQ(0, hsv.v);
}

TEST(RgbToHsv, White) {
    RGB rgb = {.r = 255, .g = 255, .b = 255};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(0,   hsv.h);
    EXPECT_EQ(0,   hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, Red) {
    RGB rgb = {.r = 255, .g = 0, .b = 0};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(0,   hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, Green) {
    RGB rgb = {.r = 0, .g = 255, .b = 0};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(85,  hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, Blue) {
    RGB rgb = {.r = 0, .g = 0, .b = 255};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(170, hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, Yellow) {
    RGB rgb = {.r = 255, .g = 255, .b = 0};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(43,  hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, Cyan) {
    RGB rgb = {.r = 0, .g = 255, .b = 255};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(128, hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, Magenta) {
    RGB rgb = {.r = 255, .g = 0, .b = 255};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(213, hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, DarkRed) {
    RGB rgb = {.r = 128, .g = 0, .b = 0};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(0,   hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(128, hsv.v);
}

TEST(RgbToHsv, Orange) {
    RGB rgb = {.r = 255, .g = 128, .b = 0};
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(21,  hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(255, hsv.v);
}

// addBounded

TEST(AddBounded, NoOverflow) {
    EXPECT_EQ(150, addBounded(100, 50));
}

TEST(AddBounded, ExactMax) {
    EXPECT_EQ(255, addBounded(200, 55));
}

TEST(AddBounded, Overflow) {
    EXPECT_EQ(255, addBounded(250, 10));
}

TEST(AddBounded, ZeroPlusZero) {
    EXPECT_EQ(0, addBounded(0, 0));
}

// subtractBounded

TEST(SubtractBounded, NoUnderflow) {
    EXPECT_EQ(50, subtractBounded(100, 50));
}

TEST(SubtractBounded, ExactZero) {
    EXPECT_EQ(0, subtractBounded(50, 50));
}

TEST(SubtractBounded, Underflow) {
    EXPECT_EQ(0, subtractBounded(10, 20));
}

TEST(SubtractBounded, ZeroMinusZero) {
    EXPECT_EQ(0, subtractBounded(0, 0));
}

// min / max

TEST(Min, ReturnsSmaller) {
    EXPECT_EQ(3, min(3, 7));
    EXPECT_EQ(3, min(7, 3));
}

TEST(Min, Equal) {
    EXPECT_EQ(5, min(5, 5));
}

TEST(Max, ReturnsLarger) {
    EXPECT_EQ(7, max(3, 7));
    EXPECT_EQ(7, max(7, 3));
}

TEST(Max, Equal) {
    EXPECT_EQ(5, max(5, 5));
}
