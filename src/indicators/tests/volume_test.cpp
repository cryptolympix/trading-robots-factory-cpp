#include <gtest/gtest.h>
#include <cmath>
#include <ctime>
#include <vector>
#include "../builder.hpp"
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

    ADL *created_adl_indicator = static_cast<ADL *>(create_indicator_from_id(adl.id, adl.params));
    std::vector<double> result_created_adl_indicator = created_adl_indicator->calculate(mock_candles);

    ASSERT_NE(created_adl_indicator, nullptr);
    ASSERT_EQ(result_created_adl_indicator, adl_values);
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

    CMF *created_cmf_indicator = static_cast<CMF *>(create_indicator_from_id(cmf.id, cmf.params));
    std::vector<double> result_created_cmf_indicator = created_cmf_indicator->calculate(mock_candles);

    ASSERT_NE(created_cmf_indicator, nullptr);
    ASSERT_EQ(result_created_cmf_indicator, cmf_values);
}

TEST_F(VolumeIndicatorsTest, FI)
{
    FI fi;
    std::vector<double> fi_values = fi.calculate(mock_candles);
    ASSERT_EQ(fi_values.size(), mock_candles.size());

    FI *created_fi_indicator = static_cast<FI *>(create_indicator_from_id(fi.id, fi.params));
    std::vector<double> result_created_fi_indicator = created_fi_indicator->calculate(mock_candles);

    ASSERT_NE(created_fi_indicator, nullptr);
    ASSERT_EQ(result_created_fi_indicator, fi_values);
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

    NVI *created_nvi_indicator = static_cast<NVI *>(create_indicator_from_id(nvi.id, nvi.params));
    std::vector<double> result_created_nvi_indicator = created_nvi_indicator->calculate(mock_candles);

    ASSERT_NE(created_nvi_indicator, nullptr);
    ASSERT_EQ(result_created_nvi_indicator, nvi_values);
}

TEST_F(VolumeIndicatorsTest, OBV)
{
    OBV obv;
    std::vector<double> obv_values = obv.calculate(mock_candles);
    ASSERT_EQ(obv_values.size(), mock_candles.size());

    OBV *created_obv_indicator = static_cast<OBV *>(create_indicator_from_id(obv.id, obv.params));
    std::vector<double> result_created_obv_indicator = created_obv_indicator->calculate(mock_candles);

    ASSERT_NE(created_obv_indicator, nullptr);
    ASSERT_EQ(result_created_obv_indicator, obv_values);
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

    POC *created_poc_indicator = static_cast<POC *>(create_indicator_from_id(poc.id, poc.params));
    std::vector<double> result_created_poc_indicator = created_poc_indicator->calculate(mock_candles);

    ASSERT_NE(created_poc_indicator, nullptr);
    ASSERT_EQ(result_created_poc_indicator, poc_values);
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

    PVI *created_pvi_indicator = static_cast<PVI *>(create_indicator_from_id(pvi.id, pvi.params));
    std::vector<double> result_created_pvi_indicator = created_pvi_indicator->calculate(mock_candles);

    ASSERT_NE(created_pvi_indicator, nullptr);
    ASSERT_EQ(result_created_pvi_indicator, pvi_values);
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

    VWAP *created_vwap_indicator = static_cast<VWAP *>(create_indicator_from_id(vwap.id, vwap.params));
    std::vector<double> result_created_vwap_indicator = created_vwap_indicator->calculate(mock_candles);

    ASSERT_NE(created_vwap_indicator, nullptr);
    ASSERT_EQ(result_created_vwap_indicator, vwap_values);
}