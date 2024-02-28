#include <gtest/gtest.h>
#include <vector>
#include <cstdlib> // For rand() function
#include <ctime>
#include "../../utils/read_data.hpp"
#include "../../utils/math.hpp"
#include "../trend.hpp"

class TestTrendIndicators : public ::testing::Test
{
protected:
    std::vector<Candle> mock_candles;

    void SetUp() override
    {
        mock_candles = read_data("EURUSD", TimeFrame::H1);
    }
};

TEST_F(TestTrendIndicators, ADXTest)
{
    ADX adx(14);
    std::vector<double> result_adx = adx.calculate(mock_candles);

    ASSERT_EQ(result_adx.size(), mock_candles.size());
}

TEST_F(TestTrendIndicators, AroonTest)
{
    AroonTrend aroon(14);
    std::vector<double> result_aroon_trend = aroon.calculate(mock_candles, false);

    ASSERT_EQ(result_aroon_trend.size(), mock_candles.size());
}
