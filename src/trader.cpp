#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <sstream>
#include <chrono>
#include <cmath>
#include "utils/logger.hpp"
#include "utils/time_frame.hpp"
#include "utils/math.hpp"
#include "trading_schedule.hpp"
#include "trading_tools.hpp"
#include "symbols.hpp"
#include "trader.hpp"

// Function to get the current time in a string format (YYYYMMDDHHMMSS)

/**
 * @brief Get the current time in a string format (YYYYMMDDHHMMSS).
 * @return Current time in a string format.
 */
time_t get_current_time()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    return in_time_t;
    // std::stringstream ss;
    // ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d%H%M%S");
    // return ss.str();
}

/**
 * @brief Convert time_t to string.
 * @param time Time in time_t format.
 * @return Time in string format.
 */
std::string time_t_to_string(time_t time)
{
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

/**
 * @brief Constructor for the Trader class.
 * @param genome Genome object.
 * @param config Configuration object.
 * @param debug Debug mode flag.
 */
Trader::Trader(Genome *genome, Config config, bool debug)
{
    this->config = config;
    this->debug = debug;
    this->symbol_info = symbol_infos[config.general.symbol];

    // Vision
    this->candles = {};
    this->current_base_currency_conversion_rate = 1.0;
    this->current_date = get_current_time();

    // Balance and history
    this->balance = config.general.initial_balance;
    this->balance_history = {config.general.initial_balance};
    this->open_orders = {};
    this->last_position_date = NULL;
    this->current_position = nullptr;

    // Statistics of the trader
    this->stats = {
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
        .max_drawdown = 0,
        .win_rate = 0,
        .long_win_rate = 0,
        .short_win_rate = 0,
        .average_profit = 0,
        .average_loss = 0,
        .profit_factor = 0,
        .sharpe_ratio = 0,
        .sortino_ratio = 0,
    };

    // Neat stuffs
    this->genome = genome;
    this->generation = 0;
    this->fitness = 0;
    this->score = 0;
    this->vision = {};
    this->decisions = {};
    this->lifespan = 0;
    this->dead = false;

    // Logger
    if (this->debug)
    {
        std::stringstream date_string;
        date_string << std::put_time(std::localtime(&this->current_date), "%Y%m%d%H%M%S");
        std::string directory = "logs/" + this->config.general.name + "/" + this->config.general.version + "/" + date_string.str();
        this->logger = new Logger(directory + "/trader_" + this->genome->id + ".log");
    }
};

/**
 * @brief Get inputs for genome.
 * @param candles Candle data for all time frames.
 * @param indicators Indicator data for all time frames.
 * @param base_currency_conversion_rate Conversion rate data when the base asset traded is different from the account currency.
 * @param position_infos Vector of position information.
 */
void Trader::look(CandlesData &candles, IndicatorsData &indicators, BaseCurrencyConversionRateData base_currency_conversion_rate, std::vector<PositionInfo> position_infos)
{
    std::vector<double> indicators_values = {};
    std::unordered_map<TimeFrame, std::vector<Indicator>> indicators_inputs = config.training.inputs.indicators;

    this->candles = candles;
    Candle last_candle = candles[config.strategy.timeframe].back();
    this->current_date = last_candle.date;
    this->current_base_currency_conversion_rate = base_currency_conversion_rate[last_candle.date];

    // Get the values of the indicators
    for (const auto &indicator : indicators_inputs)
    {
        for (const auto &ind : indicator.second)
        {
            indicators_values.push_back(indicators[indicator.first][ind.id].back());
        }
    }

    // Get the position information
    std::vector<double> position_info(position_infos.size(), 0);
    if (this->current_position != nullptr)
    {
        for (const auto &info : position_infos)
        {
            if (info == PositionInfo::TYPE)
            {
                if (this->current_position->side == PositionSide::LONG)
                {
                    position_info.push_back(1);
                }
                else if (this->current_position->side == PositionSide::SHORT)
                {
                    position_info.push_back(-1);
                }
                else
                {
                    position_info.push_back(0);
                }
            }
            else if (info == PositionInfo::PNL)
            {
                double position_pnl_percent = this->current_position->size > 0 ? this->current_position->pnl / this->balance : 0;
                position_info.push_back(position_pnl_percent);
            }
            else if (info == PositionInfo::DURATION)
            {
                // check the difference between the current date and the entry date in minutes
                std::time_t entry_date = std::mktime(&this->current_position->entry_date);
                double position_duration = std::difftime(this->current_date, entry_date) / get_time_frame_value(config.strategy.timeframe);
                position_info.push_back(position_duration);
            }
        }
    }

    // Concat the indicators values and the position information
    std::vector<double> inputs = {};
    inputs.insert(inputs.end(), indicators_values.begin(), indicators_values.end());
    inputs.insert(inputs.end(), position_info.begin(), position_info.end());

    this->vision = inputs;
}

/**
 * @brief Get the outputs from the neural network.
 */
void Trader::think()
{
    this->decisions = this->genome->feed_forward(this->vision);
}

/**
 * @brief Update the trader according to the outputs from the neural network.
 */
void Trader::update()
{
    // Increment the lifespan of the trader
    this->lifespan++;

    // Kill the traders that loose all the balance
    if (this->config.training.bad_trader_threshold.has_value() && balance <= this->stats.initial_balance * config.training.bad_trader_threshold.value())
    {
        this->dead = true;
        if (this->debug && this->logger != nullptr)
        {
            this->logger->info("[" + time_t_to_string(this->current_date) + "] [$" + std::to_string(this->balance) + "] Killed because of bad performance.");
        }
        return;
    }

    // Kill the traders that doesn't trades
    if (this->config.training.inactive_trader_threshold.has_value() && this->lifespan >= this->config.training.inactive_trader_threshold.value() && this->stats.total_trades == 0)
    {
        this->dead = true;
        if (this->debug && this->logger != nullptr)
        {
            this->logger->info("[" + time_t_to_string(this->current_date) + "] [$" + std::to_string(this->balance) + "] Killed because of inactivity.");
        }
        return;
    }

    // Update
    this->check_open_orders();
    this->check_position_liquidation();
    this->update_position_pnl();
    this->update_stats();
    this->trade();

    // Formula to calculate the score of the individual
    this->score = this->stats.total_net_profit;

    // Record the balance to history
    this->balance_history.push_back(this->balance);
}

/**
 * @brief Calculate the fitness of the trader.
 */
void Trader::calculate_fitness()
{
    EvaluationConfig goals = this->config.evaluation;

    bool use_nb_trade_eval = true;
    bool use_max_drawdown_eval = true;
    bool use_profit_factor_eval = true;
    bool use_win_rate_eval = true;
    bool use_average_profit_eval = true;

    double nb_trade_eval = 0;
    double max_drawdown_eval = 0;
    double profit_factor_eval = 0;
    double win_rate_eval = 0;
    double average_profit_eval = 0;

    double nb_trade_weight = 500;
    double max_drawdown_weight = 100;
    double profit_factor_weight = 100;
    double win_rate_weight = 100;
    double average_profit_weight = 100;

    if (goals.nb_trade_minimum.has_value() && stats.total_trades > 0)
    {
        int diff = abs(goals.nb_trade_minimum.value() - stats.total_trades);
        double nb_trade_eval;
        if (diff > 0)
        {
            nb_trade_eval = nb_trade_weight / diff;
        }
        else
        {
            nb_trade_eval = nb_trade_weight;
        }
    }

    if (goals.maximum_drawdown.has_value() && stats.max_drawdown < 0)
    {
        max_drawdown_eval = max_drawdown_weight / stats.max_drawdown;
    }

    if (goals.minimum_profit_factor.has_value() && stats.profit_factor != 0)
    {
        if (stats.profit_factor > *goals.minimum_profit_factor)
        {
            profit_factor_eval = profit_factor_weight;
        }
        else
        {
            profit_factor_eval = profit_factor_weight * (stats.profit_factor / goals.minimum_profit_factor.value());
        }
    }

    if (goals.minimum_winrate.has_value() && stats.total_trades > 0)
    {
        if (stats.win_rate < *goals.minimum_winrate)
        {
            win_rate_eval = win_rate_weight * (stats.win_rate / goals.minimum_winrate.value());
        }
        else
        {
            win_rate_eval = win_rate_weight;
        }
    }

    if (goals.average_profit.has_value() && stats.average_profit > 0)
    {
        double average_profit_percent = stats.average_profit / stats.initial_balance;
        if (average_profit_percent > goals.average_profit.value())
        {
            average_profit_eval = average_profit_weight;
        }
        else
        {
            average_profit_eval = average_profit_weight * (average_profit_percent / *goals.average_profit);
        }
    }

    if (!use_nb_trade_eval)
    {
        nb_trade_eval = 1;
        nb_trade_weight = 1;
    }

    if (!use_max_drawdown_eval)
    {
        max_drawdown_eval = 1;
        max_drawdown_weight = 1;
    }

    if (!use_profit_factor_eval)
    {
        profit_factor_eval = 1;
        profit_factor_weight = 1;
    }

    if (!use_win_rate_eval)
    {
        win_rate_eval = 1;
        win_rate_weight = 1;
    }

    if (!use_average_profit_eval)
    {
        average_profit_eval = 1;
        average_profit_weight = 1;
    }

    // ***************** FORMULA TO CALCULATE FITNESS ***************** //

    // this->fitness = (1 + nb_trade_eval * max_drawdown_eval * profit_factor_eval * win_rate_eval * average_profit_eval) / (1 + nb_trade_weight * max_drawdown_weight * profit_factor_weight * win_rate_weight * average_profit_weight);

    this->fitness = this->stats.total_trades * (this->stats.average_profit + 1) / (this->stats.average_loss + 1) * this->stats.win_rate;
}

/**
 * @brief Print the statistics of the trader.
 */
void Trader::print_stats()
{
    std::cout << "------------------------------ STATS -----------------------------" << std::endl;
    std::cout << "Initial balance: $" << decimal_floor(this->stats.initial_balance, 2) << std::endl;
    std::cout << "Final balance: $" << decimal_floor(this->stats.final_balance, 2) << std::endl;
    std::cout << "Total net profit: $" << decimal_floor(this->stats.total_net_profit, 2) << std::endl;
    std::cout << "Total profit: $" << decimal_floor(this->stats.total_profit, 2) << std::endl;
    std::cout << "Total loss: $" << decimal_floor(this->stats.total_loss, 2) << std::endl;
    std::cout << "Total fees: $" << decimal_floor(this->stats.total_fees, 2) << std::endl;
    std::cout << "Total trades: " << this->stats.total_trades << std::endl;
    std::cout << "Total long trades: " << this->stats.total_long_trades << std::endl;
    std::cout << "Total short trades: " << this->stats.total_short_trades << std::endl;
    std::cout << "Total winning trades: " << this->stats.total_winning_trades << std::endl;
    std::cout << "Total lost trades: " << this->stats.total_lost_trades << std::endl;
    std::cout << "Max drawdown: " << decimal_floor(-this->stats.max_drawdown * 100, 2) << "%" << std::endl;
    std::cout << "Win rate: " << decimal_floor(this->stats.win_rate * 100, 2) << "%" << std::endl;
    std::cout << "Long win rate: " << decimal_floor(this->stats.long_win_rate * 100, 2) << "%" << std::endl;
    std::cout << "Short win rate: " << decimal_floor(this->stats.short_win_rate * 100, 2) << "%" << std::endl;
    std::cout << "Average profit: $" << decimal_floor(this->stats.average_profit, 2) << std::endl;
    std::cout << "Average loss: $" << decimal_floor(this->stats.average_loss, 2) << std::endl;
    std::cout << "Profit factor: " << decimal_floor(this->stats.profit_factor, 2) << std::endl;
    std::cout << "Sharpe ratio: " << decimal_floor(this->stats.sharpe_ratio, 2) << std::endl;
    std::cout << "Sortino ratio: " << decimal_floor(this->stats.sortino_ratio, 2) << std::endl;
}

/**
 * @brief Trade according to the decision.
 */
void Trader::trade()
{
    int loop_interval_minutes = get_time_frame_value(this->config.strategy.timeframe);
    Candle last_candle = this->candles[this->config.strategy.timeframe].back();

    // Position information
    bool has_position = this->current_position != nullptr;
    bool has_long_position = has_position && this->current_position->side == PositionSide::LONG;
    bool has_short_position = has_position && this->current_position->side == PositionSide::SHORT;

    // Check the duration of the current trade
    if (has_position && config.strategy.maximum_trade_duration)
    {
        std::tm entry_date = this->current_position->entry_date;
        TimeFrame loop_interval = config.strategy.timeframe;
        int max_trade_duration_minutes = config.strategy.maximum_trade_duration.value() * get_time_frame_value(loop_interval);

        // Check if the position has reached the maximum trade duration
        if (std::chrono::system_clock::from_time_t(this->current_date) > std::chrono::system_clock::from_time_t(std::mktime(&entry_date)) + std::chrono::minutes(max_trade_duration_minutes))
        {
            this->close_position_by_market(last_candle.close);
        }
    }

    // Decision taken
    double maximum = *std::max_element(this->decisions.begin(), this->decisions.end());
    bool enter_long = maximum == this->decisions[0];
    bool enter_short = maximum == this->decisions[1];
    bool close_long = maximum == this->decisions[2];
    bool close_short = maximum == this->decisions[3];
    bool wait = maximum == this->decisions[4];

    // Don't trade
    if (wait)
    {
        return;
    }

    // Check if the trader can trade at the moment
    bool schedule_is_ok = true;
    std::tm current_time = *std::localtime(&this->current_date);
    TradingSchedule trading_schedule = this->config.strategy.trading_schedule;
    schedule_is_ok = is_on_trading_schedule(current_time, trading_schedule);

    // Check if the spread is ok
    bool spread_is_ok = true;
    if (this->config.strategy.maximum_spread && last_candle.spread > this->config.strategy.maximum_spread)
    {
        spread_is_ok = false;
    }

    // Check if the time after the previous trade is ok
    bool time_after_previous_trade_is_ok = true;
    if (this->last_position_date)
    {
        std::tm last_position_date = *std::localtime(&this->last_position_date);
        int minutes_after_previous_trade = std::difftime(this->current_date, std::mktime(&last_position_date)) / get_time_frame_value(this->config.strategy.timeframe);
        time_after_previous_trade_is_ok = minutes_after_previous_trade > this->config.strategy.minimum_duration_before_next_trade;
    }

    // Check if the trader can trade now
    bool can_trade_now = schedule_is_ok && spread_is_ok && time_after_previous_trade_is_ok;

    // Check if the trader can close a trade
    bool can_close_position = true;
    if (this->current_position == nullptr)
    {
        can_close_position = false;
    }
    else if (!this->config.strategy.minimum_trade_duration.has_value())
    {
        can_close_position = true;
    }
    else
    {
        std::chrono::system_clock::time_point current_date = std::chrono::system_clock::from_time_t(this->current_date);
        std::chrono::system_clock::time_point entry_date = std::chrono::system_clock::from_time_t(std::mktime(&this->current_position->entry_date));
        can_close_position = current_date > entry_date + std::chrono::minutes(loop_interval_minutes) * this->config.strategy.minimum_trade_duration.value();
    }

    if (can_trade_now)
    {
        if (has_position)
        {
            if (has_long_position && close_long && can_close_position)
            {
                this->close_position_by_market(last_candle.close);
            }
            else if (has_short_position && close_short && can_close_position)
            {
                this->close_position_by_market(last_candle.close);
            }
        }
        else
        {
            if (enter_long)
            {
                // Calculate order parameters
                auto order_prices = calculate_tp_sl_price(last_candle.close, PositionSide::LONG, this->config.strategy.take_profit_stop_loss_config, this->symbol_info);
                double tp_price = std::get<0>(order_prices);
                double sl_price = std::get<1>(order_prices);
                double sl_pips = calculate_pips(last_candle.close, sl_price, this->symbol_info);
                double size = calculate_position_size(this->balance, this->config.strategy.maximum_risk, last_candle.close, sl_pips, this->symbol_info, this->current_base_currency_conversion_rate);

                // Post orders
                this->open_position_by_market(last_candle.close, size, OrderSide::LONG);
                this->create_open_order(OrderType::TAKE_PROFIT, OrderSide::SHORT, tp_price);
                this->create_open_order(OrderType::STOP_LOSS, OrderSide::SHORT, sl_price);
            }
            else if (enter_short)
            {
                // Calculate order parameters
                auto order_prices = calculate_tp_sl_price(last_candle.close, PositionSide::SHORT, this->config.strategy.take_profit_stop_loss_config, this->symbol_info);
                double tp_price = std::get<0>(order_prices);
                double sl_price = std::get<1>(order_prices);
                double sl_pips = calculate_pips(last_candle.close, sl_price, this->symbol_info);
                double size = calculate_position_size(this->balance, this->config.strategy.maximum_risk, last_candle.close, sl_pips, this->symbol_info, this->current_base_currency_conversion_rate);

                // Post orders
                this->open_position_by_market(last_candle.close, size, OrderSide::SHORT);
                this->create_open_order(OrderType::TAKE_PROFIT, OrderSide::LONG, tp_price);
                this->create_open_order(OrderType::STOP_LOSS, OrderSide::LONG, sl_price);
            }
        }
    }
    else if (has_position)
    {
        if (has_long_position && close_long && can_close_position)
        {
            this->close_position_by_market(last_candle.close);
        }
        else if (has_short_position && close_short && can_close_position)
        {
            this->close_position_by_market(last_candle.close);
        }
    }
}

/**
 * @brief Open a position by market.
 * @param price Price of the market order.
 * @param size Size of the market order.
 * @param side Side of the market order.
 */
void Trader::open_position_by_market(double price, double size, OrderSide side)
{
    double fees = calculate_commission(this->symbol_info.commission_per_lot, size, this->current_base_currency_conversion_rate);

    if (this->current_position != nullptr)
    {
        return;
    };

    if (side == OrderSide::LONG)
    {
        std::tm date_tm = *std::localtime(&this->current_date);
        this->stats.total_trades++;
        this->stats.total_long_trades++;
        this->stats.total_fees += fees;
        balance -= fees;
        this->current_position = new Position{
            .entry_date = date_tm,
            .entry_price = price,
            .size = size,
            .side = PositionSide::LONG,
            .pnl = 0.0};
        if (debug && this->logger != nullptr)
        {
            this->logger->info("[" + time_t_to_string(this->current_date) + "] [$" + std::to_string(balance) + "] : Open long position by market at $" + std::to_string(price) + " with " + std::to_string(size) + " lots and $" + std::to_string(fees) + " of fees.");
        }
    }
    if (side == OrderSide::SHORT)
    {
        std::tm date_tm = *std::localtime(&this->current_date);
        this->stats.total_trades++;
        this->stats.total_short_trades++;
        this->stats.total_fees += fees;
        balance -= fees;
        this->current_position = new Position{
            .entry_date = date_tm,
            .entry_price = price,
            .size = size,
            .side = PositionSide::SHORT,
            .pnl = 0.0};
        if (debug && this->logger != nullptr)
        {
            this->logger->info("[" + time_t_to_string(this->current_date) + "] [$" + std::to_string(balance) + "] : Open short position by market at $" + std::to_string(price) + " with " + std::to_string(size) + " lots and $" + std::to_string(fees) + " of fees.");
        }
    }
}

/**
 * @brief Open a position by limit.
 * @param price Price of the limit order.
 * @param size Size of the limit order.
 * @param side Side of the limit order.
 */
void Trader::close_position_by_market(double price)
{
    if (price != 0.0)
    {
        this->update_position_pnl(price);
    }
    else
    {
        Candle last_candle = this->candles[this->config.strategy.timeframe].back();
        price = last_candle.close;
    }

    if (this->current_position != nullptr)
    {
        double fees = calculate_commission(this->symbol_info.commission_per_lot, this->current_position->size, this->current_base_currency_conversion_rate);

        this->balance = std::max(0.0, decimal_round(this->balance + this->current_position->pnl - fees, 2));
        this->stats.total_fees += fees;

        if (this->current_position->pnl >= 0)
        {
            this->stats.total_profit += this->current_position->pnl;
            this->stats.total_winning_trades++;
            if (this->current_position->side == PositionSide::LONG)
            {
                this->stats.total_winning_long_trades++;
            }
            else
            {
                this->stats.total_winning_short_trades++;
            }
        }
        else
        {
            this->stats.total_loss += abs(this->current_position->pnl);
            this->stats.total_lost_trades++;
            if (this->current_position->side == PositionSide::LONG)
            {
                this->stats.total_lost_long_trades++;
            }
            else
            {
                this->stats.total_lost_short_trades++;
            }
        }

        if (this->debug && this->logger != nullptr)
        {
            this->logger->info("[" + time_t_to_string(this->current_date) + "] [$" + std::to_string(this->balance) + "] : Close position by market at $" + std::to_string(price) + " with $" + std::to_string(this->current_position->pnl) + " of profit and $" + std::to_string(fees) + " of fees.");
        }

        this->current_position = nullptr;
        this->last_position_date = this->current_date;
        this->close_open_orders();
    }
}

/**
 * @brief Close a position by limit.
 * @param price Price of the limit order.
 */
void Trader::close_position_by_limit(double price)
{
    if (price)
    {
        this->update_position_pnl(price);
    }
    else
    {
        Candle last_candle = this->candles[this->config.strategy.timeframe].back();
        price = last_candle.close;
    }

    if (this->current_position != nullptr)
    {
        double fees = calculate_commission(this->symbol_info.commission_per_lot, this->current_position->size, this->current_base_currency_conversion_rate);

        this->balance = std::max(0.0, decimal_round(this->balance + this->current_position->pnl - fees, 2));
        this->stats.total_fees += fees;

        if (this->current_position->pnl >= 0)
        {
            this->stats.total_profit += this->current_position->pnl;
            this->stats.total_winning_trades++;
            if (this->current_position->side == PositionSide::LONG)
            {
                this->stats.total_winning_long_trades++;
            }
            else
            {
                this->stats.total_winning_short_trades++;
            }
        }
        else
        {
            this->stats.total_loss += abs(this->current_position->pnl);
            this->stats.total_lost_trades++;
            if (this->current_position->side == PositionSide::LONG)
            {
                this->stats.total_lost_long_trades++;
            }
            else
            {
                this->stats.total_lost_short_trades++;
            }
        }

        if (this->debug && this->logger != nullptr)
        {
            this->logger->info("[" + time_t_to_string(this->current_date) + "] [$" + std::to_string(this->balance) + "] : Close position by limit at $" + std::to_string(price) + " with $" + std::to_string(this->current_position->pnl) + " of profit and $" + std::to_string(fees) + " of fees.");
        }

        this->current_position = nullptr;
        this->last_position_date = this->current_date;
        this->close_open_orders();
    }
}

/**
 * @brief Create an open order.
 * @param type Type of the order.
 * @param side Side of the order.
 * @param price Price of the order.
 */
void Trader::create_open_order(OrderType type, OrderSide side, double price)
{
    Order order = {
        .side = side,
        .type = type,
        .price = price};
    this->open_orders.push_back(order);

    if (this->debug && this->logger != nullptr)
    {
        std::string order_type = type == OrderType::TAKE_PROFIT ? "take profit" : "stop loss";
        std::string order_side = side == OrderSide::LONG ? "long" : "short";
        this->logger->info("[" + time_t_to_string(this->current_date) + "] [$" + std::to_string(this->balance) + "] : Create " + order_type + " order at $" + std::to_string(price) + " for " + order_side + " position.");
    }
}

/**
 * @brief Check the open orders and activate them if the price crosses.
 */
void Trader::check_open_orders()
{
    Candle last_candle = this->candles[this->config.strategy.timeframe].back();

    if (this->open_orders.size() > 0)
    {
        std::vector<Order> long_orders = {};
        std::vector<Order> short_orders = {};

        for (const auto &order : this->open_orders)
        {
            if (order.side == OrderSide::LONG)
            {
                long_orders.push_back(order);
            }
            else
            {
                short_orders.push_back(order);
            }
        }

        if (this->current_position != nullptr)
        {
            if (this->current_position->side == PositionSide::LONG)
            {
                for (int i = 0; i < short_orders.size(); i++)
                {
                    if (last_candle.high >= short_orders[i].price && last_candle.low <= short_orders[i].price)
                    {
                        if (short_orders[i].type == OrderType::TAKE_PROFIT)
                        {
                            this->close_position_by_limit(short_orders[i].price);
                        }
                        if (short_orders[i].type == OrderType::STOP_LOSS)
                        {
                            this->close_position_by_market(short_orders[i].price);
                        }
                    }
                }
            }
            else if (this->current_position->side == PositionSide::SHORT)
            {
                for (int i = 0; i < long_orders.size(); i++)
                {
                    if (last_candle.high <= long_orders[i].price && last_candle.low >= long_orders[i].price)
                    {
                        if (long_orders[i].type == OrderType::TAKE_PROFIT)
                        {
                            this->close_position_by_limit(long_orders[i].price);
                        }
                        if (long_orders[i].type == OrderType::STOP_LOSS)
                        {
                            this->close_position_by_market(long_orders[i].price);
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Close all the open orders.
 */
void Trader::close_open_orders()
{
    this->open_orders.clear();
}

/**
 * @brief Check if the position got liquidated.
 */
void Trader::check_position_liquidation()
{
    if (this->current_position != nullptr)
    {
        double liquidation_price = calculate_liquidation_price(*this->current_position, this->config.general.leverage, this->symbol_info);
        double current_price = this->candles[this->config.strategy.timeframe].back().close;

        if (this->current_position->side == PositionSide::LONG)
        {
            if (current_price <= liquidation_price)
            {
                this->close_position_by_market(liquidation_price);
                this->close_open_orders();
            }
        }
        else if (this->current_position->side == PositionSide::SHORT)
        {
            if (current_price >= liquidation_price)
            {
                this->close_position_by_market(liquidation_price);
                this->close_open_orders();
            }
        }
    }
}

/**
 * @brief Update the position PNL.
 * @param price Current price of the asset.
 */
void Trader::update_position_pnl(double price)
{
    if (this->current_position != nullptr)
    {
        double current_price = price != 0.0 ? price : this->candles[this->config.strategy.timeframe].back().close;
        this->current_position->pnl = calculate_profit_loss(current_price, *this->current_position, this->symbol_info, this->current_base_currency_conversion_rate);
    }
}

/**
 * @brief Update the trader statistics.
 */
void Trader::update_stats()
{
    // Update the final capital
    this->stats.final_balance = this->balance;

    // Update the sharpe ratio
    this->stats.sharpe_ratio = 0;

    // Update the sortino ratio
    this->stats.sortino_ratio = 0;

    // Update the total net profit
    if (this->stats.total_loss != 0 || this->stats.total_fees != 0)
    {
        this->stats.total_net_profit = this->stats.total_profit - this->stats.total_loss - this->stats.total_fees;
    }

    auto calculate_drawdown = [&](std::vector<double> balance_history)
    {
        if (balance_history.empty() || balance_history.size() < 2)
        {
            return 0.0; // No drawdown if there are fewer than two data points
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

        return max_drawdown;
    };

    // Update the drawdown
    this->stats.max_drawdown = calculate_drawdown(this->balance_history);

    // Update the winrate
    if (this->stats.total_trades > 0)
    {
        this->stats.win_rate = static_cast<double>(this->stats.total_winning_trades) / static_cast<double>(this->stats.total_trades);
    }

    // Update the winrate for longs
    if (this->stats.total_long_trades > 0)
    {
        this->stats.long_win_rate = static_cast<double>(this->stats.total_winning_long_trades) / static_cast<double>(this->stats.total_long_trades);
    }

    // Update the winrate for shorts
    if (this->stats.total_short_trades > 0)
    {
        this->stats.short_win_rate = static_cast<double>(this->stats.total_winning_short_trades) / static_cast<double>(this->stats.total_short_trades);
    }

    // Update the average profit per trade
    if (this->stats.total_winning_trades > 0)
    {
        this->stats.average_profit = this->stats.total_profit / static_cast<double>(this->stats.total_winning_trades);
    }

    // Update the average loss per trade
    if (this->stats.total_lost_trades > 0)
    {
        this->stats.average_loss = this->stats.total_loss / static_cast<double>(this->stats.total_lost_trades);
    }

    // Update the profit factor
    if (this->stats.average_profit != 0 && this->stats.average_loss != 0)
    {
        this->stats.profit_factor = this->stats.average_profit / this->stats.average_loss;
    }
}