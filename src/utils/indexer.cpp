#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <chrono>
#include "../types.hpp"
#include "indexer.hpp"
#include "time_frame.hpp"

/**
 * @brief Constructs an Indexer object with candles data and a window size.
 *
 * @param candles A dictionary containing candle data for different timeframes.
 * @param window The size of the window for parsing candles.
 */
Indexer::Indexer(const CandlesData &candles, int window) : candles(candles), window(window)
{
    // Initialize indexes with start and end set to 0 for each timeframe
    for (const auto &pair : candles)
    {
        indexes[pair.first] = std::make_pair(0, 0);
    }
}

/**
 * @brief Updates the indexes based on the specified date.
 *
 * @param date The date used to update the indexes.
 */
void Indexer::update_indexes(std::chrono::system_clock::time_point &date)
{
    for (auto &pair : candles)
    {
        TimeFrame tf = pair.first;
        while (indexes[tf].second < pair.second.size() &&
               std::chrono::system_clock::from_time_t(pair.second[indexes[tf].second].date) + std::chrono::minutes(get_time_frame_value(tf)) <= date)
        {
            // Increment the index
            indexes[tf].second++;

            // Adjust window
            if (indexes[tf].second - indexes[tf].first > window)
            {
                indexes[tf].first++;
            }
        }
    }
}

/**
 * @brief Retrieves the start and end indexes for the given timeframe.
 *
 * @param timeframe The timeframe for which indexes are retrieved.
 * @return std::pair<int, int> A pair containing start and end indexes.
 * @throw std::runtime_error If the provided timeframe is unknown.
 */
std::pair<int, int> Indexer::get_indexes(TimeFrame timeframe)
{
    auto it = indexes.find(timeframe);
    if (it != indexes.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("Error: Unknown timeframe");
    }
}
