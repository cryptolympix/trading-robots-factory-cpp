#include <gtest/gtest.h>
#include "../activation_functions.hpp"

class TestActivationFunctions : public ::testing::Test
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

TEST_F(TestActivationFunctions, Step)
{
    ASSERT_DOUBLE_EQ(step(0), 0);
    ASSERT_DOUBLE_EQ(step(1), 1);
    ASSERT_DOUBLE_EQ(step(-1), 0);
}

TEST_F(TestActivationFunctions, Sigmoid)
{
    ASSERT_NEAR(sigmoid(0), 0.5, 1e-6);
    ASSERT_NEAR(sigmoid(1), 1 / (1 + std::exp(-1)), 1e-6);
    ASSERT_NEAR(sigmoid(-1), 1 / (1 + std::exp(1)), 1e-6);
}

TEST_F(TestActivationFunctions, Tanh)
{
    // ASSERT_NEAR(tanh(0), 0, 1e-6);
    // ASSERT_NEAR(tanh(1), std::tanh(1), 1e-6);
    // ASSERT_NEAR(tanh(-1), std::tanh(-1), 1e-6);
}

TEST_F(TestActivationFunctions, ReLU)
{
    ASSERT_DOUBLE_EQ(relu(0), 0);
    ASSERT_DOUBLE_EQ(relu(1), 1);
    ASSERT_DOUBLE_EQ(relu(-1), 0);
}

TEST_F(TestActivationFunctions, LeakyReLU)
{
    ASSERT_DOUBLE_EQ(leaky_relu(0), 0);
    ASSERT_DOUBLE_EQ(leaky_relu(1), 1);
    ASSERT_DOUBLE_EQ(leaky_relu(-1), -0.01);
}

TEST_F(TestActivationFunctions, PReLU)
{
    ASSERT_DOUBLE_EQ(prelu(0), 0);
    ASSERT_DOUBLE_EQ(prelu(1), 1);
    ASSERT_DOUBLE_EQ(prelu(-1), -0.1);
}

TEST_F(TestActivationFunctions, ELU)
{
    ASSERT_DOUBLE_EQ(elu(0), 0);
    ASSERT_DOUBLE_EQ(elu(1), 1);
    ASSERT_NEAR(elu(-1), -0.6321205588285577, 1e-6);
}

TEST_F(TestActivationFunctions, Softmax)
{
    // Note: Softmax is applied to an array, testing with an array for expected behavior
    // std::vector<double> input_array = {1, 2, 3};
    // std::vector<double> expected_output = {std::exp(1) / (std::exp(1) + std::exp(2) + std::exp(3)),
    //                                        std::exp(2) / (std::exp(1) + std::exp(2) + std::exp(3)),
    //                                        std::exp(3) / (std::exp(1) + std::exp(2) + std::exp(3))};
    // for (size_t i = 0; i < input_array.size(); ++i)
    // {
    //     double result = softmax(input_array[i]);
    //     ASSERT_DOUBLE_EQ(result, expected_output[i]);
    // }
}

TEST_F(TestActivationFunctions, Linear)
{
    ASSERT_DOUBLE_EQ(linear(0), 0);
    ASSERT_DOUBLE_EQ(linear(1), 1);
    ASSERT_DOUBLE_EQ(linear(-1), -1);
}

TEST_F(TestActivationFunctions, Swish)
{
    ASSERT_NEAR(swish(0), 0, 1e-6);
    ASSERT_NEAR(swish(1), 1 / (1 + std::exp(-1)), 1e-6);
    ASSERT_NEAR(swish(-1), -1 / (1 + std::exp(1)), 1e-6);
}
