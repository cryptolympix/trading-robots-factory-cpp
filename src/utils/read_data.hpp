#ifndef CANDLE_DATA_READER_H
#define CANDLE_DATA_READER_H

#include <vector>
#include <string>
#include <ctime>
#include "../types.hpp"

/**
 * @brief Read candle data from a CSV file for a specified symbol and time frame.
 *
 * @param symbol The symbol for which the data is to be read.
 * @param time_frame The time frame of the data (M1, M5, H1, etc.).
 * @param start_date The start date for filtering data (optional, default is nullptr).
 * @param end_date The end date for filtering data (optional, default is nullptr).
 * @return std::vector<Candle> A vector of Candle objects containing candle data.
 * @throw std::runtime_error If the specified data file does not exist.
 */
std::vector<Candle> read_data(const std::string &symbol, TimeFrame time_frame, std::tm *start_date = nullptr, std::tm *end_date = nullptr);

#endif /* CANDLE_DATA_READER_H */
