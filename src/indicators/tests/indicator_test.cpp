#include <gtest/gtest.h>
#include <chrono>
#include "../../types.hpp"
#include "../indicator.hpp"

// Test fixture for Indicator class
class IndicatorTest : public ::testing::Test
{
protected:
    // Common setup code, executed before each test
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
    }

    // Common member variables
    std::vector<Candle> mock_candles;
};

// Test case for Indicator::calculate method
TEST_F(IndicatorTest, Calculate)
{
    // Create an instance of the Indicator class
    Indicator indicator("Test Indicator", "test-indicator", 0);

    // Define a calculator function (dummy function for testing)
    auto calculator = [](std::vector<Candle> candles) -> std::vector<double>
    {
        std::vector<double> values;
        for (const auto &candle : candles)
        {
            values.push_back(candle.close); // Just return close prices as indicator values for testing
        }
        return values;
    };

    // Call the calculate method
    std::vector<double> result = indicator.calculate(mock_candles, calculator, false);

    // Check if the result has the same size as the input candles
    ASSERT_EQ(result.size(), mock_candles.size());

    // Check if the values in the result are correct (compare with close prices of candles)
    for (size_t i = 0; i < mock_candles.size(); ++i)
    {
        ASSERT_EQ(result[i], mock_candles[i].close);
    }
}

// Test case for negative offset
TEST_F(IndicatorTest, CalculateWithOffset)
{
    // Create an instance of the Indicator class with offset
    Indicator indicator("Test Indicator", "test-indicator", 1);

    // Define a calculator function (dummy function for testing)
    auto calculator = [](std::vector<Candle> candles) -> std::vector<double>
    {
        std::vector<double> values;
        for (const auto &candle : candles)
        {
            values.push_back(candle.close); // Just return close prices as indicator values for testing
        }
        return values;
    };

    // Call the calculate method
    std::vector<double> result = indicator.calculate(mock_candles, calculator, false);

    // Check if the result is empty
    ASSERT_EQ(result.size(), mock_candles.size());
}

// Test case for normalize_data flag
TEST_F(IndicatorTest, NormalizeData)
{
    // Create an instance of the Indicator class
    Indicator indicator("Test Indicator", "test-indicator", 0);

    // Define a calculator function (dummy function for testing)
    auto calculator = [](std::vector<Candle> candles) -> std::vector<double>
    {
        std::vector<double> values;
        for (const auto &candle : candles)
        {
            values.push_back(candle.close); // Just return close prices as indicator values for testing
        }
        return values;
    };

    // Call the calculate method with normalize_data flag set to true
    std::vector<double> result = indicator.calculate(mock_candles, calculator, true);

    // Check if the result has the same size as the input candles
    ASSERT_EQ(result.size(), mock_candles.size());

    // Check if the values in the result are normalized (between 0 and 1)
    for (const auto &value : result)
    {
        ASSERT_GE(value, 0.0); // Check if value is greater than or equal to 0
        ASSERT_LE(value, 1.0); // Check if value is less than or equal to 1
    }
}
