#include <gtest/gtest.h>
#include <filesystem>
#include <ctime>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "../configs/serialization.hpp"
#include "../neat/genome.hpp"
#include "../utils/time_frame.hpp"
#include "../trading/schedule.hpp"
#include "../indicators/momentum.hpp"
#include "../symbols.hpp"
#include "../trader.hpp"
#include "../training.hpp"

class TrainingTest : public ::testing::Test
{

protected:
    Training *training;
    SymbolInfo symbol_info;
    Config config;
    std::filesystem::path temp_dir;
    std::filesystem::path config_file_path;
    std::filesystem::path fitness_report_file;

    void SetUp() override
    {
        std::tm start_date_tm = {
            .tm_sec = 0,
            .tm_min = 0,
            .tm_hour = 0,
            .tm_mday = 7,
            .tm_mon = 1 - 1,
            .tm_year = 2023 - 1900,
            .tm_wday = 0,
            .tm_yday = 0,
            .tm_isdst = 0,
        };
        time_t start_date = std::mktime(&start_date_tm);

        std::tm end_date_tm = {
            .tm_sec = 0,
            .tm_min = 0,
            .tm_hour = 0,
            .tm_mday = 31,
            .tm_mon = 1 - 1,
            .tm_year = 2023 - 1900,
            .tm_wday = 0,
            .tm_yday = 0,
            .tm_isdst = 0,
        };

        time_t end_date = std::mktime(&end_date_tm);

        std::vector<bool> working_day = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false};
        std::vector<bool> rest_day = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
        config = Config{
            .general = {
                .name = "test",
                .version = "1.0",
                .symbol = "EURUSD",
                .leverage = 100,
                .initial_balance = 1000,
                .account_currency = "USD",
            },
            .strategy = {
                .timeframe = TimeFrame::M15,
                .risk_per_trade = 0.02,
                .maximum_trades_per_day = 2,
                .maximum_spread = 8,
                .minimum_trade_duration = 2,
                .maximum_trade_duration = 4,
                .minimum_duration_before_next_trade = 4,
                .take_profit_stop_loss_config = {
                    .type_stop_loss = TypeTakeProfitStopLoss::POINTS,
                    .stop_loss_in_points = 30,
                    .stop_loss_in_percent = 0.01,
                    .type_take_profit = TypeTakeProfitStopLoss::POINTS,
                    .take_profit_in_points = 30,
                    .take_profit_in_percent = 0.01,
                },
                .trading_schedule = TradingSchedule{
                    .monday = working_day,
                    .tuesday = working_day,
                    .wednesday = working_day,
                    .thursday = working_day,
                    .friday = working_day,
                    .saturday = rest_day,
                    .sunday = rest_day,
                },
            },
            .training = {
                .generations = 5,
                .bad_trader_threshold = 0.01,
                .inactive_trader_threshold = 500,
                .training_start_date = start_date,
                .training_end_date = end_date,
                .test_start_date = end_date,
                .test_end_date = end_date + 86400,
                .inputs = {
                    .indicators = {
                        {TimeFrame::M15, {new RSI()}},
                        {TimeFrame::M30, {new RSI()}},
                        {TimeFrame::H1, {new RSI()}},
                    },
                    .position = {
                        PositionInfo::TYPE,
                        PositionInfo::PNL,
                        PositionInfo::DURATION,
                    },
                },
            },
            .evaluation = {
                .maximum_trade_duration = 5,
                .maximum_drawdown = 0.1,
                .expected_return_per_day = 0.01,
                .expected_return_per_month = 0.1,
                .minimum_profit_factor = 2,
                .minimum_winrate = 0.5,
            },
            .neat = neat::load_config("src/configs/neat_config.txt"),
        };
        config.neat.population_size = 5;

        // Create the temp directory
        temp_dir = std::filesystem::temp_directory_path() / "training_test";
        std::filesystem::create_directories(temp_dir);
        config_file_path = temp_dir / "config_test.json";
        fitness_report_file = temp_dir / "fitness_report.png";

        // Create the config file for the training test
        std::ofstream config_file(this->config_file_path);
        config_file << config_to_json(config).dump(4);
        config_file.close();

        // Create the training
        training = new Training("test", config_file_path.generic_string(), false);
    }

    void TearDown() override
    {
        std::filesystem::remove_all(training->directory.parent_path().parent_path());
        std::filesystem::remove_all(temp_dir);
        std::filesystem::remove(training->cache_file);
        std::filesystem::remove(training->training_save_file);
        std::filesystem::remove(training->population_save_file);
        std::filesystem::remove(config_file_path);
        delete training;
    }
};

TEST_F(TrainingTest, CountIndicators)
{
    training->config.strategy.can_open_long_trade = true;
    training->config.strategy.can_open_short_trade = false;
    training->config.strategy.can_close_trade = false;
    int count = training->count_indicators();
    ASSERT_EQ(count, 3);

    training->config.strategy.can_open_long_trade = false;
    training->config.strategy.can_open_short_trade = true;
    training->config.strategy.can_close_trade = false;
    count = training->count_indicators();
    ASSERT_EQ(count, 3);

    training->config.strategy.can_open_long_trade = true;
    training->config.strategy.can_open_short_trade = true;
    training->config.strategy.can_close_trade = true;
    count = training->count_indicators();
    ASSERT_EQ(count, 6);
}

TEST_F(TrainingTest, LoadCandles)
{
    training->load_candles();

    ASSERT_FALSE(training->candles.empty());
    ASSERT_FALSE(training->dates.empty());
    ASSERT_FALSE(training->training_dates.empty());
    ASSERT_FALSE(training->test_dates.empty());

    for (int i = 0; i < training->dates.size(); ++i)
    {
        time_t date = training->dates[i];

        ASSERT_TRUE(training->candles.find(date) != training->candles.end());

        ASSERT_TRUE(training->candles[date].find(TimeFrame::M15) != training->candles[date].end());
        ASSERT_TRUE(training->candles[date].find(TimeFrame::M30) != training->candles[date].end());
        ASSERT_TRUE(training->candles[date].find(TimeFrame::H1) != training->candles[date].end());

        ASSERT_FALSE(training->candles[date][TimeFrame::M15].empty());
        ASSERT_FALSE(training->candles[date][TimeFrame::M30].empty());
        ASSERT_FALSE(training->candles[date][TimeFrame::H1].empty());

        ASSERT_EQ(training->candles[date][TimeFrame::M15].back().date, date);
        ASSERT_LE(training->candles[date][TimeFrame::M30].back().date, date);
        ASSERT_LE(training->candles[date][TimeFrame::H1].back().date, date);

        ASSERT_EQ(training->candles[date][TimeFrame::M15].size(), CANDLES_WINDOW);
        ASSERT_EQ(training->candles[date][TimeFrame::M30].size(), CANDLES_WINDOW);
        ASSERT_EQ(training->candles[date][TimeFrame::H1].size(), CANDLES_WINDOW);
    }
}

TEST_F(TrainingTest, LoadIndicators)
{
    training->load_candles();
    training->load_indicators();
    int nb_indicators = training->config.training.inputs.indicators.size();

    ASSERT_FALSE(training->indicators.empty());

    for (const auto &[date, indicators_data] : training->indicators)
    {
        ASSERT_FALSE(indicators_data.empty());
        for (const auto &[tf, data] : indicators_data)
        {
            for (const auto &[id, values] : data)
            {
                ASSERT_EQ(values.size(), INDICATOR_WINDOW);
            }
        }

        ASSERT_TRUE(training->indicators[date].find(TimeFrame::M15) != training->indicators[date].end());
        ASSERT_TRUE(training->indicators[date].find(TimeFrame::M30) != training->indicators[date].end());
        ASSERT_TRUE(training->indicators[date].find(TimeFrame::H1) != training->indicators[date].end());
        ASSERT_EQ(training->indicators[date][TimeFrame::M15].size(), 2);
        ASSERT_EQ(training->indicators[date][TimeFrame::M30].size(), 2);
        ASSERT_EQ(training->indicators[date][TimeFrame::H1].size(), 2);
    }
}

TEST_F(TrainingTest, LoadBaseCurrencyConversionRate)
{
    training->load_candles();
    training->load_base_currency_conversion_rate();
    ASSERT_FALSE(training->base_currency_conversion_rate.empty());
}

TEST_F(TrainingTest, CacheData)
{
    training->load_candles();
    training->load_indicators();
    training->load_base_currency_conversion_rate();
    training->cache_data();

    TimeFrame loop_timeframe = config.strategy.timeframe;
    int loop_timeframe_minutes = get_time_frame_in_minutes(loop_timeframe);
    int nb_dates = training->candles.size();

    // Check the dates is in the cache
    ASSERT_EQ(training->cache->data.size(), nb_dates);
    ASSERT_EQ(training->cache->data.size(), training->dates.size());

    // Get the dates from the candles of loop_timeframe
    std::vector<time_t> dates;
    for (const auto &[date, candles_data] : training->candles)
    {
        dates.push_back(date);
    }

    // Check the dates in the cache
    for (const auto &date : dates)
    {
        std::string date_string = std::to_string(date);
        ASSERT_TRUE(training->cache->has(date_string));
    }

    for (const auto &date : dates)
    {
        std::string date_string = std::to_string(date);

        for (const auto &[timeframe, candles] : training->cache->get(date_string).candles)
        {
            // Check the candles are not empty
            ASSERT_FALSE(candles.empty());

            // Check the last candle date
            if (timeframe == loop_timeframe)
            {
                ASSERT_EQ(training->cache->get(date_string).candles[timeframe].back().date, date);
            }
            else
            {
                ASSERT_LE(training->cache->get(date_string).candles[timeframe].back().date, date);
            }

            // Check the the candle dates are well ordered
            for (int j = 0; j < candles.size() - 1; ++j)
            {
                ASSERT_LT(candles[j].date, candles[j + 1].date);
            }
        }

        // Check the the indicators are in the cache
        for (const auto &[timeframe, indicators_data] : training->cache->get(date_string).indicators)
        {
            for (const auto &[id, data] : indicators_data)
            {
                ASSERT_EQ(data.size(), INDICATOR_WINDOW);
            }
        }

        // Check the the base currency conversion rate is in the cache
        ASSERT_GT(training->cache->get(date_string).base_currency_conversion_rate, 0);
    }
}

TEST_F(TrainingTest, GetAllTimeframes)
{
    std::vector<TimeFrame> timeframes = training->get_all_timeframes();
    ASSERT_EQ(timeframes.size(), 3);
    ASSERT_TRUE(std::find(timeframes.begin(), timeframes.end(), TimeFrame::M15) != timeframes.end());
    ASSERT_TRUE(std::find(timeframes.begin(), timeframes.end(), TimeFrame::M30) != timeframes.end());
    ASSERT_TRUE(std::find(timeframes.begin(), timeframes.end(), TimeFrame::H1) != timeframes.end());
}

TEST_F(TrainingTest, Run)
{
    for (int i = 0; i < 10; ++i)
    {
        Training *training = new Training("test", config_file_path, false);
        training->prepare();

        // Run the training
        int result = training->run();

        // Asserts that the training went well
        ASSERT_EQ(result, 0);
        ASSERT_NE(training->population->best_genome, nullptr);
        ASSERT_EQ(training->best_fitnesses.size(), training->config.training.generations);
        ASSERT_EQ(training->average_fitnesses.size(), training->config.training.generations);
        ASSERT_NE(training->best_trader, nullptr);
        ASSERT_EQ(training->best_trader->fitness, training->population->best_fitness);
        ASSERT_LE(training->best_trader->fitness, 1);
        ASSERT_EQ(training->current_generation, training->config.training.generations);

        for (int i = 0; i < training->config.training.generations; ++i)
        {
            ASSERT_GT(training->best_fitnesses[i], 0);
            ASSERT_GT(training->average_fitnesses[i], 0);
        }

        // Add more generations
        training->config.training.generations = config.training.generations * 2;
        training->save(); // Update the training state in the file

        // Continue the training
        Training *training2 = new Training(training->id, training->config_file_path, false);

        ASSERT_EQ(training2->id, training->id);
        ASSERT_EQ(training2->current_generation, training->current_generation);
        ASSERT_EQ(training2->best_fitnesses, training->best_fitnesses);
        ASSERT_EQ(training2->average_fitnesses, training->average_fitnesses);
        ASSERT_EQ(training2->population->genomes.size(), training->population->genomes.size());

        training2->prepare();
        training2->run();

        ASSERT_EQ(training2->current_generation, training->config.training.generations);
        ASSERT_EQ(training2->population->genomes.size(), training->population->genomes.size());
        ASSERT_EQ(training2->best_fitnesses.size(), training->config.training.generations);
        ASSERT_EQ(training2->average_fitnesses.size(), training->config.training.generations);

        // Clean up the files
        std::filesystem::remove_all(training->directory);
    }
}

TEST_F(TrainingTest, MonteCarloSimulation)
{
    // Mock the trader
    neat::Genome *genome = new neat::Genome();
    Trader *trader = new Trader(genome, config);
    trader->stats.win_rate = 0.5;
    trader->stats.profit_factor = 2;
    trader->stats.total_trades = 20;
    trader->stats.final_balance = config.general.initial_balance;

    training->load_candles();
    training->load_indicators();
    training->load_base_currency_conversion_rate();
    training->cache_data();

    int result = training->evaluate_trader_with_monte_carlo_simulation(trader);

    // Asserts that the evaluation went well
    ASSERT_EQ(result, 0);
}

TEST_F(TrainingTest, GenerateFitnessReport)
{
    for (int i = 0; i < 10; ++i)
    {
        training->best_fitnesses[i] = i;
        training->average_fitnesses[i] = i / 2;
        training->generate_fitness_report(fitness_report_file);
    }

    ASSERT_TRUE(std::filesystem::exists(fitness_report_file));
}
