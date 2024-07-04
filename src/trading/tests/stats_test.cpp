#include <gtest/gtest.h>
#include <vector>
#include <ctime>
#include "../stats.hpp"
#include "../../types.hpp"

class StatsTest : public ::testing::Test
{
protected:
    Stats stats;
    std::vector<double> balance_history;
    std::vector<Trade> trades_history;

    void SetUp() override
    {
        this->stats = Stats(1000.0);
        this->balance_history = {1000.0, 1000.0};
        this->trades_history = {};
    }

    void TearDown() override
    {
        this->stats.reset();
    }
};

TEST_F(StatsTest, Performance)
{
    this->balance_history = {1000.0, 1100.0, 900.0, 2000.0};

    stats.calculate(this->trades_history, this->balance_history);

    ASSERT_EQ(stats.performance, 1.0);
}

TEST_F(StatsTest, TotalTradesStats)
{
    this->trades_history = {
        Trade{.side = PositionSide::LONG, .pnl = 100, .closed = true},
        Trade{.side = PositionSide::LONG, .pnl = -50, .closed = true},
        Trade{.side = PositionSide::LONG, .pnl = -50, .closed = true},
        Trade{.side = PositionSide::LONG, .pnl = 100, .closed = true},
        Trade{.side = PositionSide::SHORT, .pnl = -50, .closed = true},
        Trade{.side = PositionSide::SHORT, .pnl = -50, .closed = true},
        Trade{.side = PositionSide::SHORT, .pnl = 50, .closed = true},
        Trade{.side = PositionSide::SHORT, .pnl = 50, .closed = true},
    };

    stats.calculate(this->trades_history, this->balance_history);

    ASSERT_EQ(stats.total_trades, 8);
    ASSERT_EQ(stats.total_long_trades, 4);
    ASSERT_EQ(stats.total_short_trades, 4);
    ASSERT_EQ(stats.total_winning_trades, 4);
    ASSERT_EQ(stats.total_losing_trades, 4);
}

TEST_F(StatsTest, ProfitLossStats)
{
    this->trades_history = {
        {.pnl = 1000, .fees = 10, .closed = true},
        {.pnl = -500, .fees = 10, .closed = true},
        {.pnl = -50, .fees = 10, .closed = true},
    };

    stats.calculate(this->trades_history, this->balance_history);

    ASSERT_EQ(stats.total_profit, 1000);
    ASSERT_EQ(stats.total_loss, 550);
    ASSERT_EQ(stats.total_fees, 30);
    ASSERT_EQ(stats.total_net_profit, 420);
}

TEST_F(StatsTest, MaxDrawdown)
{
    this->balance_history = {1000.0, 900.0, 1100.0, 1000.0};

    stats.calculate(this->trades_history, this->balance_history);

    ASSERT_EQ(stats.max_drawdown, 0.1);
}

TEST_F(StatsTest, WinrateStats)
{
    this->trades_history = {
        {.side = PositionSide::LONG, .pnl = 100, .closed = true},
        {.side = PositionSide::SHORT, .pnl = -50, .closed = true},
        {.side = PositionSide::LONG, .pnl = -50, .closed = true},
        {.side = PositionSide::SHORT, .pnl = 100, .closed = true},
        {.side = PositionSide::LONG, .pnl = -50, .closed = true},
        {.side = PositionSide::SHORT, .pnl = -50, .closed = true},
        {.side = PositionSide::LONG, .pnl = 50, .closed = true},
        {.side = PositionSide::SHORT, .pnl = 50, .closed = true},
    };

    stats.calculate(this->trades_history, this->balance_history);

    ASSERT_EQ(stats.win_rate, 0.5);
    ASSERT_EQ(stats.long_win_rate, 0.5);
    ASSERT_EQ(stats.short_win_rate, 0.5);
}

TEST_F(StatsTest, AverageProfitLossStats)
{
    this->trades_history = {
        {.pnl = 100, .closed = true},
        {.pnl = 50, .closed = true},
        {.pnl = -100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 100, .closed = true},
        {.pnl = 50, .closed = true},
    };

    stats.calculate(this->trades_history, this->balance_history);

    ASSERT_EQ(stats.average_profit, 75);
    ASSERT_EQ(stats.average_loss, 75);
}

TEST_F(StatsTest, ProfitFactor)
{
    stats.average_profit = 100;
    stats.average_loss = 50;
    this->trades_history = {
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
    };

    stats.calculate(this->trades_history, this->balance_history);

    ASSERT_EQ(stats.profit_factor, 2.0);
}

TEST_F(StatsTest, MaximumProfitLossStats)
{
    this->trades_history = {
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = -150, .closed = true},
        {.pnl = 50, .closed = true},
        {.pnl = 50, .closed = true},
    };

    stats.calculate(this->trades_history, this->balance_history);

    ASSERT_EQ(stats.max_profit, 100);
    ASSERT_EQ(stats.max_loss, -150);
}

TEST_F(StatsTest, MaximumConsecutiveWinningLostTradesStats)
{
    this->trades_history = {
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 50, .closed = true},
        {.pnl = 50, .closed = true},
    };

    stats.calculate(this->trades_history, this->balance_history);

    ASSERT_EQ(stats.max_consecutive_winning_trades, 2);
    ASSERT_EQ(stats.max_consecutive_losing_trades, 2);
}

TEST_F(StatsTest, MaximumConsecutiveProfitLossStats)
{
    this->trades_history = {
        {.pnl = 100, .closed = true},
        {.pnl = 50, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 50, .closed = true},
        {.pnl = 50, .closed = true},
    };

    stats.calculate(this->trades_history, this->balance_history);

    ASSERT_EQ(stats.max_consecutive_profit, 150);
    ASSERT_EQ(stats.max_consecutive_loss, -100);
}

TEST_F(StatsTest, AverageTradeDuration)
{
    this->trades_history = {
        {.closed = true, .duration = 1},
        {.closed = true, .duration = 2},
        {.closed = true, .duration = 3},
        {.closed = true, .duration = 4},
        {.closed = true, .duration = 5},
    };

    stats.calculate(this->trades_history, this->balance_history);

    ASSERT_EQ(stats.average_trade_duration, 3.0);
}

TEST_F(StatsTest, MonthlyReturns)
{
    std::tm first_month = {
        .tm_sec = 0,
        .tm_min = 0,
        .tm_hour = 0,
        .tm_mday = 1,
        .tm_mon = 0,
        .tm_year = 2023 - 1900,
    };
    std::tm second_month = {
        .tm_sec = 0,
        .tm_min = 0,
        .tm_hour = 0,
        .tm_mday = 1,
        .tm_mon = 1,
        .tm_year = 2023 - 1900,
    };
    std::tm third_month = {
        .tm_sec = 0,
        .tm_min = 0,
        .tm_hour = 0,
        .tm_mday = 1,
        .tm_mon = 2,
        .tm_year = 2023 - 1900,
    };

    this->trades_history = {
        {.pnl_net_percent = 0.2, .closed = true, .exit_date = std::mktime(&first_month)},
        {.pnl_net_percent = 0.5, .closed = true, .exit_date = std::mktime(&first_month)},
        {.pnl_net_percent = -0.1, .closed = true, .exit_date = std::mktime(&second_month)},
        {.pnl_net_percent = 0.0, .closed = true, .exit_date = std::mktime(&third_month)},
    };

    stats.calculate(this->trades_history, this->balance_history);

    ASSERT_EQ(stats.monthly_returns.size(), 3);
    ASSERT_EQ(stats.monthly_returns["2023-01"], 0.8);
    ASSERT_EQ(stats.monthly_returns["2023-02"], -0.1);
    ASSERT_EQ(stats.monthly_returns["2023-03"], 0.0);
}

TEST_F(StatsTest, SharpeRatio)
{
    // Mock data for testing
    std::tm first_month = {
        .tm_sec = 0,
        .tm_min = 0,
        .tm_hour = 0,
        .tm_mday = 1,
        .tm_mon = 0,
        .tm_year = 2023 - 1900,
    };
    std::tm second_month = {
        .tm_sec = 0,
        .tm_min = 0,
        .tm_hour = 0,
        .tm_mday = 1,
        .tm_mon = 1,
        .tm_year = 2023 - 1900,
    };
    std::tm third_month = {
        .tm_sec = 0,
        .tm_min = 0,
        .tm_hour = 0,
        .tm_mday = 1,
        .tm_mon = 2,
        .tm_year = 2023 - 1900,
    };

    this->trades_history = {
        {.pnl_net_percent = 0.2, .closed = true, .exit_date = std::mktime(&first_month)},
        {.pnl_net_percent = 0.5, .closed = true, .exit_date = std::mktime(&first_month)},
        {.pnl_net_percent = -0.1, .closed = true, .exit_date = std::mktime(&second_month)},
        {.pnl_net_percent = 0.3, .closed = true, .exit_date = std::mktime(&third_month)},
    };

    stats.calculate(this->trades_history, this->balance_history);

    ASSERT_GT(stats.sharpe_ratio, 0.0);
}

TEST_F(StatsTest, SortinoRatio)
{
    std::tm first_month = {
        .tm_sec = 0,
        .tm_min = 0,
        .tm_hour = 0,
        .tm_mday = 1,
        .tm_mon = 0,
        .tm_year = 2023 - 1900,
    };
    std::tm second_month = {
        .tm_sec = 0,
        .tm_min = 0,
        .tm_hour = 0,
        .tm_mday = 1,
        .tm_mon = 1,
        .tm_year = 2023 - 1900,
    };
    std::tm third_month = {
        .tm_sec = 0,
        .tm_min = 0,
        .tm_hour = 0,
        .tm_mday = 1,
        .tm_mon = 2,
        .tm_year = 2023 - 1900,
    };

    this->trades_history = {
        {.pnl_net_percent = -0.2, .closed = true, .exit_date = std::mktime(&first_month)},
        {.pnl_net_percent = 0.1, .closed = true, .exit_date = std::mktime(&first_month)},
        {.pnl_net_percent = -0.15, .closed = true, .exit_date = std::mktime(&second_month)},
        {.pnl_net_percent = 0.5, .closed = true, .exit_date = std::mktime(&third_month)},
    };

    stats.calculate(this->trades_history, this->balance_history);

    ASSERT_GT(stats.sortino_ratio, 0.0);
}

TEST_F(StatsTest, JSON)
{
    this->trades_history = {
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 100, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = -50, .closed = true},
        {.pnl = 50, .closed = true},
        {.pnl = 50, .closed = true},
    };

    stats.calculate(trades_history, balance_history);

    nlohmann::json json = stats.to_json();
    Stats stats_from_json = Stats::from_json(json);

    ASSERT_EQ(stats.total_trades, stats_from_json.total_trades);
    ASSERT_EQ(stats.total_long_trades, stats_from_json.total_long_trades);
    ASSERT_EQ(stats.total_short_trades, stats_from_json.total_short_trades);
    ASSERT_EQ(stats.total_winning_trades, stats_from_json.total_winning_trades);
    ASSERT_EQ(stats.total_losing_trades, stats_from_json.total_losing_trades);
    ASSERT_EQ(stats.total_profit, stats_from_json.total_profit);
    ASSERT_EQ(stats.total_loss, stats_from_json.total_loss);
    ASSERT_EQ(stats.total_fees, stats_from_json.total_fees);
    ASSERT_EQ(stats.total_net_profit, stats_from_json.total_net_profit);
    ASSERT_EQ(stats.max_drawdown, stats_from_json.max_drawdown);
    ASSERT_EQ(stats.win_rate, stats_from_json.win_rate);
    ASSERT_EQ(stats.long_win_rate, stats_from_json.long_win_rate);
    ASSERT_EQ(stats.short_win_rate, stats_from_json.short_win_rate);
    ASSERT_EQ(stats.average_profit, stats_from_json.average_profit);
    ASSERT_EQ(stats.average_loss, stats_from_json.average_loss);
    ASSERT_EQ(stats.profit_factor, stats_from_json.profit_factor);
    ASSERT_EQ(stats.max_profit, stats_from_json.max_profit);
    ASSERT_EQ(stats.max_loss, stats_from_json.max_loss);
    ASSERT_EQ(stats.max_consecutive_winning_trades, stats_from_json.max_consecutive_winning_trades);
    ASSERT_EQ(stats.max_consecutive_losing_trades, stats_from_json.max_consecutive_losing_trades);
    ASSERT_EQ(stats.max_consecutive_profit, stats_from_json.max_consecutive_profit);
    ASSERT_EQ(stats.max_consecutive_loss, stats_from_json.max_consecutive_loss);
    ASSERT_EQ(stats.average_trade_duration, stats_from_json.average_trade_duration);
    ASSERT_EQ(stats.monthly_returns, stats_from_json.monthly_returns);
    ASSERT_EQ(stats.sharpe_ratio, stats_from_json.sharpe_ratio);
    ASSERT_EQ(stats.sortino_ratio, stats_from_json.sortino_ratio);
}