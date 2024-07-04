#include <cmath>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include "../utils/math.hpp"
#include "../utils/date_conversion.hpp"
#include "../indicators/utils.hpp"
#include "../libs/json.hpp"
#include "stats.hpp"

/**
 * @brief Constructor for the Stats class.
 * @param initial_balance Initial balance.
 */
Stats::Stats(double initial_balance)
{
    this->initial_balance = initial_balance;
    this->reset();
}

/**
 * @brief Reset the statistics.
 */
void Stats::reset()
{
    this->performance = 0;
    this->total_net_profit = 0;
    this->total_profit = 0;
    this->total_loss = 0;
    this->total_fees = 0;
    this->total_trades = 0;
    this->total_long_trades = 0;
    this->total_short_trades = 0;
    this->total_winning_trades = 0;
    this->total_winning_long_trades = 0;
    this->total_winning_short_trades = 0;
    this->total_losing_trades = 0;
    this->total_losing_long_trades = 0;
    this->total_losing_short_trades = 0;
    this->max_consecutive_winning_trades = 0;
    this->max_consecutive_losing_trades = 0;
    this->profit_factor = 0;
    this->max_drawdown = 0;
    this->win_rate = 0;
    this->long_win_rate = 0;
    this->short_win_rate = 0;
    this->average_profit = 0;
    this->average_loss = 0;
    this->max_profit = 0;
    this->max_loss = 0;
    this->max_consecutive_profit = 0;
    this->max_consecutive_loss = 0;
    this->average_trade_duration = 0;
    this->average_investment_return = 0;
    this->sharpe_ratio = 0;
    this->sortino_ratio = 0;
    this->monthly_returns = {};
}

/**
 * @brief Update the statistics.
 * @param trades_history Vector of Trade objects.
 * @param balance_history Vector of double.
 */
void Stats::calculate(std::vector<Trade> &trades_history, std::vector<double> &balance_history)
{
    this->reset();

    // Select only closed trade
    std::vector<Trade> closed_trades = {};
    for (const auto &trade : trades_history)
    {
        if (trade.closed)
        {
            closed_trades.push_back(trade);
        }
    }

    if (balance_history.empty())
    {
        std::cerr << "Error: cannot calculate statistics with an empty balance history." << std::endl;
        return;
    }

    this->final_balance = balance_history.back();
    this->calculate_performance();
    this->calculate_total_trades_stats(closed_trades);
    this->calculate_profit_loss_stats(closed_trades);
    this->calculate_max_drawdown(balance_history);
    this->calculate_winrate_stats(closed_trades);
    this->calculate_average_profit_loss_stats();
    this->calculate_profit_factor();
    this->calculate_maximum_profit_loss_stats(closed_trades);
    this->calculate_maximum_consecutive_winning_lost_trades_stats(closed_trades);
    this->calculate_maximum_consecutive_profit_loss_stats(closed_trades);
    this->calculate_average_trade_duration(closed_trades);
    this->calculate_monthly_returns(closed_trades);
    this->calculate_average_investment_return(closed_trades);
    this->calculate_sharpe_ratio(closed_trades);
    this->calculate_sortino_ratio(closed_trades);
}

/**
 * @brief Convert the statistics to a JSON object.
 * @return JSON object.
 */
nlohmann::json Stats::to_json() const
{
    // Stats
    nlohmann::json json = {
        {"initial_balance", this->initial_balance},
        {"final_balance", this->final_balance},
        {"performance", this->performance},
        {"total_net_profit", this->total_net_profit},
        {"total_profit", this->total_profit},
        {"total_loss", this->total_loss},
        {"total_fees", this->total_fees},
        {"total_trades", this->total_trades},
        {"total_long_trades", this->total_long_trades},
        {"total_short_trades", this->total_short_trades},
        {"total_winning_trades", this->total_winning_trades},
        {"total_losing_trades", this->total_losing_trades},
        {"max_consecutive_winning_trades", this->max_consecutive_winning_trades},
        {"max_consecutive_losing_trades", this->max_consecutive_losing_trades},
        {"profit_factor", this->profit_factor},
        {"max_drawdown", this->max_drawdown},
        {"win_rate", this->win_rate},
        {"long_win_rate", this->long_win_rate},
        {"short_win_rate", this->short_win_rate},
        {"average_profit", this->average_profit},
        {"average_loss", this->average_loss},
        {"max_profit", this->max_profit},
        {"max_loss", this->max_loss},
        {"max_consecutive_profit", this->max_consecutive_profit},
        {"max_consecutive_loss", this->max_consecutive_loss},
        {"average_trade_duration", this->average_trade_duration},
        {"sharpe_ratio", this->sharpe_ratio},
        {"sortino_ratio", this->sortino_ratio},
    };

    // Monthly returns
    nlohmann::json monthly_returns_json = {};
    for (const auto &[key, value] : this->monthly_returns)
    {
        monthly_returns_json[key] = value;
    }
    json["monthly_returns"] = monthly_returns_json;

    return json;
}

/**
 * @brief Convert the JSON object to the statistics.
 * @param json JSON object.
 * @return Stats object.
 */
Stats Stats::from_json(const nlohmann::json &json)
{
    if (!json.contains("initial_balance") || !json.contains("final_balance"))
    {
        std::cerr << "Error: JSON object is missing required fields: initial_balance" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (!json.contains("final_balance"))
    {
        std::cerr << "Error: JSON object is missing required fields: final_balance" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    Stats stats(json.at("initial_balance"));

    stats.initial_balance = json.at("initial_balance").get<double>();
    stats.final_balance = json.at("final_balance").get<double>();
    stats.performance = json.at("performance").get<double>();
    stats.total_net_profit = json.at("total_net_profit").get<double>();
    stats.total_profit = json.at("total_profit").get<double>();
    stats.total_loss = json.at("total_loss").get<double>();
    stats.total_fees = json.at("total_fees").get<double>();
    stats.total_trades = json.at("total_trades").get<int>(),
    stats.total_long_trades = json.at("total_long_trades").get<int>(),
    stats.total_short_trades = json.at("total_short_trades").get<int>(),
    stats.total_winning_trades = json.at("total_winning_trades").get<int>(),
    stats.total_losing_trades = json.at("total_losing_trades").get<int>(),
    stats.max_consecutive_winning_trades = json.at("max_consecutive_winning_trades").get<int>(),
    stats.max_consecutive_losing_trades = json.at("max_consecutive_losing_trades").get<int>(),
    stats.profit_factor = json.at("profit_factor").get<double>();
    stats.max_drawdown = json.at("max_drawdown").get<double>();
    stats.win_rate = json.at("win_rate").get<double>();
    stats.long_win_rate = json.at("long_win_rate").get<double>();
    stats.short_win_rate = json.at("short_win_rate").get<double>();
    stats.average_profit = json.at("average_profit").get<double>();
    stats.average_loss = json.at("average_loss").get<double>();
    stats.max_profit = json.at("max_profit").get<double>();
    stats.max_loss = json.at("max_loss").get<double>();
    stats.max_consecutive_profit = json.at("max_consecutive_profit").get<double>();
    stats.max_consecutive_loss = json.at("max_consecutive_loss").get<double>();
    stats.average_trade_duration = json.at("average_trade_duration").get<double>();
    stats.sharpe_ratio = json.at("sharpe_ratio").get<double>();
    stats.sortino_ratio = json.at("sortino_ratio").get<double>();

    stats.monthly_returns = {};
    for (const auto &[key, value] : json.at("monthly_returns").items())
    {
        stats.monthly_returns[key] = value.get<double>();
    }

    return stats;
}

/**
 * @brief Print the statistics.
 */
void Stats::print() const
{
    std::cout << "------------------------------ STATS -----------------------------" << std::endl;
    std::cout << "Initial balance: " << this->initial_balance << std::endl;
    std::cout << "Final balance: " << this->final_balance << std::endl;
    std::cout << "Performance: " << decimal_floor(this->performance * 100, 2) << "%" << std::endl;
    std::cout << "Total net profit: " << this->total_net_profit << std::endl;
    std::cout << "Total profit: " << this->total_profit << std::endl;
    std::cout << "Total loss: " << this->total_loss << std::endl;
    std::cout << "Total fees: " << this->total_fees << std::endl;
    std::cout << "Total trades: " << this->total_trades << std::endl;
    std::cout << "Total long trades: " << this->total_long_trades << std::endl;
    std::cout << "Total short trades: " << this->total_short_trades << std::endl;
    std::cout << "Total winning trades: " << this->total_winning_trades << std::endl;
    std::cout << "Total lost trades: " << this->total_losing_trades << std::endl;
    std::cout << "Max consecutive winning trades: " << this->max_consecutive_winning_trades << std::endl;
    std::cout << "Max consecutive lost trades: " << this->max_consecutive_losing_trades << std::endl;
    std::cout << "Profit factor: " << decimal_floor(this->profit_factor, 2) << std::endl;
    std::cout << "Max drawdown: " << decimal_floor(-this->max_drawdown * 100, 2) << "%" << std::endl;
    std::cout << "Win rate: " << decimal_floor(this->win_rate * 100, 2) << "%" << std::endl;
    std::cout << "Long win rate: " << decimal_floor(this->long_win_rate * 100, 2) << "%" << std::endl;
    std::cout << "Short win rate: " << decimal_floor(this->short_win_rate * 100, 2) << "%" << std::endl;
    std::cout << "Average profit: " << this->average_profit << std::endl;
    std::cout << "Average loss: " << this->average_loss << std::endl;
    std::cout << "Max profit: " << this->max_profit << std::endl;
    std::cout << "Max loss: " << this->max_loss << std::endl;
    std::cout << "Max consecutive profit: " << this->max_consecutive_profit << std::endl;
    std::cout << "Max consecutive loss: " << this->max_consecutive_loss << std::endl;
    std::cout << "Average trade duration: " << this->average_trade_duration << " candles" << std::endl;
    std::cout << "Sharpe ratio: " << decimal_floor(this->sharpe_ratio, 2) << std::endl;
    std::cout << "Sortino ratio: " << decimal_floor(this->sortino_ratio, 2) << std::endl;
}

/**
 * @brief Calculate the performance.
 */
void Stats::calculate_performance()
{
    if (this->initial_balance != 0)
    {
        this->performance = (this->final_balance - this->initial_balance) / this->initial_balance;
    }
}

/**
 * @brief Calculate the total number of trades (long/short trades, winning/losing trades).
 * @param closed_trades Vector of Trade objects.
 */
void Stats::calculate_total_trades_stats(const std::vector<Trade> &closed_trades)
{
    this->total_trades = closed_trades.size();
    this->total_long_trades = std::count_if(closed_trades.begin(), closed_trades.end(), [](Trade trade)
                                            { return trade.side == PositionSide::LONG; });
    this->total_short_trades = this->total_trades - this->total_long_trades;

    for (const auto &trade : closed_trades)
    {
        if (trade.pnl >= 0)
        {
            this->total_winning_trades++;
            if (trade.side == PositionSide::LONG)
            {
                this->total_winning_long_trades++;
            }
            else
            {
                this->total_winning_short_trades++;
            }
        }
        else
        {
            this->total_losing_trades++;
            if (trade.side == PositionSide::LONG)
            {
                this->total_losing_long_trades++;
            }
            else
            {
                this->total_losing_short_trades++;
            }
        }
    }
}

/**
 * @brief Calculate the total net profit, total profit, total loss and total fees.
 * @param closed_trades Vector of Trade objects.
 */
void Stats::calculate_profit_loss_stats(const std::vector<Trade> &closed_trades)
{
    for (const auto &trade : closed_trades)
    {
        if (trade.pnl >= 0)
        {
            this->total_profit += trade.pnl;
            this->total_fees += trade.fees;
        }
        else
        {
            this->total_loss += abs(trade.pnl);
            this->total_fees += trade.fees;
        }
    }

    this->total_net_profit = this->total_profit - this->total_loss - this->total_fees;
}

/**
 * @brief Calculate the maximum drawdown.
 * @param balance_history Vector of double.
 */
void Stats::calculate_max_drawdown(const std::vector<double> &balance_history)
{
    if (balance_history.empty() || balance_history.size() < 2)
    {
        std::cerr << "Error: balance history is empty or has fewer than two data points." << std::endl;

        this->max_drawdown = 0.0; // No drawdown if there are fewer than two data points
    }

    double peak = balance_history[0];
    double trough = balance_history[0];
    double max_drawdown = 0.0;

    for (size_t i = 1; i < balance_history.size(); ++i)
    {
        double balance = balance_history[i];
        if (balance > peak)
        {
            peak = balance;
            trough = balance;
        }
        else if (balance < trough)
        {
            trough = balance;
        }

        double drawdown = (peak - trough) / peak;
        max_drawdown = std::max(max_drawdown, drawdown);
    }

    this->max_drawdown = max_drawdown;
}

/**
 * @brief Calculate the total profit.
 * @param closed_trades Vector of Trade objects.
 */
void Stats::calculate_winrate_stats(const std::vector<Trade> &closed_trades)
{
    // Calculate the winrate
    if (closed_trades.size() > 0)
    {
        this->win_rate = static_cast<double>(this->total_winning_trades) / static_cast<double>(closed_trades.size());
    }

    // Calculate the winrate for longs
    if (this->total_long_trades > 0)
    {
        this->long_win_rate = static_cast<double>(this->total_winning_long_trades) / static_cast<double>(this->total_long_trades);
    }

    // Calculate the winrate for shorts
    if (this->total_short_trades > 0)
    {
        this->short_win_rate = static_cast<double>(this->total_winning_short_trades) / static_cast<double>(this->total_short_trades);
    }
}

/**
 * @brief Calculate the average profit and loss per trade.
 */
void Stats::calculate_average_profit_loss_stats()
{
    // Calculate the average profit per trade
    if (this->total_winning_trades > 0)
    {
        this->average_profit = this->total_profit / static_cast<double>(this->total_winning_trades);
    }

    // Calculate the average loss per trade
    if (this->total_losing_trades > 0)
    {
        this->average_loss = this->total_loss / static_cast<double>(this->total_losing_trades);
    }
}

/**
 * @brief Calculate the profit factor.
 */
void Stats::calculate_profit_factor()
{
    if (this->average_profit != 0 && this->average_loss != 0)
    {
        this->profit_factor = (this->win_rate * this->average_profit) / ((1 - this->win_rate) * this->average_loss);
    }
}

/**
 * @brief Calculate the maximum profit and loss.
 * @param closed_trades Vector of Trade objects.
 */
void Stats::calculate_maximum_profit_loss_stats(const std::vector<Trade> &closed_trades)
{
    // Calculate the maximum profit
    if (this->total_winning_trades > 0)
    {
        this->max_profit = (*std::max_element(closed_trades.begin(), closed_trades.end(), [](Trade a, Trade b)
                                              { return a.pnl < b.pnl; }))
                               .pnl;
    }

    // Calculate the maximum loss
    if (this->total_losing_trades > 0)
    {
        this->max_loss = (*std::min_element(closed_trades.begin(), closed_trades.end(), [](Trade a, Trade b)
                                            { return a.pnl < b.pnl; }))
                             .pnl;
    }
}

/**
 * @brief Calculate the maximum consecutive profit and loss.
 * @param closed_trades Vector of Trade objects.
 */
void Stats::calculate_maximum_consecutive_winning_lost_trades_stats(const std::vector<Trade> &closed_trades)
{
    // Calculate the maximum consecutive winning trades
    this->max_consecutive_winning_trades = 0;
    int current_consecutive_winning_trades = 0;
    for (const auto &trade : closed_trades)
    {
        if (trade.pnl >= 0)
        {
            current_consecutive_winning_trades++;
            this->max_consecutive_winning_trades = std::max(this->max_consecutive_winning_trades, current_consecutive_winning_trades);
        }
        else
        {
            current_consecutive_winning_trades = 0;
        }
    }

    // Calculate the maximum consecutive lost trades
    this->max_consecutive_losing_trades = 0;
    int current_consecutive_lost_trades = 0;
    for (const auto &trade : closed_trades)
    {
        if (trade.pnl < 0)
        {
            current_consecutive_lost_trades++;
            this->max_consecutive_losing_trades = std::max(this->max_consecutive_losing_trades, current_consecutive_lost_trades);
        }
        else
        {
            current_consecutive_lost_trades = 0;
        }
    }
}

/**
 * @brief Calculate the maximum consecutive profit and loss.
 * @param closed_trades Vector of Trade objects.
 */
void Stats::calculate_maximum_consecutive_profit_loss_stats(const std::vector<Trade> &closed_trades)
{
    // Calculate the maximum consecutive profit
    this->max_consecutive_profit = 0;
    double current_consecutive_profit = 0;
    for (const auto &trade : closed_trades)
    {
        if (trade.pnl >= 0)
        {
            current_consecutive_profit += trade.pnl;
            this->max_consecutive_profit = std::max(this->max_consecutive_profit, current_consecutive_profit);
        }
        else
        {
            current_consecutive_profit = 0;
        }
    }

    // Calculate the maximum consecutive loss
    this->max_consecutive_loss = 0;
    double current_consecutive_loss = 0;
    for (const auto &trade : closed_trades)
    {
        if (trade.pnl < 0)
        {
            current_consecutive_loss += trade.pnl;
            this->max_consecutive_loss = std::min(this->max_consecutive_loss, current_consecutive_loss);
        }
        else
        {
            current_consecutive_loss = 0;
        }
    }
}

/**
 * @brief Calculate the average trade duration.
 * @param closed_trades Vector of Trade objects.
 */
void Stats::calculate_average_trade_duration(const std::vector<Trade> &closed_trades)
{
    if (this->total_trades > 0)
    {
        double total_duration = 0;
        for (auto &trade : closed_trades)
        {
            total_duration += trade.duration;
        }
        this->average_trade_duration = total_duration / static_cast<double>(this->total_trades);
    }
}

/**
 * @brief Calculate the monthly returns.
 * @param closed_trades Vector of Trade objects.
 */
void Stats::calculate_monthly_returns(const std::vector<Trade> &closed_trades)
{
    for (const auto &trade : closed_trades)
    {
        double trade_return = trade.pnl_net_percent;
        std::string date_key = time_t_to_string(trade.exit_date, "%Y-%m");
        if (this->monthly_returns.find(date_key) == this->monthly_returns.end())
        {
            this->monthly_returns[date_key] = 1.0;
        }
        this->monthly_returns[date_key] *= (1.0 + trade_return);
    }

    for (const auto &[month, monthly_return] : this->monthly_returns)
    {
        this->monthly_returns[month] = decimal_round(monthly_return - 1.0, 4);
    }
}

/**
 * @brief Calculate the average investment return.
 * @param closed_trades Vector of Trade objects.
 */
void Stats::calculate_average_investment_return(const std::vector<Trade> &closed_trades)
{
    if (this->monthly_returns.size() > 0)
    {
        for (const auto &[month, performance] : this->monthly_returns)
        {
            this->average_investment_return += performance / static_cast<double>(this->monthly_returns.size());
        }
    }
}

/**
 * @brief Calculate the Sharpe ratio.
 * @param closed_trades Vector of Trade objects.
 */
void Stats::calculate_sharpe_ratio(const std::vector<Trade> &closed_trades)
{
    if (this->monthly_returns.size() > 0)
    {
        std::vector<double> monthly_investment_returns = {};
        for (const auto &[month, performance] : this->monthly_returns)
        {
            monthly_investment_returns.push_back(performance);
        }

        double std_dev = calculate_standard_deviation(monthly_investment_returns, monthly_investment_returns.size()).back();
        if (std_dev != 0)
        {
            this->sharpe_ratio = (average_investment_return - 0.0) / std_dev;
        }
    }
}

/**
 * @brief Calculate the Sortino ratio.
 * @param closed_trades Vector of Trade objects.
 */
void Stats::calculate_sortino_ratio(const std::vector<Trade> &closed_trades)
{
    std::vector<double> monthly_investment_returns_negative = {};
    std::vector<double> monthly_investment_returns = {};
    for (const auto &[month, performance] : this->monthly_returns)
    {
        monthly_investment_returns.push_back(performance);
        if (performance < 0)
        {
            monthly_investment_returns_negative.push_back(performance);
        }
    }

    if (monthly_investment_returns_negative.size() > 0)
    {
        double std_dev = calculate_standard_deviation(monthly_investment_returns_negative, monthly_investment_returns_negative.size()).back();
        if (std_dev != 0)
        {
            this->sortino_ratio = (average_investment_return - 0.0) / std_dev;
        }
    }
}