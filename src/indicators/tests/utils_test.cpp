#include <gtest/gtest.h>
#include "../../utils/math.hpp"
#include "../utils.hpp"

TEST(UtilsIndicatorsTest, SimpleMovingAverage)
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

TEST(UtilsIndicatorsTest, ExponentialMovingAverage)
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
