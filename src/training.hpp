#ifndef TRAINING_HPP
#define TRAINING_HPP

#include "types.hpp"
#include "utils/indexer.hpp"
#include "utils/uid.hpp"
#include "utils/cache.hpp"
#include "utils/progress_bar.hpp"
#include "trader.hpp"
#include "neat/population.hpp"
#include "neat/genome.hpp"

/**
 * @brief Number of candles needed to calculate the indicator in all time frames.
 */
constexpr int CANDLES_WINDOW = 25;

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

    std::map<time_t, CandlesData> candles;                        // Candle data for all time frames at a specified date.
    std::map<time_t, IndicatorsData> indicators;                  // Indicator data for all time frames at a specified date.
    BaseCurrencyConversionRateData base_currency_conversion_rate; // Conversion rate when the base asset traded is different from the account currency.
    Cache *cache;                                                 // Cached data for faster access.
    
    std::vector<time_t> dates; // The list of dates to parse in the simulation

    neat::Population *population;                 // NEAT population for evolution.
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
     * @param display_progress Flag to show the progress bar. Default is false.
     */
    void load_candles(bool display_progress = false);

    /**
     * @brief Calculate and store all indicator values to the cache.
     * @param display_progress Flag to show the progress bar. Default is false.
     */
    void load_indicators(bool display_progress = false);

    /**
     * @brief Load the conversion rate when the base asset traded is different from the account currency.
     * @param display_progress Flag to show the progress bar. Default is false.
     */
    void load_base_currency_conversion_rate(bool display_progress = false);

    /**
     * @brief Cache all the data (candles and indicators values) for every datetime.
     * @param display_progress Flag to show the progress bar. Default is false.
     */
    void cache_data(bool display_progress = false);

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
    void evaluate_genome(neat::Genome *genome, int generation);

    /**
     * @brief Run the NEAT algorithm for training.
     * @return The exit code of the training process. 0 if successful, 1 otherwise.
     */
    int run();

    /**
     * @brief Run the strategy of the best trader in the test period.
     * @param genome The genome to be tested.
     * @param generation The generation number of the genome. Default is 0.
     * @return The exit code of the testing process. 0 if successful, 1 otherwise.
     */
    int test(neat::Genome *genome, int generation = 0);

    /**
     * @brief Evaluate the strategy of a trader using the Monte Carlo simulation.
     * @param trader The trader to be evaluated.
     * @param nb_simulations The number of simulations to run. Default is 1000.
     * @return The exit code of the evaluation process. 0 if the results are consistences with the training period, 1 otherwise.
     */
    int evaluate_trader_with_monte_carlo_simulation(Trader *trader, int nb_simulations = 1000);
};

#endif // TRAINING_HPP