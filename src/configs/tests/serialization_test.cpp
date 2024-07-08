#include <gtest/gtest.h>
#include <ctime>
#include <vector>
#include <string>
#include "../../indicators/momentum.hpp"
#include "../../types.hpp"
#include "../serialization.hpp"

class ConfigSerializationTest : public ::testing::Test
{
public:
    Config test_config;

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

        Config test_config = Config{
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
    }

    void TearDown() override
    {
    }
};

TEST_F(ConfigSerializationTest, ConfigSerializationJSON)
{
    nlohmann::json json = config_to_json(test_config);
    Config config = config_from_json(json);
    ASSERT_TRUE(is_same_config(test_config, config));
};