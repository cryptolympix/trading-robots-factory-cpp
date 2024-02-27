#include <gtest/gtest.h>
#include <vector>
#include <cstdlib> // For rand() function
#include "../../utils/math.hpp"
#include "../momentum.hpp"

class TestMomentumIndicators : public ::testing::Test
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

TEST_F(TestMomentumIndicators, AwesomeOscillatorTest)
{
    // Mock data
    std::vector<Candle> mock_candles = {};
    for (size_t i = 0; i < 50; ++i)
    {
        double random_close = static_cast<double>(rand()) / RAND_MAX * (10);
        mock_candles.push_back(Candle{.close = random_close});
    }

    AwesomeOscillator awesome_oscillator;
    std::vector<double> result_awesome_oscillator = awesome_oscillator.calculate(mock_candles);

    ASSERT_EQ(result_awesome_oscillator.size(), mock_candles.size());
}

TEST_F(TestMomentumIndicators, KAMATest)
{
    // Mock data
    std::vector<Candle> mock_candles = {};
    for (size_t i = 0; i < 50; ++i)
    {
        double random_close = static_cast<double>(rand()) / RAND_MAX * (10);
        mock_candles.push_back(Candle{.close = random_close});
    }

    KAMA kama(10, 2, 30);
    std::vector<double> result_kama = kama.calculate(mock_candles);

    ASSERT_EQ(result_kama.size(), mock_candles.size());
}

TEST_F(TestMomentumIndicators, PPOTest)
{
    // Mock data
    std::vector<Candle> mock_candles = {};
    for (size_t i = 0; i < 50; ++i)
    {
        double random_close = static_cast<double>(rand()) / RAND_MAX * (10);
        mock_candles.push_back(Candle{.close = random_close});
    }

    PPO ppo(10, 20);
    std::vector<double> result_ppo = ppo.calculate(mock_candles);

    ASSERT_EQ(result_ppo.size(), mock_candles.size());
}

TEST_F(TestMomentumIndicators, PVOTest)
{
    // Mock data
    std::vector<Candle> mock_candles = {};
    for (size_t i = 0; i < 50; ++i)
    {
        double random_close = static_cast<double>(rand()) / RAND_MAX * (10);
        mock_candles.push_back(Candle{.close = random_close});
    }

    PVO pov(10, 20, 5);
    std::vector<double> result_pov = pov.calculate(mock_candles);

    ASSERT_EQ(result_pov.size(), mock_candles.size());
}

TEST_F(TestMomentumIndicators, ROCTest)
{
    // Mock data
    std::vector<Candle> mock_candles = {};
    for (size_t i = 0; i < 50; ++i)
    {
        double random_close = static_cast<double>(rand()) / RAND_MAX * (10);
        mock_candles.push_back(Candle{.close = random_close});
    }

    ROC roc(10);
    std::vector<double> result_roc = roc.calculate(mock_candles);

    ASSERT_EQ(result_roc.size(), mock_candles.size());
}

TEST_F(TestMomentumIndicators, RSITest)
{
    // Mock data
    std::vector<Candle> mock_candles = {};
    for (size_t i = 0; i < 50; ++i)
    {
        double random_close = static_cast<double>(rand()) / RAND_MAX * (10);
        mock_candles.push_back(Candle{.close = random_close});
    }

    RSI rsi(14);
    std::vector<double> result_rsi = rsi.calculate(mock_candles);

    ASSERT_EQ(result_rsi.size(), mock_candles.size());
}

TEST_F(TestMomentumIndicators, StochasticRSITest)
{
    // Mock data
    std::vector<Candle> mock_candles = {};
    for (size_t i = 0; i < 50; ++i)
    {
        double random_close = static_cast<double>(rand()) / RAND_MAX * (10);
        mock_candles.push_back(Candle{.close = random_close});
    }

    StochasticRSI stoch(14, 3);
    std::vector<double> result_stoch = stoch.calculate(mock_candles);

    ASSERT_EQ(result_stoch.size(), mock_candles.size());
}

TEST_F(TestMomentumIndicators, StochasticOscillatorTest)
{
    // Mock data
    std::vector<Candle> mock_candles = {};
    for (size_t i = 0; i < 50; ++i)
    {
        double random_high = static_cast<double>(rand()) / RAND_MAX * (10);
        double random_low = static_cast<double>(rand()) / RAND_MAX * (10);
        double random_close = static_cast<double>(rand()) / RAND_MAX * (10);
        mock_candles.push_back(Candle{.high = random_high, .low = random_low, .close = random_close});
    }

    StochasticOscillator stoch(14, 3);
    std::vector<double> result_stoch = stoch.calculate(mock_candles);

    ASSERT_EQ(result_stoch.size(), mock_candles.size());
}

TEST_F(TestMomentumIndicators, TSITest)
{
    // Mock data
    std::vector<Candle> mock_candles = {};
    for (size_t i = 0; i < 50; ++i)
    {
        double random_close = static_cast<double>(rand()) / RAND_MAX * (10);
        mock_candles.push_back(Candle{.close = random_close});
    }

    TSI tsi(25, 13);
    std::vector<double> result_tsi = tsi.calculate(mock_candles);

    ASSERT_EQ(result_tsi.size(), mock_candles.size());
}

TEST_F(TestMomentumIndicators, UOTest)
{
    // Mock data
    std::vector<Candle> mock_candles = {};
    for (size_t i = 0; i < 50; ++i)
    {
        double random_close = static_cast<double>(rand()) / RAND_MAX * (10);
        mock_candles.push_back(Candle{.close = random_close});
    }

    UO uo;
    std::vector<double> result_uo = uo.calculate(mock_candles);

    ASSERT_EQ(result_uo.size(), mock_candles.size());
}

TEST_F(TestMomentumIndicators, WilliamsPercentRTest)
{
    // Mock data
    std::vector<Candle> mock_candles = {};
    for (size_t i = 0; i < 50; ++i)
    {
        double random_high = static_cast<double>(rand()) / RAND_MAX * (10);
        double random_low = static_cast<double>(rand()) / RAND_MAX * (10);
        double random_close = static_cast<double>(rand()) / RAND_MAX * (10);
        mock_candles.push_back(Candle{.high = random_high, .low = random_low, .close = random_close});
    }

    WilliamsPercentR williams_r(14);
    std::vector<double> result_williams_r = williams_r.calculate(mock_candles);

    ASSERT_EQ(result_williams_r.size(), mock_candles.size());
}
