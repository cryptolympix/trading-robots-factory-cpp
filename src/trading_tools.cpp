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
 * @param symbol_infos Symbol information including precision details.
 * @return The number of pips between the entry and exit prices.
 */
float calculate_pips(float entry_price, float exit_price, SymbolInfos symbol_infos)
{
    float price_movement = std::abs(exit_price - entry_price);
    float pips = price_movement / symbol_infos.point_value;
    return decimal_round(pips, 5);
}

/**
 * @brief Calculate the monetary value of one pip for a given position.
 *
 * @param market_price The current market price.
 * @param symbol_infos Symbol information including precision details.
 * @param base_currency_conversion_rate The base currency conversion rate. Defaults to 1.0.
 * @return The monetary value of one pip.
 */
float calculate_pip_value(float market_price, SymbolInfos symbol_infos, float base_currency_conversion_rate)
{
    return (symbol_infos.contract_size * symbol_infos.point_value) / (market_price * base_currency_conversion_rate);
}

/**
 * @brief Calculate the profit or loss of a position.
 *
 * @param market_price The current market price.
 * @param position The position.
 * @param symbol_infos Symbol information including precision details.
 * @param base_currency_conversion_rate The base currency conversion rate. Defaults to 1.0.
 * @return Profit or loss of the position.
 */
float calculate_profit_loss(float market_price, Position position, SymbolInfos symbol_infos, float base_currency_conversion_rate)
{
    float price_movement = market_price - position.entry_price;
    price_movement = decimal_round(price_movement, 5);
    int direction_multiplier = (position.side == PositionSide::LONG) ? 1 : -1;
    float pip_value = calculate_pip_value(position.entry_price, symbol_infos, base_currency_conversion_rate);
    float profit_loss = direction_multiplier * (price_movement / symbol_infos.point_value) * pip_value * position.size;
    return decimal_round(profit_loss, 2);
}

/**
 * @brief Calculate the position size based on risk parameters.
 *
 * @param market_price The current market price.
 * @param account_equity Account equity.
 * @param risk_percentage The percentage of account equity at risk.
 * @param stop_loss_pips The desired stop-loss distance in pips.
 * @param symbol_infos Symbol information including precision details.
 * @param base_currency_conversion_rate The base currency conversion rate. Defaults to 1.0.
 * @return The calculated position size.
 */
float calculate_position_size(float market_price, float account_equity, float risk_percentage, float stop_loss_pips, SymbolInfos symbol_infos, float base_currency_conversion_rate)
{
    float pip_value = calculate_pip_value(market_price, symbol_infos, base_currency_conversion_rate);
    float position_size = (account_equity * risk_percentage) / (stop_loss_pips * pip_value);
    position_size = round(position_size / symbol_infos.lot_size_step) * symbol_infos.lot_size_step;
    if (position_size > symbol_infos.min_lot_size)
    {
        return position_size;
    }
    else
    {
        return symbol_infos.min_lot_size;
    }
}

/**
 * @brief Calculate the initial margin required to open a trading position.
 *
 * @param market_price Current market price.
 * @param leverage The leverage of the trading account.
 * @param symbol_infos Symbol information including precision details.
 * @param base_currency_conversion_rate The base currency conversion rate. Defaults to 1.0.
 * @return The initial margin required.
 */
float calculate_initial_margin(float market_price, float leverage, SymbolInfos symbol_infos, float base_currency_conversion_rate)
{
    float initial_margin = symbol_infos.contract_size * market_price * (1 / leverage) * base_currency_conversion_rate;
    return initial_margin;
}

/**
 * @brief Calculate take profit and stop loss prices based on the provided configuration.
 *
 * @param market_price The current market price.
 * @param side The position side (LONG or SHORT).
 * @param config Configuration for take profit and stop loss.
 * @param symbol_infos Symbol information including precision details.
 * @return The calculated take profit and stop loss prices.
 */
std::tuple<float, float> calculate_tp_sl_price(float market_price, PositionSide side, TakeProfitStopLossConfig config, SymbolInfos symbol_infos)
{
    float tp_price = 0.0, sl_price = 0.0;
    if (config.type_take_profit == TypeTakeProfitStopLoss::POINTS)
    {
        if (side == PositionSide::LONG)
        {
            tp_price = market_price + config.take_profit_in_points * symbol_infos.point_value;
        }
        else
        {
            tp_price = market_price - config.take_profit_in_points * symbol_infos.point_value;
        }
    }
    else if (config.type_take_profit == TypeTakeProfitStopLoss::PERCENT)
    {
        if (side == PositionSide::LONG)
        {
            tp_price = decimal_floor(market_price + market_price * config.take_profit_in_percent, symbol_infos.decimal_places);
        }
        else
        {
            tp_price = decimal_ceil(market_price - market_price * config.take_profit_in_percent, symbol_infos.decimal_places);
        }
    }

    if (config.type_stop_loss == TypeTakeProfitStopLoss::POINTS)
    {
        if (side == PositionSide::LONG)
        {
            sl_price = market_price - config.stop_loss_in_points * symbol_infos.point_value;
        }
        else
        {
            sl_price = market_price + config.stop_loss_in_points * symbol_infos.point_value;
        }
    }
    else if (config.type_stop_loss == TypeTakeProfitStopLoss::PERCENT)
    {
        if (side == PositionSide::LONG)
        {
            sl_price = decimal_ceil(market_price - market_price * config.stop_loss_in_percent, symbol_infos.decimal_places);
        }
        else
        {
            sl_price = decimal_floor(market_price + market_price * config.stop_loss_in_percent, symbol_infos.decimal_places);
        }
    }
    return std::make_tuple(tp_price, sl_price);
}

/**
 * @brief Calculate the liquidation price for a trading position.
 *
 * @param position The current position.
 * @param leverage The leverage of the trading account.
 * @return The liquidation price.
 */
float calculate_liquidation_price(Position position, float leverage)
{
    int direction_multiplier = (position.side == PositionSide::LONG) ? -1 : 1;
    float initial_margin_ratio = 100 / leverage;
    float liquidation = (position.entry_price + direction_multiplier * (initial_margin_ratio / leverage));
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
float calculate_commission(float commission_per_lot, float lot_size, float base_currency_conversion_rate)
{
    float commission = commission_per_lot * lot_size * base_currency_conversion_rate;
    return decimal_round(commission, 2);
}
