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
 * @brief Read the header of a CSV file.
 *
 * @param csv_file The CSV file to read.
 * @param separator The separator used in the CSV file (optional, default is a comma).
 * @return std::vector<std::string> A vector of column names.
 */
std::vector<std::string> read_header(std::ifstream &csv_file, std::string separator = ",");

/**
 * @brief Read candle data from a CSV file for a specified symbol and time frame.
 *
 * @param symbol The symbol for which the data is to be read.
 * @param time_frame The time frame of the data (M1, M5, H1, etc.).
 * @param start_date The start date for filtering data (optional, default is the Unix epoch).
 * @param end_date The end date for filtering data (optional, default is the current date and time).
 * @param separator The separator used in the CSV file (optional, default is a comma).
 * @return std::vector<Candle> A vector of Candle objects containing candle data.
 */
std::vector<Candle> read_data(const std::string &symbol, TimeFrame time_frame, time_t start_date = 0, time_t end_date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), std::string separator = ",");

#endif /* CANDLE_DATA_READER_H */
