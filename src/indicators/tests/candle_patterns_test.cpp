#include <gtest/gtest.h>
#include <vector>
#include "../candle_patterns.hpp"

TEST(CandlePatternsTest, DojiCandlePatternPatternTest)
{
    DojiCandlePattern pattern;
    std::vector<Candle> candles = {
        {.open = 5.0, .high = 8.0, .low = 3.0, .close = 6.0},  // classic doji
        {.open = 5.0, .high = 10.0, .low = 1.0, .close = 6.0}, // long-legged doji
        {.open = 5.2, .high = 5.7, .low = 1.0, .close = 5.5},  // dragonfly doji
        {.open = 5.0, .high = 9.0, .low = 4.7, .close = 5.2},  // gravestone doji
    };

    std::vector<double> result = pattern.calculate(candles);
    std::vector<double> expected = {1, 2, 3, 4};

    ASSERT_EQ(result, expected);
}

TEST(CandlePatternsTest, SingleCandlePatternTest)
{
    SingleCandlePattern pattern;
    std::vector<Candle> candles = {
        // hammer
        {.open = 10.0, .high = 10.5, .low = 6.0, .close = 10.5},
        // inverted hammer
        {.open = 6.1, .high = 12.0, .low = 6.0, .close = 6.5},
        // shooting star
        {.open = 11.0, .high = 15.0, .low = 10.0, .close = 10.2},
        // hanging man
        {.open = 14.8, .high = 15.0, .low = 10.0, .close = 13.8},
        // bullish spinning top
        {.open = 10.0, .high = 11.5, .low = 9.0, .close = 10.5},
        // bearish spinning top
        {.open = 11.0, .high = 12.0, .low = 9.5, .close = 10.5},
        // bullish marubozu
        {.open = 10.0, .high = 12.0, .low = 10.0, .close = 11.8},
        // bearish marubozu
        {.open = 12.0, .high = 12.0, .low = 10.0, .close = 10.2},
    };

    std::vector<double> result = pattern.calculate(candles);
    std::vector<double> expected = {1, 2, -1, -2, 3, -3, 4, -4};

    ASSERT_EQ(result, expected);
}

TEST(CandlePatternsTest, DoubleCandlePatternTest)
{
    DoubleCandlePattern pattern;
    std::vector<Candle> candles = {
        // bullish engulfing
        {.open = 5.0, .high = 6.0, .low = 3.5, .close = 4.0},
        {.open = 3.0, .high = 8.0, .low = 3.0, .close = 7.0},
        // bearish engulfing
        {.open = 5.0, .high = 6.5, .low = 3.0, .close = 6.0},
        {.open = 6.5, .high = 7.0, .low = 2.0, .close = 2.5},
        // piercing line
        {.open = 5.0, .high = 6.0, .low = 2.0, .close = 3.0},
        {.open = 2.5, .high = 5.0, .low = 1.5, .close = 4.5},
        // dark cloud cover
        {.open = 5.0, .high = 8.0, .low = 4.5, .close = 7.0},
        {.open = 7.5, .high = 8.5, .low = 4.5, .close = 5.5},
    };

    std::vector<double> result = pattern.calculate(candles);
    std::vector<double> expected = {0, 1, 0, -1, 0, 2, 0, -2};

    ASSERT_EQ(result, expected);
}

TEST(CandlePatternsTest, TripleCandlePatternTest)
{
    TripleCandlePattern pattern;
    std::vector<double> result;
    std::vector<double> expected;

    std::vector<Candle> candles = {
        // three inside down
        {.open = 14.5, .high = 15.0, .low = 14.2, .close = 15.2},
        {.open = 15.3, .high = 16.0, .low = 14.8, .close = 15.7},
        {.open = 15.8, .high = 16.0, .low = 15.3, .close = 15.0},
    };

    std::vector<Candle> three_white_soldiers = {
        {.open = 5.0, .high = 6.0, .low = 4.5, .close = 5.5},
        {.open = 5.5, .high = 6.5, .low = 5.0, .close = 6.0},
        {.open = 6.0, .high = 7.0, .low = 5.5, .close = 6.5}};
    result = pattern.calculate(three_white_soldiers);
    expected = {0, 0, 1};
    ASSERT_EQ(result, expected);

    std::vector<Candle> three_black_crows = {
        {.open = 5.0, .high = 6.0, .low = 4.5, .close = 4.5},
        {.open = 4.5, .high = 5.0, .low = 4.0, .close = 4.0},
        {.open = 4.0, .high = 4.5, .low = 3.5, .close = 3.5}};
    result = pattern.calculate(three_black_crows);
    expected = {0, 0, -1};
    ASSERT_EQ(result, expected);

    std::vector<Candle> morning_star = {
        {.open = 12.0, .high = 14.0, .low = 10.0, .close = 11.8},
        {.open = 11.7, .high = 12.5, .low = 11.0, .close = 12.0},
        {.open = 11.9, .high = 13.5, .low = 11.8, .close = 13.0}};
    result = pattern.calculate(morning_star);
    expected = {0, 0, 2};
    ASSERT_EQ(result, expected);

    std::vector<Candle> evening_star = {
        {.open = 10.0, .high = 12.0, .low = 9.5, .close = 11.5},
        {.open = 11.8, .high = 12.5, .low = 11.0, .close = 11.0},
        {.open = 11.2, .high = 13.5, .low = 10.8, .close = 10.5}};
    result = pattern.calculate(evening_star);
    expected = {0, 0, -2};
    ASSERT_EQ(result, expected);

    std::vector<Candle> morning_doji_star = {
        {.open = 12.0, .high = 14.0, .low = 10.0, .close = 11.5},
        {.open = 11.6, .high = 12.0, .low = 11.0, .close = 11.6},
        {.open = 11.7, .high = 13.0, .low = 11.5, .close = 11.8}};
    result = pattern.calculate(morning_doji_star);
    expected = {0, 0, 3};
    ASSERT_EQ(result, expected);

    std::vector<Candle> evening_doji_star = {
        {.open = 11.5, .high = 14.0, .low = 10.0, .close = 12.5},
        {.open = 12.0, .high = 12.5, .low = 11.5, .close = 12.0},
        {.open = 12.4, .high = 13.0, .low = 11.8, .close = 11.6}};
    result = pattern.calculate(evening_doji_star);
    expected = {0, 0, -3};
    ASSERT_EQ(result, expected);

    std::vector<Candle> three_inside_up = {
        {.open = 14.0, .high = 14.2, .low = 13.0, .close = 13.0},
        {.open = 13.1, .high = 13.7, .low = 12.7, .close = 13.4},
        {.open = 13.4, .high = 14.1, .low = 13.2, .close = 14.3}};
    result = pattern.calculate(three_inside_up);
    expected = {0, 0, 4};
    ASSERT_EQ(result, expected);

    std::vector<Candle> three_inside_down = {
        {.open = 12.0, .high = 13.5, .low = 11.9, .close = 13.2},
        {.open = 13.1, .high = 13.3, .low = 12.5, .close = 12.9},
        {.open = 12.9, .high = 13.2, .low = 11.6, .close = 11.8}};
    result = pattern.calculate(three_inside_down);
    expected = {0, 0, -4};
    ASSERT_EQ(result, expected);

    std::vector<Candle> three_outside_up = {
        {.open = 14.0, .high = 14.2, .low = 13.0, .close = 13.0},
        {.open = 12.9, .high = 13.7, .low = 12.7, .close = 13.2},
        {.open = 13.2, .high = 14.1, .low = 13.2, .close = 14.3}};
    result = pattern.calculate(three_outside_up);
    expected = {0, 0, 5};
    ASSERT_EQ(result, expected);

    std::vector<Candle> three_outside_down = {
        {.open = 12.0, .high = 13.5, .low = 11.9, .close = 13.2},
        {.open = 13.3, .high = 13.3, .low = 12.5, .close = 13.1},
        {.open = 12.9, .high = 13.2, .low = 11.6, .close = 11.8}};
    result = pattern.calculate(three_outside_down);
    expected = {0, 0, -5};
    ASSERT_EQ(result, expected);

    std::vector<Candle> three_stars_in_the_south = {
        {.open = 10.0, .high = 10.0, .low = 7.0, .close = 9.0},
        {.open = 9.5, .high = 9.7, .low = 8.5, .close = 9.0},
        {.open = 9.0, .high = 9.0, .low = 8.5, .close = 8.5}};
    result = pattern.calculate(three_stars_in_the_south);
    expected = {0, 0, 6};
    ASSERT_EQ(result, expected);

    std::vector<Candle> three_stars_in_the_north = {
        {.open = 14.0, .high = 17.0, .low = 14.0, .close = 15.0},
        {.open = 14.5, .high = 14.3, .low = 15.5, .close = 15.0},
        {.open = 15.0, .high = 15.5, .low = 15.0, .close = 15.5}};
    result = pattern.calculate(three_stars_in_the_north);
    expected = {0, 0, -6};
    ASSERT_EQ(result, expected);
}
