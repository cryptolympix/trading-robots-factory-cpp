#include <gtest/gtest.h>
#include <ctime>
#include <vector>
#include <stdexcept>
#include <chrono>
#include "../candles_source.hpp"
#include "../../types.hpp"

class TestCandlesSource : public ::testing::Test
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
        mock_candles = {Candle{.date = date, .open = 2, .high = 3, .low = 1, .close = 2, .volume = 100},
                        Candle{.date = date, .open = 2, .high = 3, .low = 1, .close = 2, .volume = 110}};
    }

    std::vector<Candle> mock_candles;
};

TEST_F(TestCandlesSource, GetCandlesWithUnknownSource)
{
    EXPECT_THROW(get_candles_with_source(mock_candles, static_cast<CandleSource>(-1)), std::runtime_error);
}

TEST_F(TestCandlesSource, GetCandlesWithSource)
{
    // Open
    std::vector<double> candles = get_candles_with_source(mock_candles, CandleSource::Open);
    ASSERT_EQ(candles.size(), 2);
    EXPECT_EQ(candles[0], 2);
    EXPECT_EQ(candles[1], 2);

    // High
    candles = get_candles_with_source(mock_candles, CandleSource::High);
    ASSERT_EQ(candles.size(), 2);
    EXPECT_EQ(candles[0], 3);
    EXPECT_EQ(candles[1], 3);

    // Low
    candles = get_candles_with_source(mock_candles, CandleSource::Low);
    ASSERT_EQ(candles.size(), 2);
    EXPECT_EQ(candles[0], 1);
    EXPECT_EQ(candles[1], 1);

    // Close
    candles = get_candles_with_source(mock_candles, CandleSource::Close);
    ASSERT_EQ(candles.size(), 2);
    EXPECT_EQ(candles[0], 2);
    EXPECT_EQ(candles[1], 2);

    // Volume
    candles = get_candles_with_source(mock_candles, CandleSource::Volume);
    ASSERT_EQ(candles.size(), 2);
    EXPECT_EQ(candles[0], 100);
    EXPECT_EQ(candles[1], 110);

    // HL2
    candles = get_candles_with_source(mock_candles, CandleSource::HL2);
    ASSERT_EQ(candles.size(), 2);
    EXPECT_EQ(candles[0], 2);
    EXPECT_EQ(candles[1], 2);

    // HLC3
    candles = get_candles_with_source(mock_candles, CandleSource::HLC3);
    ASSERT_EQ(candles.size(), 2);
    EXPECT_EQ(candles[0], 2);
    EXPECT_EQ(candles[1], 2);

    // OHLC4
    candles = get_candles_with_source(mock_candles, CandleSource::OHLC4);
    ASSERT_EQ(candles.size(), 2);
    EXPECT_EQ(candles[0], 2);
    EXPECT_EQ(candles[1], 2);

    // HLCC4
    candles = get_candles_with_source(mock_candles, CandleSource::HLCC4);
    ASSERT_EQ(candles.size(), 2);
    EXPECT_EQ(candles[0], 2);
    EXPECT_EQ(candles[1], 2);
}
