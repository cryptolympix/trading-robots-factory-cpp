#include <cmath>
#include <tuple>
#include <string>
#include "types.hpp"
#include "utils/math.hpp"
#include "trading_tools.hpp"

/**
 * @brief Calculate the number of pips between two prices.
 *
 * @param entry_price The entry price.
 * @param exit_price The exit price.
 * @param symbol_info Symbol information including precision details.
 * @return The number of pips between the entry and exit prices.
 */
double calculate_pips(double entry_price, double exit_price, SymbolInfo symbol_info)
{
    double price_movement = std::abs(exit_price - entry_price);
    double pips = price_movement / symbol_info.point_value;
    return decimal_round(pips, 5);
}

/**
 * @brief Calculate the monetary value of one pip for a given position.
 *
 * @param market_price The current market price.
 * @param symbol_info Symbol information including precision details.
 * @param base_currency_conversion_rate The base currency conversion rate. Defaults to 1.0.
 * @return The monetary value of one pip.
 */
double calculate_pip_value(double market_price, SymbolInfo symbol_info, double base_currency_conversion_rate)
{
    return (symbol_info.contract_size * symbol_info.point_value) / (market_price * base_currency_conversion_rate);
}

/**
 * @brief Calculate the profit or loss of a position.
 *
 * @param market_price The current market price.
 * @param position The position.
 * @param symbol_info Symbol information including precision details.
 * @param base_currency_conversion_rate The base currency conversion rate. Defaults to 1.0.
 * @return Profit or loss of the position.
 */
double calculate_profit_loss(double market_price, Position position, SymbolInfo symbol_infos, double base_currency_conversion_rate)
{
    double price_movement = market_price - position.entry_price;
    price_movement = decimal_round(price_movement, 5);
    int direction_multiplier = (position.side == PositionSide::LONG) ? 1 : -1;
    double profit_loss = price_movement * symbol_infos.contract_size * position.size * direction_multiplier * base_currency_conversion_rate;
    return decimal_round(profit_loss, 2);
}

/**
 * @brief Calculate the position size based on risk parameters.
 *
 * @param market_price The current market price.
 * @param account_equity Account equity.
 * @param risk_percentage The percentage of account equity at risk.
 * @param stop_loss_pips The desired stop-loss distance in pips.
 * @param symbol_info Symbol information including precision details.
 * @param base_currency_conversion_rate The base currency conversion rate. Defaults to 1.0.
 * @return The calculated position size.
 */
double calculate_position_size(double market_price, double account_equity, double risk_percentage, double stop_loss_pips, SymbolInfo symbol_info, double base_currency_conversion_rate)
{
    double pip_value = calculate_pip_value(market_price, symbol_info, base_currency_conversion_rate);
    double position_size = (account_equity * risk_percentage) / (stop_loss_pips * pip_value);
    position_size = round(position_size / symbol_info.lot_size_step) * symbol_info.lot_size_step;
    if (position_size > symbol_info.min_lot_size)
    {
        return position_size;
    }
    else
    {
        return symbol_info.min_lot_size;
    }
}

/**
 * @brief Calculate the initial margin required to open a trading position.
 *
 * @param market_price Current market price.
 * @param leverage The leverage of the trading account.
 * @param symbol_info Symbol information including precision details.
 * @param base_currency_conversion_rate The base currency conversion rate. Defaults to 1.0.
 * @return The initial margin required.
 */
double calculate_initial_margin(double market_price, int leverage, SymbolInfo symbol_info, double base_currency_conversion_rate)
{
    double initial_margin = symbol_info.contract_size * market_price * (1 / static_cast<double>(leverage)) * base_currency_conversion_rate;
    return initial_margin;
}

/**
 * @brief Calculate take profit and stop loss prices based on the provided configuration.
 *
 * @param market_price The current market price.
 * @param side The position side (LONG or SHORT).
 * @param config Configuration for take profit and stop loss.
 * @param symbol_info Symbol information including precision details.
 * @return The calculated take profit and stop loss prices.
 */
std::tuple<double, double> calculate_tp_sl_price(double market_price, PositionSide side, TakeProfitStopLossConfig config, SymbolInfo symbol_info)
{
    double tp_price = 0.0, sl_price = 0.0;
    if (config.type_take_profit == TypeTakeProfitStopLoss::POINTS)
    {
        if (side == PositionSide::LONG)
        {
            tp_price = market_price + config.take_profit_in_points * symbol_info.point_value;
        }
        else
        {
            tp_price = market_price - config.take_profit_in_points * symbol_info.point_value;
        }
    }
    else if (config.type_take_profit == TypeTakeProfitStopLoss::PERCENT)
    {
        if (side == PositionSide::LONG)
        {
            tp_price = decimal_floor(market_price + market_price * config.take_profit_in_percent, symbol_info.decimal_places);
        }
        else
        {
            tp_price = decimal_ceil(market_price - market_price * config.take_profit_in_percent, symbol_info.decimal_places);
        }
    }

    if (config.type_stop_loss == TypeTakeProfitStopLoss::POINTS)
    {
        if (side == PositionSide::LONG)
        {
            sl_price = market_price - config.stop_loss_in_points * symbol_info.point_value;
        }
        else
        {
            sl_price = market_price + config.stop_loss_in_points * symbol_info.point_value;
        }
    }
    else if (config.type_stop_loss == TypeTakeProfitStopLoss::PERCENT)
    {
        if (side == PositionSide::LONG)
        {
            sl_price = decimal_ceil(market_price - market_price * config.stop_loss_in_percent, symbol_info.decimal_places);
        }
        else
        {
            sl_price = decimal_floor(market_price + market_price * config.stop_loss_in_percent, symbol_info.decimal_places);
        }
    }
    return std::make_tuple(tp_price, sl_price);
}

/**
 * @brief Calculate the liquidation price for a trading position.
 *
 * @param position The current position.
 * @param leverage The leverage of the trading account.
 * @param symbol_info Symbol information including precision details.
 * @return The liquidation price.
 */
double calculate_liquidation_price(Position position, int leverage, SymbolInfo symbol_info)
{
    int direction_multiplier = (position.side == PositionSide::LONG) ? -1 : 1;
    double liquidation = position.entry_price + direction_multiplier * (position.entry_price / leverage);
    return liquidation;
}

/**
 * @brief Calculate commission based on a fixed commission per lot.
 *
 * @param commission_per_lot Commission amount per lot.
 * @param lot_size Number of lots or units.
 * @param base_currency_conversion_rate The base currency conversion rate. Defaults to 1.0.
 * @return Commission amount.
 */
double calculate_commission(double commission_per_lot, double lot_size, double base_currency_conversion_rate)
{
    double commission = commission_per_lot * lot_size * base_currency_conversion_rate;
    return decimal_round(commission, 2);
}
