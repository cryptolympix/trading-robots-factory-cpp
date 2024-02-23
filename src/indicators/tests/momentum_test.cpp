#include <gtest/gtest.h>
#include <vector>
#include "../../utils/math.hpp"
#include "../momentum.hpp"

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
        mock_date = std::mktime(&timeInfo);
    };

    void TearDown() override
    {
    }

    // Variables
    time_t mock_date;
};

TEST_F(TestCandleIndicators, AwesomeOscillatorTest)
{
    // Mock data
    std::vector<Candle> mock_candles = {};

    AwesomeOscillator awesome_oscillator;
    std::vector<double> result_awesome_oscillator = awesome_oscillator.calculate(mock_candles);

    // Round to 2 decimal places
    for (size_t i = 0; i < result_awesome_oscillator.size(); ++i)
    {
        result_awesome_oscillator[i] = decimal_round(result_awesome_oscillator[i], 2);
    }

    std::vector<double> expected_awesome_oscillator = {};

    ASSERT_EQ(result_awesome_oscillator.size(), mock_candles.size());
    ASSERT_EQ(result_awesome_oscillator, expected_awesome_oscillator);
}

TEST_F(TestCandleIndicators, KAMATest)
{
    // Mock data
    std::vector<Candle> mock_candles = {};

    KAMA kama(10, 2, 30);
    std::vector<double> result_kama = kama.calculate(mock_candles);

    // Round to 2 decimal places
    for (size_t i = 0; i < result_kama.size(); ++i)
    {
        result_kama[i] = decimal_round(result_kama[i], 2);
    }

    std::vector<double> expected_kama = {};

    ASSERT_EQ(result_kama.size(), mock_candles.size());
    ASSERT_EQ(result_kama, expected_kama);
}
