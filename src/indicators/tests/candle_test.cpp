#include <gtest/gtest.h>
#include <cmath>
#include <ctime>
#include <vector>
#include "../candle.hpp"
#include "../../types.hpp"

class TestCandleIndicators : public ::testing::Test
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
        time_t date = std::mktime(&timeInfo);

        // Mock data initialization
        mock_candles = {
            Candle{.date = date, .open = 10, .high = 12, .low = 8, .close = 15},
            Candle{.date = date, .open = 15, .high = 16, .low = 8, .close = 9},
            Candle{.date = date, .open = 9, .high = 10, .low = 6, .close = 12},
            Candle{.date = date, .open = 12, .high = 13, .low = 8, .close = 6},
            Candle{.date = date, .open = 6, .high = 7, .low = 5, .close = 5},
            Candle{.date = date, .open = 8, .high = 17, .low = 5, .close = 17},
            Candle{.date = date, .open = 17, .high = 18, .low = 6, .close = 14},
            Candle{.date = date, .open = 14, .high = 15, .low = 8, .close = 11},
            Candle{.date = date, .open = 11, .high = 12, .low = 2, .close = 4},
            Candle{.date = date, .open = 4, .high = 21, .low = 8, .close = 20},
            Candle{.date = date, .open = 20, .high = 20, .low = 5, .close = 18}};
    };

    void TearDown() override
    {
    }

    // Variables
    std::vector<Candle> mock_candles;
};

TEST_F(TestCandleIndicators, Price)
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
}

TEST_F(TestCandleIndicators, PriceWithOffset)
{
    // Offset 1
    CandleOpen open_indicator(1);
    std::vector<double> result_open = open_indicator.calculate(mock_candles);
    std::vector<double> expected_open = {10, 15, 9, 12, 6, 8, 17, 14, 11, 4};
    ASSERT_EQ(result_open.size(), expected_open.size());
    for (size_t i = 0; i < result_open.size(); ++i)
    {
        ASSERT_DOUBLE_EQ(result_open[i], expected_open[i]);
    }

    // Offset 1
    CandleHigh high_indicator(1);
    std::vector<double> result_high = high_indicator.calculate(mock_candles);
    std::vector<double> expected_high = {12, 16, 10, 13, 7, 17, 18, 15, 12, 21};
    ASSERT_EQ(result_high.size(), expected_high.size());
    for (size_t i = 0; i < result_high.size(); ++i)
    {
        ASSERT_DOUBLE_EQ(result_high[i], expected_high[i]);
    }

    // Offset 1
    CandleLow low_indicator(1);
    std::vector<double> result_low = low_indicator.calculate(mock_candles);
    std::vector<double> expected_low = {8, 8, 6, 8, 5, 5, 6, 8, 2, 8};
    ASSERT_EQ(result_low.size(), expected_low.size());
    for (size_t i = 0; i < result_low.size(); ++i)
    {
        ASSERT_DOUBLE_EQ(result_low[i], expected_low[i]);
    }

    // Offset 1
    CandleClose close_indicator(1);
    std::vector<double> result_close = close_indicator.calculate(mock_candles);
    std::vector<double> expected_close = {15, 9, 12, 6, 5, 17, 14, 11, 4, 20};
    ASSERT_EQ(result_close.size(), expected_close.size());
    for (size_t i = 0; i < result_close.size(); ++i)
    {
        ASSERT_DOUBLE_EQ(result_close[i], expected_close[i]);
    }
}

TEST_F(TestCandleIndicators, WhiteCandle)
{
    WhiteCandle white_indicator;
    std::vector<double> result = white_indicator.calculate(mock_candles);
    std::vector<double> expected = {1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0};
    ASSERT_EQ(result, expected);
}

TEST_F(TestCandleIndicators, BlackCandle)
{
    BlackCandle black_indicator;
    std::vector<double> result = black_indicator.calculate(mock_candles);
    std::vector<double> expected = {0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1};
    ASSERT_EQ(result, expected);
}

TEST_F(TestCandleIndicators, PercentageChangeCandle)
{
    CandlePriceChange indicator;
    std::vector<double> result = indicator.calculate(mock_candles);
    std::vector<double> expected = {0.5, -0.4, 0.3333333333333333, -0.5, -0.16666666666666666, 1.125,
                                    -0.17647058823529413, -0.21428571428571427, -0.6363636363636364, 4.0, -0.1};
    ASSERT_EQ(result, expected);
}

TEST_F(TestCandleIndicators, PivotHigh)
{
    // PivotHigh with left_bars=2, right_bars=0
    PivotHigh pivot_high_indicator(CandleSource::High, 2, 0);
    std::vector<double> result = pivot_high_indicator.calculate(mock_candles);
    std::vector<double> expected = {1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0};
    ASSERT_EQ(result, expected);

    // PivotHigh with left_bars=2, right_bars=2
    PivotHigh pivot_high_indicator2(CandleSource::High, 2, 2);
    std::vector<double> result2 = pivot_high_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    ASSERT_EQ(result2, expected2);
}

TEST_F(TestCandleIndicators, PivotLow)
{
    // PivotLow with left_bars=2, right_bars=0
    PivotLow pivot_low_indicator(CandleSource::Low, 2, 0);
    std::vector<double> result = pivot_low_indicator.calculate(mock_candles);
    std::vector<double> expected = {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0};
    ASSERT_EQ(result, expected);

    // PivotLow with left_bars=2, right_bars=2
    PivotLow pivot_low_indicator2(CandleSource::Low, 2, 2);
    std::vector<double> result2 = pivot_low_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0};
    ASSERT_EQ(result2, expected2);
}

TEST_F(TestCandleIndicators, Highest)
{
    // Highest with left_bars=2, right_bars=0
    Highest highest_indicator(CandleSource::High, 2, 0);
    std::vector<double> result = highest_indicator.calculate(mock_candles);
    std::vector<double> expected = {12, 16, 16, 16, 16, 17, 18, 18, 18, 21, 21};
    ASSERT_EQ(result, expected);

    // Highest with left_bars=2, right_bars=2
    Highest highest_indicator2(CandleSource::High, 2, 2);
    std::vector<double> result2 = highest_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {12, 12, 12, 16, 16, 16, 16, 16, 18, 18, 18};
    ASSERT_EQ(result2, expected2);
}

TEST_F(TestCandleIndicators, Lowest)
{
    // Lowest with left_bars=2, right_bars=0
    Lowest lowest_indicator(CandleSource::Low, 2, 0);
    std::vector<double> result = lowest_indicator.calculate(mock_candles);
    std::vector<double> expected = {8, 8, 6, 6, 5, 5, 5, 5, 2, 2, 2};
    ASSERT_EQ(result, expected);

    // Lowest with left_bars=2, right_bars=2
    Lowest lowest_indicator2(CandleSource::Low, 2, 2);
    std::vector<double> result2 = lowest_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {8, 8, 8, 8, 8, 8, 5, 5, 5, 5, 2};
    ASSERT_EQ(result2, expected2);
}
