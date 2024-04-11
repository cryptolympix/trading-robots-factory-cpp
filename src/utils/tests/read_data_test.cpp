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
    std::tm startDate = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 3,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0};

    std::tm endDate = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 20,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0};

    // Convert start and end date to std::chrono::system_clock::time_point
    std::chrono::system_clock::time_point startTime = std::chrono::system_clock::from_time_t(std::mktime(&startDate));
    std::chrono::system_clock::time_point endTime = std::chrono::system_clock::from_time_t(std::mktime(&endDate));

    // Test reading data for a valid symbol, time frame, and date range
    std::vector<Candle> candles = read_data("EURUSD", TimeFrame::H1, startTime, endTime);

    // Assert that the vector is not empty
    ASSERT_TRUE(candles.size() > 0);

    int loop_timeframe_minutes = get_time_frame_value(TimeFrame::H1);
    int candles_count = 0;

    // Loop through the candles and count the number of candles within the date range
    for (std::chrono::system_clock::time_point current_date = startTime; current_date <= endTime; current_date += std::chrono::minutes(loop_timeframe_minutes))
    {
        if (candles[candles_count].date == std::chrono::system_clock::to_time_t(current_date))
        {
            candles_count++;
        }
    }

    // Assert that the number of candles is correct
    ASSERT_EQ(candles_count, candles.size());
}
