#include "gtest/gtest.h"

extern "C" {
#include "ccp_color.h"
}

static RGB make_rgb(uint8_t r, uint8_t g, uint8_t b) {
    RGB rgb;
    rgb.r = r; rgb.g = g; rgb.b = b;
    return rgb;
}

// rgb_to_hsv

TEST(RgbToHsv, Black) {
    RGB rgb = make_rgb(0, 0, 0);
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(0, hsv.h);
    EXPECT_EQ(0, hsv.s);
    EXPECT_EQ(0, hsv.v);
}

TEST(RgbToHsv, White) {
    RGB rgb = make_rgb(255, 255, 255);
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(0,   hsv.h);
    EXPECT_EQ(0,   hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, Red) {
    RGB rgb = make_rgb(255, 0, 0);
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(0,   hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, Green) {
    RGB rgb = make_rgb(0, 255, 0);
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(85,  hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, Blue) {
    RGB rgb = make_rgb(0, 0, 255);
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(170, hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, Yellow) {
    RGB rgb = make_rgb(255, 255, 0);
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(43,  hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, Cyan) {
    RGB rgb = make_rgb(0, 255, 255);
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(128, hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, Magenta) {
    RGB rgb = make_rgb(255, 0, 255);
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(213, hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, DarkRed) {
    RGB rgb = make_rgb(128, 0, 0);
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(0,   hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(128, hsv.v);
}

TEST(RgbToHsv, Orange) {
    RGB rgb = make_rgb(255, 128, 0);
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(21,  hsv.h);
    EXPECT_EQ(255, hsv.s);
    EXPECT_EQ(255, hsv.v);
}

TEST(RgbToHsv, Gray) {
    // achromatic with v > 0: exercises the chroma==0 early-return path for non-black inputs
    RGB rgb = make_rgb(128, 128, 128);
    HSV hsv = rgb_to_hsv(rgb);
    EXPECT_EQ(0,   hsv.h);
    EXPECT_EQ(0,   hsv.s);
    EXPECT_EQ(128, hsv.v);
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
    EXPECT_EQ(0,   addBounded(0, 0));
}

TEST(AddBounded, MaxPlusZero) {
    EXPECT_EQ(255, addBounded(255, 0));
}

TEST(AddBounded, ZeroPlusMax) {
    EXPECT_EQ(255, addBounded(0, 255));
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
    EXPECT_EQ(0,   subtractBounded(0, 0));
}

TEST(SubtractBounded, MaxMinusMax) {
    EXPECT_EQ(0,   subtractBounded(255, 255));
}

TEST(SubtractBounded, MaxMinusZero) {
    EXPECT_EQ(255, subtractBounded(255, 0));
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
