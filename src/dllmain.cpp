#ifdef _WIN32
#include <windows.h>
#endif

// Define the dll environment
#define DLL_EXPORT

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>
#include "neat/genome.hpp"
#include "utils/time_frame.hpp"
#include "trader.hpp"
#include "types.hpp"
#include "constants.hpp"

// Choose the configuration file to use
#include "configs/default.hpp"

#ifdef _WIN32
#define TEST_DLL_API __declspec(dllexport)
#else
#define TEST_DLL_API
#endif

Config config;
neat::Genome *genome;
Trader *trader;

TimeFrame TIMEFRAME_1 = TimeFrame::M5; // Loop timeframe
TimeFrame TIMEFRAME_2 = TimeFrame::M30;
TimeFrame TIMEFRAME_3 = TimeFrame::H4;

#ifdef _WIN32

// Function to display a message in the MetaTrader editor console
void PrintToConsole(const char *message)
{
    // Use the MessageBoxA function to display the message
    MessageBoxA(NULL, message, "Message from DLL", MB_OK | MB_ICONINFORMATION);
}

#endif

TEST_DLL_API void test_dll()
{
}

TEST_DLL_API int make_decision(
    Candle *candles_tf_1,
    int candles_tf_1_size,
    Candle *candles_tf_2,
    int candles_tf_2_size,
    Candle *candles_tf_3,
    int candles_tf_3_size,
    int position_type,
    double position_pnl,
    double position_size,
    int position_duration,
    double base_currency_conversion_rate,
    double account_balance)
{
    std::vector<TimeFrame> candles_timeframes = {};
    CandlesData candles_data = {};
    IndicatorsData indicators_data = {};
    std::vector<PositionInfo> position_infos = {};

    if (candles_tf_1_size > 0)
    {
        candles_timeframes.push_back(TIMEFRAME_1);
        for (int i = 0; i < candles_tf_1_size; ++i)
        {
            candles_data[TIMEFRAME_1].push_back(candles_tf_1[i]);
        }
    }
    if (candles_tf_2_size > 0)
    {
        candles_timeframes.push_back(TIMEFRAME_2);
        for (int i = 0; i < candles_tf_2_size; ++i)
        {
            candles_data[TIMEFRAME_2].push_back(candles_tf_2[i]);
        }
    }
    if (candles_tf_3_size > 0)
    {
        candles_timeframes.push_back(TIMEFRAME_3);
        for (int i = 0; i < candles_tf_3_size; ++i)
        {
            candles_data[TIMEFRAME_3].push_back(candles_tf_3[i]);
        }
    }

    // Calculate the indicators and add them to the inputs
    for (const auto &[timeframe, indicators] : config.training.inputs.indicators)
    {
        if (std::find(candles_timeframes.begin(), candles_timeframes.end(), timeframe) == candles_timeframes.end())
        {
#if defined(_WIN32)
            PrintToConsole("One of the timeframe is not available in the input data");
#endif
            std::exit(1);
        }

        for (const auto &indicator : indicators)
        {
            std::vector<double> values = indicator->calculate(candles_data[timeframe], true);
            indicators_data[timeframe][indicator->id] = values;
        }
    }

    // Get the position infos from the config
    for (const auto &position_info : config.training.inputs.position)
    {
        position_infos.push_back(position_info);
    }

    // ==================================================================================================== //

    // Update the trader
    trader->balance = account_balance;
    trader->update(candles_data);
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
    wchar_t *lastBackslash;
    std::filesystem::path genomePath;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Get the path to the DLL file
        wchar_t dllPath[MAX_PATH];
        GetModuleFileNameW(hModule, dllPath, MAX_PATH);

        // Extract the directory path
        lastBackslash = wcsrchr(dllPath, L'\\');
        if (lastBackslash != nullptr)
        {
            *lastBackslash = L'\0'; // Terminate the string at the last backslash
        }

        // Set the current directory to the DLL directory
        SetCurrentDirectoryW(dllPath);

        // Get the config
        config = __config__;

        // Construct the full path to the genome file
        genomePath = std::filesystem::path(dllPath) / L"genome.json";

        // Load the genome from the file
        genome = neat::Genome::load("C:\\Users\\Maxime\\AppData\\Roaming\\MetaQuotes\\Terminal\\D0E8209F77C8CF37AD8BF550E51FF075\\MQL5\\Libraries\\genome.json");

        if (genome == nullptr)
        {
            PrintToConsole("Cannot load the genome");
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
        if (genome != nullptr)
        {
            delete genome;
            genome = nullptr;
        }
        if (trader != nullptr)
        {
            delete trader;
            trader = nullptr;
        }

        break;
    }
    return TRUE;
}

#endif
