#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <optional>
#include "neat/config.hpp"
#include "indicators/indicator.hpp"

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
 * @brief Enum for different sources of candle data.
 */
enum class CandleSource
{
    Open,
    High,
    Low,
    Close,
    HL2,
    HLC3,
    OHLC4,
    HLCC4
};

struct Candle
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
using CandlesData = std::unordered_map<TimeFrame, std::vector<Candle>>;
using IndicatorsData = std::unordered_map<TimeFrame, std::unordered_map<std::string, std::vector<double>>>;
using BaseCurrencyConversionRateData = std::unordered_map<std::tm, double>;

/**
 * @brief Struct representing cached data with date information.
 */
struct DatedCache
{
    CandlesData candles;                                          // Cached candle data
    IndicatorsData indicators;                                    // Cached indicator data
    BaseCurrencyConversionRateData base_currency_conversion_rate; // Cached base currency conversion rates
};

/**
 * @brief Alias for a cache of dated data.
 */
using Cache = std::unordered_map<std::string, DatedCache>;

/**
 * @brief Enum for position side (LONG or SHORT).
 */
enum class PositionSide
{
    LONG,
    SHORT
};

/**
 * @brief Struct representing a trading position.
 */
struct Position
{
    PositionSide side;  // Side of the position
    double size;        // Size of the position
    double entry_price; // Entry price of the position
    std::tm entry_date; // Entry date of the position
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
 * @brief Struct representing trading statistics.
 */
struct Stats
{
    float initial_balance;          // Initial balance
    float final_balance;            // Final balance
    float total_net_profit;         // Total net profit
    float total_profit;             // Total profit
    float total_loss;               // Total loss
    float total_fees;               // Total fees
    float profit_factor;            // Profit factor
    float max_drawdown;             // Maximum drawdown
    int total_trades;               // Total number of trades
    int total_long_trades;          // Total number of long trades
    int total_short_trades;         // Total number of short trades
    int total_winning_trades;       // Total number of winning trades
    int total_winning_long_trades;  // Total number of winning long trades
    int total_winning_short_trades; // Total number of winning short trades
    int total_lost_trades;          // Total number of lost trades
    int total_lost_long_trades;     // Total number of lost long trades
    int total_lost_short_trades;    // Total number of lost short trades
    float win_rate;                 // Win rate
    float long_win_rate;            // Long trade win rate
    float short_win_rate;           // Short trade win rate
    float average_profit;           // Average profit per trade
    float average_loss;             // Average loss per trade
    float sharpe_ratio;             // Sharpe ratio
    float sortino_ratio;            // Sortino ratio
};

/**
 * @brief Struct representing symbol information.
 */
struct SymbolInfos
{
    std::string asset;           // Asset of the symbol
    std::string base;            // Base currency of the symbol
    int decimal_places;          // Number of decimal places for the price precision
    float point_value;           // Value of one point movement in the symbol's price
    int contract_size;           // Contract size of the symbol
    float min_lot_size;          // Minimum lot size allowed for the symbol
    float max_lot_size;          // Maximum lot size allowed for the symbol
    float lot_size_step;         // Step size for lot size increments/decrements
    float commission_per_lot;    // Commission per lot traded for the symbol
    std::string commission_base; // Base currency for commission calculation
};

/**
 * @brief Struct representing general trading configuration.
 */
struct General
{
    std::string name;             // Name of the trading configuration
    std::string version;          // Version of the trading configuration
    std::string symbol;           // Trading symbol
    float initial_balance;        // Initial account balance
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
    int stop_loss_in_points;                 // Stop loss in points
    float stop_loss_in_percent;              // Stop loss as a percentage
    TypeTakeProfitStopLoss type_take_profit; // Type of take profit
    int take_profit_in_points;               // Take profit in points
    float take_profit_in_percent;            // Take profit as a percentage
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
struct Strategy
{
    TimeFrame timeframe;                                   // Time frame
    float maximum_risk;                                    // Maximum risk
    std::optional<int> maximum_spread;                     // Maximum spread
    std::optional<int> minimum_trade_duration;             // Minimum trade duration
    std::optional<int> maximum_trade_duration;             // Maximum trade duration
    std::optional<int> minimum_duration_before_next_trade; // Minimum duration before the next trade
    TakeProfitStopLossConfig take_profit_stop_loss_config; // Take profit and stop loss configuration
    std::optional<TradingSchedule> trading_schedule;       // Trading schedule
};

/**
 * @brief Enum for position information.
 */
enum class PositionInfo
{
    TYPE,
    PNL
};

/**
 * @brief Struct representing inputs for a neural network.
 */
struct NeuralNetworkInputs
{
    std::unordered_map<TimeFrame, std::vector<Indicator>> indicators; // Indicators
    std::vector<PositionInfo> position;                               // Position information
};

/**
 * @brief Struct representing training configuration.
 */
struct Training
{
    int generations;                              // Number of generations
    std::optional<float> bad_trader_threshold;    // Threshold for identifying bad traders
    std::optional<int> inactive_trader_threshold; // Threshold for identifying inactive traders
    std::tm training_start_date;                  // Start date for training
    std::tm training_end_date;                    // End date for training
    std::tm test_start_date;                      // Start date for testing
    std::tm test_end_date;                        // End date for testing
    NeuralNetworkInputs inputs;                   // Inputs for neural network
};

/**
 * @brief Struct representing evaluation configuration.
 */
struct Evaluation
{
    std::optional<float> minimum_growth_per_month; // Minimum growth per month
    std::optional<float> maximum_drawdown;         // Maximum drawdown
    std::optional<int> nb_trade_minimum;           // Minimum number of trades
    std::optional<float> minimum_winrate;          // Minimum win rate
    std::optional<float> minimum_profit_factor;    // Minimum profit factor
    std::optional<float> average_profit;           // Average profit
};

/**
 * @brief Struct representing overall trading configuration.
 */
struct Config
{
    General general;       // General trading configuration
    Strategy strategy;     // Trading strategy configuration
    Training training;     // Training configuration
    Evaluation evaluation; // Evaluation configuration
    NeatConfig neat;       // NEAT configuration
};

#endif /* TYPES_H */
