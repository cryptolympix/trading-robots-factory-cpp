#ifndef CANDLES_SOURCE_H
#define CANDLES_SOURCE_H

#include <vector>
#include <stdexcept>
#include "../types.hpp"

/**
 * @brief Get the specified candle data source from the provided candles.
 *
 * @param candles The candles data.
 * @param source The source of candle data to retrieve.
 * @return std::vector<double> The candles with source.
 * @throws std::runtime_error If an unknown candle source is passed.
 */
std::vector<double> get_candles_with_source(const std::vector<CandleStick> &candles, std::string source);

#endif /* CANDLES_SOURCE_H */