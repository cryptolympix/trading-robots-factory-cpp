#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <regex>
#include "date_conversion.hpp"
#include "time_frame.hpp"
#include "read_data.hpp"

// Supported columns in the CSV file
std::vector<std::string> columns_names = {"Time", "Open", "High", "Low", "Close", "Volume", "Spread"};

/**
 * @brief Read the header of a CSV file.
 *
 * @param csv_file The CSV file to read.
 * @param separator The separator used in the CSV file (optional, default is a comma).
 * @return std::vector<std::string> A vector of column names.
 */
std::vector<std::string> read_header(std::ifstream &csv_file, std::string separator)
{
    std::string line;
    std::getline(csv_file, line);

    std::vector<std::string> columns;
    std::istringstream header_ss(line);
    std::string token;

    // Split the header by the separator
    while (std::getline(header_ss, token, separator[0]))
    {
        token.erase(std::remove(token.begin(), token.end(), '\r'), token.end()); // Remove carriage return
        columns.push_back(token);
    }

    // Check if the columns are valid
    for (const auto &column : columns)
    {
        if (std::find(columns_names.begin(), columns_names.end(), column) == columns_names.end())
        {
            std::cerr << "Invalid column: " << column << std::endl;
            std::cerr << "Supported columns:" << std::endl;
            for (const auto &column_name : columns_names)
            {
                std::cerr << "\t" << column_name << std::endl;
            }
            std::cerr << std::endl;
            std::exit(1);
        }
    }

    return columns;
}

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
std::vector<Candle> read_data(const std::string &symbol, TimeFrame time_frame, time_t start_date, time_t end_date, std::string separator)
{
    std::vector<Candle> candles;

    // Convert TimeFrame enum to string
    std::string time_frame_str = time_frame_to_string(time_frame);

    std::string file = "./data/" + symbol + "/" + symbol + "_" + time_frame_str + ".csv";

    std::ifstream csv_file(file);
    if (!csv_file.is_open())
    {
        std::cerr << "No data for " << symbol << " on " << time_frame_str;
        std::exit(1);
    }

    std::string line;

    // Read the header
    std::vector<std::string> columns = read_header(csv_file, separator);

    // Read the data
    while (std::getline(csv_file, line))
    {
        // Skip empty lines
        if (line.empty())
        {
            continue;
        }

        std::istringstream ss(line);
        std::string token;
        Candle candle;

        for (size_t i = 0; i < columns.size(); i++)
        {
            // Split the line by the separator
            std::getline(ss, token, separator[0]);

            // Remove carriage return
            token.erase(std::remove(token.begin(), token.end(), '\r'), token.end());

            if (columns[i] == "Time")
            {
                std::string date_str = token;                             // Copy the date string
                std::replace(date_str.begin(), date_str.end(), '.', '-'); // Replace dots with dashes
                std::string date_format = "";                             // The format of the date for the conversion

                // Find the format of the date
                std::unordered_map<std::string, std::regex> date_formats = {
                    {"%Y-%m-%d %H:%M:%S", std::regex("\\d{4}\\-\\d{2}\\-\\d{2} \\d{2}:\\d{2}:\\d{2}")},
                    {"%Y-%m-%d %H:%M", std::regex("\\d{4}\\-\\d{2}\\-\\d{2} \\d{2}:\\d{2}")},
                };

                // Check if the date string matches any of the supported formats
                for (const auto &[format, pattern] : date_formats)
                {
                    if (std::regex_match(date_str, pattern))
                    {
                        date_format = format;
                        break;
                    }
                }

                // If the date format is empty, the date string is invalid
                if (date_format == "")
                {
                    std::cerr << "Invalid date format: " << date_str << std::endl;
                    std::cerr << "Supported formats:" << std::endl;
                    for (const auto &[format, pattern] : date_formats)
                    {
                        std::cerr << "\t" << format << std::endl;
                    }
                    std::cerr << std::endl;
                    std::exit(1);
                }

                // Convert the date string to a time_t
                std::tm tm = string_to_tm(date_str, date_format);
                time_t date = std::mktime(&tm) + 60 * get_time_frame_in_minutes(time_frame);

                // Set the date of the candle
                candle.date = date; // Date corresponds to the closing time of the candle
            }
            else if (columns[i] == "Open")
            {
                candle.open = std::stod(token);
            }
            else if (columns[i] == "Hight")
            {
                candle.high = std::stod(token);
            }
            else if (columns[i] == "Low")
            {
                candle.low = std::stod(token);
            }
            else if (columns[i] == "Close")
            {
                candle.close = std::stod(token);
            }
            else if (columns[i] == "Volume")
            {
                candle.volume = std::stod(token);
            }
            else if (columns[i] == "Spread")
            {
                candle.spread = std::stod(token);
            }
        }

        // Filter candles by date
        time_t candle_date = candle.date;
        if (candle_date >= start_date && candle_date <= end_date)
        {
            candles.push_back(candle);
        }

        // Stop reading data if the date of the candle is greater than the end date
        if (candle_date > end_date)
        {
            break;
        }
    }

    csv_file.close();
    return candles;
}
