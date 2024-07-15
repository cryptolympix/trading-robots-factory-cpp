#ifndef INDICATORS_VOLUME_HPP
#define INDICATORS_VOLUME_HPP

#include <vector>
#include <functional>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include "builder.hpp"
#include "../types.hpp"
#include "indicator.hpp"

/**
 * Accumulation/Distribution Line (ADL)
 *
 * Acting as leading indicator of price movements.
 */
class ADL : public Indicator
{
public:
    /**
     * @brief Construct a new ADL object.
     *
     * @param source Source of candle data.
     * @param left_bars Number of left bars.
     * @param right_bars Number of right bars.
     * @param offset Offset value. Default is 0.
     */
    ADL(int offset = 0);

    /**
     * @brief Calculate the ADL values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    /**
     * @brief Calculate the Money Flow Multiplier for a single candle.
     *
     * @param candle The Candle data.
     * @return double The calculated Money Flow Multiplier.
     */
    double calculate_money_flow_multiplier(const Candle &candle) const;
};

// *********************************************************************************************************************

/**
 * Chaikin Money Flow (CMF)
 *
 * A volume-weighted average of the accumulation/distribution line.
 */
class CMF : public Indicator
{
public:
    /**
     * @brief Constructor for ChaikinMoneyFlow class.
     *
     * @param period Period for the CMF. Default is 20.
     * @param offset Offset value for the indicator. Default is 0.
     */
    CMF(int period = 20, int offset = 0);

    /**
     * @brief Calculate the Chaikin Money Flow (CMF) values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated CMF values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************************************

/**
 * Represents the Force Index (FI) indicator.
 *
 * FI measures the strength of a price trend based on volume.
 */
class FI : public Indicator
{
public:
    /**
     * @brief Constructor for ForceIndex class.
     *
     * @param period Period for the Force Index. Default is 13.
     * @param offset Offset value for the indicator. Default is 0.
     */
    FI(int period = 13, int offset = 0);

    /**
     * @brief Calculate the Force Index (FI) values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated FI values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************************************

/**
 * Represents the Negative Volume Index (NVI) indicator.
 *
 * NVI measures the trend of prices when the trading volume decreases. It starts at 1000 and adds the
 * percentage price change to the cumulative NVI when the volume decreases. When the volume increases, the
 * cumulative NVI remains unchanged. A 255-day Exponential Moving Average (EMA) is applied for signals.
 */
class NVI : public Indicator
{
public:
    /**
     * @brief Constructor for NVI class.
     *
     * @param offset Offset value for the indicator. Default is 0.
     */
    NVI(int offset = 0);

    /**
     * @brief Calculate the Negative Volume Index (NVI) values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated NVI values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const;
};

// *********************************************************************************************************************

/**
 * Represents the On-balance Volume (OBV) indicator.
 *
 * OBV is a cumulative indicator that uses volume flow to predict changes in stock price.
 * It adds the trading volume on days when the price closes higher than the previous day's close
 * and subtracts the trading volume on days when the price closes lower than the previous day's close.
 */
class OBV : public Indicator
{
public:
    /**
     * @brief Constructor for OBV class.
     *
     * @param offset Offset value for the indicator. Default is 0.
     */
    OBV(int offset = 0);

    /**
     * @brief Calculate the On-balance Volume (OBV) values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated OBV values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************************************

/**
 * Represents the Point of Control (POC) indicator.
 *
 * POC is the price at which the highest volume occurred in the volume profile.
 */
class POC : public Indicator
{
public:
    /**
     * @brief Constructor for POC class.
     *
     * @param period Period for the POC. Default is 14.
     * @param range_nb Number of ranges for the volume profile. Default is 10.
     * @param offset Offset value for the indicator. Default is 0.
     */
    POC(int period = 14, int range_nb = 10, int offset = 0);

    /**
     * @brief Calculate the Point of Control (POC) values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated POC values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    /**
     * @brief Calculate the volume profile for the given candles.
     *
     * @param candles Vector of Candle data.
     * @return std::map<double, double> Map containing the volume profile.
     */
    std::map<double, double> calculate_volume_profile(const std::vector<Candle> &candles) const;
};

// *********************************************************************************************************************

/**
 * Represents the Positive Volume Index (PVI) indicator.
 *
 * PVI is a cumulative indicator that increases when the volume increases compared to the previous period.
 */
class PVI : public Indicator
{
public:
    /**
     * @brief Constructor for PVI class.
     *
     * @param offset Offset value for the indicator. Default is 0.
     */
    PVI(int offset = 0);

    /**
     * @brief Calculate the Positive Volume Index (PVI) values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated PVI values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************************************

/**
 * Represents the Volume Weighted Average Price (VWAP) indicator.
 *
 * VWAP calculates the average price of a security over a specified time period,
 * weighted by the trading volume during that period.
 */
class VWAP : public Indicator
{
public:
    /**
     * @brief Constructor for VWAP class.
     *
     * @param offset Offset value for the indicator. Default is 0.
     */
    VWAP(int offset = 0);

    /**
     * @brief Calculate the Volume Weighted Average Price (VWAP) values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated VWAP values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

const std::unordered_map<std::string, std::function<Indicator *(std::unordered_map<std::string, IndicatorParam>)>> volume_indicators_map = {
    {"adl", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"offset", typeid(int)}}))
             {
                 int offset = std::get<int>(params["offset"]);
                 return new ADL(offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating ADL: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"cmf", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new CMF(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating CMF: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"fi", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new FI(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating FI: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"nvi", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"offset", typeid(int)}}))
             {
                 int offset = std::get<int>(params["offset"]);
                 return new NVI(offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating NVI: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"obv", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"offset", typeid(int)}}))
             {
                 int offset = std::get<int>(params["offset"]);
                 return new OBV(offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating OBV: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"poc", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"range_nb", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int range_nb = std::get<int>(params["range_nb"]);
                 int offset = std::get<int>(params["offset"]);
                 return new POC(period, range_nb, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating POC: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"pvi", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"offset", typeid(int)}}))
             {
                 int offset = std::get<int>(params["offset"]);
                 return new PVI(offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating PVI: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"vwap", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"offset", typeid(int)}}))
             {
                 int offset = std::get<int>(params["offset"]);
                 return new VWAP(offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating VWAP: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
};

#endif // INDICATORS_VOLUME_HPP