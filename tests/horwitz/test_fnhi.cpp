#include "gtest/gtest.h"

extern "C" {
#include "fnhi_color.h"
}

// KC_TRANSPARENT == _______ in QMK; value is 1
#define TRANSPARENT 1

TEST(FindUsedLedIndices, AllTransparent) {
    uint16_t keymap[1][3] = {{TRANSPARENT, TRANSPARENT, TRANSPARENT}};
    uint16_t holes[1][3]  = {{1, 1, 1}};
    uint8_t out[3] = {0xff, 0xff, 0xff};
    uint8_t count = find_used_led_indices(out, &keymap[0][0], &holes[0][0], 1, 3, TRANSPARENT);
    EXPECT_EQ(0, count);
}

TEST(FindUsedLedIndices, AllUsed) {
    uint16_t keymap[2][2] = {{10, 20}, {30, 40}};
    uint16_t holes[2][2]  = {{1, 1}, {1, 1}};
    uint8_t out[4] = {};
    uint8_t count = find_used_led_indices(out, &keymap[0][0], &holes[0][0], 2, 2, TRANSPARENT);
    EXPECT_EQ(4, count);
    EXPECT_EQ(0, out[0]);
    EXPECT_EQ(1, out[1]);
    EXPECT_EQ(2, out[2]);
    EXPECT_EQ(3, out[3]);
}

TEST(FindUsedLedIndices, SkipsHoles) {
    // col 1 is a hole; col 0 and 2 are non-transparent — should get LED indices 0 and 1
    uint16_t keymap[1][3] = {{10, TRANSPARENT, 20}};
    uint16_t holes[1][3]  = {{1, 0, 1}};
    uint8_t out[3] = {};
    uint8_t count = find_used_led_indices(out, &keymap[0][0], &holes[0][0], 1, 3, TRANSPARENT);
    EXPECT_EQ(2, count);
    EXPECT_EQ(0, out[0]);
    EXPECT_EQ(1, out[1]);
}

TEST(FindUsedLedIndices, HoleSkipsLedIndex) {
    // hole at (0,1): (1,0) is LED 1 (not 2), (1,1) is LED 2
    uint16_t keymap[2][2] = {{10, TRANSPARENT}, {20, 30}};
    uint16_t holes[2][2]  = {{1, 0}, {1, 1}};
    uint8_t out[4] = {};
    uint8_t count = find_used_led_indices(out, &keymap[0][0], &holes[0][0], 2, 2, TRANSPARENT);
    EXPECT_EQ(3, count);
    EXPECT_EQ(0, out[0]);  // (0,0) = LED 0
    EXPECT_EQ(1, out[1]);  // (1,0) = LED 1 (hole at (0,1) not counted)
    EXPECT_EQ(2, out[2]);  // (1,1) = LED 2
}

TEST(FindUsedLedIndices, MixedTransparentAndUsed) {
    // 1x4, no holes: positions 0 and 2 transparent, 1 and 3 used
    uint16_t keymap[1][4] = {{TRANSPARENT, 10, TRANSPARENT, 20}};
    uint16_t holes[1][4]  = {{1, 1, 1, 1}};
    uint8_t out[4] = {};
    uint8_t count = find_used_led_indices(out, &keymap[0][0], &holes[0][0], 1, 4, TRANSPARENT);
    EXPECT_EQ(2, count);
    EXPECT_EQ(1, out[0]);  // LED 1
    EXPECT_EQ(3, out[1]);  // LED 3
}
