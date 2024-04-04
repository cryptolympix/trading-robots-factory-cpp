#ifndef TRAINING_HPP
#define TRAINING_HPP

#include "types.hpp"
#include "utils/cache.hpp"
#include "utils/indexer.hpp"
#include "utils/uid.hpp"
#include "utils/progress_bar.hpp"
#include "trader.hpp"
#include "neat/population.hpp"
#include "neat/genome.hpp"

/**
 * @brief Number of candles needed to calculate the indicator in all time frames.
 */
constexpr int MINIMUM_CANDLES = 25;

/**
 * @brief The last n value(s) loaded for each iteration of training for individuals.
 */
constexpr int INDICATOR_WINDOW = 5;

class Trader;

/**
 * @class Training
 * @brief The Training class handles the training process, including data preparation, evolution, and result analysis.
 */
class Training
{
public:
    std::string id;                   // Unique identifier for the training process.
    bool debug;                       // Debug mode flag.
    Config config;                    // Configuration object.
    std::filesystem::path directory;  // Directory to save the training results.
    std::filesystem::path cache_file; // Optional cache file path.

    CandlesData candles;                                          // Candle data for all time frames.
    IndicatorsData indicators;                                    // Indicator data for all time frames.
    BaseCurrencyConversionRateData base_currency_conversion_rate; // Conversion rate when the base asset traded is different from the account currency.
    Cache cache;                                                  // Cached data for faster access.

    Population *population;                       // NEAT population for evolution.
    std::map<int, std::vector<Trader *>> traders; // Map of traders for each generation.
    std::map<int, Trader *> best_traders;         // Map of the best trader for each generation.
    Trader *best_trader;                          // Best trader overall.

    /**
     * @brief Constructor for the Training class.
     * @param id Unique identifier for the training process.
     * @param config Configuration object.
     * @param debug Debug mode flag.
     */
    Training(std::string id, Config &config, bool debug = false);

    /**
     * @brief Prepare the training data by loading candles, calculating indicators, and caching the data.
     */
    void prepare();

    /**
     * @brief Load candle data for all time frames.
     * @param progress_bar Progress bar for loading candles.
     */
    void load_candles(ProgressBar *progress_bar = nullptr);

    /**
     * @brief Calculate and store all indicator values to the cache.
     * @param progress_bar Progress bar for calculating indicators.
     */
    void load_indicators(ProgressBar *progress_bar = nullptr);

    /**
     * @brief Load the conversion rate when the base asset traded is different from the account currency.
     * @param progress_bar Progress bar for loading the conversion rate.
     */
    void load_base_currency_conversion_rate(ProgressBar *progress_bar = nullptr);

    /**
     * @brief Cache all the data (candles and indicators values) for every datetime.
     * @param progress_bar Progress bar for caching data.
     */
    void cache_data(ProgressBar *progress_bar = nullptr);

    /**
     * @brief Count the total number of indicators used in training.
     * @return Total number of indicators.
     */
    int count_indicators() const;

    /**
     * @brief Get all the timeframes from the training inputs of the config.
     * @return Vector of timeframes.
     */
    std::vector<TimeFrame> get_all_timeframes() const;

    /**
     * @brief Adjust the training start date based on available candles.
     * @return Adjusted training start date.
     */
    std::chrono::system_clock::time_point find_training_start_date() const;

    /**
     * @brief Update the best trader of all the training and the best trader of a generation.
     * @param generation The current generation number of the training.
     */
    void set_best_traders(int generation);

    /**
     * @brief Get the best trader of a generation.
     * @param generation The generation number to get the best trader.
     * @return The best trader of the specified generation.
     */
    Trader *get_best_trader_of_generation(int generation) const;

    /**
     * @brief Evaluate the performance of a trading algorithm for a given genome and generation.
     * @param genome The genome to be evaluated.
     * @param generation The current generation number.
     */
    void evaluate_genome(Genome *genome, int generation);

    /**
     * @brief Run the NEAT algorithm for training.
     * @return The exit code of the training process. 0 if successful, 1 otherwise.
     */
    int run();
};

#endif // TRAINING_HPP