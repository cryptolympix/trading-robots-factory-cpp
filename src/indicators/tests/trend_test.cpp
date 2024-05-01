#include <gtest/gtest.h>
#include <vector>
#include <cstdlib> // For rand() function
#include <ctime>
#include "../../utils/read_data.hpp"
#include "../../utils/math.hpp"
#include "../trend.hpp"

class TrendIndicatorsTest : public ::testing::Test
{
protected:
    std::vector<CandleStick> mock_candles;

    void SetUp() override
    {
        mock_candles = read_data("EURUSD", TimeFrame::H1);
    }
};

TEST_F(TrendIndicatorsTest, ADX)
{
    ADX adx(14);
    std::vector<double> result_adx = adx.calculate(mock_candles);

    ASSERT_EQ(result_adx.size(), mock_candles.size());
    for (int i = 0; i < result_adx.size(); i++)
    {
        ASSERT_TRUE(result_adx[i] >= 0 && result_adx[i] <= 100);
    }
}

TEST_F(TrendIndicatorsTest, AroonUp)
{
    AroonUp aroon_up(14);
    std::vector<double> result_aroon_up = aroon_up.calculate(mock_candles);

    ASSERT_EQ(result_aroon_up.size(), mock_candles.size());
    for (int i = 0; i < result_aroon_up.size(); i++)
    {
        ASSERT_TRUE(result_aroon_up[i] >= 0 && result_aroon_up[i] <= 100);
    }
}

TEST_F(TrendIndicatorsTest, AroonDown)
{
    AroonDown aroon_down(14);
    std::vector<double> result_aroon_down = aroon_down.calculate(mock_candles);

    ASSERT_EQ(result_aroon_down.size(), mock_candles.size());
    for (int i = 0; i < result_aroon_down.size(); i++)
    {
        ASSERT_TRUE(result_aroon_down[i] >= 0 && result_aroon_down[i] <= 100);
    }
}

TEST_F(TrendIndicatorsTest, CCI)
{
    CCI cci(20);
    std::vector<double> result_cci = cci.calculate(mock_candles);

    ASSERT_EQ(result_cci.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, DPO)
{
    DPO dpo(20);
    std::vector<double> result_dpo = dpo.calculate(mock_candles);

    ASSERT_EQ(result_dpo.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, EMA)
{
    EMA ema("close", 14);
    std::vector<double> result_ema = ema.calculate(mock_candles);

    ASSERT_EQ(result_ema.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, KST)
{
    KST kst(10, 15, 20, 30, 10, 10, 10, 15, 9);
    std::vector<double> result_kst = kst.calculate(mock_candles);

    ASSERT_EQ(result_kst.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, MACD)
{
    MACD macd(12, 26, 9);
    std::vector<double> result_macd = macd.calculate(mock_candles);

    ASSERT_EQ(result_macd.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, MI)
{
    MI mi;
    std::vector<double> result_mi = mi.calculate(mock_candles);

    ASSERT_EQ(result_mi.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, ParabolicSAR)
{
    ParabolicSAR psar(0.02, 0.2);
    std::vector<double> result_psar = psar.calculate(mock_candles);

    ASSERT_EQ(result_psar.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, SMA)
{
    SMA sma("close", 21);
    std::vector<double> result_sma = sma.calculate(mock_candles);

    ASSERT_EQ(result_sma.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, STC)
{
    STC stc(23, 50, 10);
    std::vector<double> result_stc = stc.calculate(mock_candles);

    ASSERT_EQ(result_stc.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, TRIX)
{
    TRIX trix(15);
    std::vector<double> result_trix = trix.calculate(mock_candles);

    ASSERT_EQ(result_trix.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, Vortex)
{
    Vortex vortex(14);
    std::vector<double> result_vortex = vortex.calculate(mock_candles);

    ASSERT_EQ(result_vortex.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, InstitutionalBias)
{
    InstitutionalBias ib(9, 18);
    std::vector<double> result_ib = ib.calculate(mock_candles);

    ASSERT_EQ(result_ib.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, EMADifference)
{
    EMADifference ema_diff(9, 18);
    std::vector<double> result_ema_diff = ema_diff.calculate(mock_candles);

    ASSERT_EQ(result_ema_diff.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, AroonTrend)
{
    AroonTrend aroon(14);
    std::vector<double> result_aroon_trend = aroon.calculate(mock_candles, false);

    ASSERT_EQ(result_aroon_trend.size(), mock_candles.size());
    for (int i = 0; i < result_aroon_trend.size(); i++)
    {
        ASSERT_TRUE(result_aroon_trend[i] == -1 || result_aroon_trend[i] == 0 || result_aroon_trend[i] == 1);
    }
}

TEST_F(TrendIndicatorsTest, IchimokuCloudTrend)
{
    IchimokuCloudTrend ichimoku(9, 26, 52, 26);
    std::vector<double> result_ichimoku = ichimoku.calculate(mock_candles);

    ASSERT_EQ(result_ichimoku.size(), mock_candles.size());
    for (int i = 0; i < result_ichimoku.size(); i++)
    {
        ASSERT_TRUE(result_ichimoku[i] == -1 || result_ichimoku[i] == 0 || result_ichimoku[i] == 1);
    }
}

TEST_F(TrendIndicatorsTest, IchimokuKijunTenkanTrend)
{
    IchimokuKijunTenkanTrend ichimoku(9, 26, 52, 26);
    std::vector<double> result_ichimoku = ichimoku.calculate(mock_candles);

    ASSERT_EQ(result_ichimoku.size(), mock_candles.size());
    for (int i = 0; i < result_ichimoku.size(); i++)
    {
        ASSERT_TRUE(result_ichimoku[i] == -1 || result_ichimoku[i] == 0 || result_ichimoku[i] == 1);
    }
}

TEST_F(TrendIndicatorsTest, SMASlope)
{
    SMASlope sma_slope(21, "close");
    std::vector<double> result_sma_slope = sma_slope.calculate(mock_candles);

    ASSERT_EQ(result_sma_slope.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, EMASlope)
{
    EMASlope ema_slope(21, "close");
    std::vector<double> result_ema_slope = ema_slope.calculate(mock_candles);

    ASSERT_EQ(result_ema_slope.size(), mock_candles.size());
}

TEST_F(TrendIndicatorsTest, Zigzag)
{
    Zigzag zigzag(0.1);
    std::vector<double> result_zigzag = zigzag.calculate(mock_candles);

    ASSERT_EQ(result_zigzag.size(), mock_candles.size());
}
