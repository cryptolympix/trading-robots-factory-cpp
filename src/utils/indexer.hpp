#ifndef INDEXER_H
#define INDEXER_H

#include <unordered_map>
#include <vector>
#include <ctime>
#include <stdexcept>
#include "../types.hpp"

/**
 * @brief Class for managing indexes to parse candles from start to end with a window bounded by the current date.
 */
class Indexer
{
private:
    CandleSticksData candles;                                   // A dictionary containing candle data for different timeframes. */
    int window;                                                 // The size of the window for parsing candles. */
    std::unordered_map<TimeFrame, std::pair<int, int>> indexes; // A dictionary storing start and end indexes for each timeframe. */

public:
    /**
     * @brief Constructs an Indexer object with candles data and a window size.
     *
     * @param candles_data A dictionary containing candle data for different timeframes.
     * @param window The size of the window for parsing candles.
     */
    Indexer(const CandleSticksData &candles, int window);

    /**
     * @brief Updates the indexes based on the specified date.
     *
     * @param date The date used to update the indexes.
     */
    void update_indexes(time_t date);

    /**
     * @brief Retrieves the start and end indexes for the given timeframe.
     *
     * @param timeframe The timeframe for which indexes are retrieved.
     * @return std::pair<int, int> A pair containing start and end indexes.
     * @throw std::runtime_error If the provided timeframe is unknown.
     */
    std::pair<int, int> get_indexes(TimeFrame timeframe);
};

#endif /* INDEXER_H */
