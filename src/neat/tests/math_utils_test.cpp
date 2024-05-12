#include <gtest/gtest.h>
#include <cmath>
#include "../math_utils.hpp"

// Test cases for randrange function
TEST(MathUtilsTest, Randrange)
{
    // Test within range [0.0, 1.0]
    double result = neat::randrange();
    ASSERT_GE(result, 0.0);
    ASSERT_LE(result, 1.0);

    // Test within custom range [min, max]
    double min = -5.0;
    double max = 5.0;
    result = neat::randrange(min, max);
    ASSERT_GE(result, min);
    ASSERT_LE(result, max);
}

// Test cases for uniform function
TEST(MathUtilsTest, Uniform)
{
    // Test within custom range [min, max]
    double min = -5.0;
    double max = 5.0;
    double result = neat::uniform(min, max);
    ASSERT_GE(result, min);
    ASSERT_LE(result, max);
}

// Test cases for normal function
TEST(MathUtilsTest, Normal)
{
    // Test with non-negative mean and standard deviation
    double mean = 0.0;
    double stdev = 1.0;
    double result = neat::normal(mean, stdev);
    ASSERT_TRUE(std::isfinite(result));

    // Test with arbitrary mean and standard deviation
    mean = 10.0;
    stdev = 3.0;
    result = neat::normal(mean, stdev);
    ASSERT_TRUE(std::isfinite(result));
}