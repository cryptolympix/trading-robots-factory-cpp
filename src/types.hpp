#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <ctime>
#include <chrono>
#include "neat/config.hpp"
#include "indicators/indicator.hpp"
#include "utils/optional.hpp"

class Indicator; // Forward declaration

/**
 * @brief Enum for different time frames.
 */
enum class TimeFrame
{
    M1,
    M5,
    M15,
    M30,
    H1,
    H4,
    H12,
    D1
};

/**
 * @brief Struct representing a candle.
 */
struct CandleStick
{
    time_t date;
    double open;
    double high;
    double low;
    double close;
    double tick_volume;
    double volume;
    double spread;
};

/**
 * @brief Type definitions for different data structures.
 */
using CandleSticksData = std::unordered_map<TimeFrame, std::vector<CandleStick>>;
using IndicatorsData = std::unordered_map<TimeFrame, std::unordered_map<std::string, std::vector<double>>>;
using BaseCurrencyConversionRateData = std::unordered_map<time_t, double>;

/**
 * @brief Struct representing cached data with date information.
 */
struct CacheData
{
    CandleSticksData candles;             // Cached candle data
    IndicatorsData indicators;            // Cached indicator data
    double base_currency_conversion_rate; // Cached base currency conversion rates
};

/**
 * @brief Enum for position side (LONG or SHORT).
 */
enum class PositionSide
{
    LONG,
    SHORT
};

/**
 * @brief Enum for position information.
 */
enum class PositionInfo
{
    TYPE,
    PNL,
    DURATION,
};

/**
 * @brief Struct representing a trading position.
 */
struct Position
{
    PositionSide side;  // Side of the position
    double size;        // Size of the position
    double entry_price; // Entry price of the position
    time_t entry_date;  // Entry date of the position
    double pnl;         // Profit and Loss of the position
};

/**
 * @brief Enum for order type (TAKE_PROFIT or STOP_LOSS).
 */
enum class OrderType
{
    TAKE_PROFIT,
    STOP_LOSS
};

/**
 * @brief Enum for order side (LONG or SHORT).
 */
enum class OrderSide
{
    LONG,
    SHORT
};

/**
 * @brief Struct representing an order.
 */
struct Order
{
    OrderSide side; // Side of the order
    OrderType type; // Type of the order
    double price;   // Price of the order
};

/**
 * @brief Struct representing a trade.
 */
struct Trade
{
    PositionSide side;      // Side of the trade
    time_t entry_date;      // Entry date of the trade
    time_t exit_date;       // Exit date of the trade
    double entry_price;     // Entry price of the trade
    double exit_price;      // Exit price of the trade
    double size;            // Size of the trade
    double pnl;             // Profit and Loss of the trade
    double pnl_percent;     // Profit and Loss as a percentage of the balance
    double pnl_net_percent; // Profit and Loss minus the fees of the balance
    double fees;            // Fees of the trade
    int duration;           // Duration of the trade
    bool closed;            // Whether the trade is closed
};

/**
 * @brief Struct representing trading statistics.
 */
struct Stats
{
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
    int total_lost_trades;                         // Total number of lost trades
    int total_lost_long_trades;                    // Total number of lost long trades
    int total_lost_short_trades;                   // Total number of lost short trades
    int max_consecutive_winning_trades;            // Maximum consecutive winning trades
    int max_consecutive_lost_trades;               // Maximum consecutive lost trades
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
    double sharpe_ratio;                           // Sharpe ratio
    double sortino_ratio;                          // Sortino ratio
    std::map<std::string, double> monthly_returns; // Monthly returns
};

/**
 * @brief Struct representing symbol information.
 */
struct SymbolInfo
{
    std::string asset;           // Asset of the symbol
    std::string base;            // Base currency of the symbol
    int decimal_places;          // Number of decimal places for the price precision
    double point_value;          // Value of one point movement in the symbol's price
    int contract_size;           // Contract size of the symbol
    double min_lot_size;         // Minimum lot size allowed for the symbol
    double max_lot_size;         // Maximum lot size allowed for the symbol
    double lot_size_step;        // Step size for lot size increments/decrements
    double commission_per_lot;   // Commission per lot traded for the symbol
    std::string commission_base; // Base currency for commission calculation
};

/**
 * @brief Struct representing general trading configuration.
 */
struct GeneralConfig
{
    std::string name;             // Name of the trading configuration
    std::string version;          // Version of the trading configuration
    std::string symbol;           // Trading symbol
    double initial_balance;       // Initial account balance
    std::string account_currency; // Account currency
    int leverage;                 // Leverage
};

/**
 * @brief Enum for different types of take profit/stop loss configurations.
 */
enum class TypeTakeProfitStopLoss
{
    POINTS,
    PERCENT,
    EXTREMUM
};

/**
 * @brief Struct representing take profit and stop loss configuration.
 */
struct TakeProfitStopLossConfig
{
    TypeTakeProfitStopLoss type_stop_loss;   // Type of stop loss
    double stop_loss_in_points;              // Stop loss in points
    double stop_loss_in_percent;             // Stop loss as a percentage
    TypeTakeProfitStopLoss type_take_profit; // Type of take profit
    double take_profit_in_points;            // Take profit in points
    double take_profit_in_percent;           // Take profit as a percentage
};

/**
 * @brief Struct representing trading schedule.
 */
struct TradingSchedule
{
    std::vector<bool> monday;    // Trading schedule for Monday
    std::vector<bool> tuesday;   // Trading schedule for Tuesday
    std::vector<bool> wednesday; // Trading schedule for Wednesday
    std::vector<bool> thursday;  // Trading schedule for Thursday
    std::vector<bool> friday;    // Trading schedule for Friday
    std::vector<bool> saturday;  // Trading schedule for Saturday
    std::vector<bool> sunday;    // Trading schedule for Sunday
};

/**
 * @brief Struct representing trading strategy configuration.
 */
struct StrategyConfig
{
    TimeFrame timeframe;                                   // Time frame
    double maximum_risk;                                   // Maximum risk
    Optional<int> maximum_trades_per_day;                  // Maximum trades per day
    Optional<double> maximum_spread;                       // Maximum spread
    Optional<int> minimum_trade_duration;                  // Minimum trade duration
    Optional<int> maximum_trade_duration;                  // Maximum trade duration
    Optional<int> minimum_duration_before_next_trade;      // Minimum duration before the next trade
    TakeProfitStopLossConfig take_profit_stop_loss_config; // Take profit and stop loss configuration
    Optional<TradingSchedule> trading_schedule;            // Trading schedule
};

/**
 * @brief Struct representing inputs for a neural network.
 */
struct NeuralNetworkInputs
{
    std::unordered_map<TimeFrame, std::vector<Indicator *>> indicators; // Indicators
    std::vector<PositionInfo> position;                                 // Position information
};

/**
 * @brief Struct representing training configuration.
 */
struct TrainingConfig
{
    int generations;                            // Number of generations
    Optional<double> bad_trader_threshold;      // Threshold for identifying bad traders
    Optional<double> inactive_trader_threshold; // Threshold for identifying inactive traders
    time_t training_start_date;                 // Start date for training
    time_t training_end_date;                   // End date for training
    time_t test_start_date;                     // Start date for testing
    time_t test_end_date;                       // End date for testing
    NeuralNetworkInputs inputs;                 // Inputs for neural network
};

/**
 * @brief Struct representing evaluation configuration.
 */
struct EvaluationConfig
{
    Optional<int> nb_trades_per_day;
    Optional<double> expected_return_per_day;   // Expected return per day in % of capital
    Optional<double> expected_return_per_month; // Expected return per month in % of capital
    Optional<double> expected_return;           // Expected return per year in % of capital
    Optional<double> maximum_drawdown;          // Maximum drawdown
    Optional<double> minimum_winrate;           // Minimum win rate
    Optional<double> minimum_profit_factor;     // Minimum profit factor
};

/**
 * @brief Struct representing overall trading configuration.
 */
struct Config
{
    GeneralConfig general;       // General trading configuration
    StrategyConfig strategy;     // Trading strategy configuration
    TrainingConfig training;     // Training configuration
    EvaluationConfig evaluation; // Evaluation configuration
    NeatConfig neat;             // NEAT configuration
};

#endif /* TYPES_H */
