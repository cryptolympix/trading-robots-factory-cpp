#ifndef TREND_SIGNALS_HPP
#define TREND_SIGNALS_HPP

#include <vector>
#include <string>
#include <functional>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include "builder.hpp"
#include "indicator.hpp"

class ADXSignal : public Indicator
{
public:
    /**
     * @brief Construct a new ADXSignal object.
     *
     * @param period ADX period value. Default is 14.
     * @param threshold Threshold value. Default is 25.
     * @param offset Offset value. Default is 0.
     */
    ADXSignal(int period = 14, int threshold = 25, int offset = 0);

    /**
     * @brief Calculate the ADXSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class AroonSignal : public Indicator
{
public:
    /**
     * @brief Construct a new AroonSignal object.
     *
     * @param aroon_period Aroon period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    AroonSignal(int period = 14, int offset = 0);

    /**
     * @brief Calculate the AroonSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class CCISignal : public Indicator
{
public:
    /**
     * @brief Construct a new CCISignal object.
     *
     * @param period CCI period value. Default is 20.
     * @param overbought Overbought value. Default is 100.
     * @param oversold Oversold value. Default is -100.
     * @param offset Offset value. Default is 0.
     */
    CCISignal(int period = 20, int overbought = 100, int oversold = -100, int offset = 0);

    /**
     * @brief Calculate the CCISignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class DPOSignal : public Indicator
{
public:
    /**
     * @brief Construct a new DPOSignal object.
     *
     * @param period DPO period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    DPOSignal(int period = 20, int offset = 0);

    /**
     * @brief Calculate the DPOSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class EMASignal : public Indicator
{
public:
    /**
     * @brief Construct a new EMASignal object.
     *
     * @param source The source of the data. Default is "close".
     * @param period EMA period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    EMASignal(std::string source = "close", int period = 20, int offset = 0);

    /**
     * @brief Calculate the EMASignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class KSTSignal : public Indicator
{
public:
    /**
     * @brief Construct a new KSTSignal object.
     *
     * @param roc_period1 Rate of Change period 1 value. Default is 10.
     * @param roc_period2 Rate of Change period 2 value. Default is 15.
     * @param roc_period3 Rate of Change period 3 value. Default is 20.
     * @param roc_period4 Rate of Change period 4 value. Default is 30.
     * @param sma_period1 Simple Moving Average period 1 value. Default is 10.
     * @param sma_period2 Simple Moving Average period 2 value. Default is 10.
     * @param sma_period3 Simple Moving Average period 3 value. Default is 10.
     * @param sma_period4 Simple Moving Average period 4 value. Default is 15.
     * @param offset Offset value. Default is 0.
     */
    KSTSignal(int roc_period1 = 10, int roc_period2 = 15, int roc_period3 = 20, int roc_period4 = 30, int sma_period1 = 10, int sma_period2 = 10, int sma_period3 = 10, int sma_period4 = 15, int offset = 0);

    /**
     * @brief Calculate the KSTSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class MACDSignal : public Indicator
{
public:
    std::string label = "Moving Average Convergence Divergence (Signal)";
    std::string id = "macd-signal";
    std::string id_pattern = R"(macd-signal-(\d+)-(\d+)-(\d+)-(\d+))";

    /**
     * @brief Construct a new MACDSignal object.
     *
     * @param short_period Short period value. Default is 12.
     * @param long_period Long period value. Default is 26.
     * @param signal_period Signal period value. Default is 9.
     * @param offset Offset value. Default is 0.
     */
    MACDSignal(int short_period = 12, int long_period = 26, int signal_period = 9, int offset = 0);

    /**
     * @brief Calculate the MACDSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class ParabolicSARSignal : public Indicator
{
public:
    /**
     * @brief Construct a new ParabolicSARSignal object.
     *
     * @param acceleration_factor_initial Initial acceleration factor value. Default is 0.02.
     * @param acceleration_factor_maximum Maximum acceleration factor value. Default is 0.2.
     * @param offset Offset value. Default is 0.
     */
    ParabolicSARSignal(double acceleration_factor_initial = 0.02, double acceleration_factor_maximum = 0.2, int offset = 0);

    /**
     * @brief Calculate the ParabolicSARSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class SMASignal : public Indicator
{
public:
    /**
     * @brief Construct a new SMASignal object.
     *
     * @param source The source of the data. Default is "close".
     * @param period SMA period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    SMASignal(std::string source = "close", int period = 20, int offset = 0);

    /**
     * @brief Calculate the SMASignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class TRIXSignal : public Indicator
{
public:
    /**
     * @brief Construct a new TRIXSignal object.
     *
     * @param period TRIX period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    TRIXSignal(int period = 15, int offset = 0);

    /**
     * @brief Calculate the TRIXSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class VortexSignal : public Indicator
{
public:
    /**
     * @brief Construct a new VortexSignal object.
     *
     * @param period Vortex period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    VortexSignal(int period = 14, int offset = 0);

    /**
     * @brief Calculate the VortexSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class InstitutionalBiasSignal : public Indicator
{
public:
    /**
     * @brief Construct a new InstitutionalBiasSignal object.
     *
     * @param short_period Short period value. Default is 9.
     * @param long_period Long period value. Default is 18.
     * @param offset Offset value. Default is 0.
     */
    InstitutionalBiasSignal(int short_period = 9, int long_period = 18, int offset = 0);

    /**
     * @brief Calculate the InstitutionalBiasSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class IchimokuCloudSignal : public Indicator
{
public:
    /**
     * @brief Construct a new IchimokuCloudSignal object.
     *
     * @param conversion_period Conversion line period value. Default is 9.
     * @param base_period Base line period value. Default is 26.
     * @param lagging_period Lagging span period value. Default is 26.
     * @param leading_period Leading span period value. Default is 52.
     * @param offset Offset value. Default is 0.
     */
    IchimokuCloudSignal(int conversion_period = 9, int base_period = 26, int lagging_period = 26, int leading_period = 52, int offset = 0);

    /**
     * @brief Calculate the IchimokuCloudSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class IchimokuKijunTenkanSignal : public Indicator
{
public:
    /**
     * @brief Construct a new IchimokuKijunTenkanSignal object.
     *
     * @param conversion_period Conversion line period value. Default is 9.
     * @param base_period Base line period value. Default is 26.
     * @param lagging_period Lagging span period value. Default is 26.
     * @param leading_period Leading span period value. Default is 52.
     * @param offset Offset value. Default is 0.
     */
    IchimokuKijunTenkanSignal(int conversion_period = 9, int base_period = 26, int lagging_period = 26, int leading_period = 52, int offset = 0);

    /**
     * @brief Calculate the IchimokuKijunTenkanSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

const std::unordered_map<std::string, std::function<Indicator *(std::unordered_map<std::string, IndicatorParam>)>> trend_signals_indicators_map = {
    {"adx-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"threshold", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int threshold = std::get<int>(params["threshold"]);
                 int offset = std::get<int>(params["offset"]);
                 return new ADXSignal(period, threshold, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating ADXSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"aroon-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new AroonSignal(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating AroonSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"cci-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"overbought", typeid(int)}, {"oversold", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int overbought = std::get<int>(params["overbought"]);
                 int oversold = std::get<int>(params["oversold"]);
                 int offset = std::get<int>(params["offset"]);
                 return new CCISignal(period, overbought, oversold, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating CCISignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"dpo-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new DPOSignal(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating DPOSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"ema-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"source", typeid(std::string)}, {"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 std::string source = std::get<std::string>(params["source"]);
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new EMASignal(source, period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating EMASignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"kst-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"roc_period1", typeid(int)}, {"roc_period2", typeid(int)}, {"roc_period3", typeid(int)}, {"roc_period4", typeid(int)}, {"sma_period1", typeid(int)}, {"sma_period2", typeid(int)}, {"sma_period3", typeid(int)}, {"sma_period4", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int roc_period1 = std::get<int>(params["roc_period1"]);
                 int roc_period2 = std::get<int>(params["roc_period2"]);
                 int roc_period3 = std::get<int>(params["roc_period3"]);
                 int roc_period4 = std::get<int>(params["roc_period4"]);
                 int sma_period1 = std::get<int>(params["sma_period1"]);
                 int sma_period2 = std::get<int>(params["sma_period2"]);
                 int sma_period3 = std::get<int>(params["sma_period3"]);
                 int sma_period4 = std::get<int>(params["sma_period4"]);
                 int offset = std::get<int>(params["offset"]);
                 return new KSTSignal(roc_period1, roc_period2, roc_period3, roc_period4, sma_period1, sma_period2, sma_period3, sma_period4, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating KSTSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"macd-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"short_period", typeid(int)}, {"long_period", typeid(int)}, {"signal_period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int short_period = std::get<int>(params["short_period"]);
                 int long_period = std::get<int>(params["long_period"]);
                 int signal_period = std::get<int>(params["signal_period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new MACDSignal(short_period, long_period, signal_period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating MACDSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"psar-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"acceleration_factor_initial", typeid(double)}, {"acceleration_factor_maximum", typeid(double)}, {"offset", typeid(int)}}))
             {
                 double acceleration_factor_initial = std::get<double>(params["acceleration_factor_initial"]);
                 double acceleration_factor_maximum = std::get<double>(params["acceleration_factor_maximum"]);
                 int offset = std::get<int>(params["offset"]);
                 return new ParabolicSARSignal(acceleration_factor_initial, acceleration_factor_maximum, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating ParabolicSARSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"sma-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"source", typeid(std::string)}, {"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 std::string source = std::get<std::string>(params["source"]);
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new SMASignal(source, period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating SMASignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"trix-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new TRIXSignal(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating TRIXSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"vortex-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new VortexSignal(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating VortexSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"institutional-bias-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"short_period", typeid(int)}, {"long_period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int short_period = std::get<int>(params["short_period"]);
                 int long_period = std::get<int>(params["long_period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new InstitutionalBiasSignal(short_period, long_period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating InstitutionalBiasSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"ichimoku-cloud-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"conversion_period", typeid(int)}, {"base_period", typeid(int)}, {"lagging_period", typeid(int)}, {"leading_period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int conversion_period = std::get<int>(params["conversion_period"]);
                 int base_period = std::get<int>(params["base_period"]);
                 int lagging_period = std::get<int>(params["lagging_period"]);
                 int leading_period = std::get<int>(params["leading_period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new IchimokuCloudSignal(conversion_period, base_period, lagging_period, leading_period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating IchimokuCloudSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"ichimoku-kijun-tenkan-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"conversion_period", typeid(int)}, {"base_period", typeid(int)}, {"lagging_period", typeid(int)}, {"leading_period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int conversion_period = std::get<int>(params["conversion_period"]);
                 int base_period = std::get<int>(params["base_period"]);
                 int lagging_period = std::get<int>(params["lagging_period"]);
                 int leading_period = std::get<int>(params["leading_period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new IchimokuKijunTenkanSignal(conversion_period, base_period, lagging_period, leading_period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating IchimokuKijunTenkanSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }}

};

#endif // TREND_SIGNALS_HPP