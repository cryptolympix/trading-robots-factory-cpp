#ifndef CANDLE_DATA_READER_H
#define CANDLE_DATA_READER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <chrono>
#include <iomanip>
#include "../types.hpp"

/**
 * @brief Read candle data from a CSV file for a specified symbol and time frame.
 *
 * @param symbol The symbol for which the data is to be read.
 * @param time_frame The time frame of the data (M1, M5, H1, etc.).
 * @param start_date The start date for filtering data (optional, default is the Unix epoch).
 * @param end_date The end date for filtering data (optional, default is the current date and time).
 * @return std::vector<CandleStick> A vector of CandleStick objects containing candle data.
 * @throw std::runtime_error If the specified data file does not exist.
 */
std::vector<CandleStick> read_data(const std::string &symbol, TimeFrame time_frame, time_t start_date = 0, time_t end_date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

#endif /* CANDLE_DATA_READER_H */
