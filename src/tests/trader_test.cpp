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
    std::tm date_tm;

    void SetUp() override
    {
        // Mock configurations
        config = Config{
            .general = {
                .name = "test",
                .version = "1.0",
                .symbol = "EURUSD",
                .leverage = 100,
                .initial_balance = 1000,
                .account_currency = "USD",
            },
            .strategy = {.timeframe = TimeFrame::H1, .maximum_risk = 0.02, .maximum_spread = 8, .minimum_trade_duration = 2, .maximum_trade_duration = 4, .minimum_duration_before_next_trade = 4, .take_profit_stop_loss_config = {
                                                                                                                                                                                                       .type_stop_loss = TypeTakeProfitStopLoss::POINTS,
                                                                                                                                                                                                       .stop_loss_in_points = 300,
                                                                                                                                                                                                       .stop_loss_in_percent = 0.01,
                                                                                                                                                                                                       .type_take_profit = TypeTakeProfitStopLoss::POINTS,
                                                                                                                                                                                                       .take_profit_in_points = 300,
                                                                                                                                                                                                       .take_profit_in_percent = 0.01,
                                                                                                                                                                                                   },
                         .trading_schedule = {{.monday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false}, .tuesday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false}, .wednesday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false}, .thursday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false}, .friday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false}, .saturday = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, .sunday = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}}}},
            .training = {.generations = 5, .bad_trader_threshold = 0.01, .inactive_trader_threshold = 500, .inputs = {
                                                                                                               .indicators = {{TimeFrame::M15, {new RSI()}}, {TimeFrame::M30, {new RSI()}}, {TimeFrame::H1, {new RSI()}}},
                                                                                                               .position = {PositionInfo::TYPE, PositionInfo::PNL, PositionInfo::DURATION},
                                                                                                           }},
            .evaluation = {.nb_trades_minimum = 0, .nb_trades_maximum = 40, .maximum_drawdown = 0.1, .expected_return_per_day = 0.01, .expected_return_per_month = 0.1, .minimum_profit_factor = 2, .minimum_winrate = 0.5},
            .neat = load_config("src/configs/neat_config_test.ini"),
        };
        config.neat.population_size = 5;
        config.neat.num_inputs = 4;

        symbol_info = symbol_infos.at(config.general.symbol);
        temp_dir = std::filesystem::temp_directory_path() / "trader_test";

        // 2023-01-01 00:00:00
        date_tm = {
            .tm_year = 2023 - 1900,
            .tm_mon = 0,
            .tm_mday = 1,
            .tm_hour = 0,
            .tm_min = 0,
            .tm_sec = 0};
        time_t date = std::mktime(&date_tm);

        // Trader configurations
        trader = new Trader(new Genome(config.neat), config, nullptr);
        trader->balance = config.general.initial_balance;
        trader->current_position = nullptr;
        trader->open_orders = {};
        trader->balance_history = {};
        trader->trades_history = {};
        trader->decisions = {0, 0, 0, 0, 1};
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
    trader->update();

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
    trader->update();

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
    trader->update();

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
    trader->update();

    // Check if the position is closed and open orders are cleared
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->balance, 0.0);
    ASSERT_EQ(trader->trades_history.size(), 1);
    ASSERT_EQ(trader->trades_history[0].entry_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].exit_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_LT(trader->trades_history[0].pnl, 0.0);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 0.01);
}

TEST_F(TraderTest, UpdateWithInactiveTrader)
{
    // Mock data for testing
    trader->lifespan = config.training.inactive_trader_threshold.value();

    // Call the update method
    trader->update();

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
    trader->update();

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
    ASSERT_EQ(trader->trades_history[0].entry_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].exit_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].exit_price, 1.00500);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_EQ(trader->trades_history[0].pnl, pnl);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
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
    ASSERT_EQ(trader->trades_history[0].entry_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].exit_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].exit_price, 0.99500);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_EQ(trader->trades_history[0].pnl, pnl);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
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
    ASSERT_EQ(trader->trades_history[0].entry_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].exit_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].exit_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_EQ(trader->trades_history[0].pnl, pnl);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
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
    ASSERT_EQ(trader->trades_history[0].entry_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].exit_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].exit_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_EQ(trader->trades_history[0].pnl, pnl);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
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
    ASSERT_EQ(trader->trades_history[0].entry_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].exit_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].exit_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::SHORT);
    ASSERT_EQ(trader->trades_history[0].pnl, pnl);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
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
    ASSERT_EQ(trader->trades_history[0].entry_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].exit_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].exit_price, 1.0);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::SHORT);
    ASSERT_EQ(trader->trades_history[0].pnl, pnl);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
}

TEST_F(TraderTest, TradeEnterLong)
{
    std::tm date_tm = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 5,
        .tm_hour = 12,
        .tm_min = 0,
        .tm_sec = 0};
    time_t date = std::mktime(&date_tm);

    // Set neural network output to enter long
    trader->decisions = {1.0, 0.0, 0.0, 0.0, 0.0};
    trader->current_date = date;

    // Call the trade method
    trader->trade();

    // Check if a new long position is opened
    ASSERT_TRUE(trader->current_position != nullptr);
    ASSERT_EQ(trader->current_position->side, PositionSide::LONG);
    ASSERT_GE(trader->current_position->size, 0);
    ASSERT_EQ(trader->current_position->entry_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->duration_in_position, 0);
    ASSERT_EQ(trader->open_orders.size(), 2);
    ASSERT_LE(trader->balance, 1000.0);
}

TEST_F(TraderTest, TradeEnterShort)
{
    std::tm date_tm = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 5,
        .tm_hour = 12,
        .tm_min = 0,
        .tm_sec = 0};
    time_t date = std::mktime(&date_tm);

    // Set neural network output to enter short
    trader->decisions = {0.0, 1.0, 0.0, 0.0, 0.0};
    trader->current_date = date;

    // Call the trade method
    trader->trade();

    // Check if a new short position is opened
    ASSERT_TRUE(trader->current_position != nullptr);
    ASSERT_EQ(trader->current_position->side, PositionSide::SHORT);
    ASSERT_GE(trader->current_position->size, 0);
    ASSERT_EQ(trader->current_position->entry_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->duration_in_position, 0);
    ASSERT_EQ(trader->open_orders.size(), 2);
    ASSERT_LE(trader->balance, 1000.0);
}

TEST_F(TraderTest, TradeNoAction)
{
    // Set neural network output to no action
    trader->decisions = {0.0, 0.0, 0.0, 0.0, 1.0};

    // Call the trade method
    trader->trade();

    // Check if no new position is opened
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->balance, 1000.0);
    ASSERT_EQ(trader->trades_history.size(), 0);
}

TEST_F(TraderTest, ClosePositionForDurationExceeded)
{
    // Mock data for testing
    std::tm entry_date = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0};

    trader->open_position_by_market(1.01, 1.0, OrderSide::LONG);
    trader->current_position->entry_date = std::mktime(&entry_date);
    trader->update_position_pnl();

    // Call the update method for the maximum trade duration
    for (int i = 0; i < config.strategy.maximum_trade_duration.value(); i++)
    {
        trader->update();
    }

    // Assertions
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_LT(trader->balance, 1000.0); // Balance decreased due to fees
    ASSERT_EQ(trader->trades_history.size(), 1);
    ASSERT_EQ(trader->trades_history[0].entry_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.01);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_NE(trader->trades_history[0].pnl, 0.0);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
    ASSERT_LE(trader->trades_history[0].duration, config.strategy.maximum_trade_duration.value());
}

TEST_F(TraderTest, WaitForDurationBeforeClosePosition)
{
    // Mock data for testing
    std::tm entry_date = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0};

    trader->open_position_by_market(1.00, 1.0, OrderSide::LONG);
    trader->current_position->entry_date = std::mktime(&entry_date);

    trader->decisions = {0.0, 0.0, 1.0, 0.0, 0.0};

    // Call the update method for the maximum trade duration - 1
    for (int i = 0; i < config.strategy.minimum_trade_duration.value() - 1; ++i)
    {
        trader->update();
    }

    // Assertions
    ASSERT_TRUE(trader->current_position != nullptr);
    ASSERT_EQ(trader->duration_in_position, config.strategy.minimum_trade_duration.value() - 1);

    // Call the update method for the last trade duration
    trader->update();

    // Assertions
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_LT(trader->balance, 1000.0); // Balance decreased due to fees
    ASSERT_EQ(trader->trades_history.size(), 1);
    ASSERT_EQ(trader->trades_history[0].entry_date, std::mktime(&date_tm));
    ASSERT_EQ(trader->trades_history[0].entry_price, 1.00);
    ASSERT_EQ(trader->trades_history[0].side, PositionSide::LONG);
    ASSERT_GT(trader->trades_history[0].fees, 0.0);
    ASSERT_EQ(trader->trades_history[0].size, 1.0);
    ASSERT_EQ(trader->trades_history[0].duration, config.strategy.minimum_trade_duration.value());
}

TEST_F(TraderTest, WaitForNextTrade)
{
    std::tm date_tm = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 5,
        .tm_hour = 10,
        .tm_min = 0,
        .tm_sec = 0};
    time_t date = std::mktime(&date_tm);

    trader->decisions = {1.0, 0.0, 0.0, 0.0, 0.0};
    trader->duration_without_trade = 0;
    trader->current_date = date;

    // Call the trade method
    trader->trade();

    // Check if the position is still in progress
    ASSERT_EQ(trader->current_position, nullptr);

    // Set trader's date after the minimum duration before next trade has passed
    trader->duration_without_trade = config.strategy.minimum_duration_before_next_trade.value();

    // Call the trade method
    trader->trade();

    // Check if a new position is opened
    ASSERT_TRUE(trader->current_position != nullptr);
}

TEST_F(TraderTest, CreateTpSlForLongPosition)
{
    // Set neural network output to no action
    trader->decisions = {1.0, 0.0, 0.0, 0.0, 0.0};
    std::tm date_tm = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 5,
        .tm_hour = 12,
        .tm_min = 0,
        .tm_sec = 0};
    time_t date = std::mktime(&date_tm);
    trader->current_date = date;

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
    trader->decisions = {0.0, 1.0, 0.0, 0.0, 0.0};
    std::tm date_tm = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 5,
        .tm_hour = 12,
        .tm_min = 0,
        .tm_sec = 0};
    time_t date = std::mktime(&date_tm);
    trader->current_date = date;

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
    trader->decisions = {1.0, 0.0, 0.0, 0.0, 0.0};

    // try to trade sunday
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

    // Check if no new position is opened
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->balance, 1000.0);
    ASSERT_EQ(trader->trades_history.size(), 0);

    // try to trade just before the opening of trading schedule
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

    // Check if no new position is opened
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->balance, 1000.0);
    ASSERT_EQ(trader->trades_history.size(), 0);

    // try to trade just after the closing of trading schedule
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

    // Check if no new position is opened
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->balance, 1000.0);
    ASSERT_EQ(trader->trades_history.size(), 0);
}

TEST_F(TraderTest, TradeOnTradingSchedule)
{
    // Set neural network output to enter long
    trader->decisions = {1.0, 0.0, 0.0, 0.0, 0.0};

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
    ASSERT_TRUE(trader->current_position != nullptr);
    ASSERT_EQ(trader->open_orders.size(), 2);
    ASSERT_LT(trader->balance, 1000.0);
}

TEST_F(TraderTest, TradeNotWhenSpreadHigh)
{
    // Set neural network output to enter long
    trader->decisions = {1.0, 0.0, 0.0, 0.0, 0.0};
    std::tm date_tm = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0};
    time_t date = std::mktime(&date_tm);
    trader->candles = {{TimeFrame::H1, {
                                           Candle{.date = date, .close = 1.0, .spread = config.strategy.maximum_spread.value() + 1},
                                       }}};

    // Call the trade method
    trader->trade();

    // Check if no new position is opened
    ASSERT_EQ(trader->current_position, nullptr);
    ASSERT_EQ(trader->open_orders.size(), 0);
    ASSERT_EQ(trader->balance, 1000.0);
    ASSERT_EQ(trader->trades_history.size(), 0);
}

TEST_F(TraderTest, UpdateLongPositionPnl)
{
    // Mock data for testing
    std::tm date_tm = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0};
    time_t date = std::mktime(&date_tm);
    trader->candles = {{TimeFrame::H1, {
                                           Candle{.date = date, .close = 1.00100},
                                       }}};
    trader->current_position = new Position{.entry_date = std::mktime(&date_tm), .entry_price = 1.00000, .size = 1.0, .side = PositionSide::LONG, .pnl = 0.0};

    // Call the update_position_pnl method
    trader->update_position_pnl();

    // Check if pnl is updated correctly for a long position
    ASSERT_EQ(trader->current_position->pnl, 100.0);
}

TEST_F(TraderTest, UpdateShortPositionPnl)
{
    // Mock data for testing
    std::tm date_tm = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0};
    time_t date = std::mktime(&date_tm);
    trader->candles = {{TimeFrame::H1, {
                                           Candle{.date = date, .close = 0.99900},
                                       }}};
    trader->current_position = new Position{.entry_date = std::mktime(&date_tm), .entry_price = 1.00000, .size = 1.0, .side = PositionSide::SHORT, .pnl = 0.0};

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
        {.side = PositionSide::LONG, .pnl = 100},
        {.side = PositionSide::LONG, .pnl = -50},
        {.side = PositionSide::LONG, .pnl = -50},
        {.side = PositionSide::LONG, .pnl = 100},
        {.side = PositionSide::SHORT, .pnl = -50},
        {.side = PositionSide::SHORT, .pnl = -50},
        {.side = PositionSide::SHORT, .pnl = 50},
        {.side = PositionSide::SHORT, .pnl = 50}};

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
        {.pnl = 1000},
        {.pnl = -500},
        {.pnl = -50}};

    // Call calculate method
    trader->calculate_stats();

    // Check the new stats
    ASSERT_EQ(trader->stats.total_net_profit, 1000 - 500 - 50);
}

TEST_F(TraderTest, CalculateStatsAverageProfitLoss)
{
    // Mock data for testing
    trader->trades_history = {
        {.pnl = 100},
        {.pnl = 50},
        {.pnl = -100},
        {.pnl = -50},
        {.pnl = 100},
        {.pnl = 50}};

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
        {.pnl = 100},
        {.pnl = -50},
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
        {.pnl = 100},
        {.pnl = -50},
        {.pnl = -50},
        {.pnl = 100},
        {.pnl = -50},
        {.pnl = -50},
        {.pnl = 50},
        {.pnl = 50}};

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
        {.pnl = 100},
        {.pnl = -50},
        {.pnl = -50},
        {.pnl = 100},
        {.pnl = -50},
        {.pnl = -50},
        {.pnl = 50},
        {.pnl = 50}};

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
        {.pnl = -50},
        {.pnl = -50},
        {.pnl = 200},
        {.pnl = -50},
        {.pnl = -150},
        {.pnl = 50},
        {.pnl = 50}};

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
        {.pnl = 100},
        {.pnl = 100},
        {.pnl = -50},
        {.pnl = -50},
        {.pnl = 100},
        {.pnl = -50},
        {.pnl = -50},
        {.pnl = 50},
        {.pnl = 50}};

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
        {.duration = 4},
        {.duration = 3},
        {.duration = 2},
        {.duration = 1},
        {.duration = 4},
        {.duration = 3},
        {.duration = 2},
        {.duration = 1}};

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
        {.pnl_percent = 0.1},
        {.pnl_percent = -0.05},
        {.pnl_percent = -0.1},
        {.pnl_percent = 0.1},
        {.pnl_percent = -0.05},
        {.pnl_percent = -0.05},
        {.pnl_percent = 0.05},
        {.pnl_percent = 0.05}};

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