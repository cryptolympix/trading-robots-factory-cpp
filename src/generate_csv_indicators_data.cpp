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
    if (argc != 3)
    {
        std::cerr << "Usage: ./generate_csv_candles_data <symbol> <timeframe>" << std::endl;
        return 1;
    }

    // Get the symbol and the timeframe from the arguments
    std::string symbol = argv[1];
    TimeFrame timeframe = time_frame_from_string(argv[2]);

    // Data to be written to the csv file
    std::vector<std::vector<std::string>> csv_data;

    // List of column names for the csv file
    std::vector<std::string> columns = {"date"};

    // List of indicators
    std::vector<Indicator *> indicators = {
        new CandleOpen(),
        new CandleHigh(),
        new CandleLow(),
        new CandleClose(),
        new Hour(),
        new Minute(),
        new NFPWeek(),
        new MarketSession("new-york"),
        new MarketSession("london"),
        new MarketSession("tokyo"),
        new WeekDay("monday"),
        new WeekDay("tuesday"),
        new WeekDay("wednesday"),
        new WeekDay("thursday"),
        new WeekDay("friday"),
        new CandlePriceChange(0),
        new CandlePriceChange(1),
        new CandlePriceChange(2),
        new CandlePriceChange(3),
        new CandlePriceChange(4),
        new CandlePriceChange(5),
        new CandlePriceChange(6),
        new CandlePriceChange(7),
        new CandlePriceChange(8),
        new CandlePriceChange(9),
        new CandleBody(0),
        new CandleBody(1),
        new CandleBody(2),
        new CandleBody(3),
        new CandleBody(4),
        new CandleBody(5),
        new CandleBody(6),
        new CandleBody(7),
        new CandleBody(8),
        new CandleBody(9),
        new CandleShadowUpper(0),
        new CandleShadowUpper(1),
        new CandleShadowUpper(2),
        new CandleShadowUpper(3),
        new CandleShadowUpper(4),
        new CandleShadowUpper(5),
        new CandleShadowUpper(6),
        new CandleShadowUpper(7),
        new CandleShadowUpper(8),
        new CandleShadowUpper(9),
        new CandleShadowLower(0),
        new CandleShadowLower(1),
        new CandleShadowLower(2),
        new CandleShadowLower(3),
        new CandleShadowLower(4),
        new CandleShadowLower(5),
        new CandleShadowLower(6),
        new CandleShadowLower(7),
        new CandleShadowLower(8),
        new CandleShadowLower(9),
        new AveragePriceChange(10),
        new StandardDeviation(14),
        new ATR(14),
        new RSI(14),
        new MFI(14),
        new CCI(20),
        new ADX(14),
        new CMF(20),
        new TSI(13, 25),
        new UO(7, 14, 28),
        new WPR(14),
        new InstitutionalBias(9, 18),
        new HighBreakSignal(10),
        new LowBreakSignal(10),
        new NewHighSignal(10),
        new NewLowSignal(10),
    };

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

    return 0;
}
