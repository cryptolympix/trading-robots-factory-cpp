#include <gtest/gtest.h>
#include <chrono>
#include <ctime> // Include for std::mktime
#include "../../types.hpp"
#include "../read_data.hpp"

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
    ASSERT_FALSE(candles.empty());

    // Assert that the candles start from the specified start date and ends before the specified end date
    ASSERT_GE(candles[0].date, std::mktime(&startDate));
    ASSERT_LE(candles[candles.size() - 1].date, std::mktime(&endDate));
}
