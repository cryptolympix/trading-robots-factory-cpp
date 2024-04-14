#include <gtest/gtest.h>
#include <filesystem>
#include <iostream>
#include <ctime>
#include "../utils/time_frame.hpp"
#include "../neat/config.hpp"
#include "../neat/genome.hpp"
#include "../indicators/momentum.hpp"
#include "../trading/trading_tools.hpp"
#include "../symbols.hpp"
#include "../types.hpp"
#include "../trader.hpp"

class TraderTest : public ::testing::Test
{
protected:
    std::filesystem::path temp_dir;
    Trader *trader;
    SymbolInfo symbol_info;
    Config config;
    time_t date;

    void SetUp() override
    {
        // Mock configurations
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
                .timeframe = TimeFrame::H1,
                .maximum_risk = 0.02,
                .maximum_spread = 8,
                .minimum_trade_duration = 2,
                .maximum_trade_duration = 4,
                .minimum_duration_before_next_trade = 4,
                .maximum_trades_per_day = 2,
                .take_profit_stop_loss_config = {
                    .type_stop_loss = TypeTakeProfitStopLoss::POINTS,
                    .stop_loss_in_points = 300,
                    .stop_loss_in_percent = 0.01,
                    .type_take_profit = TypeTakeProfitStopLoss::POINTS,
                    .take_profit_in_points = 300,
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
                .nb_trades_per_day = 2,
                .maximum_drawdown = 0.1,
                .expected_return_per_day = 0.01,
                .expected_return_per_month = 0.1,
                .minimum_profit_factor = 2,
                .minimum_winrate = 0.5,
            },
            .neat = load_config("src/configs/neat_config_test.ini"),
        };
        config.neat.population_size = 5;
        config.neat.num_inputs = 4;

        symbol_info = symbol_infos.at(config.general.symbol);
        temp_dir = std::filesystem::temp_directory_path() / "trader_test";

        // 2023-01-05 12:00:00
        std::tm date_tm = {
            .tm_year = 2023 - 1900,
            .tm_mon = 0,
            .tm_mday = 5,
            .tm_hour = 12,
            .tm_min = 0,
            .tm_sec = 0};
        date = std::mktime(&date_tm);

        // Trader configurations
        trader = new Trader(new Genome(config.neat), config, nullptr);
        trader->balance = config.general.initial_balance;
        trader->current_position = nullptr;
        trader->open_orders = {};
        trader->balance_history = {};
        trader->trades_history = {};
        trader->decisions = {0, 0, 1};
        trader->current_date = date;
        trader->candles = {{TimeFrame::H1, {
                                               Candle{.date = date, .close = 1.0},
                                           }}};
        trader->current_base_currency_conversion_rate = 1;
        trader->stats = {
            .initial_balance = config.general.initial_balance,
            .final_balance = config.general.initial_balance,
            .total_net_profit = 0,
            .total_profit = 0,
            .total_loss = 0,
            .total_fees = 0,
            .total_trades = 0,
            .total_long_trades = 0,
            .total_short_trades = 0,
            .total_winning_trades = 0,
            .total_winning_long_trades = 0,
            .total_winning_short_trades = 0,
            .total_lost_trades = 0,
            .total_lost_long_trades = 0,
            .total_lost_short_trades = 0,
            .max_consecutive_winning_trades = 0,
            .max_consecutive_lost_trades = 0,
            .profit_factor = 0,
            .max_drawdown = 0,
            .win_rate = 0,
            .long_win_rate = 0,
            .short_win_rate = 0,
            .average_profit = 0,
            .average_loss = 0,
            .max_profit = 0,
            .max_loss = 0,
            .max_consecutive_profit = 0,
            .max_consecutive_loss = 0,
            .average_trade_duration = 0,
            .sharpe_ratio = 0,
            .sortino_ratio = 0,
        };
    }

    void TearDown() override
    {
        delete trader;
        if (std::filesystem::exists(temp_dir))
        {
            std::filesystem::remove_all(temp_dir);
        }
    }
};

TEST_F(TraderTest, UpdateWithNoPositionAndNoOrders)
{
    // Call the update method
    trader->update(date);

    // Check if balance and stats are updated correctly
    ASSERT_EQ(trader->balance, 1000.0);
    ASSERT_EQ(trader->stats.total_profit, 0.0);
    ASSERT_EQ(trader->stats.total_winning_trades, 0);
    ASSERT_EQ(trader->stats.total_fees, 0.0);
    ASSERT_EQ(trader->stats.max_drawdown, 0.0);
    ASSERT_EQ(trader->balance_history.size(), 1);
    ASSERT_EQ(trader->trades_history.size(), 0);
}

TEST_F(TraderTest, UpdateWithPosition)
{
    std::tm date_tm = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 5,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0};

    trader->open_position_by_market(1.00000, 1.0, OrderSide::LONG);
    trader->balance = config.general.initial_balance; // Reset balance to initial value to cancel the fees at the market order

    // Call the update method
    trader->update(date);

    // Check if balance and stats are updated correctly
    ASSERT_EQ(trader->balance, 1000.0);
    ASSERT_EQ(trader->duration_in_position, 1);
    ASSERT_EQ(trader->stats.total_profit, 0);
    ASSERT_EQ(trader->stats.total_winning_trades, 0);
    ASSERT_EQ(trader->stats.max_drawdown, 0.0);
}

TEST_F(TraderTest, UpdateWithOpenOrders)
{
    // Mock data for testing
    trader->open_orders = {
        {.type = OrderType::TAKE_PROFIT, .side = OrderSide::SHORT, .price = 105.0}};

    // Call the update method
    trader->update(date);

    // Check if balance and stats are updated correctly
    ASSERT_EQ(trader->balance, 1000.0);
    ASSERT_EQ(trader->stats.total_profit, 0.0);
    ASSERT_EQ(trader->stats.total_winning_trades, 0);
    ASSERT_EQ(trader->stats.max_drawdown, 0.0);
    ASSERT_EQ(trader->balance_history.size(), 1);
}

TEST_F(TraderTest, UpdateWithPositionLiquidation)
{
    // Simulate a long position
    trader->open_position_by_market(100.0, 0.01, OrderSide::LONG);

    // Simulate liquidation condition
    trader->candles[TimeFrame::H1][0].close = 99.0;

    // Call the update method
    trader->update(date);

    // Check if the position is closed and open orders are cleared
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->balance, 0.0);
    ASSERT_EQ(trader->trades_history.size(), 1);
    ASSERT_EQ(trader->trades_history[0].entry_date, date);
    ASSERT_EQ(trader->trades_history[0].exit_date, date);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_LT(trader->trades_history[0].pnl, 0.0);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 0.01);
    ASSERT_TRUE(trader->trades_history[0].closed);
}

TEST_F(TraderTest, UpdateWithInactiveTrader)
{
    // Mock data for testing
    trader->lifespan = config.training.inactive_trader_threshold.value();

    // Call the update method
    trader->update(date);

    // Check if the position is closed and open orders are cleared
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->trades_history.size(), 0);
    ASSERT_EQ(trader->lifespan, config.training.inactive_trader_threshold.value() + 1);
    ASSERT_TRUE(trader->dead);
}

TEST_F(TraderTest, UpdateWithBadTrader)
{
    // Mock data for testing
    trader->balance = config.training.bad_trader_threshold.value() * config.general.initial_balance;

    // Call the update method
    trader->update(date);

    // Check if the trader is marked as dead
    ASSERT_TRUE(trader->dead);
}

TEST_F(TraderTest, CheckTpOrderHit)
{
    // Mock data
    trader->candles = {{TimeFrame::H1, {
                                           Candle{.close = 1.00500, .high = 1.00600, .low = 0.99900, .spread = 2},
                                       }}};

    // Simulate a long position
    trader->open_position_by_market(1.00000, 1.0, OrderSide::LONG);

    // Reset balance to initial value to cancel the fees at the market order
    trader->balance = config.general.initial_balance;

    trader->open_orders = {
        {.side = OrderSide::SHORT, .type = OrderType::TAKE_PROFIT, .price = 1.00500}};

    trader->current_position->pnl = calculate_profit_loss(1.00500, *trader->current_position, trader->symbol_info, trader->current_base_currency_conversion_rate);

    // Calculation of balance after the execution of the order
    double new_balance = config.general.initial_balance + trader->current_position->pnl - trader->symbol_info.commission_per_lot * trader->current_position->size * trader->current_base_currency_conversion_rate;

    // Save the pnl before the execution of the order
    double pnl = trader->current_position->pnl;

    // Call the update method
    trader->check_open_orders();

    // Check that order has been activated
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->balance, new_balance);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->trades_history.size(), 1);
    ASSERT_EQ(trader->trades_history[0].entry_date, date);
    ASSERT_EQ(trader->trades_history[0].exit_date, date);
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].exit_price, 1.00500);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_EQ(trader->trades_history[0].pnl, pnl);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
    ASSERT_TRUE(trader->trades_history[0].closed);
}

TEST_F(TraderTest, CheckSlOrderHit)
{
    // Mock data
    trader->candles = {{TimeFrame::H1, {
                                           Candle{.close = 0.99400, .high = 1.00500, .low = 0.99300, .spread = 2},
                                       }}};

    // Simulate a long position
    trader->open_position_by_market(1.00000, 1.0, OrderSide::LONG);

    // Reset balance to initial value to cancel the fees at the market order
    trader->balance = config.general.initial_balance;

    trader->open_orders = {
        {.side = OrderSide::SHORT, .type = OrderType::STOP_LOSS, .price = 0.99500}};

    trader->current_position->pnl = calculate_profit_loss(0.99500, *trader->current_position, trader->symbol_info, trader->current_base_currency_conversion_rate);

    // Calculation of the balance after the execution of the order
    double new_balance = config.general.initial_balance + trader->current_position->pnl - trader->symbol_info.commission_per_lot * trader->current_position->size * trader->current_base_currency_conversion_rate;

    // Save the pnl before the execution of the order
    double pnl = trader->current_position->pnl;

    // Call the update method
    trader->check_open_orders();

    // Check that order has been activated
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->balance, new_balance);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->trades_history.size(), 1);
    ASSERT_EQ(trader->trades_history[0].entry_date, date);
    ASSERT_EQ(trader->trades_history[0].exit_date, date);
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].exit_price, 0.99500);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_EQ(trader->trades_history[0].pnl, pnl);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
    ASSERT_TRUE(trader->trades_history[0].closed);
}

TEST_F(TraderTest, TradeCloseLongWithProfit)
{
    // Mock data for testing
    double pnl = 50.0;
    trader->open_position_by_market(1.00000, 1.0, OrderSide::LONG);
    trader->current_position->pnl = pnl;

    // Reset balance to initial value to cancel the fees at the market order
    trader->balance = config.general.initial_balance;

    // Calculation of the new balance after liquidation
    double new_balance = config.general.initial_balance + trader->current_position->pnl - trader->symbol_info.commission_per_lot * trader->current_position->size * trader->current_base_currency_conversion_rate;

    // Call the method
    trader->close_position_by_market();

    // Check if the position is closed, open orders are cleared, and balance is updated
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->balance, new_balance);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->trades_history.size(), 1);
    ASSERT_EQ(trader->trades_history[0].entry_date, date);
    ASSERT_EQ(trader->trades_history[0].exit_date, date);
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].exit_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_EQ(trader->trades_history[0].pnl, pnl);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
    ASSERT_TRUE(trader->trades_history[0].closed);
}

TEST_F(TraderTest, TradeCloseLongWithLoss)
{
    // Mock data for testing
    double pnl = -50.0;
    trader->open_position_by_market(1.00000, 1.0, OrderSide::LONG);
    trader->current_position->pnl = pnl;

    // Reset balance to initial value to cancel the fees at the market order
    trader->balance = config.general.initial_balance;

    // Calculation of the new balance after liquidation
    double new_balance = config.general.initial_balance + trader->current_position->pnl - trader->symbol_info.commission_per_lot * trader->current_position->size * trader->current_base_currency_conversion_rate;

    // Call the method
    trader->close_position_by_market();

    // Check if the position is closed, open orders are cleared, and balance is updated
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->balance, new_balance);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->trades_history.size(), 1);
    ASSERT_EQ(trader->trades_history[0].entry_date, date);
    ASSERT_EQ(trader->trades_history[0].exit_date, date);
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].exit_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_EQ(trader->trades_history[0].pnl, pnl);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
    ASSERT_TRUE(trader->trades_history[0].closed);
}

TEST_F(TraderTest, TradeCloseShortWithProfit)
{
    // Mock data for testing
    double pnl = 50.0;
    trader->open_position_by_market(1.00000, 1.0, OrderSide::SHORT);
    trader->current_position->pnl = pnl;

    // Reset balance to initial value to cancel the fees at the market order
    trader->balance = config.general.initial_balance;

    // Calculation of the new balance after liquidation
    double new_balance = config.general.initial_balance + trader->current_position->pnl - trader->symbol_info.commission_per_lot * trader->current_position->size * trader->current_base_currency_conversion_rate;

    // Call the method
    trader->close_position_by_market();

    // Check if the position is closed, open orders are cleared, and balance is updated
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->balance, new_balance);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->trades_history.size(), 1);
    ASSERT_EQ(trader->trades_history[0].entry_date, date);
    ASSERT_EQ(trader->trades_history[0].exit_date, date);
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].exit_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::SHORT);
    ASSERT_EQ(trader->trades_history[0].pnl, pnl);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
    ASSERT_TRUE(trader->trades_history[0].closed);
}

TEST_F(TraderTest, TradeCloseShortWithLoss)
{
    // Mock data for testing
    double pnl = -50.0;
    trader->open_position_by_market(1.00000, 1.0, OrderSide::SHORT);
    trader->current_position->pnl = pnl;

    // Reset balance to initial value to cancel the fees at the market order
    trader->balance = config.general.initial_balance;

    // Calculation of the new balance after liquidation
    double new_balance = config.general.initial_balance + trader->current_position->pnl - trader->symbol_info.commission_per_lot * trader->current_position->size * trader->current_base_currency_conversion_rate;

    // Call the method
    trader->close_position_by_market();

    // Check if the position is closed, open orders are cleared, and balance is updated
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->balance, new_balance);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->trades_history.size(), 1);
    ASSERT_EQ(trader->trades_history[0].entry_date, date);
    ASSERT_EQ(trader->trades_history[0].exit_date, date);
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].exit_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::SHORT);
    ASSERT_EQ(trader->trades_history[0].pnl, pnl);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
    ASSERT_TRUE(trader->trades_history[0].closed);
}

TEST_F(TraderTest, TradeEnterLong)
{
    // Set neural network output to enter long
    trader->decisions = {1.0, 0.0, 0.0};
    trader->current_date = date;

    // Call the trade method
    trader->trade();

    // Check if a new long position is opened
    ASSERT_NE(trader->current_position, nullptr);
    ASSERT_EQ(trader->current_position->side, PositionSide::LONG);
    ASSERT_GE(trader->current_position->size, 0);
    ASSERT_EQ(trader->current_position->entry_date, date);
    ASSERT_EQ(trader->duration_in_position, 0);
    ASSERT_EQ(trader->open_orders.size(), 2);
    ASSERT_LE(trader->balance, 1000.0);
    ASSERT_EQ(trader->trades_history.size(), 1);
    ASSERT_EQ(trader->trades_history[0].entry_date, date);
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, trader->current_position->size);
    ASSERT_FALSE(trader->trades_history[0].closed);
}

TEST_F(TraderTest, TradeEnterShort)
{
    // Set neural network output to enter short
    trader->decisions = {0.0, 1.0, 0.0};

    // Call the trade method
    trader->trade();

    // Check if a new short position is opened
    ASSERT_NE(trader->current_position, nullptr);
    ASSERT_EQ(trader->current_position->side, PositionSide::SHORT);
    ASSERT_GE(trader->current_position->size, 0);
    ASSERT_EQ(trader->current_position->entry_date, date);
    ASSERT_EQ(trader->duration_in_position, 0);
    ASSERT_EQ(trader->open_orders.size(), 2);
    ASSERT_LE(trader->balance, 1000.0);
    ASSERT_EQ(trader->trades_history.size(), 1);
    ASSERT_EQ(trader->trades_history[0].entry_date, date);
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::SHORT);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, trader->current_position->size);
    ASSERT_FALSE(trader->trades_history[0].closed);
}

TEST_F(TraderTest, TradeNoAction)
{
    // Set neural network output to no action
    trader->decisions = {0.0, 0.0, 1.0};

    // Call the trade method
    trader->trade();

    // Check if no new position is opened
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->balance, 1000.0);
}

TEST_F(TraderTest, ClosePositionForDurationExceeded)
{
    // Simulate a long position
    trader->open_position_by_market(1.01, 1.0, OrderSide::LONG);
    trader->update_position_pnl();

    // Set the duration in position to the maximum trade duration
    trader->duration_in_position = config.strategy.maximum_trade_duration.value() - 1;

    // Call the update method
    trader->update(date + get_time_frame_value(config.strategy.timeframe) * 60);

    // Assertions
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_LT(trader->balance, 1000.0); // Balance decreased due to fees
    ASSERT_EQ(trader->trades_history.size(), 1);
    ASSERT_EQ(trader->trades_history[0].entry_date, date);
    ASSERT_EQ(trader->trades_history[0].exit_date, date + get_time_frame_value(config.strategy.timeframe) * 60);
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.01);
    ASSERT_EQ(trader->trades_history[0].exit_price, 1.00);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_NE(trader->trades_history[0].pnl, 0.0);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
    ASSERT_LE(trader->trades_history[0].duration, config.strategy.maximum_trade_duration.value());
    ASSERT_TRUE(trader->trades_history[0].closed);
}

TEST_F(TraderTest, WaitForDurationBeforeClosePosition)
{
    trader->open_position_by_market(1.00, 1.0, OrderSide::LONG);
    trader->decisions = {0.0, 1.0, 0.0};

    // Call the update method for the maximum trade duration - 1
    for (int i = 0; i < config.strategy.minimum_trade_duration.value() - 1; ++i)
    {
        time_t new_date = date + i * get_time_frame_value(config.strategy.timeframe) * 60;
        trader->update(new_date);
    }

    // Assertions
    ASSERT_NE(trader->current_position, nullptr);
    ASSERT_EQ(trader->duration_in_position, config.strategy.minimum_trade_duration.value() - 1);

    // Call the update method for the last trade duration
    time_t last_date = date + config.strategy.minimum_trade_duration.value() * get_time_frame_value(config.strategy.timeframe) * 60;
    trader->update(last_date);

    // Assertions
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_LT(trader->balance, 1000.0); // Balance decreased due to fees
    ASSERT_EQ(trader->trades_history.size(), 1);
    ASSERT_EQ(trader->trades_history[0].entry_date, date);
    ASSERT_EQ(trader->trades_history[0].exit_date, last_date);
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.00);
    ASSERT_EQ(trader->trades_history[0].exit_price, 1.00);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
    ASSERT_EQ(trader->trades_history[0].duration, config.strategy.minimum_trade_duration.value());
    ASSERT_TRUE(trader->trades_history[0].closed);
}

TEST_F(TraderTest, WaitForNextTrade)
{
    trader->decisions = {1.0, 0.0, 0.0};
    trader->duration_without_trade = 0;

    // Cann the trade method
    trader->trade();

    ASSERT_FALSE(trader->can_trade());
    ASSERT_EQ(trader->current_position, nullptr);

    // Set trader's date after the minimum duration before next trade has passed
    trader->duration_without_trade = config.strategy.minimum_duration_before_next_trade.value();

    // Cann the trade method
    trader->trade();

    ASSERT_TRUE(trader->can_trade());
    ASSERT_NE(trader->current_position, nullptr);
}

TEST_F(TraderTest, CreateTpSlForLongPosition)
{
    // Set neural network output to no action
    trader->decisions = {1.0, 0.0, 0.0};

    // Call the trade method
    trader->trade();

    // Check the tp and sl limit order
    ASSERT_EQ(trader->open_orders.size(), 2);
    ASSERT_EQ(trader->open_orders[0].side, OrderSide::SHORT);
    ASSERT_EQ(trader->open_orders[0].type, OrderType::TAKE_PROFIT);
    ASSERT_EQ(trader->open_orders[0].price, 1.03000);
    ASSERT_EQ(trader->open_orders[1].side, OrderSide::SHORT);
    ASSERT_EQ(trader->open_orders[1].type, OrderType::STOP_LOSS);
    ASSERT_EQ(trader->open_orders[1].price, 0.97000);
}

TEST_F(TraderTest, CreateTpSlForShortPosition)
{
    // Set neural network output to no action
    trader->decisions = {0.0, 1.0, 0.0};

    // Call the trade method
    trader->trade();

    // Check the tp and sl limit order
    ASSERT_EQ(trader->open_orders.size(), 2);
    ASSERT_EQ(trader->open_orders[0].side, OrderSide::LONG);
    ASSERT_EQ(trader->open_orders[0].type, OrderType::TAKE_PROFIT);
    ASSERT_EQ(trader->open_orders[0].price, 0.97000);
    ASSERT_EQ(trader->open_orders[1].side, OrderSide::LONG);
    ASSERT_EQ(trader->open_orders[1].type, OrderType::STOP_LOSS);
    ASSERT_EQ(trader->open_orders[1].price, 1.03000);
}

TEST_F(TraderTest, TradeNotOutOfTradingSchedule)
{
    // Set neural network output to enter long
    trader->decisions = {1.0, 0.0, 0.0};

    // Try to trade sunday
    std::tm date_tm_1 = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0};
    time_t date1 = std::mktime(&date_tm_1);
    trader->current_date = date1;

    // Call the trade method
    trader->trade();

    // Check if trader can trade
    ASSERT_FALSE(trader->can_trade());
    ASSERT_EQ(trader->current_position, nullptr);

    // Try to trade just before the opening of trading schedule
    std::tm date_tm_2 = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 2,
        .tm_hour = 6,
        .tm_min = 0,
        .tm_sec = 0};
    time_t date2 = std::mktime(&date_tm_2);
    trader->current_date = date2;

    // Call the trade method
    trader->trade();

    // Check if trader can trade
    ASSERT_FALSE(trader->can_trade());
    ASSERT_EQ(trader->current_position, nullptr);

    // Try to trade just after the closing of trading schedule
    std::tm date_tm_3 = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 2,
        .tm_hour = 21,
        .tm_min = 0,
        .tm_sec = 0};
    time_t date3 = std::mktime(&date_tm_3);
    trader->current_date = date3;

    // Call the trade method
    trader->trade();

    // Check if trader can trade
    ASSERT_FALSE(trader->can_trade());
    ASSERT_EQ(trader->current_position, nullptr);
}

TEST_F(TraderTest, TradeOnTradingSchedule)
{
    // Set neural network output to enter long
    trader->decisions = {1.0, 0.0, 0.0};

    std::tm date_tm = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 2,
        .tm_hour = 7,
        .tm_min = 0,
        .tm_sec = 0};
    time_t date = std::mktime(&date_tm);
    trader->current_date = date;

    // Call the trade method
    trader->trade();

    // Check if no new position is opened
    ASSERT_NE(trader->current_position, nullptr);
    ASSERT_EQ(trader->open_orders.size(), 2);
    ASSERT_LT(trader->balance, 1000.0);
}

TEST_F(TraderTest, TradeNotWhenSpreadHigh)
{
    // Set neural network output to enter long
    trader->decisions = {1.0, 0.0, 0.0};

    // Mock data for testing
    trader->candles = {{TimeFrame::H1, {
                                           Candle{.date = date, .close = 1.0, .spread = config.strategy.maximum_spread.value() + 1},
                                       }}};

    // Call the trade method
    trader->trade();

    // Check if no new position is opened
    ASSERT_FALSE(trader->can_trade());
    ASSERT_EQ(trader->current_position, nullptr);
}

TEST_F(TraderTest, RespectNumberOfTradesPerDay)
{
    // Set neural network output to enter long
    trader->decisions = {1.0, 0.0, 0.0};

    // Trade the maximum number of trades per day
    for (int i = 0; i < config.strategy.maximum_trades_per_day.value(); ++i)
    {
        // Check if the trader can trade
        ASSERT_TRUE(trader->can_trade());

        trader->open_position_by_market(1.0, 1.0, OrderSide::LONG);

        // Check if a new position is opened
        ASSERT_NE(trader->current_position, nullptr);

        trader->close_position_by_market();

        // Check if the position is closed
        ASSERT_EQ(trader->current_position, nullptr);

        // Set the duration without trade to the minimum duration before next trade
        trader->duration_without_trade = config.strategy.minimum_duration_before_next_trade.value();
    }

    // Check if the trader can trade
    ASSERT_FALSE(trader->can_trade());

    // Try to trade
    trader->trade();

    // Check if no new position is opened
    ASSERT_EQ(trader->current_position, nullptr);
}

TEST_F(TraderTest, UpdateLongPositionPnl)
{
    // Mock data for testing
    trader->candles = {{TimeFrame::H1, {
                                           Candle{.date = date, .close = 1.00100},
                                       }}};
    trader->current_position = new Position{.entry_date = date, .entry_price = 1.00000, .size = 1.0, .side = PositionSide::LONG, .pnl = 0.0};

    // Call the update_position_pnl method
    trader->update_position_pnl();

    // Check if pnl is updated correctly for a long position
    ASSERT_EQ(trader->current_position->pnl, 100.0);
}

TEST_F(TraderTest, UpdateShortPositionPnl)
{
    // Mock data for testing
    trader->candles = {{TimeFrame::H1, {
                                           Candle{.date = date, .close = 0.99900},
                                       }}};
    trader->current_position = new Position{.entry_date = date, .entry_price = 1.00000, .size = 1.0, .side = PositionSide::SHORT, .pnl = 0.0};

    // Call the update_position_pnl method
    trader->update_position_pnl();

    // Check if pnl is updated correctly for a short position
    ASSERT_EQ(trader->current_position->pnl, 100.0);
}

TEST_F(TraderTest, CalculateStatsDrawdown)
{
    // Mock data for testing
    trader->balance_history = {1000.0, 900.0, 1100.0, 1000.0};

    // Call the calculate_stats method
    trader->calculate_stats();

    // Check if stats are updated correctly
    ASSERT_EQ(trader->stats.max_drawdown, 0.1);
}

TEST_F(TraderTest, CalculateStatsWinrate)
{
    // Mock data for testing
    trader->trades_history = {
        {.side = PositionSide::LONG, .pnl = 100, .closed = true},
        {.side = PositionSide::LONG, .pnl = -50, .closed = true},
        {.side = PositionSide::LONG, .pnl = -50, .closed = true},
        {.side = PositionSide::LONG, .pnl = 100, .closed = true},
        {.side = PositionSide::SHORT, .pnl = -50, .closed = true},
        {.side = PositionSide::SHORT, .pnl = -50, .closed = true},
        {.side = PositionSide::SHORT, .pnl = 50, .closed = true},
        {.side = PositionSide::SHORT, .pnl = 50, .closed = true}};

    // Call calculate method
    trader->calculate_stats();

    // Check the new stats
    ASSERT_EQ(trader->stats.win_rate, 0.5);
    ASSERT_EQ(trader->stats.long_win_rate, 0.5);
    ASSERT_EQ(trader->stats.short_win_rate, 0.5);
}

TEST_F(TraderTest, CalculateStatsTotalNetProfit)
{
    // Mock data for testing
    trader->trades_history = {
        {.pnl = 1000, .closed = true},
        {.pnl = -500, .closed = true},
        {.pnl = -50, .closed = true}};

    // Call calculate method
    trader->calculate_stats();

    // Check the new stats
    ASSERT_EQ(trader->stats.total_net_profit, 1000 - 500 - 50);
}

TEST_F(TraderTest, CalculateStatsAverageProfitLoss)
{
    // Mock data for testing
    trader->trades_history = {
        {.pnl = 100, .closed = true},
        {.pnl = 50, .closed = true},
        {.pnl = -100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 100, .closed = true},
        {.pnl = 50, .closed = true}};

    // Call calculate method
    trader->calculate_stats();

    // Check the new stats
    ASSERT_EQ(trader->stats.average_profit, 75.0);
    ASSERT_EQ(trader->stats.average_loss, 75.0);
}

TEST_F(TraderTest, CalculateStatsProfitFactor)
{
    // Mock data for testing
    trader->stats.average_profit = 100;
    trader->stats.average_loss = 50;
    trader->trades_history = {
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
    };

    // Call calculate method
    trader->calculate_stats();

    // Check the new stats
    ASSERT_EQ(trader->stats.profit_factor, 2.0);
}

TEST_F(TraderTest, CalculateStatsTotalTrades)
{
    // Mock data for testing
    trader->trades_history = {
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 50, .closed = true},
        {.pnl = 50, .closed = true}};

    // Call calculate method
    trader->calculate_stats();

    // Check the new stats
    ASSERT_EQ(trader->stats.total_trades, 8);
    ASSERT_EQ(trader->stats.total_winning_trades, 4);
    ASSERT_EQ(trader->stats.total_lost_trades, 4);
}

TEST_F(TraderTest, CalculateStatsMaximumProfitLossTrades)
{
    // Mock data for testing
    trader->trades_history = {
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 50, .closed = true},
        {.pnl = 50, .closed = true}};

    // Call calculate method
    trader->calculate_stats();

    // Check the new stats
    ASSERT_EQ(trader->stats.max_consecutive_winning_trades, 2);
    ASSERT_EQ(trader->stats.max_consecutive_lost_trades, 2);
}

TEST_F(TraderTest, CalculateStatsMaxProfitLoss)
{
    // Mock data for testing
    trader->trades_history = {
        {.pnl = -50, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 200, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = -150, .closed = true},
        {.pnl = 50, .closed = true},
        {.pnl = 50, .closed = true}};

    // Call calculate method
    trader->calculate_stats();

    // Check the new stats
    ASSERT_EQ(trader->stats.max_profit, 200);
    ASSERT_EQ(trader->stats.max_loss, -150);
}

TEST_F(TraderTest, CalculateStatsMaxConsecutiveProfitLoss)
{
    // Mock data for testing
    trader->trades_history = {
        {.pnl = 100, .closed = true},
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 50, .closed = true},
        {.pnl = 50, .closed = true}};

    // Call calculate method
    trader->calculate_stats();

    // Check the new stats
    ASSERT_EQ(trader->stats.max_consecutive_profit, 200);
    ASSERT_EQ(trader->stats.max_consecutive_loss, -100);
}

TEST_F(TraderTest, CalculateStatsAverageTradeDuration)
{
    // Mock data for testing
    trader->trades_history = {
        {.duration = 4, .closed = true},
        {.duration = 3, .closed = true},
        {.duration = 2, .closed = true},
        {.duration = 1, .closed = true},
        {.duration = 4, .closed = true},
        {.duration = 3, .closed = true},
        {.duration = 2, .closed = true},
        {.duration = 1, .closed = true}};

    // Call calculate method
    trader->calculate_stats();

    // Check the new stats
    ASSERT_EQ(trader->stats.average_trade_duration, 20.0 / 8.0);
}

TEST_F(TraderTest, CalculateStatsSharpeRatio)
{
}

TEST_F(TraderTest, CalculateStatsSortinoRatio)
{
}

TEST_F(TraderTest, CalculateFitness)
{
    // Mock data for testing
    trader->stats.total_trades = 15;
    trader->stats.max_drawdown = 0.03;
    trader->stats.profit_factor = 1.8;
    trader->stats.win_rate = 0.6;
    trader->stats.average_profit = 0.025;
    trader->stats.average_loss = 0.015;
    trader->stats.sharpe_ratio = 1.5;
    trader->trades_history = {
        {.pnl_percent = 0.1, .closed = true},
        {.pnl_percent = -0.05, .closed = true},
        {.pnl_percent = -0.1, .closed = true},
        {.pnl_percent = 0.1, .closed = true},
        {.pnl_percent = -0.05, .closed = true},
        {.pnl_percent = -0.05, .closed = true},
        {.pnl_percent = 0.05, .closed = true},
        {.pnl_percent = 0.05, .closed = true}};

    // Call the calculate_fitness method
    trader->calculate_fitness();

    // Check if fitness is calculated correctly
    ASSERT_GT(trader->fitness, 0.0);
}

TEST_F(TraderTest, GenerateBalanceHistoryGraph)
{
    // Mock data for testing
    trader->balance_history = {1000.0, 900.0, 1100.0, 1000.0};

    // Call the generate_balance_history_graphic method
    std::filesystem::path file = "cache/tests/trader_balance_history.png";
    trader->generate_balance_history_graph(file);

    // Check if the graphic is created
    ASSERT_TRUE(std::filesystem::exists(file));

    // Remove the file
    std::filesystem::remove_all(file.parent_path());
}

TEST_F(TraderTest, GenerateReport)
{
    // Mock data for testing
    trader->calculate_stats();

    // Call the generate_report method
    std::filesystem::path file = "cache/tests/trader_report.png";
    trader->generate_report(file);

    // Check if the graphic is created
    ASSERT_TRUE(std::filesystem::exists(file));

    // Remove the file
    std::filesystem::remove_all(file.parent_path());
}