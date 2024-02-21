#include <gtest/gtest.h>
#include "../math.hpp"

class MathTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Optional setup code that will be called before each test
    }

    void TearDown() override
    {
        // Optional teardown code that will be called after each test
    }
};

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

// Test case for normalize function
TEST(MathTest, Normalize)
{
    std::vector<double> input = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::pair<double, double> current_range = std::make_pair(1.0, 5.0);
    std::pair<double, double> new_range = std::make_pair(0.0, 1.0);
    std::vector<double> expected_output = {0.0, 0.25, 0.5, 0.75, 1.0};
    ASSERT_EQ(normalize(input, current_range, new_range), expected_output);
}

// Test case for average function
TEST(MathTest, Average)
{
    std::vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> expected_values = {1.0, 1.5, 2.0, 3.0, 4.0};
    std::vector<double> results = average(values, 3);

    for (int i = 0; i < values.size(); i++)
    {
        ASSERT_EQ(results[i], expected_values[i]);
    }
}
