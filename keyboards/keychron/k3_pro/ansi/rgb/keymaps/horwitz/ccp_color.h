#pragma once

#include "color.h"     // RGB, HSV types
#include "colorconst.h"

// GCC statement-expression form: evaluates each argument exactly once, safe for side-effecting expressions
#define min(a,b) \
    ({ __typeof__ (a) min_lhs_ = (a); \
       __typeof__ (b) min_rhs_ = (b); \
       min_lhs_ < min_rhs_ ? min_lhs_ : min_rhs_; })
#define max(a,b) \
    ({ __typeof__ (a) max_lhs_ = (a); \
       __typeof__ (b) max_rhs_ = (b); \
       max_lhs_ > max_rhs_ ? max_lhs_ : max_rhs_; })
// compute a + b, ensuring that the value is at most MAX_COMPONENT
// (assumes b <= MAX_COMPONENT)
#define addBounded(a,b) \
    ({ __typeof__ (a) add_lhs_ = (a); \
       __typeof__ (b) add_rhs_ = (b); \
       add_lhs_ > MAX_COMPONENT - add_rhs_ ? MAX_COMPONENT : add_lhs_ + add_rhs_; })
// compute a - b, ensuring that the value is at least 0
#define subtractBounded(a,b) \
    ({ __typeof__ (a) sub_lhs_ = (a); \
       __typeof__ (b) sub_rhs_ = (b); \
       sub_lhs_ < sub_rhs_ ? 0 : sub_lhs_ - sub_rhs_; })

HSV rgb_to_hsv(RGB rgb);