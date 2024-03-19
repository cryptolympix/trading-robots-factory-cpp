#ifndef TRADER_HPP
#define TRADER_HPP

#include <ctime>
#include "utils/logger.hpp"
#include "types.hpp"
#include "neat/genome.hpp"

class Trader
{
public:
    Config config;
    SymbolInfo symbol_info;
    bool debug;

    // Vision
    CandlesData candles;
    double current_base_currency_conversion_rate;
    time_t current_date;

    // Balance and history
    double balance;
    std::vector<double> balance_history;
    Position *current_position;
    std::vector<Order> open_orders;
    time_t last_position_date;

    // Neat stuffs
    Genome *genome;
    int generation;
    double fitness;
    double score;
    std::vector<double> vision;
    std::vector<double> decisions;
    int lifespan;
    bool dead;

    // Statistics of the trader
    Stats stats;

    // Logger
    Logger *logger;

    /**
     * @brief Constructor for the Trader class.
     * @param genome Genome object.
     * @param config Configuration object.
     * @param debug Debug mode flag.
     */
    Trader(Genome *genome, Config config, bool debug = false);

    /**
     * @brief Get inputs for genome.
     * @param candles Candle data for all time frames.
     * @param indicators Indicator data for all time frames.
     * @param base_currency_conversion_rate Conversion rate data when the base asset traded is different from the account currency.
     * @param position_infos Vector of position information.
     */
    void look(CandlesData &candles, IndicatorsData &indicators, BaseCurrencyConversionRateData base_currency_conversion_rate, std::vector<PositionInfo> position_infos);

    /**
     * @brief Get the outputs from the neural network.
     */
    void think();

    /**
     * @brief Update the trader according to the outputs from the neural network.
     */
    void update();

    /**
     * @brief Calculate the fitness of the trader.
     */
    void calculate_fitness();

    /**
     * @brief Print the statistics of the trader.
     */
    void print_stats();

    /**
     * @brief Trade according to the decision.
     */
    void trade();

    /**
     * @brief Open a position by market.
     * @param price Price of the market order.
     * @param size Size of the market order.
     * @param side Side of the market order.
     */
    void open_position_by_market(double price, double size, OrderSide side);

    /**
     * @brief Open a position by limit.
     * @param price Price of the limit order.
     * @param size Size of the limit order.
     * @param side Side of the limit order.
     */
    void close_position_by_market(double price = 0.0);

    /**
     * @brief Close a position by limit.
     * @param price Price of the limit order.
     */
    void close_position_by_limit(double price);

    /**
     * @brief Create an open order.
     * @param type Type of the order.
     * @param side Side of the order.
     * @param price Price of the order.
     */
    void create_open_order(OrderType type, OrderSide side, double price);

    /**
     * @brief Check the open orders and activate them if the price crosses.
     */
    void check_open_orders();

    /**
     * @brief Close all the open orders.
     */
    void close_open_orders();

    /**
     * @brief Check if the position got liquidated.
     */
    void check_position_liquidation();

    /**
     * @brief Update position profit and loss.
     * @param price Price of the asset.
     */
    void update_position_pnl(double price = 0.0);

    /**
     * @brief Update the trader statistics.
     */
    void update_stats();
};

#endif // TRADER_HPP