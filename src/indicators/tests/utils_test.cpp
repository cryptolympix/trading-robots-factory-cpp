#include <gtest/gtest.h>
#include "../../utils/math.hpp"
#include "../utils.hpp"

TEST(IndicatorsUtilsTest, SimpleMovingAverage)
{
    std::vector<double> values = {2, 4, 6, 8, 12, 14, 16, 18, 20};
    int period = 3;

    std::vector<double> expected_sma = {0, 0, 4, 6, 8.667, 11.333, 14, 16, 18}; // Expected SMA values

    std::vector<double> sma = calculate_simple_moving_average(values, period);

    // Round the values to 3 decimal places
    for (size_t i = 0; i < sma.size(); ++i)
    {
        sma[i] = decimal_round(sma[i], 3);
    }

    ASSERT_EQ(sma, expected_sma);
}

TEST(IndicatorsUtilsTest, ExponentialMovingAverage)
{
    std::vector<double> values = {2, 4, 6, 8, 12, 14, 16, 18, 20};
    int period = 3;

    std::vector<double> expected_ema = {0, 0, 4, 6, 9, 11.5, 13.75, 15.875, 17.938}; // Expected EMA values

    std::vector<double> ema = calculate_exponential_moving_average(values, period);

    // Round the values to 3 decimal places
    for (size_t i = 0; i < ema.size(); ++i)
    {
        ema[i] = decimal_round(ema[i], 3);
    }

    ASSERT_EQ(ema, expected_ema);
}

TEST(IndicatorsUtilsTest, StandardDeviation)
{
    std::vector<double> values = {2, 4, 6, 8, 12, 14, 16, 18, 20};
    int period = 3;

    std::vector<double> expected_stddev = {0, 0, 1.633, 1.633, 2.494, 2.494, 1.633, 1.633, 1.633}; // Expected Standard Deviation values

    std::vector<double> stddev = calculate_standard_deviation(values, period);

    // Round the values to 3 decimal places
    for (size_t i = 0; i < stddev.size(); ++i)
    {
        stddev[i] = decimal_round(stddev[i], 3);
    }

    ASSERT_EQ(stddev, expected_stddev);
}
