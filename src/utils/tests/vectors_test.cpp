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