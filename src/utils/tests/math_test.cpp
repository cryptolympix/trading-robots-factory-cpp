#include <gtest/gtest.h>
#include "../math.hpp"

// Test case for decimal_ceil function
TEST(MathTest, DecimalCeil)
{
    ASSERT_EQ(decimal_ceil(3.14159, 2), 3.15);
    ASSERT_EQ(decimal_ceil(1.5, 0), 2.0);
    ASSERT_EQ(decimal_ceil(0.123456, 4), 0.1235);
}

// Test case for decimal_floor function
TEST(MathTest, DecimalFloor)
{
    ASSERT_EQ(decimal_floor(3.14159, 2), 3.14);
    ASSERT_EQ(decimal_floor(1.5, 0), 1.0);
    ASSERT_EQ(decimal_floor(0.123456, 4), 0.1234);
}

// Test case for decimal_round function
TEST(MathTest, DecimalRound)
{
    ASSERT_EQ(decimal_round(3.14159, 2), 3.14);
    ASSERT_EQ(decimal_round(1.5, 0), 2.0);
    ASSERT_EQ(decimal_round(0.123456, 4), 0.1235);
}

// Test case for count_decimals function
TEST(MathTest, CountDecimals)
{
    ASSERT_EQ(count_decimals(3.14159), 5);
    ASSERT_EQ(count_decimals(1.5), 1);
    ASSERT_EQ(count_decimals(0.123456), 6);
}
