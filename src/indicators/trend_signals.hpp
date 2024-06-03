#ifndef TREND_SIGNALS_HPP
#define TREND_SIGNALS_HPP

#include <vector>
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

private:
    int period;
    int threshold;
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

private:
    int period;
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

private:
    int period;
    int overbought;
    int oversold;
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

private:
    int period;
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

private:
    std::string source;
    int period;
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

private:
    int roc_period1;
    int roc_period2;
    int roc_period3;
    int roc_period4;
    int sma_period1;
    int sma_period2;
    int sma_period3;
    int sma_period4;
};

// *********************************************************************************************

class MACDSignal : public Indicator
{
public:
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

private:
    int short_period;
    int long_period;
    int signal_period;
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

private:
    double acceleration_factor_initial;
    double acceleration_factor_maximum;
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

private:
    std::string source;
    int period;
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

private:
    int period;
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

private:
    int period;
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

private:
    int short_period;
    int long_period;
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

private:
    int conversion_period;
    int base_period;
    int lagging_period;
    int leading_period;
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

private:
    int conversion_period;
    int base_period;
    int lagging_period;
    int leading_period;
};

#endif // TREND_SIGNALS_HPP