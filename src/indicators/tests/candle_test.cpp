#include <gtest/gtest.h>
#include <cmath>
#include <ctime>
#include <vector>
#include <chrono>
#include "../candle.hpp"
#include "../../types.hpp"
#include "../builder.hpp"

class CandleIndicatorsTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Define a tm struct with the desired date and time
        std::tm timeInfo = {
            .tm_sec = 0,
            .tm_min = 0,
            .tm_hour = 0,
            .tm_mday = 1,
            .tm_mon = 1,
            .tm_year = 2023 - 1900,
        };

        // Convert tm struct to time_t
        std::time_t date = std::mktime(&timeInfo);

        // Mock data initialization
        mock_candles = {
            Candle{.date = date, .open = 10, .high = 15, .low = 8, .close = 15, .volume = 10, .tick_volume = 10},
            Candle{.date = date, .open = 15, .high = 16, .low = 8, .close = 9, .volume = 10, .tick_volume = 10},
            Candle{.date = date, .open = 9, .high = 12, .low = 6, .close = 12, .volume = 15, .tick_volume = 15},
            Candle{.date = date, .open = 12, .high = 13, .low = 8, .close = 6, .volume = 20, .tick_volume = 20},
            Candle{.date = date, .open = 6, .high = 7, .low = 5, .close = 5, .volume = 10, .tick_volume = 10},
            Candle{.date = date, .open = 8, .high = 17, .low = 5, .close = 17, .volume = 5, .tick_volume = 5},
            Candle{.date = date, .open = 17, .high = 18, .low = 6, .close = 14, .volume = 10, .tick_volume = 10},
            Candle{.date = date, .open = 14, .high = 21, .low = 8, .close = 11, .volume = 13, .tick_volume = 13},
            Candle{.date = date, .open = 11, .high = 12, .low = 2, .close = 4, .volume = 11, .tick_volume = 11},
            Candle{.date = date, .open = 4, .high = 21, .low = 8, .close = 20, .volume = 10, .tick_volume = 10},
            Candle{.date = date, .open = 20, .high = 20, .low = 5, .close = 18, .volume = 12, .tick_volume = 12}};
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

    CandleOpen *created_open_indicator = static_cast<CandleOpen *>(create_indicator_from_id(open_indicator.id, open_indicator.params));
    std::vector<double> result_created_open_indicator = created_open_indicator->calculate(mock_candles);
    ASSERT_NE(created_open_indicator, nullptr);
    ASSERT_EQ(result_created_open_indicator, expected_open);

    // CandleHigh
    CandleHigh high_indicator;
    std::vector<double> result_high = high_indicator.calculate(mock_candles);
    std::vector<double> expected_high = {15, 16, 12, 13, 7, 17, 18, 21, 12, 21, 20};
    ASSERT_EQ(result_high, expected_high);

    CandleHigh *created_high_indicator = static_cast<CandleHigh *>(create_indicator_from_id(high_indicator.id, high_indicator.params));
    std::vector<double> result_created_high_indicator = created_high_indicator->calculate(mock_candles);
    ASSERT_NE(created_high_indicator, nullptr);
    ASSERT_EQ(result_created_high_indicator, expected_high);

    // CandleLow
    CandleLow low_indicator;
    std::vector<double> result_low = low_indicator.calculate(mock_candles);
    std::vector<double> expected_low = {8, 8, 6, 8, 5, 5, 6, 8, 2, 8, 5};
    ASSERT_EQ(result_low, expected_low);

    CandleLow *created_low_indicator = static_cast<CandleLow *>(create_indicator_from_id(low_indicator.id, low_indicator.params));
    std::vector<double> result_created_low_indicator = created_low_indicator->calculate(mock_candles);
    ASSERT_NE(created_low_indicator, nullptr);
    ASSERT_EQ(result_created_low_indicator, expected_low);

    // CandleClose
    CandleClose close_indicator;
    std::vector<double> result_close = close_indicator.calculate(mock_candles);
    std::vector<double> expected_close = {15, 9, 12, 6, 5, 17, 14, 11, 4, 20, 18};
    ASSERT_EQ(result_close, expected_close);

    CandleClose *created_close_indicator = static_cast<CandleClose *>(create_indicator_from_id(close_indicator.id, close_indicator.params));
    std::vector<double> result_created_close_indicator = created_close_indicator->calculate(mock_candles);
    ASSERT_NE(created_close_indicator, nullptr);
    ASSERT_EQ(result_created_close_indicator, expected_close);

    // CandleVolume
    CandleVolume volume_indicator;
    std::vector<double> result_volume = volume_indicator.calculate(mock_candles);
    std::vector<double> expected_volume = {10, 10, 15, 20, 10, 5, 10, 13, 11, 10, 12};
    ASSERT_EQ(result_volume, expected_volume);

    std::unordered_map<std::string, IndicatorParam> params_volume = extract_parameters(volume_indicator.id_params, volume_indicator.id_params_pattern);
    CandleVolume *created_volume_indicator = static_cast<CandleVolume *>(create_indicator_from_id(volume_indicator.id, volume_indicator.params));
    std::vector<double> result_created_volume_indicator = created_volume_indicator->calculate(mock_candles);
    ASSERT_NE(created_volume_indicator, nullptr);
    ASSERT_EQ(result_created_volume_indicator, expected_volume);
}

TEST_F(CandleIndicatorsTest, PriceWithOffset)
{
    // Offset 1
    CandleOpen open_indicator(1);
    std::vector<double> result_open = open_indicator.calculate(mock_candles);
    std::vector<double> expected_open = {0, 10, 15, 9, 12, 6, 8, 17, 14, 11, 4};
    ASSERT_EQ(result_open, expected_open);

    CandleOpen *created_open_indicator = static_cast<CandleOpen *>(create_indicator_from_id(open_indicator.id, open_indicator.params));
    ASSERT_NE(created_open_indicator, nullptr);
    ASSERT_EQ(created_open_indicator->calculate(mock_candles), expected_open);

    // Offset 1
    CandleHigh high_indicator(1);
    std::vector<double> result_high = high_indicator.calculate(mock_candles);
    std::vector<double> expected_high = {0, 15, 16, 12, 13, 7, 17, 18, 21, 12, 21};
    ASSERT_EQ(result_high, expected_high);

    CandleHigh *created_high_indicator = static_cast<CandleHigh *>(create_indicator_from_id(high_indicator.id, high_indicator.params));
    std::vector<double> result_created_high_indicator = created_high_indicator->calculate(mock_candles);

    // Offset 1
    CandleLow low_indicator(1);
    std::vector<double> result_low = low_indicator.calculate(mock_candles);
    std::vector<double> expected_low = {0, 8, 8, 6, 8, 5, 5, 6, 8, 2, 8};
    ASSERT_EQ(result_low, expected_low);

    CandleLow *created_low_indicator = static_cast<CandleLow *>(create_indicator_from_id(low_indicator.id, low_indicator.params));
    std::vector<double> result_created_low_indicator = created_low_indicator->calculate(mock_candles);
    ASSERT_NE(created_low_indicator, nullptr);
    ASSERT_EQ(result_low, result_created_low_indicator);

    // Offset 1
    CandleClose close_indicator(1);
    std::vector<double> result_close = close_indicator.calculate(mock_candles);
    std::vector<double> expected_close = {0, 15, 9, 12, 6, 5, 17, 14, 11, 4, 20};
    ASSERT_EQ(result_close, expected_close);

    CandleClose *created_close_indicator = static_cast<CandleClose *>(create_indicator_from_id(close_indicator.id, close_indicator.params));
    std::vector<double> result_created_close_indicator = created_close_indicator->calculate(mock_candles);
    ASSERT_NE(created_close_indicator, nullptr);
    ASSERT_EQ(result_close, result_created_close_indicator);
}

TEST_F(CandleIndicatorsTest, WhiteCandle)
{
    WhiteCandle white_indicator;
    std::vector<double> result = white_indicator.calculate(mock_candles);
    std::vector<double> expected = {1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0};
    ASSERT_EQ(result, expected);

    WhiteCandle *created_indicator = static_cast<WhiteCandle *>(create_indicator_from_id(white_indicator.id, white_indicator.params));
    std::vector<double> result_created_indicator = created_indicator->calculate(mock_candles);
    ASSERT_NE(created_indicator, nullptr);
    ASSERT_EQ(result, result_created_indicator);
}

TEST_F(CandleIndicatorsTest, BlackCandle)
{
    BlackCandle black_indicator;
    std::vector<double> result = black_indicator.calculate(mock_candles);
    std::vector<double> expected = {0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1};
    ASSERT_EQ(result, expected);

    BlackCandle *created_indicator = static_cast<BlackCandle *>(create_indicator_from_id(black_indicator.id, black_indicator.params));
    std::vector<double> result_created_indicator = created_indicator->calculate(mock_candles);
    ASSERT_NE(created_indicator, nullptr);
    ASSERT_EQ(result, result_created_indicator);
}

TEST_F(CandleIndicatorsTest, CandleBody)
{
    CandleBody body_indicator;
    std::vector<double> result = body_indicator.calculate(mock_candles);
    for (int i = 0; i < result.size(); ++i)
    {
        ASSERT_GE(result[i], 0);
        ASSERT_LE(result[i], 1);
    }

    CandleBody *created_indicator = static_cast<CandleBody *>(create_indicator_from_id(body_indicator.id, body_indicator.params));
    std::vector<double> result_created_indicator = created_indicator->calculate(mock_candles);
    ASSERT_NE(created_indicator, nullptr);
    ASSERT_EQ(result, result_created_indicator);
}

TEST_F(CandleIndicatorsTest, CandleShadowUpper)
{
    CandleShadowUpper shadow_upper_indicator;
    std::vector<double> result = shadow_upper_indicator.calculate(mock_candles);
    for (int i = 0; i < result.size(); ++i)
    {
        ASSERT_GE(result[i], 0);
        ASSERT_LE(result[i], 1);
    }

    CandleShadowUpper *created_indicator = static_cast<CandleShadowUpper *>(create_indicator_from_id(shadow_upper_indicator.id, shadow_upper_indicator.params));
    std::vector<double> result_created_indicator = created_indicator->calculate(mock_candles);
    ASSERT_NE(created_indicator, nullptr);
    ASSERT_EQ(result, result_created_indicator);
}

TEST_F(CandleIndicatorsTest, CandleShadowLower)
{
    CandleShadowLower shadow_lower_indicator;
    std::vector<double> result = shadow_lower_indicator.calculate(mock_candles);
    for (int i = 0; i < result.size(); ++i)
    {
        ASSERT_GE(result[i], 0);
        ASSERT_LE(result[i], 1);
    }

    CandleShadowLower *created_indicator = static_cast<CandleShadowLower *>(create_indicator_from_id(shadow_lower_indicator.id, shadow_lower_indicator.params));
    std::vector<double> result_created_indicator = created_indicator->calculate(mock_candles);
    ASSERT_NE(created_indicator, nullptr);
    ASSERT_EQ(result, result_created_indicator);
}

TEST_F(CandleIndicatorsTest, CandlePriceChange)
{
    CandlePriceChange indicator;
    std::vector<double> result = indicator.calculate(mock_candles);
    std::vector<double> expected = {0.5, -0.4, 0.3333333333333333, -0.5, -0.16666666666666666, 1.125,
                                    -0.17647058823529413, -0.21428571428571427, -0.6363636363636364, 4.0, -0.1};
    ASSERT_EQ(result, expected);

    CandlePriceChange *created_indicator = static_cast<CandlePriceChange *>(create_indicator_from_id(indicator.id, indicator.params));
    std::vector<double> result_created_indicator = created_indicator->calculate(mock_candles);
    ASSERT_NE(created_indicator, nullptr);
    ASSERT_EQ(result, result_created_indicator);
}

TEST_F(CandleIndicatorsTest, PivotHigh)
{
    // PivotHigh with left_bars=2, right_bars=0
    PivotHigh pivot_high_indicator("high", 2, 0);
    std::vector<double> result = pivot_high_indicator.calculate(mock_candles);
    std::vector<double> expected = {1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0};
    ASSERT_EQ(result, expected);

    PivotHigh *created_indicator = static_cast<PivotHigh *>(create_indicator_from_id(pivot_high_indicator.id, pivot_high_indicator.params));
    std::vector<double> result_created_indicator = created_indicator->calculate(mock_candles);
    ASSERT_NE(created_indicator, nullptr);
    ASSERT_EQ(result, result_created_indicator);

    // PivotHigh with left_bars=2, right_bars=2
    PivotHigh pivot_high_indicator2("high", 2, 2);
    std::vector<double> result2 = pivot_high_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0};
    ASSERT_EQ(result2, expected2);

    PivotHigh *created_indicator2 = static_cast<PivotHigh *>(create_indicator_from_id(pivot_high_indicator2.id, pivot_high_indicator2.params));
    std ::vector<double> result_created_indicator2 = created_indicator2->calculate(mock_candles);
    ASSERT_NE(created_indicator2, nullptr);
    ASSERT_EQ(result2, result_created_indicator2);
}

TEST_F(CandleIndicatorsTest, PivotLow)
{
    // PivotLow with left_bars=2, right_bars=0
    PivotLow pivot_low_indicator("low", 2, 0);
    std::vector<double> result = pivot_low_indicator.calculate(mock_candles);
    std::vector<double> expected = {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0};
    ASSERT_EQ(result, expected);

    PivotLow *created_indicator = static_cast<PivotLow *>(create_indicator_from_id(pivot_low_indicator.id, pivot_low_indicator.params));
    std::vector<double> result_created_indicator = created_indicator->calculate(mock_candles);
    ASSERT_NE(created_indicator, nullptr);
    ASSERT_EQ(result, result_created_indicator);

    // PivotLow with left_bars=2, right_bars=2
    PivotLow pivot_low_indicator2("low", 2, 2);
    std::vector<double> result2 = pivot_low_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0};
    ASSERT_EQ(result2, expected2);

    PivotLow *created_indicator2 = static_cast<PivotLow *>(create_indicator_from_id(pivot_low_indicator2.id, pivot_low_indicator2.params));
    std::vector<double> result_created_indicator2 = created_indicator2->calculate(mock_candles);
    ASSERT_NE(created_indicator2, nullptr);
    ASSERT_EQ(result2, result_created_indicator2);
}

TEST_F(CandleIndicatorsTest, PivotHighValue)
{
    // with left_bars=2, right_bars=0
    PivotHighValue pivot_high_value("high", 2, 0);
    std::vector<double> result = pivot_high_value.calculate(mock_candles);
    std::vector<double> expected = {15, 16, 16, 16, 16, 17, 18, 21, 21, 21, 21};
    ASSERT_EQ(result, expected);

    PivotHighValue *created_indicator = static_cast<PivotHighValue *>(create_indicator_from_id(pivot_high_value.id, pivot_high_value.params));
    std::vector<double> result_created_indicator = created_indicator->calculate(mock_candles);
    ASSERT_NE(created_indicator, nullptr);
    ASSERT_EQ(result, result_created_indicator);

    // with left_bars=2, right_bars=2
    PivotHighValue highest_indicator2("high", 2, 2);
    std::vector<double> result2 = highest_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {15, 16, 16, 16, 16, 16, 16, 16, 16, 21, 21};
    ASSERT_EQ(result2, expected2);

    PivotHighValue *created_indicator2 = static_cast<PivotHighValue *>(create_indicator_from_id(highest_indicator2.id, highest_indicator2.params));
    std::vector<double> result_created_indicator2 = created_indicator2->calculate(mock_candles);
    ASSERT_NE(created_indicator2, nullptr);
    ASSERT_EQ(result2, result_created_indicator2);
}

TEST_F(CandleIndicatorsTest, PivotLowValue)
{
    // with left_bars=2, right_bars=0
    PivotLowValue lowest_indicator("low", 2, 0);
    std::vector<double> result = lowest_indicator.calculate(mock_candles);
    std::vector<double> expected = {8, 8, 6, 6, 5, 5, 5, 5, 2, 2, 2};
    ASSERT_EQ(result, expected);

    PivotLowValue *created_indicator = static_cast<PivotLowValue *>(create_indicator_from_id(lowest_indicator.id, lowest_indicator.params));
    std::vector<double> result_created_indicator = created_indicator->calculate(mock_candles);
    ASSERT_NE(created_indicator, nullptr);
    ASSERT_EQ(result, result_created_indicator);

    // with left_bars=2, right_bars=2
    PivotLowValue lowest_indicator2("low", 2, 2);
    std::vector<double> result2 = lowest_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {8, 8, 8, 8, 8, 8, 5, 5, 5, 5, 2};
    ASSERT_EQ(result2, expected2);

    PivotLowValue *created_indicator2 = static_cast<PivotLowValue *>(create_indicator_from_id(lowest_indicator2.id, lowest_indicator2.params));
    std::vector<double> result_created_indicator2 = created_indicator2->calculate(mock_candles);
    ASSERT_NE(created_indicator2, nullptr);
    ASSERT_EQ(result2, result_created_indicator2);
}

TEST_F(CandleIndicatorsTest, HighestHigh)
{
    // with left_bars=2, right_bars=0
    HighestHigh highest_indicator(2, 0);
    std::vector<double> result = highest_indicator.calculate(mock_candles);
    std::vector<double> expected = {15, 16, 16, 16, 13, 17, 18, 21, 21, 21, 21};
    ASSERT_EQ(result, expected);

    HighestHigh *created_indicator = static_cast<HighestHigh *>(create_indicator_from_id(highest_indicator.id, highest_indicator.params));
    std::vector<double> result_created_indicator = created_indicator->calculate(mock_candles);
    ASSERT_NE(created_indicator, nullptr);
    ASSERT_EQ(result, result_created_indicator);

    // with left_bars=2, right_bars=2
    HighestHigh highest_indicator2(2, 2);
    std::vector<double> result2 = highest_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {16, 16, 16, 17, 18, 21, 21, 21, 21, 21, 21};
    ASSERT_EQ(result2, expected2);

    HighestHigh *created_indicator2 = static_cast<HighestHigh *>(create_indicator_from_id(highest_indicator2.id, highest_indicator2.params));
    std::vector<double> result_created_indicator2 = created_indicator2->calculate(mock_candles);
    ASSERT_NE(created_indicator2, nullptr);
    ASSERT_EQ(result2, result_created_indicator2);
}

TEST_F(CandleIndicatorsTest, LowestLow)
{
    // with left_bars=2, right_bars=0
    LowestLow lowest_indicator(2, 0);
    std::vector<double> result = lowest_indicator.calculate(mock_candles);
    std::vector<double> expected = {8, 8, 6, 6, 5, 5, 5, 5, 2, 2, 2};
    ASSERT_EQ(result, expected);

    LowestLow *created_indicator = static_cast<LowestLow *>(create_indicator_from_id(lowest_indicator.id, lowest_indicator.params));
    std::vector<double> result_created_indicator = created_indicator->calculate(mock_candles);
    ASSERT_NE(created_indicator, nullptr);
    ASSERT_EQ(result, result_created_indicator);

    // with left_bars=2, right_bars=2
    LowestLow lowest_indicator2(2, 2);
    std::vector<double> result2 = lowest_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {6, 6, 5, 5, 5, 5, 2, 2, 2, 2, 2};
    ASSERT_EQ(result2, expected2);

    LowestLow *created_indicator2 = static_cast<LowestLow *>(create_indicator_from_id(lowest_indicator2.id, lowest_indicator2.params));
    std::vector<double> result_created_indicator2 = created_indicator2->calculate(mock_candles);
    ASSERT_NE(created_indicator2, nullptr);
    ASSERT_EQ(result2, result_created_indicator2);
}

TEST_F(CandleIndicatorsTest, PeakDistance)
{
    // with left_bars=2, right_bars=0
    PeakDistance peak_distance_indicator(2, 0);
    std::vector<double> result = peak_distance_indicator.calculate(mock_candles);
    std::vector<double> expected = {1.875, 2, 2.6666666666666665, 2.6666666666666665, 2.6, 3.4, 3.6, 4.2, 10.5, 10.5, 10.5};
    ASSERT_EQ(result, expected);

    PeakDistance *created_indicator = static_cast<PeakDistance *>(create_indicator_from_id(peak_distance_indicator.id, peak_distance_indicator.params));
    std::vector<double> result_created_indicator = peak_distance_indicator.calculate(mock_candles);
    ASSERT_NE(created_indicator, nullptr);
    ASSERT_EQ(result, result_created_indicator);

    // with left_bars=2, right_bars=2
    PeakDistance peak_distance_indicator2(2, 2);
    std::vector<double> result2 = peak_distance_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {2.6666666666666665, 2.6666666666666665, 3.2, 3.4, 3.6, 4.2, 10.5, 10.5, 10.5, 10.5, 10.5};
    ASSERT_EQ(result2, expected2);

    PeakDistance *created_indicator2 = static_cast<PeakDistance *>(create_indicator_from_id(peak_distance_indicator2.id, peak_distance_indicator2.params));
    std::vector<double> result_created_indicator2 = peak_distance_indicator2.calculate(mock_candles);
    ASSERT_NE(created_indicator2, nullptr);
    ASSERT_EQ(result2, result_created_indicator2);
}

TEST_F(CandleIndicatorsTest, PeakCandleDistance)
{
    // with left_bars=2, right_bars=0
    PeakCandleDistance peak_candle_distance_indicator(2, 0);
    std::vector<double> result = peak_candle_distance_indicator.calculate(mock_candles);
    std::vector<double> expected = {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 2};
    ASSERT_EQ(result, expected);

    PeakCandleDistance *created_indicator = static_cast<PeakCandleDistance *>(create_indicator_from_id(peak_candle_distance_indicator.id, peak_candle_distance_indicator.params));
    std::vector<double> result_created_indicator = peak_candle_distance_indicator.calculate(mock_candles);
    ASSERT_NE(created_indicator, nullptr);
    ASSERT_EQ(result, result_created_indicator);

    // with left_bars=2, right_bars=2
    PeakCandleDistance peak_candle_distance_indicator2(2, 2);
    std::vector<double> result2 = peak_candle_distance_indicator2.calculate(mock_candles);
    std::vector<double> expected2 = {1, 0, 1, 2, 0, 1, 2, 0, 0, 1, 2};
    ASSERT_EQ(result2, expected2);

    PeakCandleDistance *created_indicator2 = static_cast<PeakCandleDistance *>(create_indicator_from_id(peak_candle_distance_indicator2.id, peak_candle_distance_indicator2.params));
    std::vector<double> result_created_indicator2 = peak_candle_distance_indicator2.calculate(mock_candles);
    ASSERT_NE(created_indicator2, nullptr);
    ASSERT_EQ(result2, result_created_indicator2);
}
