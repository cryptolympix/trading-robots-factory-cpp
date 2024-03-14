#include <filesystem>
#include <iostream>
#include "types.hpp"
#include "utils/cache.cpp"
#include "utils/indexer.hpp"
#include "utils/uid.hpp"
#include "utils/read_data.hpp"
#include "utils/time_frame.hpp"
#include "neat/population.hpp"
#include "neat/genome.hpp"
#include "training.hpp"
#include "symbols.hpp"
#include "trader.hpp"

/**
 * @brief Constructor for the Training class.
 * @param config Configuration object.
 * @param debug Debug mode flag.
 * @param cache_file Optional cache file path.
 */
Training::Training(Config &config, bool debug, const std::string &cache_file)
    : id(generate_uid(8)), config(config), debug(debug), cache_file(cache_file)
{
    // Initialize the population.
    config.neat.num_inputs = count_indicators() + config.training.inputs.position.size();
    population = new Population(config.neat);

    candles = {};
    indicators = {};

    // Conversion rate when the base of asset traded is different of the account currency
    base_currency_conversion_rate = {};
    cache = {};
    cache_exist = false;

    // History for statistics
    int generations = config.training.generations;
    traders = {};
    best_traders = {};
    best_trader = nullptr;
}

/**
 * @brief Prepare the training data by loading candles, calculating indicators, and caching the data.
 */
void Training::prepare()
{
    std::string cache_file = "cache/data/" + config.general.name + "_" + config.general.version + ".pkl";
    if (std::filesystem::exists(cache_file))
    {
        cache_exist = true;
        std::cout << "⏳ Import the data from the cache..." << std::endl;
        cache = load_cached_dictionary<DatedCache>(cache_file);
        std::cout << "✅ Cache ready!" << std::endl;
    }
    else
    {
        std::cout << "⏳ Load the candles..." << std::endl;
        load_candles();
        std::cout << "✅ Candles loaded!" << std::endl;

        std::cout << "⏳ Load the indicators..." << std::endl;
        load_indicators();
        std::cout << "✅ Indicators loaded!" << std::endl;

        std::cout << "⏳ Load the base currency conversion rate..." << std::endl;
        load_base_currency_conversion_rate();
        std::cout << "✅ Base currency conversion rate loaded!" << std::endl;

        std::cout << "⏳ Cache the data..." << std::endl;
        cache_data();
        std::cout << "✅ Cache ready!" << std::endl;
    }
}

/**
 * @brief Load candle data for all time frames.
 */
void Training::load_candles()
{
    std::vector<TimeFrame> all_timeframes = get_all_timeframes();
    for (const auto &tf : all_timeframes)
    {
        candles[tf] = read_data(config.general.symbol, tf, config.training.training_start_date, config.training.training_end_date);
    }
}

/**
 * @brief Calculate and store all indicator values to the cache.
 */
void Training::load_indicators()
{
    auto all_timeframes = get_all_timeframes();
    for (const auto &tf : all_timeframes)
    {
        for (const auto &indicator : config.training.inputs.indicators[tf])
        {
            if (indicators[tf].find(indicator.id) == indicators[tf].end())
            {
                indicators[tf][indicator.id] = indicator.calculate(candles[tf], true);
            }
        }
    }
}

/**
 * @brief Load the conversion rate when the base asset traded is different from the account currency.
 */
void Training::load_base_currency_conversion_rate()
{
    std::string account_currency = config.general.account_currency;
    std::string base_currency_traded = symbol_infos[config.general.symbol].base;
    TimeFrame loop_timeframe = config.strategy.timeframe;

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
        std::vector<Candle> data = read_data(symbol, loop_timeframe, config.training.training_start_date, config.training.training_end_date);
        for (const auto &candle : data)
        {
            base_currency_conversion_rate[candle.date] = candle.close;
        }
    }
}

/**
 * @brief Cache all the data (candles, indicators and base currencyconversion rate values) for every datetime.
 */
void Training::cache_data()
{
    std::vector<TimeFrame> all_timeframes = get_all_timeframes();
    TimeFrame loop_timeframe = config.strategy.timeframe;
    int loop_timeframe_minutes = get_time_frame_value(loop_timeframe);
    std::__1::chrono::system_clock::time_point mock_date = find_training_start_date();
    Indexer indexer(candles, MINIMUM_CANDLES);

    while (mock_date < config.training.training_end_date)
    {
        indexer.update_indexes(mock_date);
        CandlesData current_candles = {};
        IndicatorsData current_indicators = {};
        BaseCurrencyConversionRateData current_base_currency_conversion_rate = {};

        // Get the candles for the current date
        for (const auto &tf : all_timeframes)
        {
            std::pair<int, int> index = indexer.get_indexes(tf);
            for (int i = index.first; i <= index.second; i++)
            {
                current_candles[tf].push_back(candles[tf][i]);
            }
        }

        // Get the indicators for the current date
        for (const auto &tf : all_timeframes)
        {
            std::pair<int, int> index = indexer.get_indexes(tf);
            for (const auto &indicator : config.training.inputs.indicators[tf])
            {
                std::vector<double> indicator_values = {};
                for (int i = index.second - INDICATOR_WINDOW; i <= index.second; i++)
                {
                    indicator_values.push_back(indicators[tf][indicator.id][i]);
                }
                current_indicators[tf][indicator.id] = indicator_values;
            }
        }

        // Get the base currency conversion rate for the current date
        for (const auto &candle : current_candles[loop_timeframe])
        {
            if (base_currency_conversion_rate.find(candle.date) != base_currency_conversion_rate.end())
            {
                current_base_currency_conversion_rate[candle.date] = base_currency_conversion_rate[candle.date];
            }
        }

        // Convert the date to a string and cache the data
        time_t date_time_t = std::chrono::system_clock::to_time_t(mock_date);
        std::string date_string = std::string(std::ctime(&date_time_t));

        cache[date_string].candles = current_candles;
        cache[date_string].indicators = current_indicators;
        cache[date_string].base_currency_conversion_rate = current_base_currency_conversion_rate;

        mock_date += std::chrono::minutes(loop_timeframe_minutes);
    }

    cache_dictionary(cache, cache_file);
}

/**
 * @brief Count the total number of indicators used in training.
 * @return Total number of indicators.
 */
int Training::count_indicators() const
{
    int nb_indicators = 0;

    // Count the number of indicators
    auto indicators = config.training.inputs.indicators;
    for (const auto &tf_indicators : indicators)
    {
        nb_indicators += tf_indicators.second.size();
    }

    // Count the number of position indicators
    auto position_indicators = config.training.inputs.position;
    nb_indicators += position_indicators.size();

    return nb_indicators;
}

/**
 * @brief Get all the timeframes from the training inputs of the config.
 * @return Set of timeframes.
 */
std::vector<TimeFrame> Training::get_all_timeframes() const
{
    std::vector<TimeFrame> timeframes;
    for (const auto &indicators : config.training.inputs.indicators)
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
    TimeFrame loop_timeframe = config.strategy.timeframe;
    TimeFrame max_timeframe = highest_time_frame(get_all_timeframes());

    std::__1::chrono::system_clock::time_point training_start_date = config.training.training_start_date;

    std::vector<Candle> candles_tf = candles.at(loop_timeframe);
    std::__1::chrono::system_clock::time_point candles_start_date = std::__1::chrono::system_clock::from_time_t(candles_tf[0].date);

    int max_timeframe_value = get_time_frame_value(max_timeframe);
    while (training_start_date < candles_start_date + std::chrono::minutes((MINIMUM_CANDLES - 1) * max_timeframe_value))
    {
        training_start_date += std::chrono::minutes(max_timeframe_value);
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
        if (trader->score > best_score)
        {
            best_trader = trader;
            best_score = trader->score;
        }
    }

    best_traders[generation] = best_trader;

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

    trader->print_stats();

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
    Trader *trader = new Trader(genome, this->config, this->debug);

    while (mock_date < this->config.training.training_end_date)
    {
        // Convert the date to a string and cache the data
        time_t mock_date_time_t = std::chrono::system_clock::to_time_t(mock_date);
        std::string mock_date_string = std::string(std::ctime(&mock_date_time_t));

        // Get the data from cache
        CandlesData current_candles = this->cache[mock_date_string].candles;
        IndicatorsData current_indicators = this->cache[mock_date_string].indicators;
        BaseCurrencyConversionRateData current_base_currency_conversion_rate = this->cache[mock_date_string].base_currency_conversion_rate;
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

    try
    {
        auto callback_generation = [&](Population *population, int generation)
        {
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

    std::cout << "🎉 Training finished!" << std::endl;

    // Save the best genome found
    std::string directory = "cache/" + this->config.general.name + "/" + this->config.general.version + "/training_" + this->id;
    this->population->best_genome->save(directory);

    return 0;
}
