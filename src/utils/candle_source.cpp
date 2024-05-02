#include <vector>
#include <stdexcept>
#include "../types.hpp"
#include "candles_source.hpp"

/**
 * @brief Get the specified candle data source from the provided candles.
 *
 * @param candles The candles data.
 * @param source The source of candle data to retrieve.
 * @return std::vector<double> The candles with source.
 */
std::vector<double> get_candles_with_source(const std::vector<Candle> &candles, std::string source)
{
    std::vector<double> values;
    values.reserve(candles.size());

    if (source == "open")
    {
        for (auto &candle : candles)
        {
            values.push_back(candle.open);
        }
    }
    else if (source == "high")
    {
        for (auto &candle : candles)
        {
            values.push_back(candle.high);
        }
    }
    else if (source == "low")
    {
        for (auto &candle : candles)
        {
            values.push_back(candle.low);
        }
    }
    else if (source == "close")
    {
        for (auto &candle : candles)
        {
            values.push_back(candle.close);
        }
    }
    else if (source == "volume")
    {
        for (auto &candle : candles)
        {
            values.push_back(candle.volume);
        }
    }
    else if (source == "hl2")
    {
        for (auto &candle : candles)
        {
            values.push_back((candle.high + candle.low) / 2);
        }
    }
    else if (source == "hlc3")
    {
        for (auto &candle : candles)
        {
            values.push_back((candle.high + candle.low + candle.close) / 3);
        }
    }
    else if (source == "ohlc4")
    {
        for (auto &candle : candles)
        {
            values.push_back((candle.open + candle.high + candle.low + candle.close) / 4);
        }
    }
    else if (source == "hlcc4")
    {
        for (auto &candle : candles)
        {
            values.push_back((candle.high + candle.low + candle.close + candle.close) / 4);
        }
    }
    else
    {
        throw std::runtime_error("Unknown candle source passed to get_candles_with_source: " + source);
        values = {};
    }

    return values;
}