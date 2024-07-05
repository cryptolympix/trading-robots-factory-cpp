#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>
#include <random>
#include <algorithm>
#include "utils/logger.hpp"
#include "utils/time_frame.hpp"
#include "utils/math.hpp"
#include "utils/vectors.hpp"
#include "utils/date_conversion.hpp"
#include "libs/json.hpp"
#include "libs/gnuplot-iostream.hpp"
#include "neat/genome.hpp"
#include "trading/schedule.hpp"
#include "trading/tools.hpp"
#include "trading/stats.hpp"
#include "indicators/utils.hpp"
#include "symbols.hpp"
#include "trader.hpp"

// Operaton overloading for the Trade struct
bool operator==(const Trade &t1, const Trade &t2)
{
    return t1.entry_date == t2.entry_date && t1.exit_date == t2.exit_date && t1.side == t2.side && t1.size == t2.size && t1.entry_price == t2.entry_price && t1.exit_price == t2.exit_price && t1.pnl == t2.pnl && t1.pnl_percent == t2.pnl_percent && t1.fees == t2.fees && t1.closed == t2.closed;
}

/**
 * @brief Constructor for the Trader class.
 * @param genome Genome object.
 * @param config Configuration object.
 * @param logger Logger object.
 */
Trader::Trader(neat::Genome *genome, Config config, Logger *logger)
{
    if (genome == nullptr)
    {
        std::cerr << "Genome object is null." << std::endl;
        return;
    }

    this->config = config;
    this->symbol_info = symbol_infos[config.general.symbol];
    this->logger = logger;

    // Vision
    this->candles = {};
    this->current_date = 0;
    this->current_base_currency_conversion_rate = 1.0;

    // History
    this->balance_history = {};
    this->trades_history = {};

    // Trading stuff
    this->balance = config.general.initial_balance;
    this->open_orders = {};
    this->current_position = nullptr;
    this->duration_in_position = 0;
    this->duration_without_trade = this->config.strategy.minimum_duration_before_next_trade.value_or(0);
    this->nb_trades_today = 0;

    // Statistics of the trader
    this->stats = Stats(config.general.initial_balance);

    // Neat stuffs
    this->generation = 0;
    this->fitness = genome->fitness;
    this->score = 0;
    this->lifespan = 0;
    this->dead = false;
    this->genome = genome;
    this->vision = {};
    this->decisions = {};
};

/**
 * @brief Destructor for the Trader class.
 */
Trader::~Trader()
{
    if (this->logger != nullptr)
    {
        delete this->logger;
    }
}

/**
 * @brief Get inputs for genome.
 * @param indicators_data Indicator data for all time frames.
 * @param base_currency_conversion_rate Conversion rate when the base asset traded is different from the account currency.
 * @param position_infos Vector of position information.
 */
void Trader::look(IndicatorsData &indicators_data, double base_currency_conversion_rate, std::vector<PositionInfo> position_infos)
{
    std::vector<double> indicators_values = {};
    this->current_base_currency_conversion_rate = base_currency_conversion_rate;

    for (const auto &[tf, indicators] : this->config.training.inputs.indicators)
    {
        for (const auto &indicator : indicators)
        {
            indicators_values.push_back(indicators_data[tf][indicator->id].back());
            if ((this->config.strategy.can_open_long_trade.value_or(true) && this->config.strategy.can_close_trade.value_or(false)) || (this->config.strategy.can_open_short_trade.value_or(true) && this->config.strategy.can_close_trade.value_or(false)) || (this->config.strategy.can_open_long_trade.value_or(true) && this->config.strategy.can_open_short_trade.value_or(true)))
            {
                indicators_values.push_back(indicators_data[tf][indicator->id + "-reverse"].back());
            }
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
                // Cannot normalize the duration if the maximum trade duration is not set
                position_info.push_back(0.0);
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
 * @brief Update the candles, the current position of trader and his lifespan.
 *
 * @param candles Candle data for all time frames.
 */
void Trader::update(CandlesData &candles)
{
    if (candles.empty())
    {
        std::cerr << "No candles data provided." << std::endl;
        return;
    }

    // Update the candles
    this->candles = candles;

    // Detect if it's a new day, and reset the number of trades made today
    if (this->current_date != 0)
    {
        struct tm last_date_tm = time_t_to_tm(this->current_date);
        int last_year = last_date_tm.tm_year;
        int last_month = last_date_tm.tm_mon;
        int last_day = last_date_tm.tm_mday;

        struct tm current_date_tm = time_t_to_tm(this->candles[this->config.strategy.timeframe].back().date);
        int current_year = current_date_tm.tm_year;
        int current_month = current_date_tm.tm_mon;
        int current_day = current_date_tm.tm_mday;

        bool is_new_day = last_year != current_year || last_month != current_month || last_day != current_day;
        if (is_new_day)
        {
            this->nb_trades_today = 0;
        }
    }

    // Update the current date
    this->current_date = this->candles[this->config.strategy.timeframe].back().date;

    // Increment the position duration
    if (this->current_position != nullptr)
    {
        this->duration_in_position++;
    }
    else
    {
        this->duration_without_trade++;
    }

    // Update the position
    this->update_position_pnl();
    this->update_trailing_stop_loss();
    this->check_open_orders();
    this->check_position_liquidation();

    // Check the duration of the current trade
    if (this->current_position != nullptr && config.strategy.maximum_trade_duration.has_value())
    {
        // Check if the position has reached the maximum trade duration
        if (this->duration_in_position >= config.strategy.maximum_trade_duration.value())
        {
            this->close_position_by_market();
        }
    }

    // Close the trade before the weekend
    int next_date = this->current_date + get_time_frame_in_minutes(this->config.strategy.timeframe) * 60;
    struct tm next_date_tm = time_t_to_tm(next_date);
    if (next_date_tm.tm_wday == 6 && this->current_position != nullptr)
    {
        this->close_position_by_market();
    }

    // Increment the lifespan of the trader
    this->lifespan++;

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

    // Record the balance to history
    this->balance_history.push_back(this->balance);
}

/**
 * @brief Check if the trader can trade.
 */
bool Trader::can_trade()
{
    Candle last_candle = this->candles[this->config.strategy.timeframe].back();

    // Check if the trader can trade at the moment according to the schedule
    bool schedule_is_ok = true;
    if (this->config.strategy.trading_schedule.has_value())
    {
        TradingSchedule trading_schedule = this->config.strategy.trading_schedule.value();
        schedule_is_ok = is_on_trading_schedule(this->current_date, trading_schedule);
    }

    // Check the number of trades made today
    bool number_of_trades_per_day_is_ok = this->nb_trades_today < this->config.strategy.maximum_trades_per_day.value();

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

    return schedule_is_ok && number_of_trades_per_day_is_ok && spread_is_ok && time_after_previous_trade_is_ok;
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
    double decision_threshold = this->config.training.decision_threshold.value_or(0.0);
    int decision = std::distance(this->decisions.begin(), std::max_element(this->decisions.begin(), this->decisions.end()));
    bool want_long = decision == 0 && this->decisions[0] >= decision_threshold;
    bool want_short = decision == 1 && this->decisions[1] >= decision_threshold;
    bool wait = decision == 2;

    // Get the index of the maximum value in the decisions
    int max_decision_index = std::distance(this->decisions.begin(), std::max_element(this->decisions.begin(), this->decisions.end()));

    if (!wait)
    {
        bool can_trade_now = this->can_trade();
        bool can_close_position = this->config.strategy.can_close_trade.value_or(false);

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
                if (want_long && this->config.strategy.can_open_long_trade.value_or(true))
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
                else if (want_short && this->config.strategy.can_open_short_trade.value_or(true))
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

    return 0; // Wait
}

/**
 * @brief Calculate the fitness of the trader.
 */
void Trader::calculate_fitness()
{
    EvaluationConfig goals = this->config.evaluation;

    double minimum_nb_trades_eval = 0;
    double max_trade_duration_eval = 0;
    double max_drawdown_eval = 0;
    double profit_factor_eval = 0;
    double win_rate_eval = 0;
    double expected_return_per_day_eval = 0;
    double expected_return_per_month_eval = 0;
    double expected_return_eval = 0;

    double minimum_nb_trades_weight = 1;
    double max_trade_duration_weight = 1;
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

    // Get all the dates between the first and the last day of training
    std::vector<std::string> all_dates = {};
    time_t current_date = this->config.training.training_start_date;
    while (current_date <= this->config.training.training_end_date)
    {
        // If the current date is a weekend, skip it
        struct tm current_date_tm = time_t_to_tm(current_date);
        if (current_date_tm.tm_wday == 6 || current_date_tm.tm_wday == 0)
        {
            current_date += 24 * 60 * 60;
            continue;
        }

        // If the date is not yet contained in the vector, add it
        if (std::find(all_dates.begin(), all_dates.end(), time_t_to_string(current_date, "%Y-%m-%d")) == all_dates.end())
        {
            all_dates.push_back(time_t_to_string(current_date, "%Y-%m-%d"));
        }

        // Increment the date by one day
        current_date += 24 * 60 * 60;
    }

    // Get all the months between the first and the last day of training
    std::vector<std::string> all_months = {};
    for (const auto &date : all_dates)
    {
        std::string month = date.substr(0, 7);
        if (std::find(all_months.begin(), all_months.end(), month) == all_months.end())
        {
            all_months.push_back(month);
        }
    }

    if (goals.minimum_nb_trades.has_value())
    {
        double diff = 10 * std::max(0, goals.minimum_nb_trades.value() - (int)closed_trades.size());
        minimum_nb_trades_eval = minimum_nb_trades_weight / std::exp(diff);
    }

    if (goals.maximum_trade_duration.has_value())
    {
        for (const auto &trade : closed_trades)
        {
            double diff = 10 * std::max(0, goals.maximum_trade_duration.value() - trade.duration);
            max_trade_duration_eval += max_trade_duration_weight / (closed_trades.size() * std::exp(diff));
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
        std::vector<Trade> closed_trades_copy = closed_trades;

        for (const auto &date : all_dates)
        {
            daily_returns[date] = 1.0;
            for (const auto &trade : closed_trades_copy)
            {
                double trade_return = trade.pnl_percent;
                std::string trade_date = time_t_to_string(trade.exit_date, "%Y-%m-%d");
                if (trade_date == date)
                {
                    daily_returns[date] *= 1 + trade_return;

                    // Remove the trade from the vector to avoid counting it twice
                    auto it = std::find(closed_trades_copy.begin(), closed_trades_copy.end(), trade);
                    closed_trades_copy.erase(it, closed_trades_copy.end());
                }
            }
        }

        for (auto &[day, daily_return] : daily_returns)
        {
            daily_return -= 1.0;
        }

        int nb_days = daily_returns.size();
        if (nb_days > 0)
        {
            for (const auto &daily_return : daily_returns)
            {
                double diff = 10 * std::max(0.0, goals.expected_return_per_day.value() - daily_return.second);
                expected_return_per_day_eval += expected_return_per_day_weight / (nb_days * std::exp(diff));
            }
        }
    }

    if (goals.expected_return_per_month.has_value())
    {
        std::map<std::string, double> monthly_returns = {};
        std::vector<Trade> closed_trades_copy = closed_trades;

        for (const auto &month : all_months)
        {
            monthly_returns[month] = 1.0;
            for (const auto &trade : closed_trades_copy)
            {
                double trade_return = trade.pnl_percent;
                std::string trade_month = time_t_to_string(trade.exit_date, "%Y-%m");
                if (trade_month == month)
                {
                    monthly_returns[month] *= 1 + trade_return;

                    // Remove the trade from the vector to avoid counting it twice
                    auto it = std::find(closed_trades_copy.begin(), closed_trades_copy.end(), trade);
                    closed_trades_copy.erase(it, closed_trades_copy.end());
                }
            }
        }

        for (auto &[month, monthly_return] : monthly_returns)
        {
            monthly_return -= 1.0;
        }

        int nb_months = monthly_returns.size();
        if (nb_months > 0)
        {
            for (const auto &monthly_return : monthly_returns)
            {
                double diff = 10 * std::max(0.0, goals.expected_return_per_month.value() - monthly_return.second);
                expected_return_per_month_eval += expected_return_per_month_weight / (nb_months * std::exp(diff));
            }
        }
    }

    if (goals.expected_return.has_value())
    {
        double diff = 10 * std::max(0.0, goals.expected_return.value() - stats.performance);
        expected_return_eval = expected_return_eval / std::exp(diff);
    }

    // ***************** FORMULA TO CALCULATE FITNESS ***************** //

    this->fitness = this->score > 0 ? this->score : 1;

    if (this->trades_history.empty())
    {
        this->fitness = 0;
        return;
    }
    if (goals.maximize_nb_trades.value_or(false))
    {
        this->fitness *= stats.total_trades;
    }
    if (goals.minimum_nb_trades.has_value())
    {
        this->fitness *= minimum_nb_trades_eval;
    }
    if (goals.maximum_trade_duration.has_value())
    {
        this->fitness *= max_trade_duration_eval;
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
    this->stats.calculate(this->trades_history, this->balance_history);
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
        this->nb_trades_today++;
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
        this->nb_trades_today++;
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
 * @brief Update the trailing stop loss.
 */
void Trader::update_trailing_stop_loss()
{
    // Check if the trailing stop loss is enabled
    if (!this->config.strategy.trailing_stop_loss_config.has_value() || this->current_position == nullptr)
    {
        return;
    }

    TrailingStopLossConfig config = this->config.strategy.trailing_stop_loss_config.value();
    Order *stop_loss_order = &this->open_orders[1];
    double current_price = this->candles[this->config.strategy.timeframe].back().close;

    if (config.type == TypeTrailingStopLoss::PERCENT)
    {
        if (!config.trailing_stop_loss_in_percent.has_value())
        {
            std::cerr << "The trailing stop loss in percent is not set." << std::endl;
            return;
        }

        if (this->current_position->side == PositionSide::LONG)
        {
            // Check if the activation level is reached
            if (config.activation_level_in_percent.has_value())
            {
                if (current_price < this->current_position->entry_price + (this->current_position->entry_price * config.activation_level_in_percent.value()))
                {
                    return;
                }
            }

            double trailing_stop_loss = current_price - (current_price * config.trailing_stop_loss_in_percent.value());
            if (trailing_stop_loss > stop_loss_order->price)
            {
                stop_loss_order->price = trailing_stop_loss;
            }
        }
        else if (this->current_position->side == PositionSide::SHORT)
        {
            // Check if the activation level is reached
            if (config.activation_level_in_percent.has_value())
            {
                if (current_price > this->current_position->entry_price - (this->current_position->entry_price * config.activation_level_in_percent.value()))
                {
                    return;
                }
            }

            double trailing_stop_loss = current_price + (current_price * config.trailing_stop_loss_in_percent.value());
            if (trailing_stop_loss < stop_loss_order->price)
            {
                stop_loss_order->price = trailing_stop_loss;
            }
        }
    }
    else if (config.type == TypeTrailingStopLoss::POINTS)
    {
        if (!config.trailing_stop_loss_in_points.has_value())
        {
            std::cerr << "The trailing stop loss in points is not set." << std::endl;
            return;
        }

        if (this->current_position->side == PositionSide::LONG)
        {
            // Check if the activation level is reached
            if (config.activation_level_in_points.has_value())
            {
                if (current_price < this->current_position->entry_price + config.activation_level_in_points.value() * symbol_info.point_value)
                {
                    return;
                }
            }

            double trailing_stop_loss = current_price - config.trailing_stop_loss_in_points.value() * symbol_info.point_value;
            if (trailing_stop_loss > stop_loss_order->price)
            {
                stop_loss_order->price = trailing_stop_loss;
            }
        }
        else if (this->current_position->side == PositionSide::SHORT)
        {
            // Check if the activation level is reached
            if (config.activation_level_in_points.has_value())
            {
                if (current_price > this->current_position->entry_price - config.activation_level_in_points.value() * symbol_info.point_value)
                {
                    return;
                }
            }

            double trailing_stop_loss = current_price + config.trailing_stop_loss_in_points.value() * symbol_info.point_value;
            if (trailing_stop_loss < stop_loss_order->price)
            {
                stop_loss_order->price = trailing_stop_loss;
            }
        }
    }
}

/**
 * @brief Print the statistics of the trader in the console.
 */
void Trader::print_stats_to_console()
{
    this->stats.print();
}

/**
 * @brief Converts the trader to a JSON object.
 * @return JSON representation of the trader.
 */
nlohmann::json Trader::to_json() const
{
    nlohmann::json json;

    // Simple attributes
    json["fitness"] = this->fitness;
    json["score"] = this->score;
    json["generation"] = this->generation;
    json["genome"] = this->genome->to_json();

    // Stats
    json["stats"] = this->stats.to_json();

    // Balance history
    json["balance_history"] = this->balance_history;

    // Trades history
    nlohmann::json trades_history_json = {};
    for (const auto &trade : this->trades_history)
    {
        trades_history_json.push_back({
            {"side", trade.side},
            {"entry_date", trade.entry_date},
            {"entry_price", trade.entry_price},
            {"exit_date", trade.exit_date},
            {"exit_price", trade.exit_price},
            {"size", trade.size},
            {"pnl", trade.pnl},
            {"fees", trade.fees},
            {"pnl_percent", trade.pnl_percent},
            {"pnl_net_percent", trade.pnl_net_percent},
            {"duration", trade.duration},
            {"closed", trade.closed},
        });
    }
    json["trades_history"] = trades_history_json;

    return json;
}

/**
 * @brief Creates a trader from a JSON object.
 * @param json JSON object representing the trader.
 * @param config Configuration file.
 * @param logger Logger of the trader.
 * @return Trader created from the JSON object.
 */
Trader *Trader::from_json(nlohmann::json &json, Config &config, Logger *logger)
{
    neat::Genome *genome = neat::Genome::from_json(json.at("genome"));
    Trader *trader = new Trader(genome, config, logger);

    if (!json.contains("fitness") || !json.contains("score") || !json.contains("generation") || !json.contains("stats") || !json.contains("balance_history") || !json.contains("trades_history"))
    {
        std::cerr << "The JSON object is not a valid trader." << std::endl;
        std::exit(1);
    }

    trader->fitness = json.at("fitness").get<double>();
    trader->score = json.at("score").get<double>();
    trader->generation = json.at("generation").get<int>();
    trader->stats = Stats::from_json(json.at("stats"));
    trader->balance_history = json.at("balance_history").get<std::vector<double>>();

    trader->trades_history = {};
    for (const auto &trade_json : json.at("trades_history"))
    {
        Trade trade = {
            .side = trade_json.at("side").get<PositionSide>(),
            .entry_date = trade_json.at("entry_date").get<time_t>(),
            .entry_price = trade_json.at("entry_price").get<double>(),
            .exit_date = trade_json.at("exit_date").get<time_t>(),
            .exit_price = trade_json.at("exit_price").get<double>(),
            .size = trade_json.at("size").get<double>(),
            .pnl = trade_json.at("pnl").get<double>(),
            .fees = trade_json.at("fees").get<double>(),
            .pnl_percent = trade_json.at("pnl_percent").get<double>(),
            .pnl_net_percent = trade_json.at("pnl_net_percent").get<double>(),
            .duration = trade_json.at("duration").get<int>(),
            .closed = trade_json.at("closed").get<bool>(),
        };
        trader->trades_history.push_back(trade);
    }

    return trader;
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
    gp << "set xlabel 'Time'\n";
    gp << "set ylabel 'Value'\n";

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
 * @param start_date Start date of the report.
 * @param end_date End date of the report.
 */
void Trader::generate_report(const std::string &filename, time_t start_date, time_t end_date)
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
                    <td><b>Max consecutive losing trades:</b></td>
                    <td>)"
        << this->stats.max_consecutive_losing_trades << R"(</td>
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
