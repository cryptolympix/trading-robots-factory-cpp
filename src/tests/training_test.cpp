#include <gtest/gtest.h>
#include <filesystem>
#include <ctime>
#include <chrono>
#include "../neat/genome.hpp"
#include "../utils/time_frame.hpp"
#include "../indicators/momentum.hpp"
#include "../symbols.hpp"
#include "../trader.hpp"
#include "../training.hpp"

class TrainingTest : public ::testing::Test
{

protected:
    Training *training;
    SymbolInfo symbol_info;
    std::filesystem::path temp_dir;
    Config config;

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
        std::chrono::system_clock::time_point start_date = std::chrono::system_clock::from_time_t(std::mktime(&start_date_tm));

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
        std::chrono::system_clock::time_point end_date = std::chrono::system_clock::from_time_t(std::mktime(&end_date_tm));

        config = {
            .general = {
                .name = "test",
                .version = "1.0",
                .symbol = "EURUSD",
                .leverage = 100,
                .initial_balance = 1000,
                .account_currency = "USD",
            },
            .strategy = {.timeframe = TimeFrame::H1, .maximum_risk = 0.02, .maximum_spread = 8, .minimum_trade_duration = 4, .maximum_trade_duration = 20, .take_profit_stop_loss_config = {
                                                                                                                                                               .type_stop_loss = TypeTakeProfitStopLoss::POINTS,
                                                                                                                                                               .stop_loss_in_points = 300,
                                                                                                                                                               .stop_loss_in_percent = 0.01,
                                                                                                                                                               .type_take_profit = TypeTakeProfitStopLoss::POINTS,
                                                                                                                                                               .take_profit_in_points = 300,
                                                                                                                                                               .take_profit_in_percent = 0.01,
                                                                                                                                                           },
                         .trading_schedule = {.monday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false}, .tuesday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false}, .wednesday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false}, .thursday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false}, .friday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false}, .saturday = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, .sunday = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}}},
            .training = {.generations = 5, .bad_trader_threshold = 0.01, .inactive_trader_threshold = 500, .training_start_date = start_date, .training_end_date = end_date, .inputs = {
                                                                                                                                                                                 .indicators = {{TimeFrame::M15, {new RSI()}}, {TimeFrame::M30, {new RSI()}}, {TimeFrame::H1, {new RSI()}}},
                                                                                                                                                                                 .position = {PositionInfo::TYPE, PositionInfo::PNL, PositionInfo::DURATION},
                                                                                                                                                                             }},
            .evaluation = {.nb_trades = 30, .maximum_drawdown = 0.1, .expected_return = 0.1, .expected_return_per_month = 0.1, .expected_return_per_trade = 0.01, .minimum_profit_factor = 2, .minimum_winrate = 0.5},
            .neat = load_config("src/configs/neat_config_test.ini"),
        };
        config.neat.population_size = 5;
        config.neat.num_inputs = 6;
        config.neat.num_outputs = 5;
        training = new Training("test", config, false);
        temp_dir = std::filesystem::temp_directory_path() / "training_test";
    }

    void TearDown() override
    {
        std::filesystem::remove_all(training->directory);
        std::filesystem::remove(training->cache_file);
        std::filesystem::remove_all(temp_dir);
        delete training;
    }
};

TEST_F(TrainingTest, LoadCandles)
{
    training->load_candles();
    ASSERT_TRUE(training->candles.find(TimeFrame::M15) != training->candles.end());
    ASSERT_TRUE(training->candles.find(TimeFrame::M30) != training->candles.end());
    ASSERT_TRUE(training->candles.find(TimeFrame::H1) != training->candles.end());
    ASSERT_FALSE(training->candles[TimeFrame::M15].empty());
    ASSERT_FALSE(training->candles[TimeFrame::M30].empty());
    ASSERT_FALSE(training->candles[TimeFrame::H1].empty());

    for (auto const &[timeframe, candles] : training->candles)
    {
        for (auto const &candle : candles)
        {
            ASSERT_GE(candle.date, std::chrono::system_clock::to_time_t(config.training.training_start_date));
            ASSERT_LE(candle.date, std::chrono::system_clock::to_time_t(config.training.training_end_date));
        }
    }
}

TEST_F(TrainingTest, LoadIndicators)
{
    training->load_candles();
    training->load_indicators();
    ASSERT_TRUE(training->indicators.find(TimeFrame::M15) != training->indicators.end());
    ASSERT_TRUE(training->indicators.find(TimeFrame::M30) != training->indicators.end());
    ASSERT_TRUE(training->indicators.find(TimeFrame::H1) != training->indicators.end());
    ASSERT_FALSE(training->indicators[TimeFrame::M15].empty());
    ASSERT_FALSE(training->indicators[TimeFrame::M30].empty());
    ASSERT_FALSE(training->indicators[TimeFrame::H1].empty());
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

    std::chrono::system_clock::time_point start_date = training->find_training_start_date();
    std::chrono::system_clock::time_point end_date = config.training.training_end_date;
    TimeFrame loop_timeframe = config.strategy.timeframe;
    int loop_timeframe_minutes = get_time_frame_value(loop_timeframe);

    for (auto date = start_date; date < end_date; date += std::chrono::minutes(loop_timeframe_minutes))
    {
        time_t date_time_t = std::chrono::system_clock::to_time_t(date);
        std::string date_string = std::string(std::ctime(&date_time_t));
        ASSERT_TRUE(training->cache.find(date_string) != training->cache.end());
    }
}

TEST_F(TrainingTest, CountIndicators)
{
    int count = training->count_indicators();
    ASSERT_EQ(count, 3);
}

TEST_F(TrainingTest, GetAllTimeframes)
{
    std::vector<TimeFrame> timeframes = training->get_all_timeframes();
    ASSERT_EQ(timeframes.size(), 3);
    ASSERT_TRUE(std::find(timeframes.begin(), timeframes.end(), TimeFrame::M15) != timeframes.end());
    ASSERT_TRUE(std::find(timeframes.begin(), timeframes.end(), TimeFrame::M30) != timeframes.end());
    ASSERT_TRUE(std::find(timeframes.begin(), timeframes.end(), TimeFrame::H1) != timeframes.end());
}

TEST_F(TrainingTest, FindTrainingStartDate)
{
    training->load_candles();
    std::chrono::system_clock::time_point start_date = training->find_training_start_date();
    ASSERT_GE(start_date, config.training.training_start_date);
}

TEST_F(TrainingTest, BestTraders)
{
    // Mock best traders
    Genome *genome = new Genome(config.neat);
    Trader *best_trader = new Trader(genome, config);
    best_trader->fitness = 10;
    best_trader->score = 10;
    Trader *bad_trader = new Trader(genome, config);
    best_trader->fitness = 5;
    best_trader->score = 5;

    // Add traders to the history
    training->traders[0] = {best_trader, bad_trader};

    // Set the best traders
    training->set_best_traders(0);

    ASSERT_EQ(training->get_best_trader_of_generation(0), best_trader);

    // Update the best traders
    Trader *new_best_trader = new Trader(genome, config);
    new_best_trader->fitness = 15;
    new_best_trader->score = 15;

    // Add a new best traders
    training->traders[0].push_back(new_best_trader);

    // Set the best traders
    training->set_best_traders(0);

    ASSERT_EQ(training->get_best_trader_of_generation(0), new_best_trader);
}

TEST_F(TrainingTest, Run)
{
    training->load_candles();
    training->load_indicators();
    training->load_base_currency_conversion_rate();
    training->cache_data();

    for (int i = 0; i < 10; ++i)
    {
        int result = training->run();
        // Asserts that the training went well
        ASSERT_EQ(result, 0);
    }
}
