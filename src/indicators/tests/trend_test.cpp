#include <gtest/gtest.h>
#include <vector>
#include <cstdlib> // For rand() function
#include <ctime>
#include <chrono>
#include "../builder.hpp"
#include "../../utils/read_data.hpp"
#include "../../utils/math.hpp"
#include "../trend.hpp"

class TrendIndicatorsTest : public ::testing::Test
{
protected:
    std::vector<Candle> mock_candles;

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
        ASSERT_GE(result_adx[i], 0);
        ASSERT_LE(result_adx[i], 100);
    }

    ADX *created_adx_indicator = static_cast<ADX *>(create_indicator_from_id(adx.id, adx.params));
    std::vector<double> result_created_adx_indicator = created_adx_indicator->calculate(mock_candles);

    ASSERT_NE(created_adx_indicator, nullptr);
    for (int i = 0; i < result_created_adx_indicator.size(); i++)
    {
        ASSERT_DOUBLE_EQ(result_created_adx_indicator[i], result_adx[i]);
    }
}

TEST_F(TrendIndicatorsTest, AroonUp)
{
    AroonUp aroon_up(14);
    std::vector<double> result_aroon_up = aroon_up.calculate(mock_candles);

    ASSERT_EQ(result_aroon_up.size(), mock_candles.size());
    for (int i = 0; i < result_aroon_up.size(); i++)
    {
        ASSERT_GE(result_aroon_up[i], 0);
        ASSERT_LE(result_aroon_up[i], 100);
    }

    AroonUp *created_aroon_up_indicator = static_cast<AroonUp *>(create_indicator_from_id(aroon_up.id, aroon_up.params));
    std::vector<double> result_created_aroon_up_indicator = created_aroon_up_indicator->calculate(mock_candles);

    ASSERT_NE(created_aroon_up_indicator, nullptr);
    ASSERT_EQ(result_created_aroon_up_indicator, result_aroon_up);
}

TEST_F(TrendIndicatorsTest, AroonDown)
{
    AroonDown aroon_down(14);
    std::vector<double> result_aroon_down = aroon_down.calculate(mock_candles);

    ASSERT_EQ(result_aroon_down.size(), mock_candles.size());
    for (int i = 0; i < result_aroon_down.size(); i++)
    {
        ASSERT_GE(result_aroon_down[i], 0);
        ASSERT_LE(result_aroon_down[i], 100);
    }

    AroonDown *created_aroon_down_indicator = static_cast<AroonDown *>(create_indicator_from_id(aroon_down.id, aroon_down.params));
    std::vector<double> result_created_aroon_down_indicator = created_aroon_down_indicator->calculate(mock_candles);

    ASSERT_NE(created_aroon_down_indicator, nullptr);
    ASSERT_EQ(result_created_aroon_down_indicator, result_aroon_down);
}

TEST_F(TrendIndicatorsTest, CCI)
{
    CCI cci(20);
    std::vector<double> result_cci = cci.calculate(mock_candles);

    ASSERT_EQ(result_cci.size(), mock_candles.size());

    CCI *created_cci_indicator = static_cast<CCI *>(create_indicator_from_id(cci.id, cci.params));
    std::vector<double> result_created_cci_indicator = created_cci_indicator->calculate(mock_candles);

    ASSERT_NE(created_cci_indicator, nullptr);
    ASSERT_EQ(result_created_cci_indicator, result_cci);
}

TEST_F(TrendIndicatorsTest, DPO)
{
    DPO dpo(20);
    std::vector<double> result_dpo = dpo.calculate(mock_candles);

    ASSERT_EQ(result_dpo.size(), mock_candles.size());

    DPO *created_dpo_indicator = static_cast<DPO *>(create_indicator_from_id(dpo.id, dpo.params));
    std::vector<double> result_created_dpo_indicator = created_dpo_indicator->calculate(mock_candles);

    ASSERT_NE(created_dpo_indicator, nullptr);
    ASSERT_EQ(result_created_dpo_indicator, result_dpo);
}

TEST_F(TrendIndicatorsTest, EMA)
{
    EMA ema("close", 14);
    std::vector<double> result_ema = ema.calculate(mock_candles);

    ASSERT_EQ(result_ema.size(), mock_candles.size());

    EMA *created_ema_indicator = static_cast<EMA *>(create_indicator_from_id(ema.id, ema.params));
    std::vector<double> result_created_ema_indicator = created_ema_indicator->calculate(mock_candles);

    ASSERT_NE(created_ema_indicator, nullptr);
    ASSERT_EQ(result_created_ema_indicator, result_ema);
}

TEST_F(TrendIndicatorsTest, KST)
{
    KST kst(10, 15, 20, 30, 10, 10, 10, 15, 9);
    std::vector<double> result_kst = kst.calculate(mock_candles);

    ASSERT_EQ(result_kst.size(), mock_candles.size());

    KST *created_kst_indicator = static_cast<KST *>(create_indicator_from_id(kst.id, kst.params));
    std::vector<double> result_created_kst_indicator = created_kst_indicator->calculate(mock_candles);

    ASSERT_NE(created_kst_indicator, nullptr);
    ASSERT_EQ(result_created_kst_indicator, result_kst);
}

TEST_F(TrendIndicatorsTest, MACD)
{
    MACD macd(12, 26, 9);
    std::vector<double> result_macd = macd.calculate(mock_candles);

    ASSERT_EQ(result_macd.size(), mock_candles.size());

    MACD *created_macd_indicator = static_cast<MACD *>(create_indicator_from_id(macd.id, macd.params));
    std::vector<double> result_created_macd_indicator = created_macd_indicator->calculate(mock_candles);

    ASSERT_NE(created_macd_indicator, nullptr);
    ASSERT_EQ(result_created_macd_indicator, result_macd);
}

TEST_F(TrendIndicatorsTest, MI)
{
    MI mi;
    std::vector<double> result_mi = mi.calculate(mock_candles);

    ASSERT_EQ(result_mi.size(), mock_candles.size());

    MI *created_mi_indicator = static_cast<MI *>(create_indicator_from_id(mi.id, mi.params));
    std::vector<double> result_created_mi_indicator = created_mi_indicator->calculate(mock_candles);

    ASSERT_NE(created_mi_indicator, nullptr);
    ASSERT_EQ(result_created_mi_indicator, result_mi);
}

TEST_F(TrendIndicatorsTest, ParabolicSAR)
{
    ParabolicSAR psar(0.02, 0.2);
    std::vector<double> result_psar = psar.calculate(mock_candles);

    ASSERT_EQ(result_psar.size(), mock_candles.size());

    ParabolicSAR *created_psar_indicator = static_cast<ParabolicSAR *>(create_indicator_from_id(psar.id, psar.params));
    std::vector<double> result_created_psar_indicator = created_psar_indicator->calculate(mock_candles);

    ASSERT_NE(created_psar_indicator, nullptr);
    ASSERT_EQ(result_created_psar_indicator, result_psar);
}

TEST_F(TrendIndicatorsTest, SMA)
{
    SMA sma("close", 21);
    std::vector<double> result_sma = sma.calculate(mock_candles);

    ASSERT_EQ(result_sma.size(), mock_candles.size());

    SMA *created_sma_indicator = static_cast<SMA *>(create_indicator_from_id(sma.id, sma.params));
    std::vector<double> result_created_sma_indicator = created_sma_indicator->calculate(mock_candles);

    ASSERT_NE(created_sma_indicator, nullptr);
    ASSERT_EQ(result_created_sma_indicator, result_sma);
}

TEST_F(TrendIndicatorsTest, STC)
{
    STC stc(23, 50, 10);
    std::vector<double> result_stc = stc.calculate(mock_candles);

    ASSERT_EQ(result_stc.size(), mock_candles.size());

    STC *created_stc_indicator = static_cast<STC *>(create_indicator_from_id(stc.id, stc.params));
    std::vector<double> result_created_stc_indicator = created_stc_indicator->calculate(mock_candles);

    ASSERT_NE(created_stc_indicator, nullptr);
    ASSERT_EQ(result_created_stc_indicator, result_stc);
}

TEST_F(TrendIndicatorsTest, TRIX)
{
    TRIX trix(15);
    std::vector<double> result_trix = trix.calculate(mock_candles);

    ASSERT_EQ(result_trix.size(), mock_candles.size());

    TRIX *created_trix_indicator = static_cast<TRIX *>(create_indicator_from_id(trix.id, trix.params));
    std::vector<double> result_created_trix_indicator = created_trix_indicator->calculate(mock_candles);

    ASSERT_NE(created_trix_indicator, nullptr);
    ASSERT_EQ(result_created_trix_indicator, result_trix);
}

TEST_F(TrendIndicatorsTest, Vortex)
{
    Vortex vortex(14);
    std::vector<double> result_vortex = vortex.calculate(mock_candles);

    ASSERT_EQ(result_vortex.size(), mock_candles.size());

    Vortex *created_vortex_indicator = static_cast<Vortex *>(create_indicator_from_id(vortex.id, vortex.params));
    std::vector<double> result_created_vortex_indicator = created_vortex_indicator->calculate(mock_candles);

    ASSERT_NE(created_vortex_indicator, nullptr);
    ASSERT_EQ(result_created_vortex_indicator, result_vortex);
}

TEST_F(TrendIndicatorsTest, InstitutionalBias)
{
    InstitutionalBias ib(9, 18);
    std::vector<double> result_ib = ib.calculate(mock_candles);

    ASSERT_EQ(result_ib.size(), mock_candles.size());
    for (int i = 0; i < result_ib.size(); i++)
    {
        ASSERT_TRUE(result_ib[i] == -1 || result_ib[i] == 0 || result_ib[i] == 1);
    }

    InstitutionalBias *created_ib_indicator = static_cast<InstitutionalBias *>(create_indicator_from_id(ib.id, ib.params));
    std::vector<double> result_created_ib_indicator = created_ib_indicator->calculate(mock_candles);

    ASSERT_NE(created_ib_indicator, nullptr);
    ASSERT_EQ(result_created_ib_indicator, result_ib);
}

TEST_F(TrendIndicatorsTest, EMADifference)
{
    EMADifference ema_diff(9, 18);
    std::vector<double> result_ema_diff = ema_diff.calculate(mock_candles);

    ASSERT_EQ(result_ema_diff.size(), mock_candles.size());

    EMADifference *created_ema_diff_indicator = static_cast<EMADifference *>(create_indicator_from_id(ema_diff.id, ema_diff.params));
    std::vector<double> result_created_ema_diff_indicator = created_ema_diff_indicator->calculate(mock_candles);

    ASSERT_NE(created_ema_diff_indicator, nullptr);
    ASSERT_EQ(result_created_ema_diff_indicator, result_ema_diff);
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

    AroonTrend *created_aroon_trend_indicator = static_cast<AroonTrend *>(create_indicator_from_id(aroon.id, aroon.params));
    std::vector<double> result_created_aroon_trend_indicator = created_aroon_trend_indicator->calculate(mock_candles, false);

    ASSERT_NE(created_aroon_trend_indicator, nullptr);
    ASSERT_EQ(result_created_aroon_trend_indicator, result_aroon_trend);
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

    IchimokuCloudTrend *created_ichimoku_indicator = static_cast<IchimokuCloudTrend *>(create_indicator_from_id(ichimoku.id, ichimoku.params));
    std::vector<double> result_created_ichimoku_indicator = created_ichimoku_indicator->calculate(mock_candles);

    ASSERT_NE(created_ichimoku_indicator, nullptr);
    ASSERT_EQ(result_created_ichimoku_indicator, result_ichimoku);
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

    IchimokuKijunTenkanTrend *created_ichimoku_indicator = static_cast<IchimokuKijunTenkanTrend *>(create_indicator_from_id(ichimoku.id, ichimoku.params));
    std::vector<double> result_created_ichimoku_indicator = created_ichimoku_indicator->calculate(mock_candles);

    ASSERT_NE(created_ichimoku_indicator, nullptr);
    ASSERT_EQ(result_created_ichimoku_indicator, result_ichimoku);
}

TEST_F(TrendIndicatorsTest, SMASlope)
{
    SMASlope sma_slope(21, "close");
    std::vector<double> result_sma_slope = sma_slope.calculate(mock_candles);

    ASSERT_EQ(result_sma_slope.size(), mock_candles.size());

    SMASlope *created_sma_slope_indicator = static_cast<SMASlope *>(create_indicator_from_id(sma_slope.id, sma_slope.params));
    std::vector<double> result_created_sma_slope_indicator = created_sma_slope_indicator->calculate(mock_candles);

    ASSERT_NE(created_sma_slope_indicator, nullptr);
    ASSERT_EQ(result_created_sma_slope_indicator, result_sma_slope);
}

TEST_F(TrendIndicatorsTest, EMASlope)
{
    EMASlope ema_slope(21, "close");
    std::vector<double> result_ema_slope = ema_slope.calculate(mock_candles);

    ASSERT_EQ(result_ema_slope.size(), mock_candles.size());

    EMASlope *created_ema_slope_indicator = static_cast<EMASlope *>(create_indicator_from_id(ema_slope.id, ema_slope.params));
    std::vector<double> result_created_ema_slope_indicator = created_ema_slope_indicator->calculate(mock_candles);

    ASSERT_NE(created_ema_slope_indicator, nullptr);
    ASSERT_EQ(result_created_ema_slope_indicator, result_ema_slope);
}

TEST_F(TrendIndicatorsTest, Zigzag)
{
    Zigzag zigzag(0.1);
    std::vector<double> result_zigzag = zigzag.calculate(mock_candles);

    ASSERT_EQ(result_zigzag.size(), mock_candles.size());

    Zigzag *created_zigzag_indicator = static_cast<Zigzag *>(create_indicator_from_id(zigzag.id, zigzag.params));
    std::vector<double> result_created_zigzag_indicator = created_zigzag_indicator->calculate(mock_candles);

    ASSERT_NE(created_zigzag_indicator, nullptr);
    ASSERT_EQ(result_created_zigzag_indicator, result_zigzag);
}
