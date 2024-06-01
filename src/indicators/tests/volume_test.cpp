#include <gtest/gtest.h>
#include <cmath>
#include <ctime>
#include <vector>
#include "../../utils/read_data.hpp"
#include "../../types.hpp"
#include "../../utils/math.hpp"
#include "../volume.hpp"

class VolumeIndicatorsTest : public ::testing::Test
{
protected:
    // Variables
    std::vector<Candle> mock_candles;

    void SetUp() override
    {
        mock_candles = read_data("EURUSD", TimeFrame::H1);
    }
};

TEST_F(VolumeIndicatorsTest, ADL)
{
    ADL adl;
    std::vector<double> adl_values = adl.calculate(mock_candles);
    ASSERT_EQ(adl_values.size(), mock_candles.size());
};

TEST_F(VolumeIndicatorsTest, CMF)
{
    CMF cmf;
    std::vector<double> cmf_values = cmf.calculate(mock_candles);
    ASSERT_EQ(cmf_values.size(), mock_candles.size());
    for (size_t i = 0; i < cmf_values.size(); ++i)
    {
        ASSERT_TRUE(cmf_values[i] >= -1 && cmf_values[i] <= 1);
    }
}

TEST_F(VolumeIndicatorsTest, FI)
{
    FI fi;
    std::vector<double> fi_values = fi.calculate(mock_candles);
    ASSERT_EQ(fi_values.size(), mock_candles.size());
}

TEST_F(VolumeIndicatorsTest, NVI)
{
    NVI nvi;
    std::vector<double> nvi_values = nvi.calculate(mock_candles);
    ASSERT_EQ(nvi_values.size(), mock_candles.size());
    for (size_t i = 0; i < nvi_values.size(); ++i)
    {
        ASSERT_TRUE(nvi_values[i] >= 0);
    }
}

TEST_F(VolumeIndicatorsTest, OBV)
{
    OBV obv;
    std::vector<double> obv_values = obv.calculate(mock_candles);
    ASSERT_EQ(obv_values.size(), mock_candles.size());
}

TEST_F(VolumeIndicatorsTest, POC)
{
    POC poc;
    std::vector<double> poc_values = poc.calculate(mock_candles);
    ASSERT_EQ(poc_values.size(), mock_candles.size());
    for (size_t i = 0; i < poc_values.size(); ++i)
    {
        ASSERT_TRUE(poc_values[i] >= 0);
    }
}

TEST_F(VolumeIndicatorsTest, PVI)
{
    PVI pvi;
    std::vector<double> pvi_values = pvi.calculate(mock_candles);
    ASSERT_EQ(pvi_values.size(), mock_candles.size());
    for (size_t i = 0; i < pvi_values.size(); ++i)
    {
        ASSERT_TRUE(pvi_values[i] >= 0);
    }
}

TEST_F(VolumeIndicatorsTest, VWAP)
{
    VWAP vwap;
    std::vector<double> vwap_values = vwap.calculate(mock_candles);
    ASSERT_EQ(vwap_values.size(), mock_candles.size());
    for (size_t i = 0; i < vwap_values.size(); ++i)
    {
        ASSERT_TRUE(vwap_values[i] >= 0);
    }
}