#ifndef TIME_INDICATORS_HPP
#define TIME_INDICATORS_HPP

#include "../types.hpp"
#include "indicator.hpp"

/**
 * @brief Class for Hour indicator.
 */
class Hour : public Indicator
{
public:
    std::string label = "Hour";
    std::string id = "hour";
    std::string id_pattern = R"(hour-(\d+))";

    /**
     * @brief Construct a new Hour object.
     *
     * @param offset Offset value. Default is 0.
     */
    Hour(int offset = 0);

    /**
     * @brief Calculate the Hour values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

/**
 * @brief Class for Minute indicator.
 */
class Minute : public Indicator
{
public:
    std::string label = "Minute";
    std::string id = "minute";
    std::string id_pattern = R"(minute-(\d+))";

    /**
     * @brief Construct a new Minute object.
     *
     * @param offset Offset value. Default is 0.
     */
    Minute(int offset = 0);

    /**
     * @brief Calculate the Minute values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class NFPWeek : public Indicator
{
public:
    std::string label = "NFP Week";
    std::string id = "nfp-week";
    std::string id_pattern = R"(nfp-week-(\d+))";

    /**
     * @brief Construct a new NFPWeek object.
     *
     * @param offset Offset value. Default is 0.
     */
    NFPWeek(int offset = 0);

    /**
     * @brief Check if the candle is on NFP week.
     *
     * @param candles Vector of Candle data.
     * @return std::vector<double> Vector containing 1 if the candle is on NFP week, 0 otherwise.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class MarketSession : public Indicator
{
public:
    std::string label = "Market Session";
    std::string id = "market-session";
    std::string id_pattern = R"(market-session-(\w+)-(\d+))";

    /**
     * @brief Construct a new MarketSession object.
     *
     * @param zone Market session zone.
     * @param offset Offset value. Default is 0.
     */
    MarketSession(std::string zone, int offset = 0);

    /**
     * @brief Check if the candle is on a market session.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing 1 if the candle is on the market session, 0 otherwise.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    std::string zone; // Market session zone
};

// *********************************************************************************************

class WeekDay : public Indicator
{
public:
    std::string label = "Week Day";
    std::string id = "week-day";
    std::string id_pattern = R"(week-day-(\w+)-(\d+))";

    /**
     * @brief Construct a new WeekDay object.
     *
     * @param day Day of the week.
     * @param offset Offset value. Default is 0.
     */
    WeekDay(std::string day, int offset = 0);

    /**
     * @brief Get the days of the candle.
     *
     * @param candles Vector of Candle data.
     * @return std::vector<double> Vector containing the number of each candle day (0-6).
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    std::string day; // Day of the week (monday, tuesday, wednesday, thursday, friday, saturday, sunday)
};

#endif // TIME_INDICATORS_HPP