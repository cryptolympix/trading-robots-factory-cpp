#include <filesystem>
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <functional>
#include <chrono>
#include <algorithm>
#include <random>
#include "types.hpp"
#include "configs/serialization.hpp"
#include "utils/logger.hpp"
#include "utils/indexer.hpp"
#include "utils/uid.hpp"
#include "utils/cache.hpp"
#include "utils/read_data.hpp"
#include "utils/time_frame.hpp"
#include "utils/date_conversion.hpp"
#include "utils/progress_bar.hpp"
#include "utils/math.hpp"
#include "utils/vectors.hpp"
#include "neat/population.hpp"
#include "neat/genome.hpp"
#include "libs/json.hpp"
#include "libs/gnuplot-iostream.hpp"
#include "indicators/utils.hpp"
#include "training.hpp"
#include "symbols.hpp"
#include "trader.hpp"

/**
 * @brief Constructor for the Training class.
 * @param id Unique identifier for the training process.
 * @param config_file_path Path to the configuration file.
 * @param debug Debug mode flag.
 */
Training::Training(std::string id, std::filesystem::path config_file_path, bool debug)
{
    this->id = id;
    this->debug = debug;

    std::cout << "🚀 Start the training process with the id '" << id << "'" << std::endl;

    if (this->debug)
    {
        std::cout << "🐞 Debug mode is enabled." << std::endl;
    }

    // Create the directory
    this->directory = "reports/" + id;
    if (!std::filesystem::exists(this->directory))
    {
        std::cout << "📂 Create the directory '" << this->directory.generic_string() << "'" << std::endl;
        std::filesystem::create_directories(this->directory);
    }

    // Set the file paths
    this->config_file_path = config_file_path;
    this->cache_file = "cache/data_" + id + ".json";
    this->fitness_report_file = this->directory.generic_string() + "/fitness_report.png";
    this->population_save_file = this->directory.generic_string() + "/population_save.json";
    this->training_save_file = this->directory.generic_string() + "/training_save.json";

    // Initialize the data structures
    this->candles = {};
    this->indicators = {};
    this->training_dates = {};
    this->test_dates = {};

    // Conversion rate when the base of asset traded is different of the account currency
    this->base_currency_conversion_rate = {};
    this->cache = new Cache(this->cache_file.generic_string());

    // History for statistics
    this->current_generation = 0;
    this->best_trader = nullptr;
    this->current_generation_traders = {};
    this->best_fitnesses = {};
    this->average_fitnesses = {};

    // Check if a training state exists
    if (std::filesystem::exists(this->training_save_file) && std::filesystem::exists(this->population_save_file))
    {
        std::cout << "⏳ Load the training progress..." << std::endl;
        this->load();
    }
    else
    {
        // Import the file of the configuration
        std::ifstream config_file(this->config_file_path.generic_string());
        if (std::filesystem::exists(config_file_path.generic_string()) && config_file.is_open())
        {
            // If it's a json file
            if (config_file_path.extension() == ".json")
            {
                // Parse the configuration file
                nlohmann::json config_json;
                config_file >> config_json;
                this->config = config_from_json(config_json);
                std::cout << "📄 Import the configuration file '" << this->config_file_path.generic_string() << "'" << std::endl;
            }
            else if (config_file_path.extension() == ".hpp" || config_file_path.extension() == ".h")
            {
                std::cerr << "Error: you need to generate the JSON configuration file from the C++ header file." << std::endl;
                std::exit(EXIT_FAILURE);
            }
            else
            {
                std::cerr << "Error: the configuration file '" << this->config_file_path.generic_string() << "' has an unsupported extension." << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }
        else
        {
            std::cerr << "Error: the configuration file '" << this->config_file_path.generic_string() << "' does not exist." << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // Set the number of inputs and outputs for the NEAT algorithm
        this->config.neat.num_inputs = this->count_indicators() + this->config.training.inputs.position.size();
        this->config.neat.num_outputs = 3; // Buy, Sell, Wait

        std::cout << "⏳ Initialize the population..." << std::endl;
        this->population = new neat::Population(this->config.neat, true);
    }
}

/**
 * @brief Destructor for the Training class to ensure proper cleanup.
 */
Training::~Training()
{
    // Delete the dynamically allocated population
    delete population;

    // Delete the dynamically allocated cache
    delete cache;

    // Delete the dynamically allocated best trader overall
    if (best_trader != nullptr)
    {
        delete best_trader;
    }
}

/**
 * @brief Prepare the training data by loading candles, calculating indicators, and caching the data.
 */
void Training::prepare()
{
    std::vector<TimeFrame> all_timeframes = this->get_all_timeframes();
    TimeFrame loop_fimeframes = this->config.strategy.timeframe;

    // Check if the loop timeframe is in the list of timeframes
    if (std::find(all_timeframes.begin(), all_timeframes.end(), loop_fimeframes) == all_timeframes.end())
    {
        std::cerr << "Error: the loop timeframe must be in the list of timeframes used for the indicators." << std::endl;
        std::exit(1);
    }

    // Check that that the training end date is after the training start date
    if (this->config.training.training_end_date <= this->config.training.training_start_date)
    {
        std::cerr << "Error: the training end date must be after the training start date." << std::endl;
        std::exit(1);
    }

    // Check that the training start date is before the test start date
    if (this->config.training.training_start_date >= this->config.training.test_start_date)
    {
        std::cerr << "Error: the training period must be before the test period." << std::endl;
        std::exit(1);
    }

    // Check that the test end date is after the test start date
    if (this->config.training.test_end_date <= this->config.training.test_start_date)
    {
        std::cerr << "Error: the test end date must be after the test start date." << std::endl;
        std::exit(1);
    }

    if (this->cache->exist())
    {
        std::cout << "⏳ Import the data from the cache..." << std::endl;
        this->cache = Cache::load(this->cache_file.generic_string());

        // Save the dates for the training and test periods
        for (const auto &[date_string, data] : this->cache->data)
        {
            time_t date = std::stol(date_string);
            if (date >= this->config.training.training_start_date && date <= this->config.training.test_end_date)
            {
                this->dates.push_back(date);
            }
            if (date >= this->config.training.training_start_date && date <= this->config.training.training_end_date)
            {
                this->training_dates.push_back(date);
            }
            if (date >= this->config.training.test_start_date && date <= this->config.training.test_end_date)
            {
                this->test_dates.push_back(date);
            }
        }

        std::cout << "✅ Cache loaded!" << std::endl;
    }
    else
    {
        // Progress bar
        std::cout << "⏳ Load the candles..." << std::endl;
        this->load_candles(true);
        std::cout << "✅ Candles loaded!" << std::endl;

        std::cout << "⏳ Load the indicators..." << std::endl;
        this->load_indicators(true);
        std::cout << "✅ Indicators loaded!" << std::endl;

        std::cout << "⏳ Load the base currency conversion rate..." << std::endl;
        this->load_base_currency_conversion_rate(true);
        std::cout << "✅ Base currency conversion rate loaded!" << std::endl;

        std::cout << "⏳ Cache the data..." << std::endl;
        this->cache_data(true);
        std::cout << "✅ Cache created!" << std::endl;
    }
}

/**
 * @brief Load candle data for all time frames.
 * @param display_progress Flag to show the progress bar. Default is false.
 */
void Training::load_candles(bool display_progress)
{
    std::vector<TimeFrame> all_timeframes = this->get_all_timeframes();
    TimeFrame highest_timeframe = highest_time_frame(all_timeframes);
    TimeFrame loop_timeframe = this->config.strategy.timeframe;
    CandlesData candles = {};

    // Load the candles from data for all the timeframes
    for (const TimeFrame &tf : all_timeframes)
    {
        time_t start_date = this->config.training.training_start_date - 7 * 24 * 60 * 60; // 7 days before the training start date
        time_t end_date = this->config.training.test_end_date;
        candles[tf] = read_data(config.general.symbol, tf, start_date, end_date);
    }

    // Save the dates for the training and test periods
    for (const auto &candle : candles[loop_timeframe])
    {
        time_t date = candle.date;
        if (date >= this->config.training.training_start_date && date <= this->config.training.test_end_date)
        {
            this->dates.push_back(date);
        }
        if (date >= this->config.training.training_start_date && date <= this->config.training.training_end_date)
        {
            this->training_dates.push_back(date);
        }
        if (date >= this->config.training.test_start_date && date <= this->config.training.test_end_date)
        {
            this->test_dates.push_back(date);
        }
    }

    // Progress bar
    ProgressBar *progress_bar = display_progress ? new ProgressBar(100, this->dates.size()) : nullptr;

    // Loop through the dates and get the candles for each timeframe
    for (const auto &date : this->dates)
    {
        CandlesData current_candles = {};
        Indexer *indexer = new Indexer(candles, CANDLES_WINDOW);
        indexer->update_indexes(date);

        // Get the candles for the current date
        for (const auto &tf : all_timeframes)
        {
            std::pair<int, int> index = indexer->get_indexes(tf);

            for (int i = index.first; i <= index.second; i++)
            {
                current_candles[tf].push_back(candles[tf][i]);
            }

            if (current_candles[tf].size() < CANDLES_WINDOW)
            {
                // Remove the date from the list
                this->dates.erase(std::remove(this->dates.begin(), this->dates.end(), date), this->dates.end());
                this->training_dates.erase(std::remove(this->training_dates.begin(), this->training_dates.end(), date), this->training_dates.end());
                this->test_dates.erase(std::remove(this->test_dates.begin(), this->test_dates.end(), date), this->test_dates.end());
            }
        }

        // If the date is in the list of dates (with at least CANDLES_WINDOW candles for each timeframe)
        if (std::find(this->dates.begin(), this->dates.end(), date) != this->dates.end())
        {
            // Save the candles for the current date
            this->candles[date] = current_candles;
        }

        // Free the indexer
        delete indexer;

        if (progress_bar)
        {
            progress_bar->update(1);
        }
    }

    if (progress_bar)
    {
        progress_bar->complete();
        delete progress_bar;
    }

    if (this->candles.empty())
    {
        std::cerr << "Error: no candles loaded for the training." << std::endl;
        std::exit(1);
    }
}

/**
 * @brief Calculate and store all indicator values to the cache.
 * @param display_progress Flag to show the progress bar. Default is false.
 */
void Training::load_indicators(bool display_progress)
{
    if (this->config.training.inputs.indicators.empty())
    {
        std::cerr << "Error: no indicators found in the configuration." << std::endl;
        return;
    }

    // Check if there are no doubloons in the indicators
    std::unordered_map<TimeFrame, std::vector<std::string>> check_indicators = {};
    for (const auto &[tf, indicators] : this->config.training.inputs.indicators)
    {
        for (const auto &indicator : indicators)
        {
            if (std::find(check_indicators[tf].begin(), check_indicators[tf].end(), indicator->id_params) != check_indicators[tf].end())
            {
                std::cerr << "Error: the indicator '" << indicator->id_params << "' is duplicated." << std::endl;
                std::exit(1);
            }
            else
            {
                check_indicators[tf].push_back(indicator->id_params);
            }
        }
    }

    std::map<TimeFrame, std::vector<Indicator *>> all_indicators = config.training.inputs.indicators;
    ProgressBar *progress_bar = display_progress ? new ProgressBar(100, this->dates.size()) : nullptr;

    // Loop through the dates
    for (const auto &date : this->dates)
    {
        this->indicators[date] = {};

        // Loop through all the indicators and calculate the values
        for (auto const &[tf, indicators] : all_indicators)
        {
            for (auto const &indicator : indicators)
            {
                // Get the candles for the current date
                std::vector<Candle> current_candles = this->candles[date][tf];

                // Calculate the indicator values
                if (current_candles.size() >= CANDLES_WINDOW)
                {
                    std::vector<double> values = indicator->calculate(current_candles, !this->debug);
                    std::vector<double> reverse_values = reverse_vector(values, indicator->values_range);

                    for (int i = 0; i < INDICATOR_WINDOW; i++)
                    {
                        if (this->config.strategy.can_open_long_trade.value_or(true) || (this->config.strategy.can_open_short_trade.value_or(true) && this->config.strategy.can_close_trade.value_or(false)))
                        {
                            this->indicators[date][tf][indicator->id].push_back(values[values.size() - INDICATOR_WINDOW + i]);
                        }
                        if (this->config.strategy.can_open_short_trade.value_or(true) || (this->config.strategy.can_open_long_trade.value_or(true) && this->config.strategy.can_close_trade.value_or(false)))
                        {
                            this->indicators[date][tf][indicator->id + "-reverse"].push_back(reverse_values[reverse_values.size() - INDICATOR_WINDOW + i]);
                        }
                    }
                }
                else
                {
                    if (this->config.strategy.can_open_long_trade.value_or(true) || (this->config.strategy.can_open_short_trade.value_or(true) && this->config.strategy.can_close_trade.value_or(false)))
                    {
                        this->indicators[date][tf][indicator->id] = std::vector<double>(INDICATOR_WINDOW, 0.0);
                    }
                    if (this->config.strategy.can_open_short_trade.value_or(true) || (this->config.strategy.can_open_long_trade.value_or(true) && this->config.strategy.can_close_trade.value_or(false)))
                    {
                        this->indicators[date][tf][indicator->id + "-reverse"] = std::vector<double>(INDICATOR_WINDOW, 0.0);
                    }
                }
            }
        }

        if (progress_bar)
        {
            progress_bar->update(1);
        }
    }

    if (progress_bar)
    {
        progress_bar->complete();
        delete progress_bar;
    }
}

/**
 * @brief Load the conversion rate when the base asset traded is different from the account currency.
 * @param display_progress Flag to show the progress bar. Default is false.
 */
void Training::load_base_currency_conversion_rate(bool display_progress)
{
    std::string account_currency = this->config.general.account_currency;
    std::string base_currency_traded = symbol_infos[this->config.general.symbol].base;
    TimeFrame loop_timeframe = this->config.strategy.timeframe;

    if (account_currency == base_currency_traded)
    {
        ProgressBar *progress_bar = display_progress ? new ProgressBar(100, this->candles.size()) : nullptr;

        for (const auto &[date, candles_data] : this->candles)
        {
            base_currency_conversion_rate[date] = 1.0;

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
    else
    {
        std::string symbol = account_currency + base_currency_traded;
        time_t start_date = this->config.training.training_start_date;
        time_t end_date = this->config.training.test_end_date;

        std::vector<Candle> data = read_data(symbol, loop_timeframe, start_date, end_date);
        ProgressBar *progress_bar = display_progress ? new ProgressBar(100, data.size()) : nullptr;

        for (const auto &candle : data)
        {
            base_currency_conversion_rate[candle.date] = candle.close;

            if (progress_bar)
            {
                progress_bar->update(1);
            }
        }

        if (progress_bar)
        {
            progress_bar->complete();
            delete progress_bar;
        }
    }
}

/**
 * @brief Cache all the data (candles, indicators and base currencyconversion rate values) for every datetime.
 * @param display_progress Flag to show the progress bar. Default is false.
 */
void Training::cache_data(bool display_progress)
{
    std::vector<TimeFrame> all_timeframes = get_all_timeframes();
    TimeFrame loop_timeframe = config.strategy.timeframe;
    int loop_timeframe_minutes = get_time_frame_in_minutes(loop_timeframe);

    ProgressBar *progress_bar = display_progress ? new ProgressBar(100, dates.size()) : nullptr;

    for (const auto &date : this->dates)
    {
        CandlesData current_candles = {};
        IndicatorsData current_indicators = {};
        double current_base_currency_conversion_rate = {};

        // Get the candles for the current date
        for (const auto &[timeframe, candles] : this->candles[date])
        {
            current_candles[timeframe] = candles;
        }

        // Get the indicators for the current date
        for (const auto &[timeframe, indicators] : this->indicators[date])
        {
            current_indicators[timeframe] = indicators;
        }

        // Get the base currency conversion rate for the current date
        for (const auto &candle : current_candles[loop_timeframe])
        {
            if (base_currency_conversion_rate.find(candle.date) != base_currency_conversion_rate.end())
            {
                current_base_currency_conversion_rate = base_currency_conversion_rate[candle.date];
            }
            else
            {
                current_base_currency_conversion_rate = 1.0;
            }
        }

        // Cache the data
        this->cache->add(std::to_string(date), CachedData{
                                                   .candles = current_candles,
                                                   .indicators = current_indicators,
                                                   .base_currency_conversion_rate = current_base_currency_conversion_rate,
                                               });

        if (progress_bar)
        {
            progress_bar->update(1);
        }
    }

    // Create the file of the cache
    this->cache->create();

    if (progress_bar)
    {
        progress_bar->complete();
        delete progress_bar;
    }
}

/**
 * @brief Count the total number of indicators used in training.
 * @return Total number of indicators.
 */
int Training::count_indicators() const
{
    int nb_indicators = 0;

    // Count the number of indicators
    for (const auto &tf_indicators : this->config.training.inputs.indicators)
    {
        nb_indicators += tf_indicators.second.size();
    }

    if (!this->config.strategy.can_open_long_trade.value_or(true) && !this->config.strategy.can_open_short_trade.value_or(true))
    {
        std::cerr << "Error: the strategy must allow to open long or short trades at least." << std::endl;
        std::exit(1);
    }

    if (this->config.strategy.can_open_long_trade.value_or(true) && this->config.strategy.can_close_trade.value_or(false))
    {
        return nb_indicators *= 2;
    }

    if (this->config.strategy.can_open_short_trade.value_or(true) && this->config.strategy.can_close_trade.value_or(false))
    {
        return nb_indicators *= 2;
    }

    if (this->config.strategy.can_open_long_trade.value_or(true) && this->config.strategy.can_open_short_trade.value_or(true))
    {
        return nb_indicators *= 2;
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
 * @brief Evaluate the performance of a trading algorithm for a given genome and generation.
 * @param genome The genome to be evaluated.
 * @param generation The current generation number.
 */
void Training::evaluate_genome(neat::Genome *genome, int generation)
{
    TimeFrame loop_timeframe = this->config.strategy.timeframe;
    int loop_timeframe_minutes = get_time_frame_in_minutes(loop_timeframe);

    Trader *trader = new Trader(genome, this->config);
    if (this->debug)
    {
        trader->logger = new Logger(this->directory.generic_string() + "/logs/training/trader_" + genome->id + ".log");
    }

    // Loop through the dates and update the trader
    for (const auto &date : this->training_dates)
    {
        std::string date_string = std::to_string(date);
        if (this->cache->has(date_string))
        {
            // Get the data from cache
            CandlesData current_candles = this->cache->get(date_string).candles;
            IndicatorsData current_indicators = this->cache->get(date_string).indicators;
            double current_base_currency_conversion_rate = this->cache->get(date_string).base_currency_conversion_rate;
            std::vector<PositionInfo> position = this->config.training.inputs.position;

            // Do not continue if the trader is dead
            if (trader->dead)
            {
                break;
            }

            // Trader in action
            trader->update(current_candles);
            if (trader->can_trade())
            {
                trader->look(current_indicators, current_base_currency_conversion_rate, position);
                trader->think();
                trader->trade();
            }
        }
    }

    // Calculate the stats of the trader
    trader->calculate_stats();

    // Calculate fitness
    trader->calculate_score();
    trader->calculate_fitness();
    genome->fitness = trader->fitness;

    // Close the logger
    if (this->debug && trader->logger != nullptr)
    {
        trader->logger->close();
    }

    // Save the trader to the current generation
    this->current_generation_traders.push_back(trader);
}

/**
 * @brief Run the NEAT algorithm for training.
 * @return The exit code of the training process. 0 if successful, 1 otherwise.
 */
int Training::run()
{
    // Number of generations to train for the current run of the algorithm (remaining generations)
    int nb_generations = this->config.training.generations - this->current_generation;

    // Init the progress bar
    ProgressBar *progress_bar = new ProgressBar(100, nb_generations);

    try
    {
        auto callback_generation = [&](neat::Population *population, int generation)
        {
            // Update the progress bar
            progress_bar->update(1);

            // Save the best fitness of the generation
            this->best_fitnesses[this->current_generation] = population->best_fitness;

            // Calculate the average fitness of the generation
            this->average_fitnesses[this->current_generation] = population->average_fitness;

            // Find the trader with the best genome of the generation
            for (const auto &trader : this->current_generation_traders)
            {
                if (trader->genome->id == population->best_genome->id)
                {
                    this->best_trader = trader;
                    break;
                }
            }

            if (this->best_trader == nullptr)
            {
                std::cerr << "Error: the genome of the best trader of the generation " << this->current_generation << " is not found." << std::endl;
                std::exit(1);
            }

            if (this->debug)
            {
                // Print the best trader stats
                std::cout << "========================== BEST TRADER ==========================" << std::endl;
                std::cout << "📋 Genome ID: " << this->best_trader->genome->id << std::endl;
                std::cout << "📈 Fitness"
                          << ": " << this->best_trader->fitness << std::endl;
                std::cout << "📈 Score"
                          << ": " << this->best_trader->score << std::endl;
                std::cout << "==================================================================" << std::endl;
                this->best_trader->print_stats_to_console();
                std::cout << "==================================================================" << std::endl;
            }

            // Save the best trader info of the generation
            std::string genome_save_file = this->directory.generic_string() + "/trader_" + std::to_string(this->current_generation) + "_" + this->best_trader->genome->id + "_genome_save.json";
            std::string graphic_file = this->directory.generic_string() + "/trader_" + std::to_string(this->current_generation) + "_" + this->best_trader->genome->id + "_training_balance_history.png";
            std::string report_file = this->directory.generic_string() + "/trader_" + std::to_string(this->current_generation) + "_" + this->best_trader->genome->id + "_training_report.html";

            std::cout << std::endl;
            this->best_trader->genome->save(genome_save_file);
            std::cout << "💾 Genome saved to '" << genome_save_file << "'" << std::endl;
            this->best_trader->generate_balance_history_graph(graphic_file);
            std::cout << "📈 Balance history graph generated at '" << graphic_file << "'" << std::endl;
            this->best_trader->generate_report(report_file, this->config.training.training_start_date, this->config.training.training_end_date);
            std::cout << "📊 Trader report generated at '" << report_file << "'" << std::endl;
            this->generate_fitness_report(this->fitness_report_file);

            if (this->debug)
            {
                std::cout << "📃 Trader log generated at '" << this->directory.generic_string() + "/logs/training/trader_" + this->best_trader->genome->id + ".log" << "'" << std::endl;
            }

            // The training of generation is finished
            std::cout << "✅ Training of generation " << this->current_generation << " finished!" << std::endl;

            // Display the fitness of the best trader
            std::cout << "🧬 Fitness of the best trader: " << this->best_trader->fitness << std::endl;
            this->best_trader->print_fitness_details_to_console();

            // Test the trader on a the testing period
            this->test(this->best_trader->genome, this->current_generation);
            std::cout << "✅ Testing of the best trader of generation " << this->current_generation << " finished!" << std::endl;

            // Update the current generation
            this->current_generation++;

            // Reset the traders of the current generation
            this->current_generation_traders.clear();

            // Save the training process
            this->save();

            if (this->best_trader->fitness >= 1)
            {
                std::cout << "🏁 The training is complete!" << std::endl;
                std::exit(0);
            }

            std::cout << std::endl;
        };

        if (nb_generations == this->config.training.generations)
        {
            std::cout << "🚀 Start the training..." << std::endl;
        }
        else if (nb_generations > 0)
        {
            std::cout << "🚀 Continue the training..." << std::endl;
        }
        else
        {
            std::cout << "🚀 The training is already finished!" << std::endl;
            return 0;
        }

        // Train the population on the training data
        this->population->run(std::bind(&Training::evaluate_genome, this, std::placeholders::_1, std::placeholders::_2), nb_generations, callback_generation);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    progress_bar->complete();
    std::cout << "🎉 Training finished!" << std::endl;

    return 0;
}

/**
 * @brief Run the strategy of the best trader in the test period.
 * @param genome The genome to be tested.
 * @param generation The generation number of the genome.
 * @return The exit code of the testing process. 0 if successful, 1 otherwise.
 */
int Training::test(neat::Genome *genome, int generation)
{
    // Create a trader with the genome
    Trader *trader = new Trader(genome, this->config);
    if (this->debug)
    {
        trader->logger = new Logger(this->directory.generic_string() + "/logs/test/trader_" + genome->id + ".log");
    }

    // Debug files
    std::ofstream decisions_file;
    std::ofstream vision_file;
    if (this->debug)
    {
        std::string decisions_file_path = this->directory.generic_string() + "/trader_" + std::to_string(generation) + "_" + trader->genome->id + "_test_decisions.csv";
        std::string vision_file_path = this->directory.generic_string() + "/trader_" + std::to_string(generation) + "_" + trader->genome->id + "_test_vision_debug.csv";
        decisions_file = std::ofstream(decisions_file_path);
        vision_file = std::ofstream(vision_file_path);
    }

    // Loop through the dates and update the trader
    for (const auto &date : this->test_dates)
    {
        std::string date_string = std::to_string(date);
        if (this->cache->has(date_string))
        {
            // Get the data from cache
            CandlesData current_candles = this->cache->get(date_string).candles;
            IndicatorsData current_indicators = this->cache->get(date_string).indicators;
            double current_base_currency_conversion_rate = this->cache->get(date_string).base_currency_conversion_rate;
            std::vector<PositionInfo> position = this->config.training.inputs.position;

            // Update the individual
            trader->update(current_candles);
            trader->look(current_indicators, current_base_currency_conversion_rate, position);
            trader->think();
            int decision = trader->trade();

            if (this->debug)
            {
                // Save the decision to the file
                std::string date_string = time_t_to_string(date);

                decisions_file << date_string << ";" << decision << std::endl;

                // Save the vision data to the file
                vision_file << date_string << ";";
                for (const auto &vision : trader->vision)
                {
                    vision_file << vision << ";";
                }
                vision_file << std::endl;
            }
        }
    }

    // Close the debug files
    if (this->debug)
    {
        decisions_file.close();
        vision_file.close();
    }

    // Calculate the stats of the trader
    trader->calculate_stats();

    // Generate the report
    std::string report_file = this->directory.generic_string() + "/trader_" + std::to_string(generation) + "_" + trader->genome->id + "_test_report.html";
    trader->generate_report(report_file, this->config.training.test_start_date, this->config.training.test_end_date);

    // Generate the balance history graph
    std::string graphic_file = this->directory.generic_string() + "/trader_" + std::to_string(generation) + "_" + trader->genome->id + "_test_balance_history.png";
    trader->generate_balance_history_graph(graphic_file);

    // Close the logger
    if (this->debug && trader->logger != nullptr)
    {
        trader->logger->close();
    }

    return 0;
}

/**
 * @brief Evaluate the strategy of a trader using the Monte Carlo simulation.
 * @param trader The trader to be evaluated.
 * @param nb_simulations The number of simulations to run. Default is 1000.
 * @param note_threshold The threshold to consider the results consistent with the training period. Default is 0.1.
 * @return The exit code of the evaluation process. 0 if the results are consistences with the training period, 1 otherwise.
 */
int Training::evaluate_trader_with_monte_carlo_simulation(Trader *trader, int nb_simulations, double note_threshold)
{
    // Number of trades to simulate in the testing period proportionally to the number of trades in the training period
    int nb_trades_to_simulate = trader->stats.total_trades * this->test_dates.size() / this->training_dates.size();

    // Monte Carlo simulation
    std::vector<double> simulation_final_balance = std::vector<double>(nb_simulations, this->config.general.initial_balance);
    for (int i = 0; i < nb_simulations; i++)
    {
        // Create a balance for the simulation
        double balance = this->config.general.initial_balance;

        // Simulate the trades
        for (int j = 0; j < nb_trades_to_simulate; j++)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<double> distribution(0, 1);
            double random = distribution(gen);
            bool winning_trade = random < trader->stats.win_rate;
            double risk_amount = balance * this->config.strategy.risk_per_trade;

            if (winning_trade)
            {
                balance += risk_amount * trader->stats.profit_factor;
            }
            else
            {
                balance -= risk_amount;
            }
        }

        simulation_final_balance[i] = balance;
    }

    // Lambda to find lower quartile
    auto find_lower_quartile = [](std::vector<double> &data) -> double
    {
        std::sort(data.begin(), data.end());
        return data[data.size() / 4];
    };

    // Lambda to find upper quartile
    auto find_upper_quartile = [](std::vector<double> &data) -> double
    {
        std::sort(data.begin(), data.end());
        return data[data.size() * 3 / 4];
    };

    std::sort(simulation_final_balance.begin(), simulation_final_balance.end());
    double median = find_median(simulation_final_balance);
    double lower_quartile = find_lower_quartile(simulation_final_balance);
    double upper_quartile = find_upper_quartile(simulation_final_balance);
    double worse_case = simulation_final_balance[0];
    double best_case = simulation_final_balance[simulation_final_balance.size() - 1];

    // Check if the results are consistent with the training period by calculate a note
    double note = 0.0;
    if (trader->stats.final_balance < worse_case)
    {
        note = 0.0;
    }
    else if (trader->stats.final_balance > best_case)
    {
        note = 1.0;
    }
    else
    {
        note = (trader->stats.final_balance - worse_case) / (best_case - worse_case);
    }

    if (this->debug)
    {
        // Print the results
        std::cout << "📊 Monte Carlo simulation results:" << std::endl;
        std::cout << "📈 Median: " << median << std::endl;
        std::cout << "📈 Lower quartile: " << lower_quartile << std::endl;
        std::cout << "📈 Upper quartile: " << upper_quartile << std::endl;
        std::cout << "📈 Worse case: " << worse_case << std::endl;
        std::cout << "📈 Best case: " << best_case << std::endl;
        std::cout << "📈 Note: " << note << std::endl;
    }

    if (note < note_threshold)
    {
        std::cout << "❌ The results are not consistent with the training period." << std::endl;
        return 1;
    }

    return 0;
}

/**
 * @brief Generate a fitness report for the training process.
 * @param file_path The file path to save the fitness report. Default is 'fitness_report.png'.
 */
void Training::generate_fitness_report(std::string file_path)
{
    // Check if there are enough data points to generate the report
    if (this->best_fitnesses.size() < 2)
    {
        return;
    }

    // Check if the directory exists
    std::filesystem::path dir = std::filesystem::path(file_path).parent_path();
    if (!std::filesystem::exists(dir))
    {
        try
        {
            std::filesystem::create_directories(dir);
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "Error creating directories: " << e.what() << std::endl;
        }
    }

    // Create a Gnuplot object
    Gnuplot gp;

    // Generate data for the fitness evolution and average fitness
    std::vector<std::pair<int, double>> best_fitness_data;
    std::vector<std::pair<int, double>> average_fitness_data;
    for (int i = 0; i < this->best_fitnesses.size(); i++)
    {
        best_fitness_data.push_back(std::make_pair(i, this->best_fitnesses[i]));
        average_fitness_data.push_back(std::make_pair(i, this->average_fitnesses[i]));
    }

    // Specify terminal type and output file
    gp << "set term png\n";
    gp << "set output '" + file_path + "'\n";

    // Set plot options
    gp << "set title 'Fitness Evolution'\n";
    gp << "set xlabel 'Generation'\n";
    gp << "set ylabel 'Fitness'\n";

    // Log scale for the y-axis
    gp << "set logscale y\n";

    // Plot data for fitness evolution and average fitness
    gp << "plot '-' with lines title 'Best fitness', '-' with lines title 'Average fitness'\n";
    gp.send1d(best_fitness_data);    // Send the fitness evolution data to Gnuplot for plotting
    gp.send1d(average_fitness_data); // Send the average fitness data to Gnuplot for plotting

    // Close output and terminate Gnuplot
    gp << "unset output\n";
    gp << "exit\n";
}

/**
 * @brief Save the training process to a file.
 */
void Training::save()
{
    // Save the population
    this->population->save(this->population_save_file);
    std::cout << "💾 Population saved to '" << this->population_save_file.generic_string() << "'" << std::endl;

    // Save the training process
    std::ofstream file(this->training_save_file);
    if (file.is_open())
    {
        nlohmann::json best_fitnesses_json;
        nlohmann::json average_fitnesses_json;
        for (int i = 0; i < this->best_fitnesses.size(); i++)
        {
            best_fitnesses_json[std::to_string(i)] = this->best_fitnesses[i];
            average_fitnesses_json[std::to_string(i)] = this->average_fitnesses[i];
        }

        nlohmann::json data = {
            {"id", this->id},
            {"config", config_to_json(this->config)},
            {"current_generation", this->current_generation},
            {"best_trader", this->best_trader->to_json()},
            {"best_fitnesses", best_fitnesses_json},
            {"average_fitnesses", average_fitnesses_json},
        };

        file << data.dump();
        file.close();
        std::cout << "💾 Training progress saved to '" << this->training_save_file.generic_string() << "'" << std::endl;
    }
    else
    {
        std::cerr << "Error: unable to save the training process to '" << this->training_save_file << "'" << std::endl;
    }
}

/**
 * @brief Load a training process from a file.
 * @return The loaded training process.
 */
void Training::load()
{
    // Check if the training process file exists
    if (!std::filesystem::exists(this->directory))
    {
        std::cerr << "Error: the training directory '" << this->directory.generic_string() << "' does not exist." << std::endl;
        std::exit(1);
    }

    if (!std::filesystem::exists(this->training_save_file))
    {
        std::cerr << "Error: the training process file '" << this->training_save_file.generic_string() << "' does not exist." << std::endl;
        std::exit(1);
    }

    // Load the training process
    std::ifstream file(this->training_save_file);
    nlohmann::json data;
    file >> data;
    file.close();

    // Load the configuration
    if (!data.contains("config"))
    {
        std::cerr << "Error: the configuration is missing in the training process file." << std::endl;
        std::exit(1);
    }

    // Load the configuration
    this->config = config_from_json(data["config"]);

    // Load the population
    this->population = neat::Population::load(this->population_save_file, this->config.neat);

    // Load the training process data
    this->current_generation = data["current_generation"];

    for (const auto &[key, value] : data["best_fitnesses"].items())
    {
        this->best_fitnesses[std::stoi(key)] = value;
    }

    for (const auto &[key, value] : data["average_fitnesses"].items())
    {
        this->average_fitnesses[std::stoi(key)] = value;
    }

    this->best_trader = Trader::from_json(data["best_trader"], this->config);
    if (this->debug)
    {
        this->best_trader->logger = new Logger(this->directory.generic_string() + "/logs/training/trader_" + this->best_trader->genome->id + ".log");
    }

    std::cout << "✅ Training loaded from '" << this->training_save_file.generic_string() << "'" << std::endl;
}