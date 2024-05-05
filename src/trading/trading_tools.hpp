#ifndef TRADING_TOOLS_H
#define TRADING_TOOLS_H

#include <cmath>
#include <tuple>
#include <string>
#include "../types.hpp"

/**
 * @brief Calculate the number of pips between two prices.
 *
 * @param entry_price The entry price.
 * @param exit_price The exit price.
 * @param symbol_info Symbol information including precision details.
 * @return The number of pips between the entry and exit prices.
 */
double calculate_pips(double entry_price, double exit_price, SymbolInfo symbol_info);

/**
 * @brief Calculate the monetary value of one pip for a given position.
 *
 * @param market_price The current market price.
 * @param symbol_info Symbol information including precision details.
 * @param base_currency_conversion_rate The base currency conversion rate. Defaults to 1.0.
 * @return The monetary value of one pip.
 */
double calculate_pip_value(double market_price, SymbolInfo symbol_info, double base_currency_conversion_rate = 1.0);

/**
 * @brief Calculate the profit or loss of a position.
 *
 * @param market_price The current market price.
 * @param position The position.
 * @param symbol_info Symbol information including precision details.
 * @param base_currency_conversion_rate The base currency conversion rate. Defaults to 1.0.
 * @return Profit or loss of the position.
 */
double calculate_profit_loss(double market_price, Position position, SymbolInfo symbol_info, double base_currency_conversion_rate = 1.0);

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
double calculate_position_size(double market_price, double account_equity, double risk_percentage, double stop_loss_pips, SymbolInfo symbol_info, double base_currency_conversion_rate = 1.0);

/**
 * @brief Calculate the initial margin required to open a trading position.
 *
 * @param market_price Current market price.
 * @param leverage The leverage of the trading account.
 * @param symbol_info Symbol information including precision details.
 * @param base_currency_conversion_rate The base currency conversion rate. Defaults to 1.0.
 * @return The initial margin required.
 */
double calculate_initial_margin(double market_price, int leverage, SymbolInfo symbol_info, double base_currency_conversion_rate = 1.0);

/**
 * @brief Calculate take profit and stop loss prices based on the provided configuration.
 *
 * @param market_price The current market price.
 * @param side The position side (LONG or SHORT).
 * @param config Configuration for take profit and stop loss.
 * @param symbol_info Symbol information including precision details.
 * @return The calculated take profit and stop loss prices.
 */
std::tuple<double, double> calculate_tp_sl_price(double market_price, PositionSide side, TakeProfitStopLossConfig config, SymbolInfo symbol_info);

/**
 * @brief Calculate the liquidation price for a trading position.
 *
 * @param position The current position.
 * @param leverage The leverage of the trading account.
 * @param symbol_info Symbol information including precision details.
 * @return The liquidation price.
 */
double calculate_liquidation_price(Position position, int leverage, SymbolInfo symbol_info);

/**
 * @brief Calculate commission based on a fixed commission per lot.
 *
 * @param commission_per_lot Commission amount per lot.
 * @param lot_size Number of lots or units.
 * @param base_currency_conversion_rate The base currency conversion rate. Defaults to 1.0.
 * @return Commission amount.
 */
double calculate_commission(double commission_per_lot, double lot_size, double base_currency_conversion_rate = 1.0);

#endif // TRADING_TOOLS_H
