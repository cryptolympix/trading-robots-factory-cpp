#include <gtest/gtest.h>
#include "../optional.cpp"

// Test fixture for Optional class
class OptionalTest : public ::testing::Test
{
protected:
    // Optional objects used in the tests
    Optional<int> opt_empty;
    Optional<int> opt_with_value;

    // Test data
    int test_value = 42;

    // Set up test cases
    void SetUp() override
    {
        opt_with_value = Optional<int>(test_value);
    }
};

// Test default constructor
TEST_F(OptionalTest, DefaultConstructor)
{
    EXPECT_FALSE(opt_empty.has_value());
    EXPECT_THROW(opt_empty.value(), std::logic_error);
}

// Test constructor with value
TEST_F(OptionalTest, ConstructorWithValue)
{
    EXPECT_TRUE(opt_with_value.has_value());
    EXPECT_EQ(opt_with_value.value(), test_value);
}

// Test hasValue()
TEST_F(OptionalTest, HasValue)
{
    EXPECT_FALSE(opt_empty.has_value());
    EXPECT_TRUE(opt_with_value.has_value());
}

// Test getValue()
TEST_F(OptionalTest, GetValue)
{
    EXPECT_THROW(opt_empty.value(), std::logic_error);
    EXPECT_EQ(opt_with_value.value(), test_value);
}

// Test changing value
TEST_F(OptionalTest, ChangeValue)
{
    int new_value = 99;
    opt_with_value = Optional<int>(new_value);
    EXPECT_TRUE(opt_with_value.has_value());
    EXPECT_EQ(opt_with_value.value(), new_value);
}

TEST_F(OptionalTest, GetValueOrDefault)
{
    int default_value = 123;
    EXPECT_EQ(opt_empty.value_or(default_value), default_value);
    EXPECT_EQ(opt_with_value.value_or(default_value), test_value);
}
