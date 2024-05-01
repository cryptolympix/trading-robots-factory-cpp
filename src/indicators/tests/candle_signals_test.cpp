#include <gtest/gtest.h>
#include "../../types.hpp"
#include "../candle_signals.hpp"

TEST(SignalsIndicatorsTest, NewHighSignal)
{
    // left_bars = 2, right_bars = 0
    NewHighSignal newHighSignal(2, 0);
    std::vector<CandleStick> candles = {
        {.open = 10, .high = 12, .low = 8, .close = 15},
        {.open = 15, .high = 16, .low = 8, .close = 9},
        {.open = 9, .high = 10, .low = 6, .close = 12},
        {.open = 12, .high = 13, .low = 8, .close = 6},
        {.open = 6, .high = 7, .low = 5, .close = 5},
        {.open = 8, .high = 17, .low = 5, .close = 17},
        {.open = 17, .high = 18, .low = 6, .close = 14},
        {.open = 14, .high = 15, .low = 8, .close = 11},
        {.open = 11, .high = 12, .low = 2, .close = 4},
        {.open = 4, .high = 21, .low = 8, .close = 20},
        {.open = 20, .high = 20, .low = 5, .close = 18}};

    std::vector<double> result_signals1 = newHighSignal.calculate(candles);
    std::vector<double> expected1 = {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0};
    ASSERT_EQ(result_signals1, expected1);

    // left_bars = 2, right_bars = 2
    NewHighSignal newHighSignal2(2, 2);
    std::vector<double> result_signals2 = newHighSignal2.calculate(candles);
    std::vector<double> expected2 = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    ASSERT_EQ(result_signals2, expected2);
}

TEST(SignalsIndicatorsTest, NewLowSignal)
{
    // left_bars = 2, right_bars = 0
    NewLowSignal newLowSignal(2, 0);
    std::vector<CandleStick> candles = {
        {.open = 10, .high = 12, .low = 8, .close = 15},
        {.open = 15, .high = 16, .low = 8, .close = 9},
        {.open = 9, .high = 10, .low = 6, .close = 12},
        {.open = 12, .high = 13, .low = 8, .close = 6},
        {.open = 6, .high = 7, .low = 5, .close = 5},
        {.open = 8, .high = 17, .low = 5, .close = 17},
        {.open = 17, .high = 18, .low = 6, .close = 14},
        {.open = 14, .high = 15, .low = 8, .close = 11},
        {.open = 11, .high = 12, .low = 2, .close = 4},
        {.open = 4, .high = 21, .low = 8, .close = 20},
        {.open = 20, .high = 20, .low = 5, .close = 18}};

    std::vector<double> result_signals1 = newLowSignal.calculate(candles);
    std::vector<double> expected1 = {0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0};
    ASSERT_EQ(result_signals1, expected1);

    // left_bars = 2, right_bars = 2
    NewLowSignal newLowSignal2(2, 2);
    std::vector<double> result_signals2 = newLowSignal2.calculate(candles);
    std::vector<double> expected2 = {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0};
    ASSERT_EQ(result_signals2, expected2);
}

TEST(SignalsIndicatorsTest, HighBreakSignal)
{
    // left_bars = 2, right_bars = 0
    HighBreakSignal highBreakSignal1(2, 0);
    std::vector<CandleStick> candles = {
        {.open = 10, .high = 12, .low = 8, .close = 15},
        {.open = 15, .high = 16, .low = 8, .close = 9},
        {.open = 9, .high = 10, .low = 6, .close = 12},
        {.open = 12, .high = 13, .low = 8, .close = 6},
        {.open = 6, .high = 7, .low = 5, .close = 5},
        {.open = 8, .high = 17, .low = 5, .close = 17},
        {.open = 17, .high = 18, .low = 6, .close = 14},
        {.open = 14, .high = 15, .low = 8, .close = 11},
        {.open = 11, .high = 12, .low = 2, .close = 4},
        {.open = 4, .high = 21, .low = 8, .close = 20},
        {.open = 20, .high = 20, .low = 5, .close = 18}};

    std::vector<double> result_signals1 = highBreakSignal1.calculate(candles);
    std::vector<double> expected1 = {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0};
    ASSERT_EQ(result_signals1, expected1);

    // left_bars = 2, right_bars = 2
    HighBreakSignal highBreakSignal2(2, 2);
    std::vector<double> result_signals2 = highBreakSignal2.calculate(candles);
    std::vector<double> expected2 = {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0};
    ASSERT_EQ(result_signals2, expected2);
}

TEST(SignalsIndicatorsTest, LowBreakSignal)
{
    // left_bars = 2, right_bars = 0
    LowBreakSignal lowBreakSignal1(2, 0);
    std::vector<CandleStick> candles = {
        {.open = 10, .high = 12, .low = 8, .close = 15},
        {.open = 15, .high = 16, .low = 8, .close = 9},
        {.open = 9, .high = 10, .low = 6, .close = 12},
        {.open = 12, .high = 13, .low = 8, .close = 6},
        {.open = 6, .high = 7, .low = 5, .close = 5},
        {.open = 8, .high = 17, .low = 5, .close = 17},
        {.open = 17, .high = 18, .low = 6, .close = 14},
        {.open = 14, .high = 15, .low = 8, .close = 11},
        {.open = 11, .high = 12, .low = 2, .close = 4},
        {.open = 4, .high = 21, .low = 8, .close = 20},
        {.open = 20, .high = 20, .low = 5, .close = 18}};

    std::vector<double> result_signals1 = lowBreakSignal1.calculate(candles);
    std::vector<double> expected1 = {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0};
    ASSERT_EQ(result_signals1, expected1);

    // left_bars = 2, right_bars = 2
    LowBreakSignal lowBreakSignal2(2, 2);
    std::vector<double> result_signals2 = lowBreakSignal2.calculate(candles);
    std::vector<double> expected2 = {0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0};

    ASSERT_EQ(result_signals2, expected2);
};
