#include <gtest/gtest.h>
#include <cmath>
#include <ctime>
#include <vector>
#include "../builder.hpp"
#include "../../utils/read_data.hpp"
#include "../volatility.hpp"
#include "../../types.hpp"
#include "../../utils/math.hpp"

class VolatilityIndicatorsTest : public ::testing::Test
{
protected:
    // Variables
    std::vector<Candle> mock_candles;

    void SetUp() override
    {
        mock_candles = read_data("EURUSD", TimeFrame::H1);
    }
};

TEST_F(VolatilityIndicatorsTest, ATR)
{
    ATR atr_indicator(3);
    std::vector<double> result_atr = atr_indicator.calculate(mock_candles);
    ASSERT_EQ(result_atr.size(), mock_candles.size());
    for (size_t i = 0; i < result_atr.size(); i++)
    {
        ASSERT_TRUE(result_atr[i] >= 0);
    }

    ATR *created_atr_indicator = static_cast<ATR *>(create_indicator_from_id(atr_indicator.id, atr_indicator.params));
    std::vector<double> result_created_atr_indicator = created_atr_indicator->calculate(mock_candles);

    ASSERT_NE(created_atr_indicator, nullptr);
    ASSERT_EQ(result_created_atr_indicator, result_atr);
}

TEST_F(VolatilityIndicatorsTest, BollingerChannelHighBand)
{
    BollingerChannelHighBand bollinger_channel_high_band_indicator(3, 2.0);
    std::vector<double> result_bollinger_channel_high_band = bollinger_channel_high_band_indicator.calculate(mock_candles);
    ASSERT_EQ(result_bollinger_channel_high_band.size(), mock_candles.size());
    for (size_t i = 0; i < result_bollinger_channel_high_band.size(); i++)
    {
        ASSERT_TRUE(result_bollinger_channel_high_band[i] >= 0);
    }

    BollingerChannelHighBand *created_bollinger_channel_high_band_indicator = static_cast<BollingerChannelHighBand *>(create_indicator_from_id(bollinger_channel_high_band_indicator.id, bollinger_channel_high_band_indicator.params));
    std::vector<double> result_created_bollinger_channel_high_band_indicator = created_bollinger_channel_high_band_indicator->calculate(mock_candles);

    ASSERT_NE(created_bollinger_channel_high_band_indicator, nullptr);
    ASSERT_EQ(result_created_bollinger_channel_high_band_indicator, result_bollinger_channel_high_band);
}

TEST_F(VolatilityIndicatorsTest, BollingerChannelLowBand)
{
    BollingerChannelLowBand bollinger_channel_low_band_indicator(3, 2.0);
    std::vector<double> result_bollinger_channel_low_band = bollinger_channel_low_band_indicator.calculate(mock_candles);
    ASSERT_EQ(result_bollinger_channel_low_band.size(), mock_candles.size());
    for (size_t i = 0; i < result_bollinger_channel_low_band.size(); i++)
    {
        ASSERT_TRUE(result_bollinger_channel_low_band[i] >= 0);
    }

    BollingerChannelLowBand *created_bollinger_channel_low_band_indicator = static_cast<BollingerChannelLowBand *>(create_indicator_from_id(bollinger_channel_low_band_indicator.id, bollinger_channel_low_band_indicator.params));
    std::vector<double> result_created_bollinger_channel_low_band_indicator = created_bollinger_channel_low_band_indicator->calculate(mock_candles);

    ASSERT_NE(created_bollinger_channel_low_band_indicator, nullptr);
    ASSERT_EQ(result_created_bollinger_channel_low_band_indicator, result_bollinger_channel_low_band);
}

TEST_F(VolatilityIndicatorsTest, BollingerChannelMiddleBand)
{
    BollingerChannelMiddleBand bollinger_channel_middle_band_indicator(3);
    std::vector<double> result_bollinger_channel_middle_band = bollinger_channel_middle_band_indicator.calculate(mock_candles);
    ASSERT_EQ(result_bollinger_channel_middle_band.size(), mock_candles.size());
    for (size_t i = 0; i < result_bollinger_channel_middle_band.size(); i++)
    {
        ASSERT_TRUE(result_bollinger_channel_middle_band[i] >= 0);
    }

    BollingerChannelMiddleBand *created_bollinger_channel_middle_band_indicator = static_cast<BollingerChannelMiddleBand *>(create_indicator_from_id(bollinger_channel_middle_band_indicator.id, bollinger_channel_middle_band_indicator.params));
    std::vector<double> result_created_bollinger_channel_middle_band_indicator = created_bollinger_channel_middle_band_indicator->calculate(mock_candles);

    ASSERT_NE(created_bollinger_channel_middle_band_indicator, nullptr);
    ASSERT_EQ(result_created_bollinger_channel_middle_band_indicator, result_bollinger_channel_middle_band);
}

TEST_F(VolatilityIndicatorsTest, BollingerChannelPercentageBand)
{
    BollingerChannelPercentageBand bollinger_channel_percentage_indicator(3, 2.0);
    std::vector<double> result_bolliner_channel_percentage = bollinger_channel_percentage_indicator.calculate(mock_candles);
    ASSERT_EQ(result_bolliner_channel_percentage.size(), mock_candles.size());

    BollingerChannelPercentageBand *created_bollinger_channel_percentage_indicator = static_cast<BollingerChannelPercentageBand *>(create_indicator_from_id(bollinger_channel_percentage_indicator.id, bollinger_channel_percentage_indicator.params));
    std::vector<double> result_created_bollinger_channel_percentage_indicator = created_bollinger_channel_percentage_indicator->calculate(mock_candles);

    ASSERT_NE(created_bollinger_channel_percentage_indicator, nullptr);
    ASSERT_EQ(result_created_bollinger_channel_percentage_indicator, result_bolliner_channel_percentage);
}

TEST_F(VolatilityIndicatorsTest, BollingerChannelWidthBand)
{
    BollingerChannelWidthBand bollinger_channel_width_indicator(3, 2.0);
    std::vector<double> result_bollinger_channel_width = bollinger_channel_width_indicator.calculate(mock_candles);
    ASSERT_EQ(result_bollinger_channel_width.size(), mock_candles.size());
    for (size_t i = 0; i < result_bollinger_channel_width.size(); i++)
    {
        ASSERT_TRUE(result_bollinger_channel_width[i] >= 0);
    }

    BollingerChannelWidthBand *created_bollinger_channel_width_indicator = static_cast<BollingerChannelWidthBand *>(create_indicator_from_id(bollinger_channel_width_indicator.id, bollinger_channel_width_indicator.params));
    std::vector<double> result_created_bollinger_channel_width_indicator = created_bollinger_channel_width_indicator->calculate(mock_candles);

    ASSERT_NE(created_bollinger_channel_width_indicator, nullptr);
    ASSERT_EQ(result_created_bollinger_channel_width_indicator, result_bollinger_channel_width);
}

TEST_F(VolatilityIndicatorsTest, DonchianChannelHighBand)
{
    DonchianChannelHighBand donchian_channel_high_band_indicator(3);
    std::vector<double> result_donchian_channel_high_band = donchian_channel_high_band_indicator.calculate(mock_candles);
    ASSERT_EQ(result_donchian_channel_high_band.size(), mock_candles.size());
    for (size_t i = 0; i < result_donchian_channel_high_band.size(); i++)
    {
        ASSERT_TRUE(result_donchian_channel_high_band[i] >= 0);
    }

    DonchianChannelHighBand *created_donchian_channel_high_band_indicator = static_cast<DonchianChannelHighBand *>(create_indicator_from_id(donchian_channel_high_band_indicator.id, donchian_channel_high_band_indicator.params));
    std::vector<double> result_created_donchian_channel_high_band_indicator = created_donchian_channel_high_band_indicator->calculate(mock_candles);

    ASSERT_NE(created_donchian_channel_high_band_indicator, nullptr);
    ASSERT_EQ(result_created_donchian_channel_high_band_indicator, result_donchian_channel_high_band);
}

TEST_F(VolatilityIndicatorsTest, DonchianChannelLowBand)
{
    DonchianChannelLowBand donchian_channel_low_band_indicator(3);
    std::vector<double> result_donchian_channel_low_band = donchian_channel_low_band_indicator.calculate(mock_candles);
    ASSERT_EQ(result_donchian_channel_low_band.size(), mock_candles.size());
    for (size_t i = 0; i < result_donchian_channel_low_band.size(); i++)
    {
        ASSERT_TRUE(result_donchian_channel_low_band[i] >= 0);
    }

    DonchianChannelLowBand *created_donchian_channel_low_band_indicator = static_cast<DonchianChannelLowBand *>(create_indicator_from_id(donchian_channel_low_band_indicator.id, donchian_channel_low_band_indicator.params));
    std::vector<double> result_created_donchian_channel_low_band_indicator = created_donchian_channel_low_band_indicator->calculate(mock_candles);

    ASSERT_NE(created_donchian_channel_low_band_indicator, nullptr);
    ASSERT_EQ(result_created_donchian_channel_low_band_indicator, result_donchian_channel_low_band);
}

TEST_F(VolatilityIndicatorsTest, DonchianChannelMiddleBand)
{
    DonchianChannelMiddleBand donchian_channel_middle_band_indicator(3);
    std::vector<double> result_donchian_channel_middle_band = donchian_channel_middle_band_indicator.calculate(mock_candles);
    ASSERT_EQ(result_donchian_channel_middle_band.size(), mock_candles.size());
    for (size_t i = 0; i < result_donchian_channel_middle_band.size(); i++)
    {
        ASSERT_TRUE(result_donchian_channel_middle_band[i] >= 0);
    }

    DonchianChannelMiddleBand *created_donchian_channel_middle_band_indicator = static_cast<DonchianChannelMiddleBand *>(create_indicator_from_id(donchian_channel_middle_band_indicator.id, donchian_channel_middle_band_indicator.params));
    std::vector<double> result_created_donchian_channel_middle_band_indicator = created_donchian_channel_middle_band_indicator->calculate(mock_candles);

    ASSERT_NE(created_donchian_channel_middle_band_indicator, nullptr);
    ASSERT_EQ(result_created_donchian_channel_middle_band_indicator, result_donchian_channel_middle_band);
}

TEST_F(VolatilityIndicatorsTest, KeltnerChannelHighBand)
{
    KeltnerChannelHighBand keltner_channel_high_band_indicator(3, 2.0);
    std::vector<double> result_keltner_channel_high_band = keltner_channel_high_band_indicator.calculate(mock_candles);
    ASSERT_EQ(result_keltner_channel_high_band.size(), mock_candles.size());
    for (size_t i = 0; i < result_keltner_channel_high_band.size(); i++)
    {
        ASSERT_TRUE(result_keltner_channel_high_band[i] >= 0);
    }

    KeltnerChannelHighBand *created_keltner_channel_high_band_indicator = static_cast<KeltnerChannelHighBand *>(create_indicator_from_id(keltner_channel_high_band_indicator.id, keltner_channel_high_band_indicator.params));
    std::vector<double> result_created_keltner_channel_high_band_indicator = created_keltner_channel_high_band_indicator->calculate(mock_candles);

    ASSERT_NE(created_keltner_channel_high_band_indicator, nullptr);
    ASSERT_EQ(result_created_keltner_channel_high_band_indicator, result_keltner_channel_high_band);
}

TEST_F(VolatilityIndicatorsTest, KeltnerChannelLowBand)
{
    KeltnerChannelLowBand keltner_channel_low_band_indicator(3, 2.0);
    std::vector<double> result_keltner_channel_low_band = keltner_channel_low_band_indicator.calculate(mock_candles);
    ASSERT_EQ(result_keltner_channel_low_band.size(), mock_candles.size());
    for (size_t i = 0; i < result_keltner_channel_low_band.size(); i++)
    {
        ASSERT_TRUE(result_keltner_channel_low_band[i] >= 0);
    }

    KeltnerChannelLowBand *created_keltner_channel_low_band_indicator = static_cast<KeltnerChannelLowBand *>(create_indicator_from_id(keltner_channel_low_band_indicator.id, keltner_channel_low_band_indicator.params));
    std::vector<double> result_created_keltner_channel_low_band_indicator = created_keltner_channel_low_band_indicator->calculate(mock_candles);

    ASSERT_NE(created_keltner_channel_low_band_indicator, nullptr);
    ASSERT_EQ(result_created_keltner_channel_low_band_indicator, result_keltner_channel_low_band);
}

TEST_F(VolatilityIndicatorsTest, KeltnerChannelMiddleBand)
{
    KeltnerChannelMiddleBand keltner_channel_middle_band_indicator(3);
    std::vector<double> result_keltner_channel_middle_band = keltner_channel_middle_band_indicator.calculate(mock_candles);
    ASSERT_EQ(result_keltner_channel_middle_band.size(), mock_candles.size());
    for (size_t i = 0; i < result_keltner_channel_middle_band.size(); i++)
    {
        ASSERT_TRUE(result_keltner_channel_middle_band[i] >= 0);
    }

    KeltnerChannelMiddleBand *created_keltner_channel_middle_band_indicator = static_cast<KeltnerChannelMiddleBand *>(create_indicator_from_id(keltner_channel_middle_band_indicator.id, keltner_channel_middle_band_indicator.params));
    std::vector<double> result_created_keltner_channel_middle_band_indicator = created_keltner_channel_middle_band_indicator->calculate(mock_candles);

    ASSERT_NE(created_keltner_channel_middle_band_indicator, nullptr);
    ASSERT_EQ(result_created_keltner_channel_middle_band_indicator, result_keltner_channel_middle_band);
}

TEST_F(VolatilityIndicatorsTest, UI)
{
    UI ui_indicator(3);
    std::vector<double> result_ui = ui_indicator.calculate(mock_candles);
    ASSERT_EQ(result_ui.size(), mock_candles.size());
    for (size_t i = 0; i < result_ui.size(); i++)
    {
        ASSERT_TRUE(result_ui[i] >= 0);
    }

    UI *created_ui_indicator = static_cast<UI *>(create_indicator_from_id(ui_indicator.id, ui_indicator.params));
    std::vector<double> result_created_ui_indicator = created_ui_indicator->calculate(mock_candles);

    ASSERT_NE(created_ui_indicator, nullptr);
    ASSERT_EQ(result_created_ui_indicator, result_ui);
}

TEST_F(VolatilityIndicatorsTest, StandardDeviation)
{
    std::vector<Candle> candles = {
        {.close = 2},
        {.close = 4},
        {.close = 6},
        {.close = 8},
        {.close = 12},
        {.close = 14},
        {.close = 16},
        {.close = 18},
        {.close = 20},
    };

    StandardDeviation stddev_indicator(3);
    std::vector<double> result_stddev = stddev_indicator.calculate(candles);

    // Round the values to 3 decimal places
    for (size_t i = 0; i < result_stddev.size(); ++i)
    {
        result_stddev[i] = decimal_round(result_stddev[i], 3);
    }

    std::vector<double> expected_stddev = {0, 0, 1.633, 1.633, 2.494, 2.494, 1.633, 1.633, 1.633}; // Expected Standard Deviation values

    ASSERT_EQ(result_stddev, expected_stddev);

    StandardDeviation *created_stddev_indicator = static_cast<StandardDeviation *>(create_indicator_from_id(stddev_indicator.id, stddev_indicator.params));
    std::vector<double> result_created_stddev_indicator = created_stddev_indicator->calculate(candles);

    ASSERT_NE(created_stddev_indicator, nullptr);
    for (int i = 0; i < result_stddev.size(); i++)
    {
        ASSERT_NEAR(result_stddev[i], result_created_stddev_indicator[i], 0.001);
    }
}

TEST_F(VolatilityIndicatorsTest, AveragePriceChange)
{
    AveragePriceChange apc_indicator(3);
    std::vector<double> result_apc = apc_indicator.calculate(mock_candles);
    ASSERT_EQ(result_apc.size(), mock_candles.size());

    AveragePriceChange *created_apc_indicator = static_cast<AveragePriceChange *>(create_indicator_from_id(apc_indicator.id, apc_indicator.params));
    std::vector<double> result_created_apc_indicator = created_apc_indicator->calculate(mock_candles);

    ASSERT_NE(created_apc_indicator, nullptr);
}
