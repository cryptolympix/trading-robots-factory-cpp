#include <limits>
#include "../types.hpp"
#include "../utils/candles_source.hpp"
#include "../utils/math.hpp"
#include "indicator.hpp"
#include "volume.hpp"

/**
 * @brief Construct a new ADL object.
 *
 * @param offset Offset value. Default is 0.
 */
ADL::ADL(int offset) : Indicator("Accumulation Distribution Line", "adl-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the ADL values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> ADL::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> adi_values(candles.size(), 0);
            double previous_adi = 0.0;

            for (size_t i = 0; i < candles.size(); ++i)
            {
                double adi = previous_adi + calculate_money_flow_multiplier(candles[i]) * candles[i].volume;
                adi_values[i] = adi;
                previous_adi = adi;
            }

            return adi_values; },

        normalize_data);
}

/**
 * @brief Calculate the Money Flow Multiplier for a single candle.
 *
 * @param candle The Candle data.
 * @return double The calculated Money Flow Multiplier.
 */
double ADL::calculate_money_flow_multiplier(const Candle &candle) const
{
    double money_flow_multiplier = 0.0;
    if (candle.high != candle.low)
    {
        money_flow_multiplier = ((candle.close - candle.low) - (candle.high - candle.close)) / (candle.high - candle.low);
    }
    return money_flow_multiplier;
}

// *********************************************************************************************************************

/**
 * @brief Constructor for ChaikinMoneyFlow class.
 *
 * @param period Period for the CMF. Default is 20.
 * @param offset Offset value for the indicator. Default is 0.
 */
CMF::CMF(int period, int offset) : Indicator("Chaikin Money Flow", "cmf-" + std::to_string(period) + "-" + std::to_string(offset), offset, {-1, 1}), period(period) {}

/**
 * @brief Calculate the Chaikin Money Flow (CMF) values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated CMF values.
 */
std::vector<double> CMF::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            // Initialize the CMF values vector with zeros.
            std::vector<double> cmf_values(candles.size(), 0.0);

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                double mf_volume_sum = 0.0;
                double volume_sum = 0.0;

                for (int j = i - period + 1; j <= i; ++j)
                {
                    double high = candles[j].high;
                    double low = candles[j].low;
                    double close = candles[j].close;
                    double volume = candles[j].volume;

                    // Calculate the Money Flow Multiplier
                    double mf_multiplier = ((close - low) - (high - close)) / (high - low);
                    
                    // Calculate the Money Flow Volume
                    double mf_volume = mf_multiplier * volume;

                    // Accumulate sums
                    mf_volume_sum += mf_volume;
                    volume_sum += volume;
                }

                if (volume_sum != 0) {
                    cmf_values[i] = mf_volume_sum / volume_sum;
                }
            }
            
            return cmf_values; },

        normalize_data);
}

// *********************************************************************************************************************

/**
 * @brief Constructor for ForceIndex class.
 *
 * @param period Period for the Force Index. Default is 13.
 * @param offset Offset value for the indicator. Default is 0.
 */
FI::FI(int period, int offset) : Indicator("Force Index", "fi-" + std::to_string(period) + "-" + std::to_string(offset), offset), period(period) {}

/**
 * @brief Calculate the Force Index (FI) values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated FI values.
 */
std::vector<double> FI::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            // Initialize the Force Index values vector with zeros.
            std::vector<double> fi_values(candles.size(), 0.0);

            for (int i = period - 1; i < candles.size(); ++i)
            {
                double fi = 0.0;
                for (int j = i - period; j <= i; ++j)
                {
                    fi += (candles[i].close - candles[i - period + 1].close) * candles[i].volume;
                }
                fi_values[i] = fi;
            }

            return fi_values; },

        normalize_data);
}

// *********************************************************************************************************************

/**
 * @brief Constructor for NVI class.
 *
 * @param offset Offset value for the indicator. Default is 0.
 */
NVI::NVI(int offset) : Indicator("Negative Volume Index", "nvi-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the Negative Volume Index (NVI) values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated NVI values.
 */
std::vector<double> NVI::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles,
        [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            std::vector<double> nvi_values(candles.size(), 0.0);
            double cumulative_nvi = 1000.0; // Cumulative NVI starts at 1000

            for (size_t i = 1; i < candles.size(); ++i)
            {
                if (candles[i].volume < candles[i - 1].volume)
                {
                    // Volume decreased
                    double percentage_change = (candles[i].close - candles[i - 1].close) / candles[i - 1].close;
                    cumulative_nvi += percentage_change;
                }
                // Cumulative NVI is unchanged when volume increases (do nothing)

                nvi_values[i] = cumulative_nvi;
            }

            return nvi_values;
        },
        normalize_data);
}

// *********************************************************************************************************************

/**
 * @brief Constructor for OBV class.
 *
 * @param offset Offset value for the indicator. Default is 0.
 */
OBV::OBV(int offset) : Indicator("On-balance Volume", "obv-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the On-balance Volume (OBV) values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated OBV values.
 */
std::vector<double> OBV::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles,
        [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            std::vector<double> obv_values(candles.size(), 0.0);
            double obv = 0.0;

            for (int i = 1; i < candles.size(); ++i)
            {
                if (candles[i].close > candles[i - 1].close)
                {
                    // Price closed higher than the previous day's close
                    obv += candles[i].volume;
                }
                else if (candles[i].close < candles[i - 1].close)
                {
                    // Price closed lower than the previous day's close
                    obv -= candles[i].volume;
                }
                // If the price remained unchanged, OBV remains unchanged (do nothing)

                obv_values[i] = obv;
            }

            return obv_values;
        },
        normalize_data);
}

// *********************************************************************************************************************

/**
 * @brief Constructor for POC class.
 *
 * @param period Period for the indicator. Default is 14.
 * @param range_nb Number of ranges for the volume profile. Default is 10.
 * @param offset Offset value for the indicator. Default is 0.
 */
POC::POC(int period, int range_nb, int offset) : Indicator("Point of Control", "poc-" + std::to_string(period) + "-" + std::to_string(range_nb) + "-" + std::to_string(offset), offset), period(period), range_nb(range_nb) {}

/**
 * @brief Calculate the Point of Control (POC) values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated POC values.
 */
std::vector<double> POC::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles,
        [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            std::vector<double> poc_values(candles.size(), 0.0);

            for (int i = 0; i < candles.size(); ++i)
            {
                // Get the candles for the current period
                std::vector<Candle> period_candles(candles.begin() + std::max(0, i - period + 1), candles.begin() + i + 1);
                std::map<double, double> volume_profile = calculate_volume_profile(period_candles);

                // Find the tick with the highest volume
                auto max_volume_tick = std::max_element(volume_profile.begin(), volume_profile.end(), [](const std::pair<double, double> &p1, const std::pair<double, double> &p2)
                                                        { return p1.second < p2.second; });

                // Set POC to the price of the tick with the highest volume
                poc_values[i] = max_volume_tick->first;
            }

            return poc_values;
        },

        normalize_data);
}

/**
 * @brief Calculate the volume profile for the given candles.
 *
 * @param candles Vector of Candle data.
 * @return std::map<double, double> Map containing the volume profile.
 */
std::map<double, double> POC::calculate_volume_profile(const std::vector<Candle> &candles) const
{
    std::map<double, double> volume_profile;

    double min_val = std::numeric_limits<double>::max();
    double max_val = std::numeric_limits<double>::lowest();

    // Find the maximum and minimum closes in the input list to
    for (int i = 0; i < candles.size(); ++i)
    {
        if (candles[i].close < min_val)
            min_val = candles[i].close;
        if (candles[i].close > max_val)
            max_val = candles[i].close;
    }

    // Calculate the range based on the difference between maximum and minimum values
    double range = max_val - min_val;

    // Calculate the range size that will be used to divide the range into a number of ranges
    double range_size = range / range_nb;

    // For each candle, find the range index and add the volume to the corresponding range
    // in the volume profile
    for (int i = 0; i < candles.size(); ++i)
    {
        int range_index = (candles[i].close - min_val) / range_size;
        range_index = static_cast<int>(range_index);
        double rangeStart = min_val + range_index * range_size;
        volume_profile[rangeStart] += candles[i].volume;
    }

    return volume_profile;
}

// *********************************************************************************************************************

/**
 * @brief Constructor for PVI class.
 *
 * @param offset Offset value for the indicator. Default is 0.
 */
PVI::PVI(int offset) : Indicator("Positive Volume Index", "pvi-" + std::to_string(offset), offset)
{
}

/**
 * @brief Calculate the Positive Volume Index (PVI) values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated PVI values.
 */
std::vector<double> PVI::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles,
        [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            std::vector<double> pvi_values(candles.size(), 0.0);
            double cumulative_pvi = 1000.0; // PVI starts at 1000

            for (size_t i = 1; i < candles.size(); ++i)
            {
                if (candles[i].volume > candles[i - 1].volume)
                {
                    // Volume increased compared to the previous period
                    cumulative_pvi += ((candles[i].close - candles[i - 1].close) / candles[i - 1].close) * cumulative_pvi;
                }
                // If volume remains the same or decreases, PVI remains unchanged (do nothing)

                pvi_values[i] = cumulative_pvi;
            }

            return pvi_values;
        },

        normalize_data);
}

// *********************************************************************************************************************

/**
 * @brief Constructor for VWAP class.
 *
 * @param offset Offset value for the indicator. Default is 0.
 */
VWAP::VWAP(int offset) : Indicator("Volume Weighted Average Price", "vwap-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the Volume Weighted Average Price (VWAP) values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated VWAP values.
 */
std::vector<double> VWAP::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles,
        [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            std::vector<double> vwap_values(candles.size(), 0.0);
            double cumulative_price_volume = 0.0;
            double cumulative_volume = 0.0;

            std::vector<double> typical_prices = get_candles_with_source(candles, "hlc3");

            for (size_t i = 0; i < candles.size(); ++i)
            {
                double price_volume = typical_prices[i] * candles[i].volume;

                cumulative_price_volume += price_volume;
                cumulative_volume += candles[i].volume;

                // Calculate VWAP for each period
                if (cumulative_volume != 0)
                {
                    vwap_values[i] = cumulative_price_volume / cumulative_volume;
                }
            }

            return vwap_values;
        },

        normalize_data);
}