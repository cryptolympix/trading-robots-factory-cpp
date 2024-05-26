#include <gtest/gtest.h>
#include <vector>
#include "../vectors.hpp"

// Google Test case for add_vectors function
TEST(VectorsTest, AddVectors)
{
    std::vector<double> vec1 = {1.5, 2.5, 3.5, 4.5};
    std::vector<double> vec2 = {0.5, 1.5, 2.5, 3.5};

    std::vector<double> expected_result = {2.0, 4.0, 6.0, 8.0};
    std::vector<double> result = add_vectors(vec1, vec2);

    EXPECT_EQ(result, expected_result);
}

// Google Test case for subtract_vectors function
TEST(VectorsTest, SubstractVectors)
{
    std::vector<double> vec1 = {5.5, 6.5, 7.5, 8.5};
    std::vector<double> vec2 = {4.5, 3.5, 2.5, 1.5};

    std::vector<double> expected_result = {1.0, 3.0, 5.0, 7.0};
    std::vector<double> result = subtract_vectors(vec1, vec2);

    EXPECT_EQ(result, expected_result);
}

// Google Test case for divide_vectors function
TEST(VectorsTest, DivideVectors)
{
    std::vector<double> vec1 = {10.0, 15.0, 18.0, 16.0};
    std::vector<double> vec2 = {2.0, 3.0, 6.0, 4.0};

    std::vector<double> expected_result = {5.0, 5.0, 3.0, 4.0};
    std::vector<double> result = divide_vectors(vec1, vec2);

    EXPECT_EQ(result, expected_result);
}

// Google Test case for normalize_vector function
TEST(VectorsTest, NormalizeVectorsWithPositiveValues)
{
    std::vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::pair<double, double> current_range = std::make_pair(1.0, 5.0);
    std::pair<double, double> new_range = std::make_pair(0.0, 1.0);
    std::vector<double> expected_output = {0.0, 0.25, 0.5, 0.75, 1.0};
    std::vector<double> results1 = normalize_vector(values, current_range, new_range);
    std::vector<double> results2 = normalize_vector(values);

    ASSERT_EQ(results1, expected_output);
    ASSERT_EQ(results2, expected_output);
    ASSERT_EQ(results1.size(), values.size());
    ASSERT_EQ(results2.size(), values.size());
}

// Google Test case for normalize_vector function
TEST(VectorsTest, NormalizeVectorsWithNegativeValues)
{
    std::vector<double> values = {-1.0, -2.0, -3.0, -4.0, -5.0};
    std::pair<double, double> current_range = std::make_pair(-5.0, -1.0);
    std::pair<double, double> new_range = std::make_pair(0.0, 1.0);
    std::vector<double> results1 = normalize_vector(values, current_range, new_range);
    std::vector<double> results2 = normalize_vector(values);
    std::vector<double> expected_output1 = {1.0, 0.75, 0.5, 0.25, 0.0};
    std::vector<double> expected_output2 = {0.0, -0.25, -0.5, -0.75, -1.0};

    ASSERT_EQ(results1, expected_output1);
    ASSERT_EQ(results2, expected_output2);
    ASSERT_EQ(results1.size(), values.size());
    ASSERT_EQ(results2.size(), values.size());
}

// Google Test case for normalize_vector function
TEST(VectorsTest, NormalizeVectorsWithMixedValues)
{
    std::vector<double> values = {-2.0, -1.0, 0.0, 1.0, 2.0};
    std::pair<double, double> current_range = std::make_pair(-2.0, 2.0);
    std::pair<double, double> new_range = std::make_pair(0.0, 1.0);
    std::vector<double> results1 = normalize_vector(values, current_range, new_range);
    std::vector<double> results2 = normalize_vector(values);
    std::vector<double> expected_output1 = {0.0, 0.25, 0.5, 0.75, 1.0};
    std::vector<double> expected_output2 = {-1.0, -0.5, 0.0, 0.5, 1.0};

    ASSERT_EQ(results1, expected_output1);
    ASSERT_EQ(results2, expected_output2);
    ASSERT_EQ(results1.size(), values.size());
    ASSERT_EQ(results2.size(), values.size());
}

// Test case for calculate_sum_subvector function
TEST(VectorsTest, SumSubVectors)
{
    std::vector<double> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<double> expected_sum = {0, 0, 6, 9, 12, 15, 18, 21, 24, 27};
    std::vector<double> results = calculate_sum_subvector(values, 3);

    ASSERT_EQ(results, expected_sum);
    ASSERT_EQ(results.size(), expected_sum.size());
}