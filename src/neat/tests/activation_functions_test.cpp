#include <gtest/gtest.h>
#include "../activation_functions.hpp"

TEST(TestActivationFunctions, Step)
{
    ASSERT_DOUBLE_EQ(neat::step(0), 0);
    ASSERT_DOUBLE_EQ(neat::step(1), 1);
    ASSERT_DOUBLE_EQ(neat::step(-1), 0);
}

TEST(TestActivationFunctions, Sigmoid)
{
    ASSERT_NEAR(neat::sigmoid(0), 0.5, 1e-6);
    ASSERT_NEAR(neat::sigmoid(1), 1 / (1 + std::exp(-1)), 1e-6);
    ASSERT_NEAR(neat::sigmoid(-1), 1 / (1 + std::exp(1)), 1e-6);
}

TEST(TestActivationFunctions, Tanh)
{
    ASSERT_NEAR(neat::tanh(0), 0, 1e-6);
    ASSERT_NEAR(neat::tanh(1), std::tanh(1), 1e-6);
    ASSERT_NEAR(neat::tanh(-1), std::tanh(-1), 1e-6);
}

TEST(TestActivationFunctions, ReLU)
{
    ASSERT_DOUBLE_EQ(neat::relu(0), 0);
    ASSERT_DOUBLE_EQ(neat::relu(1), 1);
    ASSERT_DOUBLE_EQ(neat::relu(-1), 0);
}

TEST(TestActivationFunctions, LeakyReLU)
{
    ASSERT_DOUBLE_EQ(neat::leaky_relu(0), 0);
    ASSERT_DOUBLE_EQ(neat::leaky_relu(1), 1);
    ASSERT_DOUBLE_EQ(neat::leaky_relu(-1), -0.01);
}

TEST(TestActivationFunctions, PReLU)
{
    ASSERT_DOUBLE_EQ(neat::prelu(0), 0);
    ASSERT_DOUBLE_EQ(neat::prelu(1), 1);
    ASSERT_DOUBLE_EQ(neat::prelu(-1), -0.1);
}

TEST(TestActivationFunctions, ELU)
{
    ASSERT_DOUBLE_EQ(neat::elu(0), 0);
    ASSERT_DOUBLE_EQ(neat::elu(1), 1);
    ASSERT_NEAR(neat::elu(-1), -0.6321205588285577, 1e-6);
}

TEST(TestActivationFunctions, Softmax)
{
    // Note: Softmax is applied to an array, testing with an array for expected behavior
    std::vector<double> input_array = {1, 2, 3};
    std::vector<double> expected_output = {std::exp(1) / (1 + std::exp(1)),
                                           std::exp(2) / (1 + std::exp(2)),
                                           std::exp(3) / (1 + std::exp(3))};
    for (size_t i = 0; i < input_array.size(); ++i)
    {
        double result = neat::softmax(input_array[i]);
        ASSERT_DOUBLE_EQ(result, expected_output[i]);
    }
}

TEST(TestActivationFunctions, Linear)
{
    ASSERT_DOUBLE_EQ(neat::linear(0), 0);
    ASSERT_DOUBLE_EQ(neat::linear(1), 1);
    ASSERT_DOUBLE_EQ(neat::linear(-1), -1);
}

TEST(TestActivationFunctions, Swish)
{
    ASSERT_NEAR(neat::swish(0), 0, 1e-6);
    ASSERT_NEAR(neat::swish(1), 1 / (1 + std::exp(-1)), 1e-6);
    ASSERT_NEAR(neat::swish(-1), -1 / (1 + std::exp(1)), 1e-6);
}
