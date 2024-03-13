#include <vector>
#include "../types.hpp"
#include "candles_source.hpp"

/**
 * @brief Map for different sources of candle data.
 */
std::map<CandleSource, std::string> CandleSourceMap = {
    {CandleSource::Open, "open"},
    {CandleSource::High, "high"},
    {CandleSource::Low, "low"},
    {CandleSource::Close, "close"},
    {CandleSource::Volume, "volume"},
    {CandleSource::HL2, "hl2"},
    {CandleSource::HLC3, "hlc3"},
    {CandleSource::OHLC4, "ohlc4"},
    {CandleSource::HLCC4, "hlcc4"}};

/**
 * @brief Get the specified candle data source from the provided candles.
 *
 * @param candles The candles data.
 * @param source The source of candle data to retrieve.
 * @return std::vector<double> The candles with source.
 */
std::vector<double> get_candles_with_source(const std::vector<Candle> &candles, CandleSource source)
{
    std::vector<double> values;
    values.reserve(candles.size());

    switch (source)
    {
    case CandleSource::Open:
        for (auto &candle : candles)
        {
            values.push_back(candle.open);
        }
        break;
    case CandleSource::High:
        for (auto &candle : candles)
        {
            values.push_back(candle.high);
        }
        break;
    case CandleSource::Low:
        for (auto &candle : candles)
        {
            values.push_back(candle.low);
        }
        break;
    case CandleSource::Close:
        for (auto &candle : candles)
        {
            values.push_back(candle.close);
        }
        break;
    case CandleSource::Volume:
        for (auto &candle : candles)
        {
            values.push_back(candle.volume);
        }
        break;
    case CandleSource::HL2:
        for (auto &candle : candles)
        {
            values.push_back((candle.high + candle.low) / 2);
        }
        break;
    case CandleSource::HLC3:
        for (auto &candle : candles)
        {
            values.push_back((candle.high + candle.low + candle.close) / 3);
        }
        break;
    case CandleSource::OHLC4:
        for (auto &candle : candles)
        {
            values.push_back((candle.open + candle.high + candle.low + candle.close) / 4);
        }
        break;
    case CandleSource::HLCC4:
        for (auto &candle : candles)
        {
            values.push_back((candle.high + candle.low + candle.close + candle.close) / 4);
        }
        break;
    default:
        throw std::runtime_error("Unknown candle source passed to get_candles_with_source: " + std::to_string(static_cast<int>(source)));
        values = {};
    }

    return values;
}