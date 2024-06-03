#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>
#include <random>
#include "utils/logger.hpp"
#include "utils/time_frame.hpp"
#include "utils/math.hpp"
#include "utils/vectors.hpp"
#include "utils/date_conversion.hpp"
#include "libs/gnuplot-iostream.hpp"
#include "neat/genome.hpp"
#include "trading/trading_schedule.hpp"
#include "trading/trading_tools.hpp"
#include "indicators/utils.hpp"
#include "symbols.hpp"
#include "trader.hpp"

/**
 * @brief Constructor for the Trader class.
 * @param genome Genome object.
 * @param config Configuration object.
 * @param logger Logger object.
 */
Trader::Trader(neat::Genome *genome, Config config, Logger *logger)
{
    this->config = config;
    this->symbol_info = symbol_infos[config.general.symbol];
    this->logger = logger;

    // Vision
    this->candles = {};
    this->current_base_currency_conversion_rate = 1.0;

    // Balance and history
    this->balance = config.general.initial_balance;
    this->balance_history = {};
    this->trades_history = {};
    this->open_orders = {};
    this->current_position = nullptr;
    this->duration_in_position = 0;
    this->duration_without_trade = this->config.strategy.minimum_duration_before_next_trade.value_or(0);

    // Statistics of the trader
    this->stats = {
        .initial_balance = config.general.initial_balance,
        .final_balance = config.general.initial_balance,
        .performance = 0,
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

    // Neat stuffs
    this->genome = genome;
    this->generation = 0;
    this->fitness = 0;
    this->score = 0;
    this->vision = {};
    this->decisions = {};
    this->lifespan = 0;
    this->dead = false;
};

/**
 * @brief Get inputs for genome.
 * @param indicators_data Indicator data for all time frames.
 * @param base_currency_conversion_rate Conversion rate when the base asset traded is different from the account currency.
 * @param position_infos Vector of position information.
 */
void Trader::look(IndicatorsData &indicators_data, double base_currency_conversion_rate, std::vector<PositionInfo> position_infos)
{
    std::vector<double> indicators_values = {};
    std::map<TimeFrame, std::vector<Indicator *>> indicators_inputs = config.training.inputs.indicators;
    this->current_base_currency_conversion_rate = base_currency_conversion_rate;

    // Get the values of the indicators
    for (const auto &[tf, indicators] : indicators_inputs)
    {
        for (const auto &indicator : indicators)
        {
            indicators_values.push_back(indicators_data[tf][indicator->id].back());
        }
    }

    // Get the position information
    std::vector<double> position_info = {};
    for (const auto &info : position_infos)
    {
        if (this->current_position == nullptr)
        {
            position_info.push_back(0);
        }
        else if (info == PositionInfo::TYPE)
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
            if (this->config.strategy.maximum_trade_duration.has_value())
            {
                position_info.push_back(static_cast<double>(this->duration_in_position) / static_cast<double>(this->config.strategy.maximum_trade_duration.value_or(1)));
            }
            else
            {
                position_info.push_back(this->duration_in_position);
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
 * @param candles Candle data for all time frames.
 */
void Trader::update(CandlesData &candles)
{
    this->candles = candles;
    this->current_date = candles[this->config.strategy.timeframe].back().date;

    // Increment the lifespan of the trader
    this->lifespan++;

    // Increment the position duration
    if (this->current_position != nullptr)
    {
        this->duration_in_position++;
    }
    else
    {
        this->duration_without_trade++;
    }

    // Kill the traders that loose all the balance
    bool bad_trader = this->config.training.bad_trader_threshold.has_value() && balance <= this->stats.initial_balance * config.training.bad_trader_threshold.value();

    // Kill the traders that doesn't trades
    bool inactive_trader = this->config.training.inactive_trader_threshold.has_value() && this->lifespan >= this->config.training.inactive_trader_threshold.value() && this->stats.total_trades == 0;

    if (bad_trader || inactive_trader)
    {
        this->dead = true;

        if (this->logger != nullptr)
        {
            if (bad_trader)
            {
                this->logger->info("[" + time_t_to_string(this->current_date) + "] [" + std::to_string(this->balance) + "] Killed because of bad performance.");
            }
            else if (inactive_trader)
            {
                this->logger->info("[" + time_t_to_string(this->current_date) + "] [" + std::to_string(this->balance) + "] Killed because of inactivity.");
            }
        }

        return;
    }

    // Update
    this->update_position_pnl();
    this->check_open_orders();
    this->check_position_liquidation();

    // Record the balance to history
    this->balance_history.push_back(this->balance);
}

/**
 * @brief Check if the trader can trade.
 */
bool Trader::can_trade()
{
    Candle last_candle = this->candles[this->config.strategy.timeframe].back();

    // Check if the trader can trade at the moment
    bool schedule_is_ok = true;
    if (this->config.strategy.trading_schedule.has_value())
    {
        TradingSchedule trading_schedule = this->config.strategy.trading_schedule.value();
        schedule_is_ok = is_on_trading_schedule(this->current_date, trading_schedule);
    }

    // Check the number of trades made today
    bool number_of_trades_per_day_is_ok = true;
    if (this->config.strategy.maximum_trades_per_day.has_value())
    {
        int number_of_trades_today = 0;
        for (const auto &trade : this->trades_history)
        {
            struct tm current_date_tm = time_t_to_tm(this->current_date);
            int current_year = current_date_tm.tm_year;
            int current_month = current_date_tm.tm_mon;
            int current_day = current_date_tm.tm_mday;

            struct tm trade_date_tm = time_t_to_tm(trade.exit_date);
            int trade_year = trade_date_tm.tm_year;
            int trade_month = trade_date_tm.tm_mon;
            int trade_day = trade_date_tm.tm_mday;

            if (trade_year == current_year && trade_month == current_month && trade_day == current_day)
            {
                number_of_trades_today++;
            }
        }
        number_of_trades_per_day_is_ok = number_of_trades_today < this->config.strategy.maximum_trades_per_day.value();
    }

    // Check if the spread is ok
    bool spread_is_ok = true;
    if (this->config.strategy.maximum_spread.has_value())
    {
        spread_is_ok = last_candle.spread <= this->config.strategy.maximum_spread.value();
    }

    // Check if the time after the previous trade is ok
    bool time_after_previous_trade_is_ok = true;
    if (this->config.strategy.minimum_duration_before_next_trade.has_value())
    {
        time_after_previous_trade_is_ok = this->duration_without_trade >= this->config.strategy.minimum_duration_before_next_trade.value();
    }

    // Check if the trader can trade now
    bool can_trade_now = schedule_is_ok && number_of_trades_per_day_is_ok && spread_is_ok && time_after_previous_trade_is_ok;

    return can_trade_now;
}

/**
 * @brief Trade according to the decision.
 * @return 1 if the trader opened a long position, 2 if the trader opened a short position, 3 if the trader closed a the position, 0 if the trader wait.
 */
int Trader::trade()
{
    int loop_interval_minutes = get_time_frame_in_minutes(this->config.strategy.timeframe);
    Candle last_candle = this->candles[this->config.strategy.timeframe].back();

    // Position information
    bool has_position = this->current_position != nullptr;
    bool has_long_position = has_position && this->current_position->side == PositionSide::LONG;
    bool has_short_position = has_position && this->current_position->side == PositionSide::SHORT;

    // Decision taken
    std::vector<double>::iterator result = std::max_element(this->decisions.begin(), this->decisions.end());
    int decision = std::distance(this->decisions.begin(), result);
    bool want_long = decision == 0;
    bool want_short = decision == 1;
    bool wait = decision == 2;

    if (!wait)
    {
        bool can_trade_now = this->can_trade();

        // Check if the trader can close a trade
        bool can_close_position = this->config.strategy.can_close_trade.value_or(true);
        if (!has_position)
        {
            can_close_position = false;
        }
        else if (this->config.strategy.minimum_trade_duration.has_value())
        {
            can_close_position = this->duration_in_position >= this->config.strategy.minimum_trade_duration.value();
        }

        if (can_trade_now)
        {
            if (has_position)
            {
                if (has_long_position && want_short && can_close_position)
                {
                    this->close_position_by_market(last_candle.close);
                    return 3; // Close
                }
                else if (has_short_position && want_long && can_close_position)
                {
                    this->close_position_by_market(last_candle.close);
                    return 3; // Close
                }
            }
            else
            {
                if (want_long)
                {
                    // Calculate order parameters
                    std::vector<Candle> candles = this->candles[this->config.strategy.timeframe];
                    auto order_prices = calculate_tp_sl_price(last_candle.close, candles, PositionSide::LONG, this->config.strategy.take_profit_stop_loss_config, this->symbol_info);
                    double tp_price = std::get<0>(order_prices);
                    double sl_price = std::get<1>(order_prices);
                    double sl_pips = calculate_pips(last_candle.close, sl_price, this->symbol_info);
                    double size = calculate_position_size(this->balance, this->config.strategy.risk_per_trade, last_candle.close, sl_pips, this->symbol_info, this->current_base_currency_conversion_rate);

                    // Post orders
                    if (size > 0.0)
                    {
                        this->open_position_by_market(last_candle.close, size, OrderSide::LONG);
                        this->create_open_order(OrderType::TAKE_PROFIT, OrderSide::SHORT, tp_price);
                        this->create_open_order(OrderType::STOP_LOSS, OrderSide::SHORT, sl_price);
                        return 1; // Long
                    }
                }
                else if (want_short)
                {
                    // Calculate order parameters
                    std::vector<Candle> candles = this->candles[this->config.strategy.timeframe];
                    auto order_prices = calculate_tp_sl_price(last_candle.close, candles, PositionSide::SHORT, this->config.strategy.take_profit_stop_loss_config, this->symbol_info);
                    double tp_price = std::get<0>(order_prices);
                    double sl_price = std::get<1>(order_prices);
                    double sl_pips = calculate_pips(last_candle.close, sl_price, this->symbol_info);
                    double size = calculate_position_size(this->balance, this->config.strategy.risk_per_trade, last_candle.close, sl_pips, this->symbol_info, this->current_base_currency_conversion_rate);

                    // Post orders
                    if (size > 0.0)
                    {
                        this->open_position_by_market(last_candle.close, size, OrderSide::SHORT);
                        this->create_open_order(OrderType::TAKE_PROFIT, OrderSide::LONG, tp_price);
                        this->create_open_order(OrderType::STOP_LOSS, OrderSide::LONG, sl_price);
                        return 2; // Short
                    }
                }
            }
        }
        else if (has_position)
        {
            if (has_long_position && want_short && can_close_position)
            {
                this->close_position_by_market(last_candle.close);
                return 3; // Close
            }
            else if (has_short_position && want_long && can_close_position)
            {
                this->close_position_by_market(last_candle.close);
                return 3; // Close
            }
        }
    }

    // Check the duration of the current trade
    if (has_position && config.strategy.maximum_trade_duration.has_value())
    {
        // Check if the position has reached the maximum trade duration
        if (this->duration_in_position >= config.strategy.maximum_trade_duration.value())
        {
            this->close_position_by_market(last_candle.close);
            return 3; // Close
        }
    }

    return 0; // Wait
}

/**
 * @brief Open a position by market.
 * @param price Price of the market order.
 * @param size Size of the market order.
 * @param side Side of the market order.
 */
void Trader::open_position_by_market(double price, double size, OrderSide side)
{
    if (this->current_position != nullptr)
    {
        return;
    };

    double fees = calculate_commission(this->symbol_info.commission_per_lot, size, this->current_base_currency_conversion_rate);

    if (side == OrderSide::LONG)
    {
        this->stats.total_trades++;
        this->stats.total_long_trades++;
        this->balance -= fees;
        this->duration_in_position = 0;
        this->current_position = new Position{
            .side = PositionSide::LONG,
            .size = size,
            .entry_price = price,
            .entry_date = this->current_date,
            .pnl = 0.0,
        };
        this->trades_history.push_back(Trade{
            .side = PositionSide::LONG,
            .entry_date = this->current_position->entry_date,
            .entry_price = this->current_position->entry_price,
            .size = this->current_position->size,
            .fees = fees,
            .closed = false,
        });

        if (this->logger != nullptr)
        {
            this->logger->info("[" + time_t_to_string(this->current_date) + "] [" + std::to_string(balance) + "] : Open long position by market at " + std::to_string(price) + " with " + std::to_string(size) + " lots and " + std::to_string(fees) + " of fees.");
        }
    }
    else if (side == OrderSide::SHORT)
    {
        this->stats.total_trades++;
        this->stats.total_short_trades++;
        this->balance -= fees;
        this->duration_in_position = 0;
        this->current_position = new Position{
            .side = PositionSide::SHORT,
            .size = size,
            .entry_price = price,
            .entry_date = this->current_date,
            .pnl = 0.0,
        };
        this->trades_history.push_back(Trade{
            .side = PositionSide::SHORT,
            .entry_date = this->current_position->entry_date,
            .entry_price = this->current_position->entry_price,
            .size = this->current_position->size,
            .fees = fees,
            .closed = false,
        });

        if (this->logger != nullptr)
        {
            this->logger->info("[" + time_t_to_string(this->current_date) + "] [" + std::to_string(balance) + "] : Open short position by market at " + std::to_string(price) + " with " + std::to_string(size) + " lots and " + std::to_string(fees) + " of fees.");
        }
    }
}

/**
 * @brief Calculate the fitness of the trader.
 */
void Trader::calculate_fitness()
{
    EvaluationConfig goals = this->config.evaluation;

    double nb_trades_eval = 0;
    double max_drawdown_eval = 0;
    double profit_factor_eval = 0;
    double win_rate_eval = 0;
    double expected_return_per_day_eval = 0;
    double expected_return_per_month_eval = 0;
    double expected_return_eval = 0;

    double nb_trades_weight = 1;
    double max_drawdown_weight = 1;
    double profit_factor_weight = 1;
    double win_rate_weight = 1;
    double expected_return_per_day_weight = 1;
    double expected_return_per_month_weight = 1;
    double expected_return_weight = 1;

    // Select only closed trade
    std::vector<Trade> closed_trades = {};
    for (const auto &trade : this->trades_history)
    {
        if (trade.closed)
        {
            closed_trades.push_back(trade);
        }
    }

    if (goals.nb_trades_per_day.has_value())
    {
        // Calculate the number of trades per day
        std::map<std::string, int> daily_trades = {};
        for (const auto &trade : closed_trades)
        {
            std::string date_key = time_t_to_string(trade.exit_date, "%Y-%m-%d");
            daily_trades[date_key]++;
        }

        int nb_days = daily_trades.size();
        for (const auto &[day, nb_trade] : daily_trades)
        {
            double diff = 10 * std::abs(goals.nb_trades_per_day.value() - nb_trade);
            nb_trades_eval += nb_trades_weight / (nb_days * std::exp(diff));
        }
    }

    if (goals.maximum_drawdown.has_value())
    {
        double diff = 10 * std::max(0.0, stats.max_drawdown - goals.maximum_drawdown.value());
        max_drawdown_eval = max_drawdown_weight / std::exp(diff);
    }

    if (goals.minimum_profit_factor.has_value())
    {
        double diff = 10 * std::max(0.0, goals.minimum_profit_factor.value() - stats.profit_factor);
        profit_factor_eval = profit_factor_weight / std::exp(diff);
    }

    if (goals.minimum_winrate.has_value())
    {
        double diff = 10 * std::max(0.0, goals.minimum_winrate.value() - stats.win_rate);
        win_rate_eval = win_rate_weight / std::exp(diff);
    }

    if (goals.expected_return_per_day.has_value())
    {
        std::map<std::string, double> daily_returns = {};
        for (const auto &trade : closed_trades)
        {
            double trade_return = trade.pnl_percent;
            std::string date_key = time_t_to_string(trade.exit_date, "%Y-%m-%d");
            daily_returns[date_key] = trade_return;
        }

        int nb_days = daily_returns.size();
        for (const auto &daily_return : daily_returns)
        {
            double diff = 10 * std::max(0.0, goals.expected_return_per_day.value() - daily_return.second);
            expected_return_per_day_eval += (expected_return_per_day_weight * nb_days) / (nb_days * std::exp(diff));
        }
    }

    if (goals.expected_return_per_month.has_value())
    {
        std::map<std::string, double> monthly_returns = {};
        for (const auto &trade : closed_trades)
        {
            double trade_return = trade.pnl_percent;
            std::string date_key = time_t_to_string(trade.exit_date, "%Y-%m");
            monthly_returns[date_key] += trade_return;
        }

        int nb_months = monthly_returns.size();
        for (const auto &monthly_return : monthly_returns)
        {
            double diff = 10 * std::max(0.0, goals.expected_return_per_month.value() - monthly_return.second);
            expected_return_per_month_eval += (expected_return_per_month_weight * nb_months) / (nb_months * std::exp(diff));
        }
    }

    if (goals.expected_return.has_value())
    {
        double diff = 10 * std::max(0.0, goals.expected_return.value() - stats.performance);
        expected_return_eval = expected_return_eval / std::exp(diff);
    }

    // ***************** FORMULA TO CALCULATE FITNESS ***************** //

    this->fitness = 1;

    if (this->trades_history.empty())
    {
        this->fitness = 0;
        return;
    }
    if (goals.nb_trades_per_day.has_value())
    {
        this->fitness *= nb_trades_eval;
    }
    if (goals.maximum_drawdown.has_value())
    {
        this->fitness *= max_drawdown_eval;
    }
    if (goals.minimum_profit_factor.has_value())
    {
        this->fitness *= profit_factor_eval;
    }
    if (goals.minimum_winrate.has_value())
    {
        this->fitness *= win_rate_eval;
    }
    if (goals.expected_return_per_day.has_value())
    {
        this->fitness *= expected_return_per_day_eval;
    }
    if (goals.expected_return_per_month.has_value())
    {
        this->fitness *= expected_return_per_month_eval;
    }
    if (goals.expected_return.has_value())
    {
        this->fitness *= expected_return_eval;
    }
}

/**
 * @brief Calculate the score of the trader.
 */
void Trader::calculate_score()
{
    this->score = this->stats.total_net_profit;
}

/**
 * @brief Calcule the trader statistics.
 */
void Trader::calculate_stats()
{
    // Select only closed trade
    std::vector<Trade> closed_trades = {};
    for (const auto &trade : this->trades_history)
    {
        if (trade.closed)
        {
            closed_trades.push_back(trade);
        }
    }

    // Update the total trades
    this->stats.total_trades = closed_trades.size();

    // Calcualte the number of long/short trades
    this->stats.total_long_trades = std::count_if(closed_trades.begin(), closed_trades.end(), [](Trade trade)
                                                  { return trade.side == PositionSide::LONG; });
    this->stats.total_short_trades = this->stats.total_trades - this->stats.total_long_trades;

    // Update the stats of the trades
    for (const auto &trade : closed_trades)
    {
        if (trade.pnl >= 0)
        {
            this->stats.total_profit += trade.pnl;
            this->stats.total_winning_trades++;
            this->stats.total_fees += trade.fees;
            if (trade.side == PositionSide::LONG)
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
            this->stats.total_loss += abs(trade.pnl);
            this->stats.total_lost_trades++;
            this->stats.total_fees += trade.fees;
            if (trade.side == PositionSide::LONG)
            {
                this->stats.total_lost_long_trades++;
            }
            else
            {
                this->stats.total_lost_short_trades++;
            }
        }
    }

    // Calculate the final capital
    this->stats.final_balance = this->balance;

    // Calculate the performance
    this->stats.performance = (this->stats.final_balance - this->stats.initial_balance) / this->stats.initial_balance;

    // Calculate the total net profit
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

    // Calculate the drawdown
    this->stats.max_drawdown = calculate_drawdown(this->balance_history);

    // Calculate the winrate
    if (closed_trades.size() > 0)
    {
        this->stats.win_rate = static_cast<double>(this->stats.total_winning_trades) / static_cast<double>(this->trades_history.size());
    }

    // Calculate the winrate for longs
    if (this->stats.total_long_trades > 0)
    {
        this->stats.long_win_rate = static_cast<double>(this->stats.total_winning_long_trades) / static_cast<double>(this->stats.total_long_trades);
    }

    // Calculate the winrate for shorts
    if (this->stats.total_short_trades > 0)
    {
        this->stats.short_win_rate = static_cast<double>(this->stats.total_winning_short_trades) / static_cast<double>(this->stats.total_short_trades);
    }

    // Calculate the average profit per trade
    if (this->stats.total_winning_trades > 0)
    {
        this->stats.average_profit = this->stats.total_profit / static_cast<double>(this->stats.total_winning_trades);
    }

    // Calculate the average loss per trade
    if (this->stats.total_lost_trades > 0)
    {
        this->stats.average_loss = this->stats.total_loss / static_cast<double>(this->stats.total_lost_trades);
    }

    // Calculate the profit factor
    if (this->stats.average_profit != 0 && this->stats.average_loss != 0)
    {
        this->stats.profit_factor = (this->stats.win_rate * this->stats.average_profit) / ((1 - this->stats.win_rate) * this->stats.average_loss);
    }

    // Calculate the maximum profit
    if (this->stats.total_winning_trades > 0)
    {
        this->stats.max_profit = (*std::max_element(closed_trades.begin(), closed_trades.end(), [](Trade a, Trade b)
                                                    { return a.pnl < b.pnl; }))
                                     .pnl;
    }

    // Calculate the maximum loss
    if (this->stats.total_lost_trades > 0)
    {
        this->stats.max_loss = (*std::min_element(closed_trades.begin(), closed_trades.end(), [](Trade a, Trade b)
                                                  { return a.pnl < b.pnl; }))
                                   .pnl;
    }

    // Calculate the maximum consecutive winning trades
    this->stats.max_consecutive_winning_trades = 0;
    int current_consecutive_winning_trades = 0;
    for (const auto &trade : closed_trades)
    {
        if (trade.pnl >= 0)
        {
            current_consecutive_winning_trades++;
            this->stats.max_consecutive_winning_trades = std::max(this->stats.max_consecutive_winning_trades, current_consecutive_winning_trades);
        }
        else
        {
            current_consecutive_winning_trades = 0;
        }
    }

    // Calculate the maximum consecutive lost trades
    this->stats.max_consecutive_lost_trades = 0;
    int current_consecutive_lost_trades = 0;
    for (const auto &trade : closed_trades)
    {
        if (trade.pnl < 0)
        {
            current_consecutive_lost_trades++;
            this->stats.max_consecutive_lost_trades = std::max(this->stats.max_consecutive_lost_trades, current_consecutive_lost_trades);
        }
        else
        {
            current_consecutive_lost_trades = 0;
        }
    }

    // Calculate the maximum consecutive profit
    this->stats.max_consecutive_profit = 0;
    double current_consecutive_profit = 0;
    for (const auto &trade : closed_trades)
    {
        if (trade.pnl >= 0)
        {
            current_consecutive_profit += trade.pnl;
            this->stats.max_consecutive_profit = std::max(this->stats.max_consecutive_profit, current_consecutive_profit);
        }
        else
        {
            current_consecutive_profit = 0;
        }
    }

    // Calculate the maximum consecutive loss
    this->stats.max_consecutive_loss = 0;
    double current_consecutive_loss = 0;
    for (const auto &trade : closed_trades)
    {
        if (trade.pnl < 0)
        {
            current_consecutive_loss += trade.pnl;
            this->stats.max_consecutive_loss = std::min(this->stats.max_consecutive_loss, current_consecutive_loss);
        }
        else
        {
            current_consecutive_loss = 0;
        }
    }

    // Calculate the average trade duration
    if (this->stats.total_trades > 0)
    {
        double total_duration = 0;
        for (auto &trade : closed_trades)
        {
            total_duration += trade.duration;
        }
        this->stats.average_trade_duration = total_duration / static_cast<double>(this->stats.total_trades);
    }

    // Calculate the monthly returns
    for (const auto &trade : closed_trades)
    {
        double trade_return = trade.pnl_net_percent;
        std::string date_key = time_t_to_string(trade.exit_date, "%Y-%m");
        if (this->stats.monthly_returns.find(date_key) == this->stats.monthly_returns.end())
        {
            this->stats.monthly_returns[date_key] = 1.0;
        }
        this->stats.monthly_returns[date_key] *= (1.0 + trade_return);
    }
    for (const auto &[month, monthly_return] : this->stats.monthly_returns)
    {
        this->stats.monthly_returns[month] = decimal_round(monthly_return - 1.0, 4);
    }

    // Calculate the average investment return
    double average_investment_return = 0;
    std::vector<double> monthly_investment_returns = {};
    if (this->stats.monthly_returns.size() > 0)
    {
        for (const auto &[month, performance] : this->stats.monthly_returns)
        {
            average_investment_return += performance / static_cast<double>(this->stats.monthly_returns.size());
            monthly_investment_returns.push_back(performance);
        }
    }

    // Calculate the sharpe ratio
    this->stats.sharpe_ratio = 0;
    if (monthly_investment_returns.size() > 0)
    {
        double std_dev = calculate_standard_deviation(monthly_investment_returns, monthly_investment_returns.size()).back();
        if (std_dev != 0)
        {
            this->stats.sharpe_ratio = (average_investment_return - 0.0) / std_dev;
        }
    }

    // Calculate the sortino ratio
    this->stats.sortino_ratio = 0;
    std::vector<double> monthly_investment_returns_negative = {};
    std::copy_if(monthly_investment_returns.begin(), monthly_investment_returns.end(), std::back_inserter(monthly_investment_returns_negative), [](double x)
                 { return x < 0; });
    if (monthly_investment_returns_negative.size() > 0)
    {
        double std_dev = calculate_standard_deviation(monthly_investment_returns_negative, monthly_investment_returns_negative.size()).back();
        if (std_dev != 0)
        {
            this->stats.sortino_ratio = (average_investment_return - 0.0) / std_dev;
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
        // Calculate the fees
        double fees = calculate_commission(this->symbol_info.commission_per_lot, this->current_position->size, this->current_base_currency_conversion_rate);

        this->trades_history.back().exit_date = this->current_date;
        this->trades_history.back().exit_price = price;
        this->trades_history.back().duration = duration_in_position;
        this->trades_history.back().pnl = this->current_position->pnl;
        this->trades_history.back().fees += fees;
        this->trades_history.back().pnl_percent = this->current_position->pnl / (this->balance + fees);
        this->trades_history.back().pnl_net_percent = (this->current_position->pnl - this->trades_history.back().fees) / (this->balance + fees);
        this->trades_history.back().closed = true;

        // Update the balance
        this->balance = std::max(0.0, this->balance + this->current_position->pnl - fees);

        if (this->logger != nullptr)
        {
            this->logger->info("[" + time_t_to_string(this->current_date) + "] [" + std::to_string(this->balance) + "] : Close position by market at " + std::to_string(price) + " with " + std::to_string(this->current_position->pnl) + " of profit and " + std::to_string(fees) + " of fees.");
        }

        this->current_position = nullptr;
        this->duration_without_trade = 0;
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
        // Calculate the fees
        double fees = calculate_commission(this->symbol_info.commission_per_lot, this->current_position->size, this->current_base_currency_conversion_rate);

        this->trades_history.back().exit_date = this->current_date;
        this->trades_history.back().exit_price = price;
        this->trades_history.back().duration = duration_in_position;
        this->trades_history.back().pnl = this->current_position->pnl;
        this->trades_history.back().fees += fees;
        this->trades_history.back().pnl_percent = this->current_position->pnl / (this->balance + fees);
        this->trades_history.back().pnl_net_percent = (this->current_position->pnl - this->trades_history.back().fees) / (this->balance + fees);
        this->trades_history.back().closed = true;

        // Update the balance
        this->balance = std::max(0.0, this->balance + this->current_position->pnl - fees);

        if (this->logger != nullptr)
        {
            this->logger->info("[" + time_t_to_string(this->current_date) + "] [" + std::to_string(this->balance) + "] : Close position by limit at " + std::to_string(price) + " with " + std::to_string(this->current_position->pnl) + " of profit and " + std::to_string(fees) + " of fees.");
        }

        this->current_position = nullptr;
        this->duration_without_trade = 0;
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

    if (this->logger != nullptr)
    {
        std::string order_type = type == OrderType::TAKE_PROFIT ? "take profit" : "stop loss";
        std::string order_side = side == OrderSide::LONG ? "long" : "short";
        this->logger->info("[" + time_t_to_string(this->current_date) + "] [" + std::to_string(this->balance) + "] : Create " + order_type + " order at " + std::to_string(price) + " for " + order_side + " position.");
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
                    if (short_orders[i].type == OrderType::TAKE_PROFIT && last_candle.high >= short_orders[i].price)
                    {
                        this->close_position_by_limit(short_orders[i].price);
                    }
                    if (short_orders[i].type == OrderType::STOP_LOSS && last_candle.low <= short_orders[i].price)
                    {
                        this->close_position_by_market(short_orders[i].price);
                    }
                }
            }
            else if (this->current_position->side == PositionSide::SHORT)
            {
                for (int i = 0; i < long_orders.size(); i++)
                {
                    if (long_orders[i].type == OrderType::TAKE_PROFIT && last_candle.low <= long_orders[i].price)
                    {
                        this->close_position_by_limit(long_orders[i].price);
                    }
                    if (long_orders[i].type == OrderType::STOP_LOSS && last_candle.high >= long_orders[i].price)
                    {
                        this->close_position_by_market(long_orders[i].price);
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
        double liquidation_price = calculate_liquidation_price(this->current_position, this->config.general.leverage, this->symbol_info);
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
 * @brief Print the statistics of the trader in the console.
 */
void Trader::print_stats_to_console()
{
    std::cout << "------------------------------ STATS -----------------------------" << std::endl;
    std::cout << "Initial balance: " << this->stats.initial_balance << std::endl;
    std::cout << "Final balance: " << this->stats.final_balance << std::endl;
    std::cout << "Performance: " << decimal_floor(this->stats.performance * 100, 2) << "%" << std::endl;
    std::cout << "Total net profit: " << this->stats.total_net_profit << std::endl;
    std::cout << "Total profit: " << this->stats.total_profit << std::endl;
    std::cout << "Total loss: " << this->stats.total_loss << std::endl;
    std::cout << "Total fees: " << this->stats.total_fees << std::endl;
    std::cout << "Total trades: " << this->stats.total_trades << std::endl;
    std::cout << "Total long trades: " << this->stats.total_long_trades << std::endl;
    std::cout << "Total short trades: " << this->stats.total_short_trades << std::endl;
    std::cout << "Total winning trades: " << this->stats.total_winning_trades << std::endl;
    std::cout << "Total lost trades: " << this->stats.total_lost_trades << std::endl;
    std::cout << "Max consecutive winning trades: " << this->stats.max_consecutive_winning_trades << std::endl;
    std::cout << "Max consecutive lost trades: " << this->stats.max_consecutive_lost_trades << std::endl;
    std::cout << "Profit factor: " << decimal_floor(this->stats.profit_factor, 2) << std::endl;
    std::cout << "Max drawdown: " << decimal_floor(-this->stats.max_drawdown * 100, 2) << "%" << std::endl;
    std::cout << "Win rate: " << decimal_floor(this->stats.win_rate * 100, 2) << "%" << std::endl;
    std::cout << "Long win rate: " << decimal_floor(this->stats.long_win_rate * 100, 2) << "%" << std::endl;
    std::cout << "Short win rate: " << decimal_floor(this->stats.short_win_rate * 100, 2) << "%" << std::endl;
    std::cout << "Average profit: " << this->stats.average_profit << std::endl;
    std::cout << "Average loss: " << this->stats.average_loss << std::endl;
    std::cout << "Max profit: " << this->stats.max_profit << std::endl;
    std::cout << "Max loss: " << this->stats.max_loss << std::endl;
    std::cout << "Max consecutive profit: " << this->stats.max_consecutive_profit << std::endl;
    std::cout << "Max consecutive loss: " << this->stats.max_consecutive_loss << std::endl;
    std::cout << "Average trade duration: " << this->stats.average_trade_duration << " candles" << std::endl;
    std::cout << "Sharpe ratio: " << decimal_floor(this->stats.sharpe_ratio, 2) << std::endl;
    std::cout << "Sortino ratio: " << decimal_floor(this->stats.sortino_ratio, 2) << std::endl;
}

/**
 * @brief Generate the graph of the balance history.
 * @param filename Filename of the graph.
 */
void Trader::generate_balance_history_graph(const std::string &filename)
{
    if (this->balance_history.empty())
    {
        std::cerr << "No balance history to generate a graph." << std::endl;
        return;
    }

    // Check if the directory exists
    std::filesystem::path dir = std::filesystem::path(filename).parent_path();
    if (!std::filesystem::exists(dir))
    {
        try
        {
            std::filesystem::create_directories(dir);
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "Error creating directories: " << e.what() << std::endl;
        }
    }

    // Create a Gnuplot object
    Gnuplot gp;

    // Generate data for the sine wave
    std::vector<std::pair<double, double>> data;
    for (int i = 0; i < this->balance_history.size(); ++i)
    {
        data.push_back(std::make_pair(i, this->balance_history[i]));
    }

    // Specify terminal type and output file
    gp << "set term png\n";
    gp << "set output '" + filename + "'\n";

    // Set plot options
    gp << "set title 'Balance History'\n";
    gp << "set xlabel 'Temps'\n";
    gp << "set ylabel 'Balance'\n";

    // Plot data
    gp << "plot '-' with lines title 'balance'\n";
    gp.send(data); // Send the data to Gnuplot for plotting

    // Close output and terminate Gnuplot
    gp << "unset output\n";
    gp << "exit\n";
}

/**
 * @brief Print the statistics and the trades list of the trader in a HTML file.
 * @param filename Filename of the HTML file.
 */
void Trader::generate_report(const std::string &filename)
{
    // Select only closed trade
    std::vector<Trade> closed_trades = {};
    for (const auto &trade : this->trades_history)
    {
        if (trade.closed)
        {
            closed_trades.push_back(trade);
        }
    }

    if (closed_trades.empty())
    {
        std::cerr << "No closed trades to generate a report." << std::endl;
        return;
    }

    // Check if the directory exists
    std::filesystem::path dir = std::filesystem::path(filename).parent_path();
    if (!std::filesystem::exists(dir))
    {
        try
        {
            std::filesystem::create_directories(dir);
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "Error creating directories: " << e.what() << std::endl;
        }
    }

    // Get the start and end date of the trades
    time_t start_date = closed_trades[0].entry_date;
    time_t end_date = closed_trades.back().exit_date;

    // Open the file
    std::ofstream file(filename);

    // Write the HTML content using the provided template
    file
        << R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/hammer.js/2.0.8/hammer.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/chartjs-plugin-zoom@1.2.1/dist/chartjs-plugin-zoom.min.js"></script>
    <title>Strategy Report</title>
    <style>
        html, body {
            font-family: 'Avenir';
            padding: 20px;
        }
        body {
            margin: 0;
            text-align: center;
        }
        h1 {
            margin-top: 10px;
        }
        h2 {
            margin-top: 20px;
            margin-bottom: 20px;
        }
        h3 {
            color: #666;
            font-style: italic;
        }
        #parameters {
            margin: 30px auto;
        }
        #parameters th {
            padding: 0px 10px;
            min-width: 100px;
        }
        #report {
            display: flex;
            flex-direction: row;
            justify-content: center;
            align-items: center;
            border: solid 1px black;
            width: max-content;
            margin: 50px auto;
        }
        #report .report-frame {
            display: block;
            margin: 10px;
            padding: 15px;
            max-width: 600px;
        }
        #balance_history {
            padding: auto;
            margin: 30px auto;
            width: 1200px;
        }
        #monthly_returns {
            padding: auto;
            margin: 30px auto;
            width: 1200px;
        }
        #trade_history {
            text-align: left;
            width: auto;
            margin: auto;
            border-collapse: collapse;
        }
        #trade_history th {
            padding: 10px;
            min-width: 65px;
            border: solid 1px #BBB;
        }
        #trade_history tbody th {
            font-weight: 200;
        }
        #trade_history td {
            padding: 10px;
            border: solid 1px #BBB;
        }
    </style>
</head>
<body>
    <h1>Strategy Report</h1>
    <h3>)"
        << this->config.general.name + " " + this->config.general.version << R"(</h3>

    <div id="report">
        <div class="report-frame">
            <table>
                <tr>
                    <td><b>Period:</b></td>
                    <td>)"
        << time_t_to_string(start_date) + " to " + time_t_to_string(end_date) << R"(</td>
                </tr>
                <tr>
                    <td><b>Initial balance:</b></td>
                    <td>)"
        << this->stats.initial_balance << R"(</td>
                </tr>
                <tr>
                    <td><b>Final balance:</b></td>
                    <td>)"
        << this->stats.final_balance << R"(</td>
                </tr>
                <tr>
                    <td><b>Performance:</b></td>
                    <td>)"
        << decimal_floor(this->stats.performance * 100, 2) << "%" << R"(</td>
                </tr>
                <tr>
                    <td><b>Sharpe ratio:</b></td>
                    <td>)"
        << decimal_floor(this->stats.sharpe_ratio, 2) << R"(</td>
                </tr>
                <tr>
                    <td><b>Sortino ratio:</b></td>
                    <td>)"
        << decimal_floor(this->stats.sortino_ratio, 2) << R"(</td>
                </tr>
                <tr>
                    <td><b>Total net profit:</b></td>
                    <td>)"
        << this->stats.total_net_profit << R"(</td>
                </tr>
                <tr>
                    <td><b>Total profit:</b></td>
                    <td>)"
        << this->stats.total_profit << R"(</td>
                </tr>
                <tr>
                    <td><b>Total loss:</b></td>
                    <td>)"
        << this->stats.total_loss << R"(</td>
                </tr>
                <tr>
                    <td><b>Total fees:</b></td>
                    <td>)"
        << this->stats.total_fees << R"(</td>
                </tr>
                <tr>
                    <td><b>Profit factor:</b></td>
                    <td>)"
        << decimal_floor(this->stats.profit_factor, 2) << R"(</td>
                </tr>
                <tr>
                    <td><b>Max drawdown:</b></td>
                    <td>)"
        << -decimal_floor(this->stats.max_drawdown * 100, 2) << "%" << R"(</td>
                </tr>
            </table>
        </div>
        <div class="report-frame">
            <table>
                <tr>
                    <td><b>Total trades:</b></td>
                    <td>)"
        << this->stats.total_trades << R"(</td>
                </tr>
                <tr>
                    <td><b>Total win rate:</b></td>
                    <td>)"
        << decimal_floor(this->stats.win_rate * 100, 2) << "%" << " (" << this->stats.total_winning_trades << "/" << this->stats.total_trades << ")" << R"(</td>
                </tr>
                <tr>
                    <td><b>Long win rate:</b></td>
                    <td>)"
        << decimal_floor(this->stats.long_win_rate * 100, 2) << "%" << " (" << this->stats.total_winning_long_trades << "/" << this->stats.total_long_trades << ")" << R"(</td>
                </tr>
                <tr>
                    <td><b>Short win rate:</b></td>
                    <td>)"
        << decimal_floor(this->stats.short_win_rate * 100, 2) << "%" << " (" << this->stats.total_winning_short_trades << "/" << this->stats.total_short_trades << ")" << R"(</td>
                </tr>
                <tr>
                    <td><b>Max profit:</b></td>
                    <td>)"
        << this->stats.max_profit << R"(</td>
                </tr>
                <tr>
                    <td><b>Max loss:</b></td>
                    <td>)"
        << this->stats.max_loss << R"(</td>
                </tr>
                <tr>
                    <td><b>Max consecutive profit:</b></td>
                    <td>)"
        << this->stats.max_consecutive_profit << R"(</td>
                </tr>
                <tr>
                    <td><b>Max consecutive loss:</b></td>
                    <td>)"
        << this->stats.max_consecutive_loss << R"(</td>
                </tr>
                <tr>
                    <td><b>Max consecutive winning trades:</b></td>
                    <td>)"
        << this->stats.max_consecutive_winning_trades << R"(</td>
                </tr>
                <tr>
                    <td><b>Max consecutive lost trades:</b></td>
                    <td>)"
        << this->stats.max_consecutive_lost_trades << R"(</td>
                </tr>
                <tr>
                    <td><b>Average trade duration:</b></td>
                    <td>)"
        << decimal_floor(this->stats.average_trade_duration, 2) << " candles" << R"(</td>
                </tr>
            </table>
        </div>
    </div>

    <canvas id="balance_history"></canvas>
    <canvas id="monthly_returns"></canvas>

    <h2>Trades</h2>
        <table id="trade_history">
          <thead>
            <tr>
              <th>#</th>
              <th>Entry date</th>
              <th>Exit date</th>
              <th>Symbol</th>
              <th>Side</th>
              <th>Size</th>
              <th>Entry price</th>
              <th>Exit price</th>
              <th>Pnl</th>
              <th>Fees</th>
              <th>Balance</th>
            </tr>
          </thead>
          <tbody>)";

    // Write the trades history to the file
    double balance = this->stats.initial_balance;
    for (int i = 0; i < this->trades_history.size(); ++i)
    {
        Trade trade = this->trades_history[i];
        balance += trade.pnl - trade.fees;

        file << R"(
        <tr>
            <td>)"
             << i << R"(</td>
            <td>)"
             << time_t_to_string(trade.entry_date) << R"(</td>
            <td>)"
             << time_t_to_string(trade.exit_date) << R"(</td>
            <td>)"
             << this->config.general.symbol << R"(</td>
            <td style=)"
             << (trade.side == PositionSide::LONG ? "\"color: green;\"" : "\"color: red;\"") << R"(>)"
             << (trade.side == PositionSide::LONG ? "LONG" : "SHORT") << R"(</td>
            <td>)"
             << trade.size << R"(</td>
            <td>)"
             << trade.entry_price << R"(</td>
            <td>)"
             << trade.exit_price << R"(</td>
            <td style=)"
             << (trade.pnl >= 0 ? "\"color: green;\"" : "\"color: red;\"") << R"(>)"
             << trade.pnl << " (" << decimal_round(trade.pnl_percent * 100, 2) << "%)"
             << R"(</td>
            <td>)"
             << decimal_floor(trade.fees, 2) << R"(</td>
            <td>)"
             << balance << R"(</td>
        </tr>)";
    }

    std::string balance_history_labels = "";
    std::string balance_history_data = "";
    std::string monthly_returns_labels = "";
    std::string monthly_returns_data = "";

    balance = this->stats.initial_balance;
    balance_history_labels += "\"" + time_t_to_string(start_date) + "\",";
    balance_history_data += std::to_string(balance) + ",";

    // Create the data for the balance history line chart
    for (const auto &trade : closed_trades)
    {
        balance += trade.pnl - trade.fees;
        balance_history_labels += "\"" + time_t_to_string(trade.exit_date) + "\",";
        balance_history_data += std::to_string(balance) + ",";
    }

    // Create the data for the monthly returns bar chart
    for (const auto &[monthly_returns_data_key, monthly_returns_data_value] : this->stats.monthly_returns)
    {
        monthly_returns_labels += "\"" + monthly_returns_data_key + "\",";
        monthly_returns_data += std::to_string(monthly_returns_data_value) + ",";
    }

    file << R"(
        <script>
            var balance_history_ctx = document.getElementById('balance_history').getContext('2d');
            var monthly_returns_ctx = document.getElementById('monthly_returns').getContext('2d');

            var balance_history_data = {
                labels : [)"
         << balance_history_labels <<
        R"(],
                datasets : [{
                    label : 'Balance',
                    data : [)"
         << balance_history_data <<
        R"(],
                    fill : false,
                    borderColor : '#007FFF',
                    tension : 0.1,
                }],
            };

            var monthly_returns_data = {
                labels : [)"
         << monthly_returns_labels <<
        R"(],
                datasets: [{
                    label: 'Monthly returns',
                    data : [)"
         << monthly_returns_data <<
        R"(],
                    borderWidth: 1,
                }]
            };

            var balance_history_config = {
                type : 'line',
                data : balance_history_data,
                options : {
                    pointRadius : 0,
                    scales: {
                        x: {
                            display: true,
                        },
                        y: {
                            display: true,
                            type: 'logarithmic',
                        }
                    },
                    plugins : {
                        title : {
                            display : true,
                            text : 'Balance history',
                            font : {
                                size : 32
                            }
                        },
                        zoom : {
                            zoom : {
                                wheel : {
                                    enabled : true,
                                },
                                mode : 'x',
                            }
                        }
                    }
                }
            };

            var monthly_returns_config = {
                type : 'bar',
                data : monthly_returns_data,
                options : {
                    scales: {
                        y: {
                            beginAtZero: true
                        }
                    },
                    plugins : {
                        title : {
                            display : true,
                            text : 'Monthly returns',
                            font : {
                                size : 32
                            }
                        }
                    },
                },
            };

            balance_history_data = new Chart(balance_history_ctx, balance_history_config);
            monthly_returns_data = new Chart(monthly_returns_ctx, monthly_returns_config);
        
        </script>
    </body>
</html>)";

    // Close the file
    file.close();
}
