#include <gtest/gtest.h>
#include <vector>
#include <cstdlib>
#include "../../utils/read_data.hpp"
#include "../../utils/math.hpp"
#include "../momentum.hpp"

class MomentumIndicatorsTest : public ::testing::Test
{
protected:
    std::vector<Candle> mock_candles;

    void SetUp() override
    {
        mock_candles = read_data("EURUSD", TimeFrame::H1);
    }
};

TEST_F(MomentumIndicatorsTest, AwesomeOscillator)
{
    AwesomeOscillator awesome_oscillator;
    std::vector<double> result_awesome_oscillator = awesome_oscillator.calculate(mock_candles);

    ASSERT_EQ(result_awesome_oscillator.size(), mock_candles.size());
}

TEST_F(MomentumIndicatorsTest, KAMA)
{
    KAMA kama(10, 2, 30);
    std::vector<double> result_kama = kama.calculate(mock_candles);

    ASSERT_EQ(result_kama.size(), mock_candles.size());
}

TEST_F(MomentumIndicatorsTest, MFI)
{
    MFI mfi(14);
    std::vector<double> result_mfi = mfi.calculate(mock_candles);

    ASSERT_EQ(result_mfi.size(), result_mfi.size());
    for (size_t i = 0; i < result_mfi.size(); ++i)
    {
        ASSERT_TRUE(result_mfi[i] >= 0 && result_mfi[i] <= 100);
    }
}

TEST_F(MomentumIndicatorsTest, PPO)
{
    PPO ppo(10, 20);
    std::vector<double> result_ppo = ppo.calculate(mock_candles);

    ASSERT_EQ(result_ppo.size(), mock_candles.size());
}

TEST_F(MomentumIndicatorsTest, PVO)
{
    PVO pov(10, 20, 5);
    std::vector<double> result_pov = pov.calculate(mock_candles);

    ASSERT_EQ(result_pov.size(), mock_candles.size());
}

TEST_F(MomentumIndicatorsTest, ROC)
{
    ROC roc(10);
    std::vector<double> result_roc = roc.calculate(mock_candles);

    ASSERT_EQ(result_roc.size(), mock_candles.size());
}

TEST_F(MomentumIndicatorsTest, RSI)
{
    RSI rsi(14);
    std::vector<double> result_rsi = rsi.calculate(mock_candles);

    ASSERT_EQ(result_rsi.size(), mock_candles.size());
    for (size_t i = 0; i < result_rsi.size(); ++i)
    {
        ASSERT_TRUE(result_rsi[i] >= 0 && result_rsi[i] <= 100);
    }
}

TEST_F(MomentumIndicatorsTest, StochasticRSI)
{
    StochasticRSI stoch(14, 3);
    std::vector<double> result_stoch = stoch.calculate(mock_candles);

    ASSERT_EQ(result_stoch.size(), mock_candles.size());
    for (size_t i = 0; i < result_stoch.size(); ++i)
    {
        ASSERT_TRUE(result_stoch[i] >= 0 && result_stoch[i] <= 100);
    }
}

TEST_F(MomentumIndicatorsTest, StochasticOscillator)
{
    StochasticOscillator stoch(14, 3);
    std::vector<double> result_stoch = stoch.calculate(mock_candles);

    ASSERT_EQ(result_stoch.size(), mock_candles.size());
    for (size_t i = 0; i < result_stoch.size(); ++i)
    {
        ASSERT_TRUE(result_stoch[i] >= 0 && result_stoch[i] <= 100);
    }
}

TEST_F(MomentumIndicatorsTest, TSI)
{
    TSI tsi(25, 13);
    std::vector<double> result_tsi = tsi.calculate(mock_candles);

    ASSERT_EQ(result_tsi.size(), mock_candles.size());
    for (size_t i = 0; i < result_tsi.size(); ++i)
    {
        ASSERT_TRUE(result_tsi[i] >= -100 && result_tsi[i] <= 100);
    }
}

TEST_F(MomentumIndicatorsTest, UO)
{
    UO uo;
    std::vector<double> result_uo = uo.calculate(mock_candles);

    ASSERT_EQ(result_uo.size(), mock_candles.size());
    for (size_t i = 0; i < result_uo.size(); ++i)
    {
        ASSERT_TRUE(result_uo[i] >= 0 && result_uo[i] <= 100);
    }
}

TEST_F(MomentumIndicatorsTest, WPR)
{
    WPR williams_r(14);
    std::vector<double> result_williams_r = williams_r.calculate(mock_candles);

    ASSERT_EQ(result_williams_r.size(), mock_candles.size());
    for (size_t i = 0; i < result_williams_r.size(); ++i)
    {
        ASSERT_TRUE(result_williams_r[i] >= -100 && result_williams_r[i] <= 0);
    }
}
