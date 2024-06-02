#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>
#include <cstring>
#include "neat/genome.hpp"
#include "utils/time_frame.hpp"
#include "trader.hpp"
#include "types.hpp"

// Choose the configuration file to use
#include "configs/default.hpp"

#ifdef _WIN32
#define TEST_DLL_API __declspec(dllexport)
#else
#define TEST_DLL_API
#endif

Config config;
neat::Genome* genome = nullptr;
Trader* trader = nullptr;

#ifdef _WIN32

// Function to display a message in the MetaTrader editor console
void PrintToConsole(const char* message)
{
    // Use the MessageBoxA function to display the message
    MessageBoxA(NULL, message, "Message from DLL", MB_OK | MB_ICONINFORMATION);
}

#endif

// Function to get the timeframe for a given code 
TimeFrame TimeFrameFromCode(int timeframe_code)
{
    switch (timeframe_code) {
        case 0:
            return TimeFrame::M1;
        case 1:
            return TimeFrame::M5;
        case 2:
            return TimeFrame::M15;
        case 3:
            return TimeFrame::M30;
        case 4:
            return TimeFrame::H1;
        case 5:
            return TimeFrame::H4;
        case 6:
            return TimeFrame::D1;
        default:
#if defined(_WIN32)
            PrintToConsole("One of the timeframes is not available.");
#endif
            std::exit(1);
    }
}

TEST_DLL_API void test_dll()
{
    // Example function body
}

TEST_DLL_API double make_decision(
    Candle* candles_tf_1,
    int candles_tf_1_size,
    int tf_1_code,
    Candle* candles_tf_2,
    int candles_tf_2_size,
    int tf_2_code,
    Candle* candles_tf_3,
    int candles_tf_3_size,
    int tf_3_code,
    int position_type,
    double position_pnl,
    double position_size,
    int position_duration,
    double base_currency_conversion_rate,
    double account_balance)
{
    std::vector<TimeFrame> candles_timeframes;
    CandlesData candles_data;
    IndicatorsData indicators_data;
    std::vector<PositionInfo> position_infos;

    if (candles_tf_1_size > 0)
    {
        TimeFrame tf_1 = TimeFrameFromCode(tf_1_code);
        candles_timeframes.push_back(tf_1);
        for (int i = 0; i < candles_tf_1_size; ++i)
        {
            candles_data[tf_1].push_back(candles_tf_1[i]);
        }
    }
    if (candles_tf_2_size > 0)
    {
        TimeFrame tf_2 = TimeFrameFromCode(tf_2_code);
        candles_timeframes.push_back(tf_2);
        for (int i = 0; i < candles_tf_2_size; ++i)
        {
            candles_data[tf_2].push_back(candles_tf_2[i]);
        }
    }
    if (candles_tf_3_size > 0)
    {
        TimeFrame tf_3 = TimeFrameFromCode(tf_3_code);
        candles_timeframes.push_back(tf_3);
        for (int i = 0; i < candles_tf_3_size; ++i)
        {
            candles_data[tf_3].push_back(candles_tf_3[i]);
        }
    }

    // Calculate the indicators and add them to the inputs
    for (const auto& [timeframe, indicators] : config.training.inputs.indicators)
    {
        if (std::find(candles_timeframes.begin(), candles_timeframes.end(), timeframe) == candles_timeframes.end())
        {
#if defined(_WIN32)
            PrintToConsole("One of the timeframes is not available in the input data.");
#endif
            std::exit(1);
        }

        for (const auto& indicator : indicators)
        {
            std::vector<double> values = indicator->calculate(candles_data[timeframe], true);
            indicators_data[timeframe][indicator->id] = values;
        }
    }

    // Get the position infos from the config
    for (const auto& position_info : config.training.inputs.position)
    {
        position_infos.push_back(position_info);
    }

    // Update the trader
    trader->balance = account_balance;
    trader->update(candles_data);

    // Update the current position
    if (position_type != 0 && trader->current_position != nullptr)
    {
        trader->current_position->pnl = position_pnl;
    }
    if (position_type == 0 && trader->current_position != nullptr)
    {
        trader->close_position_by_market();
    }

    // Look at the data
    trader->look(indicators_data, base_currency_conversion_rate, position_infos);

    // Make the decision
    trader->think();

    // Return the decision
    return trader->trade();
}

#ifdef _WIN32

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    std::string genomePath;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Get the config
        config = __config__;

        // Construct the full path to the genome file
        genomePath = "C:\\Users\\Maxime\\AppData\\Roaming\\MetaQuotes\\Terminal\\D0E8209F77C8CF37AD8BF550E51FF075\\MQL5\\Libraries\\genome.json";

        // Load the genome from the file
        genome = neat::Genome::load(genomePath);

        if (genome == nullptr)
        {
            PrintToConsole("Cannot load the genome.");
            std::exit(1);
        }

        trader = new Trader(genome, config);

        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        // Cleanup resources allocated during initialization
        delete genome;
        genome = nullptr;

        delete trader;
        trader = nullptr;

        break;
    }
    return TRUE;
}

#endif
