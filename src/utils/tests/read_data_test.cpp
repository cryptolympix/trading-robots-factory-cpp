#include <gtest/gtest.h>
#include <chrono>
#include <ctime> // Include for std::mktime
#include "../../types.hpp"
#include "../read_data.hpp"
#include "../time_frame.hpp"

// Test case for reading data with valid inputs
TEST(ReadDataTest, TestReadValidData)
{
    // Test reading data for a valid symbol and time frame
    std::vector<Candle> candles = read_data("EURUSD", TimeFrame::H1);

    // Assert that the vector is not empty
    ASSERT_FALSE(candles.empty());
}

// Test case for reading data with invalid symbol
TEST(ReadDataTest, TestReadInvalidSymbol)
{
    // Test reading data for an invalid symbol
    ASSERT_THROW(read_data("INVALID_SYMBOL", TimeFrame::H1), std::runtime_error);
}

// Test case for reading data with valid time frame and date range
TEST(ReadDataTest, TestReadDataWithDateRange)
{
    std::tm start_date = {
        .tm_sec = 0,
        .tm_min = 0,
        .tm_hour = 0,
        .tm_mday = 3,
        .tm_mon = 0,
        .tm_year = 2023 - 1900,
    };

    std::tm end_date = {
        .tm_sec = 0,
        .tm_min = 0,
        .tm_hour = 0,
        .tm_mday = 20,
        .tm_mon = 0,
        .tm_year = 2023 - 1900,
    };

    // Convert start and end date to time_t
    time_t start_time = std::mktime(&start_date);
    time_t end_time = std::mktime(&end_date);

    // Test reading data for a valid symbol, time frame, and date range
    std::vector<Candle> candles = read_data("EURUSD", TimeFrame::H1, start_time, end_time);

    // Assert that the vector is not empty
    ASSERT_TRUE(candles.size() > 0);

    int loop_timeframe_minutes = get_time_frame_in_minutes(TimeFrame::H1);
    int candles_count = 0;

    // Loop through the candles and count the number of candles within the date range
    for (time_t current_date = start_time; current_date <= end_time; current_date += loop_timeframe_minutes * 60)
    {
        if (candles[candles_count].date == current_date)
        {
            candles_count++;
        }
    }

    // Assert that the number of candles is correct
    ASSERT_EQ(candles_count, candles.size());

    // Check the candles are sorted by their dates
    for (int i = 1; i < candles.size(); i++)
    {
        ASSERT_TRUE(candles[i - 1].date < candles[i].date);
    }
}
