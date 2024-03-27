#include <filesystem>
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <functional>
#include <chrono>
#include "types.hpp"
#include "utils/logger.hpp"
#include "utils/cache.cpp"
#include "utils/indexer.hpp"
#include "utils/uid.hpp"
#include "utils/read_data.hpp"
#include "utils/time_frame.hpp"
#include "utils/progress_bar.hpp"
#include "neat/population.hpp"
#include "neat/genome.hpp"
#include "training.hpp"
#include "symbols.hpp"
#include "trader.hpp"

/**
 * @brief Constructor for the Training class.
 * @param id Unique identifier for the training process.
 * @param config Configuration object.
 * @param debug Debug mode flag.
 * @param cache_file Optional cache file path.
 */
Training::Training(std::string id, Config &config, bool debug, const std::string &cache_file)
    : id(id), config(config), debug(debug), cache_file(cache_file)
{
    this->directory = "cache/" + this->config.general.name + "/" + this->config.general.version + "/" + id;

    // Initialize the population.
    this->config.neat.num_inputs = this->count_indicators() + this->config.training.inputs.position.size();
    this->population = new Population(this->config.neat);

    this->candles = {};
    this->indicators = {};

    // Conversion rate when the base of asset traded is different of the account currency
    this->base_currency_conversion_rate = {};
    this->cache = {};

    // History for statistics
    int generations = config.training.generations;
    this->traders = {};
    this->best_traders = {};
    this->best_trader = nullptr;
}

/**
 * @brief Prepare the training data by loading candles, calculating indicators, and caching the data.
 */
void Training::prepare()
{
    if (this->cache_file.empty())
    {
        this->cache_file = "cache/data/" + config.general.name + "_" + config.general.version + ".pkl";
    }
    if (std::filesystem::exists(this->cache_file))
    {
        std::cout << "⏳ Import the data from the cache..." << std::endl;
        cache = load_cached_dictionary<DatedCache>(this->cache_file);
        std::cout << "✅ Cache ready!" << std::endl;
    }
    else
    {
        // Progress bar
        std::cout << "⏳ Load the candles..." << std::endl;
        int total_iter1 = this->get_all_timeframes().size();
        ProgressBar *progress_bar1 = new ProgressBar(100, total_iter1);
        this->load_candles(progress_bar1);
        std::cout << "✅ Candles loaded!" << std::endl;

        std::cout << "⏳ Load the indicators..." << std::endl;
        int total_iter2 = this->count_indicators();
        ProgressBar *progress_bar2 = new ProgressBar(100, total_iter2);
        this->load_indicators(progress_bar2);
        std::cout << "✅ Indicators loaded!" << std::endl;

        std::cout << "⏳ Load the base currency conversion rate..." << std::endl;
        int total_iter3 = 1;
        ProgressBar *progress_bar3 = new ProgressBar(100, total_iter3);
        this->load_base_currency_conversion_rate(progress_bar3);
        std::cout << "✅ Base currency conversion rate loaded!" << std::endl;

        std::cout << "⏳ Cache the data..." << std::endl;
        TimeFrame loop_timeframe = this->config.strategy.timeframe;
        int loop_timeframe_minutes = get_time_frame_value(loop_timeframe);
        std::chrono::system_clock::time_point training_start_date = this->find_training_start_date();
        std::chrono::system_clock::time_point training_end_date = this->config.training.training_end_date;
        int total_iter4 = (std::chrono::duration_cast<std::chrono::minutes>(training_end_date - training_start_date).count()) / loop_timeframe_minutes;
        ProgressBar *progress_bar4 = new ProgressBar(100, total_iter4);
        this->cache_data(progress_bar4);
        std::cout << "✅ Cache ready!" << std::endl;
    }
}

/**
 * @brief Load candle data for all time frames.
 * @param progress_bar Progress bar for loading candles.
 */
void Training::load_candles(ProgressBar *progress_bar)
{
    std::vector<TimeFrame> all_timeframes = this->get_all_timeframes();
    for (int i = 0; i < all_timeframes.size(); i++)
    {
        TimeFrame tf = all_timeframes[i];
        candles[tf] = read_data(config.general.symbol, tf, config.training.training_start_date, config.training.training_end_date);
        if (progress_bar)
        {
            progress_bar->update(1);
        }
    }

    if (progress_bar)
    {
        progress_bar->complete();
    }
}

/**
 * @brief Calculate and store all indicator values to the cache.
 * @param progress_bar Progress bar for loading indicators.
 */
void Training::load_indicators(ProgressBar *progress_bar)
{
    std::__1::unordered_map<TimeFrame, std::vector<Indicator *>> all_indicators = config.training.inputs.indicators;

    for (auto const &[tf, indicators] : all_indicators)
    {
        for (auto const &tf_indicator : indicators)
        {
            bool included = false;
            for (auto const &[id, indicator] : this->indicators[tf])
            {
                if (id == tf_indicator->id)
                {
                    included = true;
                }
            }

            if (!included)
            {
                this->indicators[tf][tf_indicator->id] = tf_indicator->calculate(this->candles[tf], true);
            }

            if (progress_bar)
            {
                progress_bar->update(1);
            }
        }
    }

    for (auto const &position_indicator : this->config.training.inputs.position)
    {
        if (progress_bar)
        {
            progress_bar->update(1);
        }
    }

    if (progress_bar)
    {
        progress_bar->complete();
    }
}

/**
 * @brief Load the conversion rate when the base asset traded is different from the account currency.
 * @param progress_bar Progress bar for loading the conversion rate.
 */
void Training::load_base_currency_conversion_rate(ProgressBar *progress_bar)
{
    std::string account_currency = this->config.general.account_currency;
    std::string base_currency_traded = symbol_infos[this->config.general.symbol].base;
    TimeFrame loop_timeframe = this->config.strategy.timeframe;

    if (account_currency == base_currency_traded)
    {
        for (const auto &candle : candles[loop_timeframe])
        {
            base_currency_conversion_rate[candle.date] = 1.0;
        }
    }
    else
    {
        std::string symbol = account_currency + base_currency_traded;
        std::chrono::system_clock::time_point start_date = this->config.training.training_start_date;
        std::chrono::system_clock::time_point end_date = this->config.training.training_end_date;
        std::vector<Candle> data = read_data(symbol, loop_timeframe, start_date, end_date);

        for (const auto &candle : data)
        {
            base_currency_conversion_rate[candle.date] = candle.close;
        }
    }

    if (progress_bar)
    {
        progress_bar->update(1);
        progress_bar->complete();
    }
}

/**
 * @brief Cache all the data (candles, indicators and base currencyconversion rate values) for every datetime.
 * @param progress_bar Progress bar for caching data.
 */
void Training::cache_data(ProgressBar *progress_bar)
{
    std::vector<TimeFrame> all_timeframes = get_all_timeframes();
    TimeFrame loop_timeframe = config.strategy.timeframe;
    int loop_timeframe_minutes = get_time_frame_value(loop_timeframe);
    std::chrono::system_clock::time_point mock_date = find_training_start_date();
    Indexer *indexer = new Indexer(candles, MINIMUM_CANDLES);

    while (mock_date < config.training.training_end_date)
    {
        indexer->update_indexes(mock_date);
        CandlesData current_candles = {};
        IndicatorsData current_indicators = {};
        double current_base_currency_conversion_rate = {};

        // Get the candles for the current date
        for (const auto &tf : all_timeframes)
        {
            std::pair<int, int> index = indexer->get_indexes(tf);
            for (int i = index.first; i <= index.second; i++)
            {
                current_candles[tf].push_back(candles[tf][i]);
            }
        }

        // Get the indicators for the current date
        for (const auto &tf : all_timeframes)
        {
            std::pair<int, int> index = indexer->get_indexes(tf);
            for (const auto &indicator : config.training.inputs.indicators[tf])
            {
                std::vector<double> indicator_values = {};
                for (int i = std::max(index.second - INDICATOR_WINDOW, 0); i <= index.second; i++)
                {
                    indicator_values.push_back(indicators[tf][indicator->id][i]);
                }
                current_indicators[tf][indicator->id] = indicator_values;
            }
        }

        // Get the base currency conversion rate for the current date
        for (const auto &candle : current_candles[loop_timeframe])
        {
            if (base_currency_conversion_rate.find(candle.date) != base_currency_conversion_rate.end())
            {
                current_base_currency_conversion_rate = base_currency_conversion_rate[candle.date];
            }
        }

        // Convert the date to a string and cache the data
        time_t date_time_t = std::chrono::system_clock::to_time_t(mock_date);
        std::string date_string = std::string(std::ctime(&date_time_t));

        cache[date_string].candles = current_candles;
        cache[date_string].indicators = current_indicators;
        cache[date_string].base_currency_conversion_rate = current_base_currency_conversion_rate;

        mock_date += std::chrono::minutes(loop_timeframe_minutes);

        if (progress_bar)
        {
            progress_bar->update(1);
        }
    }

    cache_dictionary(cache, cache_file);

    if (progress_bar)
    {
        progress_bar->complete();
    };
}

/**
 * @brief Count the total number of indicators used in training.
 * @return Total number of indicators.
 */
int Training::count_indicators() const
{
    int nb_indicators = 0;

    // Count the number of indicators
    auto indicators = this->config.training.inputs.indicators;
    for (const auto &tf_indicators : indicators)
    {
        nb_indicators += tf_indicators.second.size();
    }

    return nb_indicators;
}

/**
 * @brief Get all the timeframes from the training inputs of the config.
 * @return Set of timeframes.
 */
std::vector<TimeFrame> Training::get_all_timeframes() const
{
    std::vector<TimeFrame> timeframes;
    for (const auto &indicators : this->config.training.inputs.indicators)
    {
        timeframes.push_back(indicators.first);
    }
    return timeframes;
}

/**
 * @brief Adjust the training start date based on available candles.
 * @return Adjusted training start date.
 */
std::chrono::system_clock::time_point Training::find_training_start_date() const
{
    TimeFrame loop_timeframe = this->config.strategy.timeframe;
    TimeFrame highest_timeframe = highest_time_frame(this->get_all_timeframes());

    std::__1::chrono::system_clock::time_point training_start_date = this->config.training.training_start_date;

    std::vector<Candle> candles_tf = this->candles.at(loop_timeframe);
    std::__1::chrono::system_clock::time_point candles_start_date = std::__1::chrono::system_clock::from_time_t(candles_tf[0].date);

    int highest_timeframe_value = get_time_frame_value(highest_timeframe);
    while (training_start_date < candles_start_date + std::chrono::minutes((MINIMUM_CANDLES - 1) * highest_timeframe_value))
    {
        training_start_date += std::chrono::minutes(highest_timeframe_value);
    }

    return training_start_date;
}

/**
 * @brief Update the best trader of all the training and the best trader of a generation.
 * @param generation The current generation number of the training.
 */
void Training::set_best_traders(int generation)
{
    Trader *best_trader = nullptr;
    double best_fitness = -std::numeric_limits<double>::infinity();
    double best_score = -std::numeric_limits<double>::infinity();

    for (auto &trader : traders[generation])
    {
        // if (trader->score > best_score)
        // {
        //     best_trader = trader;
        //     best_score = trader->score;
        // }
        if (trader->fitness > best_fitness)
        {
            best_trader = trader;
            best_fitness = trader->fitness;
        }
    }

    this->best_traders[generation] = best_trader;

    if (!best_trader)
    {
        std::cout << "No best trader found for generation " << generation << std::endl;
        return;
    }

    // Update the best trader of all the training
    if (!this->best_trader || best_trader->fitness > this->best_trader->fitness)
    {
        this->best_trader = best_trader;
    }
}

/**
 * @brief Get the best trader of a generation.
 * @param generation The generation number to get the best trader.
 * @return The best trader of the specified generation.
 */
Trader *Training::get_best_trader_of_generation(int generation) const
{
    return this->best_traders.at(generation);
}

/**
 * @brief Print the statistics and details of a given trader.
 * @param trader The Trader object for which to print the statistics.
 */
void Training::print_trader_stats(Trader *trader)
{
    std::cout << "========================== BEST TRADER ==========================" << std::endl;
    std::cout << "📋 Genome ID: " << trader->genome->id << std::endl;
    std::cout << "📈 Fitness"
              << ": " << trader->fitness << std::endl;
    std::cout << "📈 Score"
              << ": " << trader->score << std::endl;
    std::cout << "==================================================================" << std::endl;

    trader->print_stats_to_console();

    std::cout << "==================================================================" << std::endl;
}

/**
 * @brief Evaluate the performance of a trading algorithm for a given genome and generation.
 * @param genome The genome to be evaluated.
 * @param generation The current generation number.
 */
void Training::evaluate_genome(Genome *genome, int generation)
{
    TimeFrame loop_timeframe = this->config.strategy.timeframe;
    int loop_timeframe_minutes = get_time_frame_value(loop_timeframe);
    std::chrono::system_clock::time_point mock_date = this->find_training_start_date();

    Logger *logger = new Logger(this->directory + "/logs/trader_" + genome->id + ".log");
    Trader *trader = new Trader(genome, this->config, logger);

    while (mock_date < this->config.training.training_end_date)
    {
        // Convert the date to a string and cache the data
        time_t mock_date_time_t = std::chrono::system_clock::to_time_t(mock_date);
        std::string mock_date_string = std::string(std::ctime(&mock_date_time_t));

        if (this->cache.find(mock_date_string) != this->cache.end())
        {
            // Get the data from cache
            CandlesData current_candles = this->cache[mock_date_string].candles;
            IndicatorsData current_indicators = this->cache[mock_date_string].indicators;
            double current_base_currency_conversion_rate = this->cache[mock_date_string].base_currency_conversion_rate;
            std::vector<PositionInfo> position = this->config.training.inputs.position;

            // Update the individual
            if (!trader->dead)
            {
                trader->look(current_candles, current_indicators, current_base_currency_conversion_rate, position);
                trader->think();
                trader->update();
            }
            else
            {
                break;
            }
        }

        mock_date += std::chrono::minutes(loop_timeframe_minutes);
    }

    // Calculate fitness
    trader->calculate_fitness();
    genome->fitness = trader->fitness;

    // Add the traders to the history
    this->traders[generation].push_back(trader);

    // Close the logger
    if (this->debug && trader->logger != nullptr)
    {
        trader->logger->close();
    }
}

/**
 * @brief Run the NEAT algorithm for training.
 */
int Training::run()
{
    int nb_generations = this->config.training.generations;

    // Init the progress bar
    ProgressBar *progress_bar = new ProgressBar(100, nb_generations);

    try
    {
        auto callback_generation = [&](Population *population, int generation)
        {
            // Update the progress bar
            progress_bar->update(1);

            // Update the best traders
            this->set_best_traders(generation);

            // Print the best trader stats
            this->print_trader_stats(this->best_trader);

            // The training of generation is finished
            std::cout << "✅ Training of generation " << generation << " finished!" << std::endl;
        };

        this->population->run(std::bind(&Training::evaluate_genome, this, std::placeholders::_1, std::placeholders::_2), nb_generations, callback_generation);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // progress_bar->complete();
    std::cout << "🎉 Training finished!" << std::endl;

    return 0;
}

/**
 * @brief Report the results of the training process.
 */
void Training::report()
{
    // Save all the best genome of each generation
    std::vector<Genome *> genomes_saved = {};
    for (int i = 0; i < this->config.training.generations; i++)
    {
        Trader *best_trader = this->get_best_trader_of_generation(i);
        if (std::find(genomes_saved.begin(), genomes_saved.end(), best_trader->genome) == genomes_saved.end())
        {
            std::string genome_save_file = this->directory + "/trader_" + std::to_string(i) + "_" + best_trader->genome->id + "_genome_save.pkl";
            std::string graphic_file = this->directory + "/trader_" + std::to_string(i) + "_" + best_trader->genome->id + "_balance_history.png";
            std::string stats_file = this->directory + "/trader_" + std::to_string(i) + "_" + best_trader->genome->id + "_stats.txt";
            best_trader->genome->save(genome_save_file);
            best_trader->print_balance_history_graph(graphic_file);
            best_trader->print_stats_to_file(stats_file);
            genomes_saved.push_back(best_trader->genome);
        }
    }
}
