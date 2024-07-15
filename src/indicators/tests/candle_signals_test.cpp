#include <gtest/gtest.h>
#include <vector>
#include <unordered_map>
#include "../../types.hpp"
#include "../candle_signals.hpp"
#include "../builder.hpp"

TEST(SignalsIndicatorsTest, NewHighSignal)
{
    std::vector<Candle> candles = {
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

    // left_bars = 2, right_bars = 0
    NewHighSignal new_high_signal_1(2, 0);
    NewHighSignal *created_indicator_1 = static_cast<NewHighSignal *>(create_indicator_from_id(new_high_signal_1.id, new_high_signal_1.params));
    std::vector<double> result_signals_1 = new_high_signal_1.calculate(candles);
    std::vector<double> expected1 = {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0};
    ASSERT_EQ(result_signals_1, expected1);
    ASSERT_NE(created_indicator_1, nullptr);
    ASSERT_EQ(created_indicator_1->calculate(candles), expected1);

    // left_bars = 2, right_bars = 2
    NewHighSignal new_high_signal_2(2, 2);
    NewHighSignal *created_indicator_2 = static_cast<NewHighSignal *>(create_indicator_from_id(new_high_signal_2.id, new_high_signal_2.params));
    std::vector<double> result_signals2 = new_high_signal_2.calculate(candles);
    std::vector<double> expected2 = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    ASSERT_EQ(result_signals2, expected2);
    ASSERT_NE(created_indicator_2, nullptr);
    ASSERT_EQ(created_indicator_2->calculate(candles), expected2);
}

TEST(SignalsIndicatorsTest, NewLowSignal)
{
    std::vector<Candle> candles = {
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

    // left_bars = 2, right_bars = 0
    NewLowSignal new_low_signal_1(2, 0);
    std::vector<double> result_signals_1 = new_low_signal_1.calculate(candles);
    NewLowSignal *created_indicator_1 = static_cast<NewLowSignal *>(create_indicator_from_id(new_low_signal_1.id, new_low_signal_1.params));
    std::vector<double> expected1 = {0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0};
    ASSERT_EQ(result_signals_1, expected1);
    ASSERT_NE(created_indicator_1, nullptr);
    ASSERT_EQ(created_indicator_1->calculate(candles), expected1);

    // left_bars = 2, right_bars = 2
    NewLowSignal new_low_signal_2(2, 2);
    NewLowSignal *created_indicator_2 = static_cast<NewLowSignal *>(create_indicator_from_id(new_low_signal_2.id, new_low_signal_2.params));
    std::vector<double> result_signals2 = new_low_signal_2.calculate(candles);
    std::vector<double> expected2 = {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0};
    ASSERT_EQ(result_signals2, expected2);
    ASSERT_NE(created_indicator_2, nullptr);
    ASSERT_EQ(created_indicator_2->calculate(candles), expected2);
}

TEST(SignalsIndicatorsTest, HighBreakSignal)
{
    std::vector<Candle> candles = {
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

    // left_bars = 2, right_bars = 0
    HighBreakSignal high_break_signal_1(2, 0);
    HighBreakSignal *created_indicator_1 = static_cast<HighBreakSignal *>(create_indicator_from_id(high_break_signal_1.id, high_break_signal_1.params));
    std::vector<double> result_signals_1 = high_break_signal_1.calculate(candles);
    std::vector<double> expected1 = {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0};
    ASSERT_EQ(result_signals_1, expected1);
    ASSERT_NE(created_indicator_1, nullptr);
    ASSERT_EQ(created_indicator_1->calculate(candles), expected1);

    // left_bars = 2, right_bars = 2
    HighBreakSignal high_break_signal_2(2, 2);
    HighBreakSignal *created_indicator_2 = static_cast<HighBreakSignal *>(create_indicator_from_id(high_break_signal_2.id, high_break_signal_2.params));
    std::vector<double> result_signals2 = high_break_signal_2.calculate(candles);
    std::vector<double> expected2 = {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0};
    ASSERT_EQ(result_signals2, expected2);
    ASSERT_NE(created_indicator_2, nullptr);
    ASSERT_EQ(created_indicator_2->calculate(candles), expected2);
}

TEST(SignalsIndicatorsTest, LowBreakSignal)
{
    std::vector<Candle> candles = {
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

    // left_bars = 2, right_bars = 0
    LowBreakSignal low_break_signal_1(2, 0);
    LowBreakSignal *created_indicator_1 = static_cast<LowBreakSignal *>(create_indicator_from_id(low_break_signal_1.id, low_break_signal_1.params));
    std::vector<double> result_signals_1 = low_break_signal_1.calculate(candles);
    std::vector<double> expected1 = {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0};
    ASSERT_EQ(result_signals_1, expected1);
    ASSERT_NE(created_indicator_1, nullptr);
    ASSERT_EQ(created_indicator_1->calculate(candles), expected1);

    // left_bars = 2, right_bars = 2
    LowBreakSignal low_break_signal_2(2, 2);
    LowBreakSignal *created_indicator_2 = static_cast<LowBreakSignal *>(create_indicator_from_id(low_break_signal_2.id, low_break_signal_2.params));
    std::vector<double> result_signals2 = low_break_signal_2.calculate(candles);
    std::vector<double> expected2 = {0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0};
    ASSERT_EQ(result_signals2, expected2);
    ASSERT_NE(created_indicator_2, nullptr);
    ASSERT_EQ(created_indicator_2->calculate(candles), expected2);
};
