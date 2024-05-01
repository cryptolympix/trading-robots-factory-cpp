#include <gtest/gtest.h>
#include <cmath>
#include <ctime>
#include <vector>
#include <chrono>
#include "../candle.hpp"
#include "../../types.hpp"

class CandleIndicatorsTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Define a tm struct with the desired date and time
        std::tm timeInfo = {
            .tm_year = 2023 - 1900,
            .tm_mon = 1,
            .tm_mday = 1,
            .tm_hour = 0,
            .tm_min = 0,
            .tm_sec = 0,
        };

        // Convert tm struct to time_t
        std::time_t date = std::mktime(&timeInfo);

        // Mock data initialization
        mock_candles = {
            Candle{.date = date, .open = 10, .high = 12, .low = 8, .close = 15, .volume = 10},
            Candle{.date = date, .open = 15, .high = 16, .low = 8, .close = 9, .volume = 10},
            Candle{.date = date, .open = 9, .high = 10, .low = 6, .close = 12, .volume = 15},
            Candle{.date = date, .open = 12, .high = 13, .low = 8, .close = 6, .volume = 20},
            Candle{.date = date, .open = 6, .high = 7, .low = 5, .close = 5, .volume = 10},
            Candle{.date = date, .open = 8, .high = 17, .low = 5, .close = 17, .volume = 5},
            Candle{.date = date, .open = 17, .high = 18, .low = 6, .close = 14, .volume = 10},
            Candle{.date = date, .open = 14, .high = 15, .low = 8, .close = 11, .volume = 13},
            Candle{.date = date, .open = 11, .high = 12, .low = 2, .close = 4, .volume = 11},
            Candle{.date = date, .open = 4, .high = 21, .low = 8, .close = 20, .volume = 10},
            Candle{.date = date, .open = 20, .high = 20, .low = 5, .close = 18, .volume = 12}};
    };

    // Variables
    std::vector<Candle> mock_candles;
};

TEST_F(CandleIndicatorsTest, Price)
{
    // CandleOpen
    CandleOpen open_indicator;
    std::vector<double> result_open = open_indicator.calculate(mock_candles);
    std::vector<double> expected_open = {10, 15, 9, 12, 6, 8, 17, 14, 11, 4, 20};
    ASSERT_EQ(result_open, expected_open);

    // CandleHigh
    CandleHigh high_indicator;
    std::vector<double> result_high = high_indicator.calculate(mock_candles);
    std::vector<double> expected_high = {12, 16, 10, 13, 7, 17, 18, 15, 12, 21, 20};
    ASSERT_EQ(result_high, expected_high);

    // CandleLow
    CandleLow low_indicator;
    std::vector<double> result_low = low_indicator.calculate(mock_candles);
    std::vector<double> expected_low = {8, 8, 6, 8, 5, 5, 6, 8, 2, 8, 5};
    ASSERT_EQ(result_low, expected_low);

    // CandleClose
    CandleClose close_indicator;
    std::vector<double> result_close = close_indicator.calculate(mock_candles);
    std::vector<double> expected_close = {15, 9, 12, 6, 5, 17, 14, 11, 4, 20, 18};
    ASSERT_EQ(result_close, expected_close);

    // CandleVolume
    CandleVolume volume_indicator;
    std::vector<double> result_volume = volume_indicator.calculate(mock_candles);
    std::vector<double> expected_volume = {10, 10, 15, 20, 10, 5, 10, 13, 11, 10, 12};
    ASSERT_EQ(result_volume, expected_volume);
}

TEST_F(CandleIndicatorsTest, PriceWithOffset)
{
    // Offset 1
    CandleOpen open_indicator(1);
    std::vector<double> result_open = open_indicator.calculate(mock_candles);
    std::vector<double> expected_open = {0, 10, 15, 9, 12, 6, 8, 17, 14, 11, 4};
    ASSERT_EQ(result_open.size(), expected_open.size());
    for (size_t i = 0; i < result_open.size(); ++i)
    {
        ASSERT_DOUBLE_EQ(result_open[i], expected_open[i]);
    }

    // Offset 1
    CandleHigh high_indicator(1);
    std::vector<double> result_high = high_indicator.calculate(mock_candles);
    std::vector<double> expected_high = {0, 12, 16, 10, 13, 7, 17, 18, 15, 12, 21};
    ASSERT_EQ(result_high.size(), expected_high.size());
    for (size_t i = 0; i < result_high.size(); ++i)
    {
        ASSERT_DOUBLE_EQ(result_high[i], expected_high[i]);
    }

    // Offset 1
    CandleLow low_indicator(1);
    std::vector<double> result_low = low_indicator.calculate(mock_candles);
    std::vector<double> expected_low = {0, 8, 8, 6, 8, 5, 5, 6, 8, 2, 8};
    ASSERT_EQ(result_low.size(), expected_low.size());
    for (size_t i = 0; i < result_low.size(); ++i)
    {
        ASSERT_DOUBLE_EQ(result_low[i], expected_low[i]);
    }

    // Offset 1
    CandleClose close_indicator(1);
    std::vector<double> result_close = close_indicator.calculate(mock_candles);
    std::vector<double> expected_close = {0, 15, 9, 12, 6, 5, 17, 14, 11, 4, 20};
    ASSERT_EQ(result_close.size(), expected_close.size());
    for (size_t i = 0; i < result_close.size(); ++i)
    {
        ASSERT_DOUBLE_EQ(result_close[i], expected_close[i]);
    }
}

TEST_F(CandleIndicatorsTest, WhiteCandle)
{
    WhiteCandle white_indicator;
    std::vector<double> result = white_indicator.calculate(mock_candles);
    std::vector<double> expected = {1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0};
    ASSERT_EQ(result, expected);
}

TEST_F(CandleIndicatorsTest, BlackCandle)
{
    BlackCandle black_indicator;
    std::vector<double> result = black_indicator.calculate(mock_candles);
    std::vector<double> expected = {0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1};
    ASSERT_EQ(result, expected);
}

TEST_F(CandleIndicatorsTest, CandleBody)
{
    CandleBody body_indicator;
    std::vector<double> result = body_indicator.calculate(mock_candles);
    std::vector<double> expected = {1.25, 0.75, 0.75, 1.2, 0.5, 0.75, 0.25, 0.42857142857142855, 0.7, 1.2307692307692308, 0.13333333333333333};
    ASSERT_EQ(result, expected);
}

TEST_F(CandleIndicatorsTest, CandleShadowUpper)
{
    CandleShadowUpper shadow_upper_indicator;
    std::vector<double> result = shadow_upper_indicator.calculate(mock_candles);
    std::vector<double> expected = {-0.75, 0.125, -0.5, 0.2, 0.5, 0, 0.083333333333333329, 0.14285714285714285, 0.1, 0.076923076923076927, 0};
    ASSERT_EQ(result, expected);
}

TEST_F(CandleIndicatorsTest, CandleShadowLower)
{
    CandleShadowLower shadow_lower_indicator;
    std::vector<double> result = shadow_lower_indicator.calculate(mock_candles);
    std::vector<double> expected = {0.5, 0.125, 0.75, -0.4, 0, 0.25, 0.66666666666666663, 0.42857142857142855, 0.2, -0.30769230769230771, 0.8666666666666667};
    ASSERT_EQ(result, expected);
}

TEST_F(CandleIndicatorsTest, CandlePriceChange)
{
    CandlePriceChange indicator;
    std::vector<double> result = indicator.calculate(mock_candles);
    std::vector<double> expected = {0.5, -0.4, 0.3333333333333333, -0.5, -0.16666666666666666, 1.125,
                                    -0.17647058823529413, -0.21428571428571427, -0.6363636363636364, 4.0, -0.1};
    ASSERT_EQ(result, expected);
}

TEST_F(CandleIndicatorsTest, PivotHigh)
{
    // PivotHigh with left_bars=2, right_bars=0
    PivotHigh pivot_high_indicator("high", 2, 0);
    std::vector<double> result = pivot_high_indicator.calculate(mock_candles);
    std::vector<double> expected = {1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0};
    ASSERT_EQ(result, expected);

    // PivotHigh with left_bars=2, right_bars=2
    PivotHigh pivot_high_indicator2("high", 2, 2);
    std::vector<double> result2 = pivot_high_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    ASSERT_EQ(result2, expected2);
}

TEST_F(CandleIndicatorsTest, PivotLow)
{
    // PivotLow with left_bars=2, right_bars=0
    PivotLow pivot_low_indicator("low", 2, 0);
    std::vector<double> result = pivot_low_indicator.calculate(mock_candles);
    std::vector<double> expected = {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0};
    ASSERT_EQ(result, expected);

    // PivotLow with left_bars=2, right_bars=2
    PivotLow pivot_low_indicator2("low", 2, 2);
    std::vector<double> result2 = pivot_low_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0};
    ASSERT_EQ(result2, expected2);
}

TEST_F(CandleIndicatorsTest, PivotHighValue)
{
    // with left_bars=2, right_bars=0
    PivotHighValue pivot_high_value("high", 2, 0);
    std::vector<double> result = pivot_high_value.calculate(mock_candles);
    std::vector<double> expected = {12, 16, 16, 16, 16, 17, 18, 18, 18, 21, 21};
    ASSERT_EQ(result, expected);

    // with left_bars=2, right_bars=2
    PivotHighValue highest_indicator2("high", 2, 2);
    std::vector<double> result2 = highest_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {12, 16, 16, 16, 16, 16, 16, 16, 18, 18, 18};
    ASSERT_EQ(result2, expected2);
}

TEST_F(CandleIndicatorsTest, PivotLowValue)
{
    // with left_bars=2, right_bars=0
    PivotLowValue lowest_indicator("low", 2, 0);
    std::vector<double> result = lowest_indicator.calculate(mock_candles);
    std::vector<double> expected = {8, 8, 6, 6, 5, 5, 5, 5, 2, 2, 2};
    ASSERT_EQ(result, expected);

    // with left_bars=2, right_bars=2
    PivotLowValue lowest_indicator2("low", 2, 2);
    std::vector<double> result2 = lowest_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {8, 8, 8, 8, 8, 8, 5, 5, 5, 5, 2};
    ASSERT_EQ(result2, expected2);
}

TEST_F(CandleIndicatorsTest, HighestHigh)
{
    // with left_bars=2, right_bars=0
    HighestHigh highest_indicator(2, 0);
    std::vector<double> result = highest_indicator.calculate(mock_candles);
    std::vector<double> expected = {12, 16, 16, 16, 13, 17, 18, 18, 18, 21, 21};
    ASSERT_EQ(result, expected);

    // with left_bars=2, right_bars=2
    HighestHigh highest_indicator2(2, 2);
    std::vector<double> result2 = highest_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {16, 16, 16, 17, 18, 18, 18, 21, 21, 21, 21};
    ASSERT_EQ(result2, expected2);
}

TEST_F(CandleIndicatorsTest, LowestLow)
{
    // with left_bars=2, right_bars=0
    LowestLow lowest_indicator(2, 0);
    std::vector<double> result = lowest_indicator.calculate(mock_candles);
    std::vector<double> expected = {8, 8, 6, 6, 5, 5, 5, 5, 2, 2, 2};
    ASSERT_EQ(result, expected);

    // with left_bars=2, right_bars=2
    LowestLow lowest_indicator2(2, 2);
    std::vector<double> result2 = lowest_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {6, 6, 5, 5, 5, 5, 2, 2, 2, 2, 2};
    ASSERT_EQ(result2, expected2);
}

TEST_F(CandleIndicatorsTest, PeakDistance)
{
    // with left_bars=2, right_bars=0
    PeakDistance peak_distance_indicator(2, 0);
    std::vector<double> result = peak_distance_indicator.calculate(mock_candles);
    std::vector<double> expected = {1.5, 2, 2.6666666666666665, 2.6666666666666665, 2.6, 3.4, 3.6, 3.6, 9, 10.5, 10.5};
    ASSERT_EQ(result, expected);

    // with left_bars=2, right_bars=2
    PeakDistance peak_distance_indicator2(2, 2);
    std::vector<double> result2 = peak_distance_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {2.6666666666666665, 2.6666666666666665, 3.2, 3.4, 3.6, 3.6, 9, 10.5, 10.5, 10.5, 10.5};
    ASSERT_EQ(result2, expected2);
}

TEST_F(CandleIndicatorsTest, PeakCandleDistance)
{
    // with left_bars=2, right_bars=0
    PeakCandleDistance peak_candle_distance_indicator(2, 0);
    std::vector<double> result = peak_candle_distance_indicator.calculate(mock_candles);
    std::vector<double> expected = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1};
    ASSERT_EQ(result, expected);

    // with left_bars=2, right_bars=2
    PeakCandleDistance peak_candle_distance_indicator2(2, 2);
    std::vector<double> result2 = peak_candle_distance_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {1, 0, 1, 2, 0, 1, 0, 1, 0, 1, 2};
    ASSERT_EQ(result2, expected2);
}
