#include <gtest/gtest.h>
#include <vector>
#include <cstdlib>
#include "../../utils/read_data.hpp"
#include "../../utils/math.hpp"
#include "../momentum.hpp"
#include "../builder.hpp"

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

    AwesomeOscillator *created_open_indicator = static_cast<AwesomeOscillator *>(create_indicator_from_id(awesome_oscillator.id, awesome_oscillator.params));
    std::vector<double> result_created_open_indicator = created_open_indicator->calculate(mock_candles);

    ASSERT_NE(created_open_indicator, nullptr);
    ASSERT_EQ(result_awesome_oscillator, result_created_open_indicator);
}

TEST_F(MomentumIndicatorsTest, KAMA)
{
    KAMA kama(10, 2, 30);
    std::vector<double> result_kama = kama.calculate(mock_candles);

    ASSERT_EQ(result_kama.size(), mock_candles.size());

    KAMA *created_kama = static_cast<KAMA *>(create_indicator_from_id(kama.id, kama.params));
    std::vector<double> result_created_kama = created_kama->calculate(mock_candles);

    ASSERT_NE(created_kama, nullptr);
    ASSERT_EQ(result_kama, result_created_kama);
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

    MFI *created_mfi = static_cast<MFI *>(create_indicator_from_id(mfi.id, mfi.params));
    std::vector<double> result_created_mfi = created_mfi->calculate(mock_candles);

    ASSERT_NE(created_mfi, nullptr);
    ASSERT_EQ(result_mfi, result_created_mfi);
}

TEST_F(MomentumIndicatorsTest, PPO)
{
    PPO ppo(10, 20);
    std::vector<double> result_ppo = ppo.calculate(mock_candles);

    ASSERT_EQ(result_ppo.size(), mock_candles.size());

    PPO *created_ppo = static_cast<PPO *>(create_indicator_from_id(ppo.id, ppo.params));
    std::vector<double> result_created_ppo = created_ppo->calculate(mock_candles);

    ASSERT_NE(created_ppo, nullptr);
    ASSERT_EQ(result_ppo, result_created_ppo);
}

TEST_F(MomentumIndicatorsTest, PVO)
{
    PVO pvo(10, 20, 5);
    std::vector<double> result_pvo = pvo.calculate(mock_candles);

    ASSERT_EQ(result_pvo.size(), mock_candles.size());

    PVO *created_pvo = static_cast<PVO *>(create_indicator_from_id(pvo.id, pvo.params));
    std::vector<double> result_created_pvo = created_pvo->calculate(mock_candles);

    ASSERT_NE(created_pvo, nullptr);
    for (size_t i = 0; i < result_pvo.size(); ++i)
    {
        ASSERT_DOUBLE_EQ(result_pvo[i], result_created_pvo[i]);
    }
}

TEST_F(MomentumIndicatorsTest, ROC)
{
    ROC roc(10);
    std::vector<double> result_roc = roc.calculate(mock_candles);

    ASSERT_EQ(result_roc.size(), mock_candles.size());

    ROC *created_roc = static_cast<ROC *>(create_indicator_from_id(roc.id, roc.params));
    std::vector<double> result_created_roc = created_roc->calculate(mock_candles);

    ASSERT_NE(created_roc, nullptr);
    ASSERT_EQ(result_roc, result_created_roc);
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

    RSI *created_rsi = static_cast<RSI *>(create_indicator_from_id(rsi.id, rsi.params));
    std::vector<double> result_created_rsi = created_rsi->calculate(mock_candles);

    ASSERT_NE(created_rsi, nullptr);
    ASSERT_EQ(result_rsi, result_created_rsi);
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

    StochasticRSI *created_stoch = static_cast<StochasticRSI *>(create_indicator_from_id(stoch.id, stoch.params));
    std::vector<double> result_created_stoch = created_stoch->calculate(mock_candles);

    ASSERT_NE(created_stoch, nullptr);
    ASSERT_EQ(result_stoch, result_created_stoch);
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

    StochasticOscillator *created_stoch = static_cast<StochasticOscillator *>(create_indicator_from_id(stoch.id, stoch.params));
    std::vector<double> result_created_stoch = created_stoch->calculate(mock_candles);

    ASSERT_NE(created_stoch, nullptr);
    ASSERT_EQ(result_stoch, result_created_stoch);
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

    TSI *created_tsi = static_cast<TSI *>(create_indicator_from_id(tsi.id, tsi.params));
    std::vector<double> result_created_tsi = created_tsi->calculate(mock_candles);

    ASSERT_NE(created_tsi, nullptr);
    ASSERT_EQ(result_tsi, result_created_tsi);
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

    UO *created_uo = static_cast<UO *>(create_indicator_from_id(uo.id, uo.params));
    std::vector<double> result_created_uo = created_uo->calculate(mock_candles);

    ASSERT_NE(created_uo, nullptr);
    ASSERT_EQ(result_uo, result_created_uo);
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

    WPR *created_williams_r = static_cast<WPR *>(create_indicator_from_id(williams_r.id, williams_r.params));
    std::vector<double> result_created_williams_r = created_williams_r->calculate(mock_candles);

    ASSERT_NE(created_williams_r, nullptr);
    ASSERT_EQ(result_williams_r, result_created_williams_r);
}
