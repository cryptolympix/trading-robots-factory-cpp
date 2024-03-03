#include <gtest/gtest.h>
#include <vector>
#include "../vectors.hpp"

// Google Test case for add_vectors function
TEST(VectorsTest, AddVectorsTest)
{
    std::vector<double> vec1 = {1.5, 2.5, 3.5, 4.5};
    std::vector<double> vec2 = {0.5, 1.5, 2.5, 3.5};

    std::vector<double> expected_result = {2.0, 4.0, 6.0, 8.0};
    std::vector<double> result = add_vectors(vec1, vec2);

    EXPECT_EQ(result, expected_result);
}

// Google Test case for subtract_vectors function
TEST(VectorsTest, SubstractVectorsTest)
{
    std::vector<double> vec1 = {5.5, 6.5, 7.5, 8.5};
    std::vector<double> vec2 = {4.5, 3.5, 2.5, 1.5};

    std::vector<double> expected_result = {1.0, 3.0, 5.0, 7.0};
    std::vector<double> result = subtract_vectors(vec1, vec2);

    EXPECT_EQ(result, expected_result);
}

// Google Test case for divide_vectors function
TEST(VectorsTest, DivideVectorsTest)
{
    std::vector<double> vec1 = {10.0, 15.0, 18.0, 16.0};
    std::vector<double> vec2 = {2.0, 3.0, 6.0, 4.0};

    std::vector<double> expected_result = {5.0, 5.0, 3.0, 4.0};
    std::vector<double> result = divide_vectors(vec1, vec2);

    EXPECT_EQ(result, expected_result);
}

// Google Test case for normalize_vectors function
TEST(VectorsTest, NormalizeVectorsTest)
{
    std::vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::pair<double, double> current_range = std::make_pair(1.0, 5.0);
    std::pair<double, double> new_range = std::make_pair(0.0, 1.0);
    std::vector<double> expected_output = {0.0, 0.25, 0.5, 0.75, 1.0};
    std::vector<double> results = normalize_vectors(values, current_range, new_range);

    ASSERT_EQ(results, expected_output);
    ASSERT_EQ(results.size(), values.size());
}

// Test case for calculate_sum_subvector function
TEST(VectorsTest, SumSubVectorsTest)
{
    std::vector<double> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<double> expected_sum = {0, 0, 6, 9, 12, 15, 18, 21, 24, 27};
    std::vector<double> results = calculate_sum_subvector(values, 3);

    ASSERT_EQ(results, expected_sum);
    ASSERT_EQ(results.size(), expected_sum.size());
}