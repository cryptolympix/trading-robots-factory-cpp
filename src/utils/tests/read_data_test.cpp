#include <gtest/gtest.h>
#include "../read_data.hpp"

// Fixture for ReadData tests
class ReadDataTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Set up any necessary resources before each test
    }

    void TearDown() override
    {
        // Tear down any allocated resources after each test
    }
};

// Test case for reading data with valid inputs
TEST_F(ReadDataTest, TestReadValidData)
{
    // Test reading data for a valid symbol and time frame
    std::vector<Candle> candles = read_data("EURUSD", TimeFrame::H1);

    // Assert that the vector is not empty
    ASSERT_FALSE(candles.empty());
}

// Test case for reading data with invalid symbol
TEST_F(ReadDataTest, TestReadInvalidSymbol)
{
    // Test reading data for an invalid symbol
    ASSERT_THROW(read_data("INVALID_SYMBOL", TimeFrame::H1), std::runtime_error);
}

// Test case for reading data with valid time frame and date range
TEST_F(ReadDataTest, TestReadDataWithDateRange)
{
    std::tm startDate = {};
    startDate.tm_year = 2023 - 1900; // Years since 1900
    startDate.tm_mon = 1 - 1;        // Months since January (0-based index)
    startDate.tm_mday = 3;           // Day of the month
    startDate.tm_hour = 0;           // Hour (24-hour clock)
    startDate.tm_min = 0;            // Minute
    startDate.tm_sec = 0;            // Second

    std::tm endDate = {};
    endDate.tm_year = 2023 - 1900; // Years since 1900
    endDate.tm_mon = 1 - 1;        // Months since January (0-based index)
    endDate.tm_mday = 20;          // Day of the month
    endDate.tm_hour = 0;           // Hour (24-hour clock)
    endDate.tm_min = 0;            // Minute
    endDate.tm_sec = 0;            // Second

    // Test reading data for a valid symbol, time frame, and date range
    std::vector<Candle> candles = read_data("EURUSD", TimeFrame::H1, &startDate, &endDate);

    // Assert that the vector is not empty
    ASSERT_FALSE(candles.empty());

    // Assert that the candles start from the specified start date
    ASSERT_GE(candles[0].date, std::mktime(&startDate));

    // Assert that the candles end at the specified end date
    ASSERT_LE(candles[candles.size() - 1].date, std::mktime(&endDate));
}
