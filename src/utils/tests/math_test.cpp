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
    ASSERT_EQ(decimal_ceil(3.14159f, 2), 3.15f);
    ASSERT_EQ(decimal_ceil(1.5f, 0), 2.0f);
    ASSERT_EQ(decimal_ceil(0.123456f, 4), 0.1235f);
}

// Test case for decimal_floor function
TEST(MathTest, DecimalFloor)
{
    ASSERT_EQ(decimal_floor(3.14159f, 2), 3.14f);
    ASSERT_EQ(decimal_floor(1.5f, 0), 1.0f);
    ASSERT_EQ(decimal_floor(0.123456f, 4), 0.1234f);
}

// Test case for decimal_round function
TEST(MathTest, DecimalRound)
{
    ASSERT_EQ(decimal_round(3.14159f, 2), 3.14f);
    ASSERT_EQ(decimal_round(1.5f, 0), 2.0f);
    ASSERT_EQ(decimal_round(0.123456f, 4), 0.1235f);
}

// Test case for count_decimals function
TEST(MathTest, CountDecimals)
{
    ASSERT_EQ(count_decimals(3.14159f), 5);
    ASSERT_EQ(count_decimals(1.5f), 1);
    ASSERT_EQ(count_decimals(0.123456f), 6);
}

// Test case for normalize function
TEST(MathTest, Normalize)
{
    std::vector<float> input = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    std::pair<float, float> current_range = std::make_pair(1.0f, 5.0f);
    std::pair<float, float> new_range = std::make_pair(0.0f, 1.0f);
    std::vector<float> expected_output = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
    ASSERT_EQ(normalize(input, current_range, new_range), expected_output);
}
