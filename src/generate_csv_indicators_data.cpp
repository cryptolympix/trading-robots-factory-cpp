#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include "types.hpp"
#include "utils/read_data.hpp"
#include "utils/time_frame.hpp"
#include "utils/date_conversion.hpp"
#include "indicators/indicator.hpp"
#include "configs/serialization.hpp"
#include "libs/json.hpp"

// Include the indicators
#include "indicators/candle_patterns.hpp"
#include "indicators/candle_signals.hpp"
#include "indicators/candle.hpp"
#include "indicators/momentum_signals.hpp"
#include "indicators/momentum.hpp"
#include "indicators/time.hpp"
#include "indicators/trend_signals.hpp"
#include "indicators/trend.hpp"
#include "indicators/volatility_signals.hpp"
#include "indicators/volatility.hpp"
#include "indicators/volume_signals.hpp"
#include "indicators/volume.hpp"

int main(int argc, char *argv[])
{
    // Check if the number of arguments is correct
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <config_file_path>" << std::endl;
        return 1;
    }

    // Get the configuration file path from the arguments
    std::string config_file_path = argv[1];

    // Open the file
    std::ifstream config_file(config_file_path);

    // Check if the file was successfully opened
    if (!config_file.is_open())
    {
        std::cerr << "Could not open the configuration file: " + config_file_path;
        return 1;
    }

    // Parse the JSON file into a nlohmann::json object
    nlohmann::json config_json_data;
    config_file >> config_json_data;

    // Close the file
    config_file.close();

    // Create the config object
    Config config = config_from_json(config_json_data);

    for (const auto &indicator : config.training.inputs.indicators)
    {
        TimeFrame timeframe = indicator.first;
        std::vector<Indicator *> indicators = indicator.second;
        std::string symbol = config.general.symbol;

        // Data to be written to the csv file
        std::vector<std::vector<std::string>> csv_data;

        // List of column names for the csv file
        std::vector<std::string> columns = {"date"};

        // Add the indicators id to the columns
        for (Indicator *&indicator : indicators)
        {
            columns.push_back(indicator->id);
        }

        // Read the candles data
        std::vector<Candle> candles = read_data(symbol, timeframe);

        // Print message
        std::cout << "Loaded " << candles.size() << " candles for " << symbol << " " << time_frame_to_string(timeframe) << std::endl;

        // Indicators values
        std::unordered_map<std::string, std::vector<double>> indicator_values;

        // Calculate the indicators
        for (Indicator *&indicator : indicators)
        {
            indicator_values[indicator->id] = indicator->calculate(candles);
        }

        // Prepare the data to be written to the csv file
        for (size_t i = 0; i < candles.size(); i++)
        {
            std::vector<std::string> row;
            row.reserve(columns.size());

            for (const auto &column : columns)
            {
                if (column == "date")
                {
                    row.push_back(time_t_to_string(candles[i].date));
                }
                else
                {
                    row.push_back(std::to_string(indicator_values[column][i]));
                }
            }

            csv_data.push_back(row);
        }

        // Directory to save the csv file
        std::filesystem::path directory = "data/" + symbol;

        // Check if the directory exists
        if (!std::filesystem::exists(directory))
        {
            std::filesystem::create_directories(directory);
        }

        // Csv file
        std::ofstream csv_file(directory / (symbol + "_" + time_frame_to_string(timeframe) + "_indicators.csv"));

        // Write the header
        for (size_t i = 0; i < columns.size(); i++)
        {
            csv_file << columns[i];
            if (i < columns.size() - 1)
            {
                csv_file << ",";
            }
        }
        csv_file << std::endl;

        // Write the data by respecting the columns order
        for (const auto &row : csv_data)
        {
            for (size_t j = 0; j < row.size(); j++)
            {
                csv_file << row[j];
                if (j < row.size() - 1)
                {
                    csv_file << ",";
                }
            }
            csv_file << std::endl;
        }

        // Close the csv file
        csv_file.close();

        // Free the memory
        for (Indicator *&indicator : indicators)
        {
            delete indicator;
        }

        std::cout << "📁 CSV file saved to " << directory << std::endl;
    }

    return 0;
}
