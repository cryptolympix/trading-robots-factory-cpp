#ifndef TRADING_STATS_HPP
#define TRADING_STATS_HPP

#include <vector>
#include <map>
#include "../libs/json.hpp"
#include "../types.hpp"

class Stats
{
public:
    // Statistics
    double initial_balance;                        // Initial balance
    double final_balance;                          // Final balance
    double performance;                            // Performance
    double total_net_profit;                       // Total net profit
    double total_profit;                           // Total profit
    double total_loss;                             // Total loss
    double total_fees;                             // Total fees
    int total_trades;                              // Total number of trades
    int total_long_trades;                         // Total number of long trades
    int total_short_trades;                        // Total number of short trades
    int total_winning_trades;                      // Total number of winning trades
    int total_winning_long_trades;                 // Total number of winning long trades
    int total_winning_short_trades;                // Total number of winning short trades
    int total_losing_trades;                       // Total number of losing trades
    int total_losing_long_trades;                  // Total number of losing long trades
    int total_losing_short_trades;                 // Total number of losing short trades
    int max_consecutive_winning_trades;            // Maximum consecutive winning trades
    int max_consecutive_losing_trades;             // Maximum consecutive losing trades
    double profit_factor;                          // Profit factor
    double max_drawdown;                           // Maximum drawdown
    double win_rate;                               // Win rate
    double long_win_rate;                          // Long trade win rate
    double short_win_rate;                         // Short trade win rate
    double average_profit;                         // Average profit per trade
    double average_loss;                           // Average loss per trade
    double max_profit;                             // Maximum profit
    double max_loss;                               // Maximum loss
    double max_consecutive_profit;                 // Maximum consecutive profit
    double max_consecutive_loss;                   // Maximum consecutive losses
    double average_trade_duration;                 // Average trade duration
    double average_investment_return;              // Average investment return
    double sharpe_ratio;                           // Sharpe ratio
    double sortino_ratio;                          // Sortino ratio
    std::map<std::string, double> monthly_returns; // Monthly returns

    Stats() = default;

    /**
     * @brief Constructor for the Stats class.
     * @param initial_balance Initial balance.
     */
    Stats(double initial_balance);

    /**
     * @brief Reset the statistics.
     */
    void reset();

    /**
     * @brief Update the statistics.
     * @param trades_history Vector of Trade objects.
     * @param balance_history Vector of double.
     */
    void calculate(std::vector<Trade> &trades_history, std::vector<double> &balance_history);

    /**
     * @brief Convert the statistics to a JSON object.
     * @return JSON object.
     */
    nlohmann::json to_json() const;

    /**
     * @brief Convert the JSON object to the statistics.
     * @param json JSON object.
     * @return Stats object.
     */
    static Stats from_json(const nlohmann::json &json);

    /**
     * @brief Print the statistics.
     */
    void print() const;

private:
    /**
     * @brief Calculate the performance.
     */
    void calculate_performance();

    /**
     * @brief Calculate the total number of trades (long/short trades, winning/lost trades).
     * @param closed_trades Vector of Trade objects.
     */
    void calculate_total_trades_stats(const std::vector<Trade> &closed_trades);

    /**
     * @brief Calculate the total net profit, total profit, total loss and total fees.
     * @param closed_trades Vector of Trade objects.
     */
    void calculate_profit_loss_stats(const std::vector<Trade> &closed_trades);

    /**
     * @brief Calculate the maximum drawdown.
     * @param balance_history Vector of double.
     */
    void calculate_max_drawdown(const std::vector<double> &balance_history);

    /**
     * @brief Calculate the total profit.
     * @param closed_trades Vector of Trade objects.
     */
    void calculate_winrate_stats(const std::vector<Trade> &closed_trades);

    /**
     * @brief Calculate the average profit and loss per trade.
     */
    void calculate_average_profit_loss_stats();

    /**
     * @brief Calculate the profit factor.
     */
    void calculate_profit_factor();

    /**
     * @brief Calculate the maximum profit and loss.
     * @param closed_trades Vector of Trade objects.
     */
    void calculate_maximum_profit_loss_stats(const std::vector<Trade> &closed_trades);

    /**
     * @brief Calculate the maximum consecutive profit and loss.
     * @param closed_trades Vector of Trade objects.
     */
    void calculate_maximum_consecutive_winning_lost_trades_stats(const std::vector<Trade> &closed_trades);

    /**
     * @brief Calculate the maximum consecutive profit and loss.
     * @param closed_trades Vector of Trade objects.
     */
    void calculate_maximum_consecutive_profit_loss_stats(const std::vector<Trade> &closed_trades);

    /**
     * @brief Calculate the average trade duration.
     * @param closed_trades Vector of Trade objects.
     */
    void calculate_average_trade_duration(const std::vector<Trade> &closed_trades);

    /**
     * @brief Calculate the monthly returns.
     * @param closed_trades Vector of Trade objects.
     */
    void calculate_monthly_returns(const std::vector<Trade> &closed_trades);

    /**
     * @brief Calculate the average investment return.
     * @param closed_trades Vector of Trade objects.
     */
    void calculate_average_investment_return(const std::vector<Trade> &closed_trades);

    /**
     * @brief Calculate the Sharpe ratio.
     * @param closed_trades Vector of Trade objects.
     */
    void calculate_sharpe_ratio(const std::vector<Trade> &closed_trades);

    /**
     * @brief Calculate the Sortino ratio.
     * @param closed_trades Vector of Trade objects.
     */
    void calculate_sortino_ratio(const std::vector<Trade> &closed_trades);
};

#endif // TRADING_STATS_HPP