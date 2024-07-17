#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <chrono>
#include <iomanip>
#include "date_conversion.hpp"
#include "time_frame.hpp"
#include "read_data.hpp"

/**
 * @brief Read candle data from a CSV file for a specified symbol and time frame.
 *
 * @param symbol The symbol for which the data is to be read.
 * @param time_frame The time frame of the data (M1, M5, H1, etc.).
 * @param start_date The start date for filtering data (optional, default is the Unix epoch).
 * @param end_date The end date for filtering data (optional, default is the current date and time).
 * @return std::vector<Candle> A vector of Candle objects containing candle data.
 * @throw std::runtime_error If the specified data file does not exist.
 */
std::vector<Candle> read_data(const std::string &symbol, TimeFrame time_frame, time_t start_date, time_t end_date)
{
    std::vector<Candle> candles;

    // Convert TimeFrame enum to string
    std::string time_frame_str;
    switch (time_frame)
    {
    case TimeFrame::M1:
        time_frame_str = "M1";
        break;
    case TimeFrame::M5:
        time_frame_str = "M5";
        break;
    case TimeFrame::M15:
        time_frame_str = "M15";
        break;
    case TimeFrame::M30:
        time_frame_str = "M30";
        break;
    case TimeFrame::H1:
        time_frame_str = "H1";
        break;
    case TimeFrame::H4:
        time_frame_str = "H4";
        break;
    case TimeFrame::H12:
        time_frame_str = "H12";
        break;
    case TimeFrame::D1:
        time_frame_str = "D1";
        break;
    }

    std::string file = "./data/" + symbol + "/" + symbol + "_" + time_frame_str + ".csv";

    std::ifstream csv_file(file);
    if (!csv_file.is_open())
    {
        throw std::runtime_error("No data for " + symbol + " on " + time_frame_str);
    }

    std::string line;
    std::string header = "<DATE>\t<TIME>\t<OPEN>\t<HIGH>\t<LOW>\t<CLOSE>\t<TICKVOL>\t<VOL>\t<SPREAD>";

    while (std::getline(csv_file, line))
    {

        if (line.empty())
        {
            continue;
        }

        if (line == header)
        {
            continue;
        }

        std::istringstream ss(line);
        std::string token;
        Candle candle;

        // Read candle data from CSV line
        std::getline(ss, token, '\t'); // DATE
        std::string date_str = token;

        std::getline(ss, token, '\t'); // TIME
        date_str += " " + token;

        std::tm tm = string_to_tm(date_str, "%Y.%m.%d %H:%M:%S");
        time_t date = std::mktime(&tm) + 60 * get_time_frame_in_minutes(time_frame); // To have the closing time of the candle
        candle.date = date;

        std::getline(ss, token, '\t'); // OPEN
        candle.open = std::stod(token);

        std::getline(ss, token, '\t'); // HIGH
        candle.high = std::stod(token);

        std::getline(ss, token, '\t'); // LOW
        candle.low = std::stod(token);

        std::getline(ss, token, '\t'); // CLOSE
        candle.close = std::stod(token);

        std::getline(ss, token, '\t'); // TICK_VOLUME
        candle.tick_volume = std::stod(token);

        // Check if the volume is available in the data
        if (ss.eof())
        {
            candle.volume = candle.tick_volume;
            candle.spread = 0;
        }
        else
        {
            std::getline(ss, token, '\t'); // VOLUME
            // If the value is equals to zero, use the tick volume instead
            if (std::stod(token) == 0)
            {
                candle.volume = candle.tick_volume;
            }
            else
            {
                candle.volume = std::stod(token);
            }

            std::getline(ss, token, '\t'); // SPREAD
            candle.spread = std::stod(token);
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
