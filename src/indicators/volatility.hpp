#ifndef INDICATORS_VOLATILITY_HPP
#define INDICATORS_VOLATILITY_HPP

#include <vector>
#include <functional>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include "builder.hpp"
#include "indicator.hpp"

/**
 * Average True Range (ATR)
 *
 * The indicator provide an indication of the degree of price volatility. Strong moves, in either direction, are often accompanied by large ranges, or large True Ranges.
 *
 * http://stockcharts.com/school/doku.php?id=chart_school:technical_indicators:average_true_range_atr
 */
class ATR : public Indicator
{
public:
    /**
     * @brief Construct a new ATR object.
     *
     * @param period Period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    ATR(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Average True Range values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    /**
     * @brief Calculate the True Range for a specific candle.
     *
     * @param candles Vector of Candle data.
     * @return std::vector<double> The calculated True Range values.
     */
    std::vector<double> calculateTrueRange(const std::vector<Candle> &candles) const;
};

// *****************************************************************************

class BollingerChannelHighBand : public Indicator
{
public:
    /**
     * @brief Construct a new BollingerChannelHighBand object.
     *
     * @param period Period value. Default is 20.
     * @param multiplier Multiplier value. Default is 2.0.
     * @param offset Offset value. Default is 0.
     */
    BollingerChannelHighBand(int period = 20, double multiplier = 2.0, int offset = 0);

    /**
     * @brief Calculate the Bollinger Channel High Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *****************************************************************************

class BollingerChannelLowBand : public Indicator
{
public:
    /**
     * @brief Construct a new BollingerChannelLowBand object.
     *
     * @param period Period value. Default is 20.
     * @param multiplier Multiplier value. Default is 2.0.
     * @param offset Offset value. Default is 0.
     */
    BollingerChannelLowBand(int period = 20, double multiplier = 2.0, int offset = 0);

    /**
     * @brief Calculate the Bollinger Channel Low Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *****************************************************************************

class BollingerChannelMiddleBand : public Indicator
{
public:
    /**
     * @brief Construct a new BollingerChannelMiddleBand object.
     *
     * @param period Period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    BollingerChannelMiddleBand(int period = 20, int offset = 0);

    /**
     * @brief Calculate the Bollinger Channel Middle Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *****************************************************************************

/**
 * Bollinger Band Percent (BB %B) quantifies a symbol's price relative to the upper and lower Bollinger Band. There are six basic relationship levels:
 * - %B equals 1 when price is at the upper band
 * - %B equals 0 when price is at the lower band
 * - %B is above 1 when price is above the upper band
 * - %B is below 0 when price is below the lower band
 * - %B is above .50 when price is above the middle band (20-day SMA)
 * - %B is below .50 when price is below the middle band (20-day SMA)
 *
 * @see https://www.barchart.com/education/technical-indicators/bollinger_bands_percent
 */
class BollingerChannelPercentageBand : public Indicator
{
public:
    /**
     * @brief Construct a new BollingerChannelPercentageBand object.
     *
     * @param period Period value. Default is 20.
     * @param multiplier Multiplier value. Default is 2.0.
     * @param offset Offset value. Default is 0.
     */
    BollingerChannelPercentageBand(int period = 20, double multiplier = 2.0, int offset = 0);

    /**
     * @brief Calculate the Bollinger Channel Percentage Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *****************************************************************************

class BollingerChannelWidthBand : public Indicator
{
public:
    /**
     * @brief Construct a new BollingerChannelWidthBand object.
     *
     * @param period Period value. Default is 20.
     * @param multiplier Multiplier value. Default is 2.0.
     * @param offset Offset value. Default is 0.
     */
    BollingerChannelWidthBand(int period = 20, double multiplier = 2.0, int offset = 0);

    /**
     * @brief Calculate the Bollinger Channel Width Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *****************************************************************************

class DonchianChannelHighBand : public Indicator
{
public:
    /**
     * @brief Construct a new DonchianChannelHighBand object.
     *
     * @param period Period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    DonchianChannelHighBand(int period = 20, int offset = 0);

    /**
     * @brief Calculate the Donchian Channel High Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *****************************************************************************

class DonchianChannelLowBand : public Indicator
{
public:
    /**
     * @brief Construct a new DonchianChannelLowBand object.
     *
     * @param period Period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    DonchianChannelLowBand(int period = 20, int offset = 0);

    /**
     * @brief Calculate the Donchian Channel Low Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *****************************************************************************

class DonchianChannelMiddleBand : public Indicator
{
public:
    /**
     * @brief Construct a new DonchianChannelMiddleBand object.
     *
     * @param period Period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    DonchianChannelMiddleBand(int period = 20, int offset = 0);

    /**
     * @brief Calculate the Donchian Channel Middle Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *****************************************************************************

class KeltnerChannelHighBand : public Indicator
{
public:
    /**
     * @brief Construct a new KeltnerChannelHighBand object.
     *
     * @param period Period value. Default is 20.
     * @param atr_period ATR period value. Default is 10.
     * @param multiplier Multiplier value. Default is 2.0.
     * @param offset Offset value. Default is 0.
     */
    KeltnerChannelHighBand(int period = 20, int atr_period = 10, double multiplier = 2.0, int offset = 0);

    /**
     * @brief Calculate the Keltner Channel High Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *****************************************************************************

class KeltnerChannelLowBand : public Indicator
{
public:
    /**
     * @brief Construct a new KeltnerChannelLowBand object.
     *
     * @param period Period value. Default is 20.
     * @param atr_period ATR period value. Default is 10.
     * @param multiplier Multiplier value. Default is 2.0.
     * @param offset Offset value. Default is 0.
     */
    KeltnerChannelLowBand(int period = 20, int atr_period = 10, double multiplier = 2.0, int offset = 0);

    /**
     * @brief Calculate the Keltner Channel Low Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *****************************************************************************

class KeltnerChannelMiddleBand : public Indicator
{
public:
    /**
     * @brief Construct a new KeltnerChannelMiddleBand object.
     *
     * @param period Period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    KeltnerChannelMiddleBand(int period = 20, int offset = 0);

    /**
     * @brief Calculate the Keltner Channel Middle Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *****************************************************************************

/**
 * The Ulcer Index (UI) is a technical indicator that measures downside risk in terms of price declines both in magnitude and duration.
 */
class UI : public Indicator
{
public:
    /**
     * @brief Construct a new Ulcer Index object.
     *
     * @param period Period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    UI(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Ulcer Index values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *****************************************************************************

class StandardDeviation : public Indicator
{
public:
    /**
     * @brief Construct a new Standard Deviation object.
     *
     * @param period Period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    StandardDeviation(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Standard Deviation values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *****************************************************************************

class AveragePriceChange : public Indicator
{
public:
    /**
     * @brief Construct a new Average Price Change object.
     *
     * @param period Period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    AveragePriceChange(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Average Price Change values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

const std::unordered_map<std::string, std::function<Indicator *(std::unordered_map<std::string, IndicatorParam>)>> volatility_indicators_map = {
    {"atr", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new ATR(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating WPR: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"bollinger-channel-high-band", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"multiplier", typeid(double)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 double multiplier = std::get<double>(params["multiplier"]);
                 int offset = std::get<int>(params["offset"]);
                 return new BollingerChannelHighBand(period, multiplier, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating BollingerChannelHighBand: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"bollinger-channel-low-band", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"multiplier", typeid(double)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 double multiplier = std::get<double>(params["multiplier"]);
                 int offset = std::get<int>(params["offset"]);
                 return new BollingerChannelLowBand(period, multiplier, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating BollingerChannelLowBand: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"bollinger-channel-middle-band", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new BollingerChannelMiddleBand(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating BollingerChannelMiddleBand: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"bollinger-channel-percentage-band", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"multiplier", typeid(double)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 double multiplier = std::get<double>(params["multiplier"]);
                 int offset = std::get<int>(params["offset"]);
                 return new BollingerChannelPercentageBand(period, multiplier, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating BollingerChannelPercentageBand: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"bollinger-channel-width-band", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"multiplier", typeid(double)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 double multiplier = std::get<double>(params["multiplier"]);
                 int offset = std::get<int>(params["offset"]);
                 return new BollingerChannelWidthBand(period, multiplier, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating BollingerChannelWidthBand: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"donchian-channel-high-band", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new DonchianChannelHighBand(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating DonchianChannelHighBand: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"donchian-channel-low-band", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new DonchianChannelLowBand(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating DonchianChannelLowBand: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"donchian-channel-middle-band", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new DonchianChannelMiddleBand(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating DonchianChannelMiddleBand: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"keltner-channel-high-band", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"atr_period", typeid(int)}, {"multiplier", typeid(double)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int atr_period = std::get<int>(params["atr_period"]);
                 double multiplier = std::get<double>(params["multiplier"]);
                 int offset = std::get<int>(params["offset"]);
                 return new KeltnerChannelHighBand(period, atr_period, multiplier, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating KeltnerChannelHighBand: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"keltner-channel-low-band", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"atr_period", typeid(int)}, {"multiplier", typeid(double)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int atr_period = std::get<int>(params["atr_period"]);
                 double multiplier = std::get<double>(params["multiplier"]);
                 int offset = std::get<int>(params["offset"]);
                 return new KeltnerChannelLowBand(period, atr_period, multiplier, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating KeltnerChannelLowBand: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"keltner-channel-middle-band", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new KeltnerChannelMiddleBand(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating KeltnerChannelMiddleBand: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"ui", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new UI(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating UI: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"standard-deviation", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new StandardDeviation(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating StandardDeviation: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"average-price-change", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new AveragePriceChange(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating AveragePriceChange: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
};

#endif // INDICATORS_VOLATILITY_HPP