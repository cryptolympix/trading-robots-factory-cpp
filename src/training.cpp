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
#include "utils/indexer.hpp"
#include "utils/uid.hpp"
#include "utils/cache.hpp"
#include "utils/read_data.hpp"
#include "utils/time_frame.hpp"
#include "utils/date_conversion.hpp"
#include "utils/progress_bar.hpp"
#include "utils/math.hpp"
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
 */
Training::Training(std::string id, Config &config, bool debug)
    : id(id), config(config), debug(debug)
{
    this->directory = "reports/" + this->config.general.name + "/" + this->config.general.version + "/" + id;
    this->cache_file = "cache/" + this->config.general.name + "/" + this->config.general.version + "/data.json";

    // Initialize the population.
    this->config.neat.num_inputs = this->count_indicators() + this->config.training.inputs.position.size();
    this->population = new neat::Population(this->config.neat);

    this->candles = {};
    this->indicators = {};

    // Conversion rate when the base of asset traded is different of the account currency
    this->base_currency_conversion_rate = {};
    this->cache = new Cache(this->cache_file.generic_string());

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
    TimeFrame loop_timeframe = this->config.strategy.timeframe;
    CandlesData candles = {};

    // Load the candles from data for all the timeframes
    for (int i = 0; i < all_timeframes.size(); i++)
    {
        TimeFrame tf = all_timeframes[i];
        time_t start_date = this->config.training.training_start_date;
        time_t end_date = this->config.training.test_end_date;
        candles[tf] = read_data(config.general.symbol, tf, start_date, end_date);
    }

    // Get all the dates from the candles in the loop timeframe
    std::vector<time_t> dates = {};
    for (const auto &candle : candles[loop_timeframe])
    {
        dates.push_back(candle.date);
    }

    // Progress bar
    ProgressBar *progress_bar = display_progress ? new ProgressBar(100, dates.size()) : nullptr;

    // Loop through the dates and get the candles for each timeframe
    for (const auto &date : dates)
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
        }

        // Save the candles
        this->candles[date] = current_candles;

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
 * @param display_progress Flag to show the progress bar. Default is false.
 */
void Training::load_indicators(bool display_progress)
{
    if (this->config.training.inputs.indicators.empty())
    {
        std::cerr << "Error: no indicators found in the configuration." << std::endl;
        return;
    }

    // Get all the dates from the candles in the loop timeframe
    std::vector<time_t> dates = {};
    for (const auto &[date, candles_data] : this->candles)
    {
        dates.push_back(date);
    }

    std::map<TimeFrame, std::vector<Indicator *>> all_indicators = config.training.inputs.indicators;
    ProgressBar *progress_bar = display_progress ? new ProgressBar(100, dates.size()) : nullptr;

    // Loop through the dates
    for (const auto &date : dates)
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
                    for (int i = 0; i < INDICATOR_WINDOW; i++)
                    {
                        this->indicators[date][tf][indicator->id].push_back(values[values.size() - INDICATOR_WINDOW + i]);
                    }
                }
                else
                {
                    this->indicators[date][tf][indicator->id] = std::vector<double>(INDICATOR_WINDOW, 0.0);
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
    int loop_timeframe_minutes = get_time_frame_value(loop_timeframe);

    // Get all the dates from the candles in the loop timeframe
    std::vector<time_t> dates = {};
    for (const auto &[date, candles_data] : this->candles)
    {
        dates.push_back(date);
    }

    ProgressBar *progress_bar = display_progress ? new ProgressBar(100, dates.size()) : nullptr;

    for (const auto &date : dates)
    {
        CandlesData current_candles = {};
        IndicatorsData current_indicators = {};
        double current_base_currency_conversion_rate = {};

        // Get the candles for the current date
        for (const auto &[date, candles] : this->candles[date])
        {
            current_candles[date] = candles;
        }

        // Get the indicators for the current date
        for (const auto &[date, indicators] : this->indicators[date])
        {
            current_indicators[date] = indicators;
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
        this->cache->add(std::to_string(date), CachedData{.candles = current_candles, .indicators = current_indicators, .base_currency_conversion_rate = current_base_currency_conversion_rate});

        if (progress_bar)
        {
            progress_bar->update(1);
        }
    }

    // Create the file of the cache
    cache->create();

    if (progress_bar)
    {
        progress_bar->complete();
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
 * @brief Evaluate the performance of a trading algorithm for a given genome and generation.
 * @param genome The genome to be evaluated.
 * @param generation The current generation number.
 */
void Training::evaluate_genome(neat::Genome *genome, int generation)
{
    TimeFrame loop_timeframe = this->config.strategy.timeframe;
    int loop_timeframe_minutes = get_time_frame_value(loop_timeframe);

    Logger *logger = new Logger(this->directory.generic_string() + "/logs/training/trader_" + genome->id + ".log");
    Trader *trader = new Trader(genome, this->config, this->debug ? logger : nullptr);

    // Get the dates for the training from the candles in the loop timeframe
    std::vector<std::string> dates = {};
    for (const auto &[date_string, value] : this->cache->data)
    {
        time_t date = std::stoll(date_string); // Convert the string to time_t
        if (date >= this->config.training.training_start_date && date <= this->config.training.training_end_date)
        {
            dates.push_back(date_string);
        }
    }

    // Loop through the dates and update the trader
    for (const auto &date : dates)
    {
        if (this->cache->has(date))
        {
            // Get the data from cache
            CandlesData current_candles = this->cache->get(date).candles;
            IndicatorsData current_indicators = this->cache->get(date).indicators;
            double current_base_currency_conversion_rate = this->cache->get(date).base_currency_conversion_rate;
            std::vector<PositionInfo> position = this->config.training.inputs.position;

            // Update the individual
            if (!trader->dead)
            {
                trader->update(current_candles);
                trader->look(current_indicators, current_base_currency_conversion_rate, position);
                trader->think();
                trader->trade();
            }
            else
            {
                break;
            }
        }
    }

    // Calculate the stats of the trader
    trader->calculate_stats();

    // Calculate fitness
    trader->calculate_score();
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
        auto callback_generation = [&](neat::Population *population, int generation)
        {
            // Update the progress bar
            progress_bar->update(1);

            // Update the best traders
            this->set_best_traders(generation);

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
            std::string genome_save_file = this->directory.generic_string() + "/trader_" + std::to_string(generation) + "_" + best_trader->genome->id + "_genome_save.json";
            std::string graphic_file = this->directory.generic_string() + "/trader_" + std::to_string(generation) + "_" + best_trader->genome->id + "_training_balance_history.png";
            std::string report_file = this->directory.generic_string() + "/trader_" + std::to_string(generation) + "_" + best_trader->genome->id + "_training_report.html";

            std::cout << std::endl;
            best_trader->genome->save(genome_save_file);
            std::cout << "💾 Genome saved to '" << genome_save_file << "'" << std::endl;
            best_trader->generate_balance_history_graph(graphic_file);
            std::cout << "📈 Balance history graph generated!" << std::endl;
            best_trader->generate_report(report_file);
            std::cout << "📊 Trader report generated!" << std::endl;

            // The training of generation is finished
            std::cout << "✅ Training of generation " << generation << " finished!" << std::endl;

            // Test the trader on a the new period
            this->test(best_trader->genome, generation);
            std::cout << "🧪 Testing of the best trader of generation " << generation << " finished!" << std::endl;
        };

        std::cout << "🚀 Start the training..." << std::endl;

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
    Logger *logger = new Logger(this->directory.generic_string() + "/logs/test/trader_" + genome->id + ".log");
    Trader *trader = new Trader(genome, this->config, this->debug ? logger : nullptr);

    // Debug files
    std::string decisions_file_path = this->directory.generic_string() + "/trader_" + std::to_string(generation) + "_" + trader->genome->id + "_test_decisions.csv";
    std::string vision_file_path = this->directory.generic_string() + "/trader_" + std::to_string(generation) + "_" + trader->genome->id + "_test_vision_debug.csv";
    std::ofstream decisions_file = std::ofstream(decisions_file_path);
    std::ofstream vision_file = std::ofstream(vision_file_path);

    // Get the dates for the test from the candles in the loop timeframe
    std::vector<std::string> dates = {};
    for (const auto &[date_string, value] : this->cache->data)
    {
        time_t date = std::stoll(date_string); // Convert the string to time_t
        if (date >= this->config.training.test_start_date && date <= this->config.training.test_end_date)
        {
            dates.push_back(date_string);
        }
    }

    // Loop through the dates and update the trader
    for (const auto &date : dates)
    {
        if (this->cache->has(date))
        {
            // Get the data from cache
            CandlesData current_candles = this->cache->get(date).candles;
            IndicatorsData current_indicators = this->cache->get(date).indicators;
            double current_base_currency_conversion_rate = this->cache->get(date).base_currency_conversion_rate;
            std::vector<PositionInfo> position = this->config.training.inputs.position;

            // Update the individual
            if (!trader->dead)
            {
                trader->update(current_candles);
                trader->look(current_indicators, current_base_currency_conversion_rate, position);
                trader->think();
                int decision = trader->trade();

                // Save the decision to the file
                time_t time = std::stoll(date);
                std::string date_string = time_t_to_string(time);

                decisions_file << date_string << ";" << decision << std::endl;

                // Save the vision data to the file
                vision_file << date_string << ";";
                for (const auto &vision : trader->vision)
                {
                    vision_file << vision << ";";
                }
                vision_file << std::endl;
            }
            else
            {
                break;
            }
        }
    }

    // Close the debug files
    decisions_file.close();
    vision_file.close();

    // Calculate the stats of the trader
    trader->calculate_stats();

    // Generate the report
    std::string report_file = this->directory.generic_string() + "/trader_" + std::to_string(generation) + "_" + trader->genome->id + "_test_report.html";
    trader->generate_report(report_file);

    // Generate the balance history graph
    std::string graphic_file = this->directory.generic_string() + "/trader_" + std::to_string(generation) + "_" + trader->genome->id + "_test_balance_history.png";
    trader->generate_balance_history_graph(graphic_file);

    return 0;
}

/**
 * @brief Evaluate the strategy of a trader using the Monte Carlo simulation.
 * @param trader The trader to be evaluated.
 * @param nb_simulations The number of simulations to run. Default is 1000.
 * @return The exit code of the evaluation process. 0 if the results are consistences with the training period, 1 otherwise.
 */
int Training::evaluate_trader_with_monte_carlo_simulation(Trader *trader, int nb_simulations)
{
    // Get the dates for the testing period from the candles in the loop timeframe
    std::vector<std::string> training_dates = {};
    for (const auto &[date_string, value] : this->cache->data)
    {
        time_t date = std::stoll(date_string); // Convert the string to time_t
        if (date >= this->config.training.training_start_date && date <= this->config.training.training_end_date)
        {
            training_dates.push_back(date_string);
        }
    }

    // Get the dates for the test period from the candles in the loop timeframe
    std::vector<std::string> test_dates = {};
    for (const auto &[date_string, value] : this->cache->data)
    {
        time_t date = std::stoll(date_string); // Convert the string to time_t
        if (date >= this->config.training.test_start_date && date <= this->config.training.test_end_date)
        {
            test_dates.push_back(date_string);
        }
    }

    // Number of trades to simulate in the testing period proportionally to the number of trades in the training period
    int nb_trades_to_simulate = trader->stats.total_trades * test_dates.size() / training_dates.size();

    // Monte Carlo simulation
    std::vector<double> simulation_final_balance = std::vector<double>(nb_simulations, this->config.general.initial_balance);
    for (int i = 0; i < nb_simulations; i++)
    {
        // Create a new trader with the same genome
        Trader *simulation_trader = new Trader(trader->genome, this->config, nullptr);

        // Simulate the trades
        for (int j = 0; j < nb_trades_to_simulate; j++)
        {
            double random = (double)rand() / RAND_MAX;
            bool winning_trade = random < trader->stats.win_rate;

            if (winning_trade)
            {
            }
            else
            {
            }
        }
    }

    return 0;
}
