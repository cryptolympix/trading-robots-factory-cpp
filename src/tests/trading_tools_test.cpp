#include <gtest/gtest.h>
#include "../trading_tools.hpp"
#include "../types.hpp"

// Test fixture for trading tools
class TradingToolsTest : public ::testing::Test
{
protected:
    SymbolInfos symbol_infos;
    double base_currency_conversion_rate;

    virtual void SetUp() override
    {
        // Initialize any necessary objects or variables
        symbol_infos.asset = "EUR";
        symbol_infos.base = "USD";
        symbol_infos.decimal_places = 5;
        symbol_infos.point_value = 0.0001;
        symbol_infos.contract_size = 100000;
        symbol_infos.min_lot_size = 0;
        symbol_infos.max_lot_size = 10;
        symbol_infos.lot_size_step = 0.01;
        symbol_infos.commission_per_lot = 2.0;
        symbol_infos.commission_base = "EUR";

        base_currency_conversion_rate = 1.0;
    }
};

// Test case for calculate_pips function
TEST_F(TradingToolsTest, TestCalculatePips)
{
    double entry_price = 1.12000;
    double exit_price = 1.12050;
    double expected_pips = 5.0; // Expected number of pips

    double result_pips = calculate_pips(entry_price, exit_price, symbol_infos);

    // Assert that the result matches the expected number of pips
    ASSERT_DOUBLE_EQ(expected_pips, result_pips);
}

// Test case for calculate_pip_value function
TEST_F(TradingToolsTest, TestCalculatePipValue)
{
    double market_price = 1.00000;
    double expected_pip_value = 10.0;

    double result_pip_value = calculate_pip_value(market_price, symbol_infos);

    // Assert that the result matches the expected pip value
    ASSERT_DOUBLE_EQ(expected_pip_value, result_pip_value);
}

// Test case for calculate_profit_loss function
TEST_F(TradingToolsTest, TestCalculateProfitLoss)
{
    Position position;

    position.entry_price = 1.00000;
    position.side = PositionSide::LONG;
    position.size = 1.0;
    ASSERT_DOUBLE_EQ(100.0, calculate_profit_loss(1.00100, position, symbol_infos, base_currency_conversion_rate));

    position.entry_price = 1.00000;
    position.side = PositionSide::LONG;
    position.size = 1.0;
    ASSERT_DOUBLE_EQ(-100.0, calculate_profit_loss(0.99900, position, symbol_infos, base_currency_conversion_rate));

    position.entry_price = 1.00000;
    position.side = PositionSide::SHORT;
    position.size = 1.0;
    ASSERT_DOUBLE_EQ(-100.0, calculate_profit_loss(1.00100, position, symbol_infos, base_currency_conversion_rate));

    position.entry_price = 1.00000;
    position.side = PositionSide::SHORT;
    position.size = 1.0;
    ASSERT_DOUBLE_EQ(100.0, calculate_profit_loss(0.99900, position, symbol_infos, base_currency_conversion_rate));
}

// Test case for calculate_position_size function
TEST_F(TradingToolsTest, TestCalculatePositionSize)
{
    double market_price = 1.0000;
    double account_equity = 10000.0;
    double risk_percentage = 0.01;
    double stop_loss_pips = 50.0;

    // Assert that the result matches the expected position size
    ASSERT_DOUBLE_EQ(0.2, calculate_position_size(market_price, account_equity, risk_percentage, stop_loss_pips, symbol_infos, base_currency_conversion_rate));
}

// Test case for calculate_initial_margin function
TEST_F(TradingToolsTest, TestCalculateInitialMargin)
{
    double market_price = 1.00000;
    int leverage = 100;

    // Assert that the result matches the expected initial margin
    ASSERT_DOUBLE_EQ(1000.0, calculate_initial_margin(market_price, leverage, symbol_infos, base_currency_conversion_rate));
}

// Test case for calculate_tp_sl_price function
TEST_F(TradingToolsTest, TestCalculateTpSlPriceWithPoints)
{
    double market_price = 1.00000;
    TakeProfitStopLossConfig config;

    config.type_take_profit = TypeTakeProfitStopLoss::POINTS;
    config.take_profit_in_points = 100.0;
    config.type_stop_loss = TypeTakeProfitStopLoss::POINTS;
    config.stop_loss_in_points = 100.0;
    auto result_tp_sl_prices_long = calculate_tp_sl_price(market_price, PositionSide::LONG, config, symbol_infos);
    auto result_tp_sl_prices_short = calculate_tp_sl_price(market_price, PositionSide::SHORT, config, symbol_infos);

    // Assert that the result matches the expected take profit and stop loss prices
    ASSERT_DOUBLE_EQ(1.01, std::get<0>(result_tp_sl_prices_long));
    ASSERT_DOUBLE_EQ(0.99, std::get<1>(result_tp_sl_prices_long));
    ASSERT_DOUBLE_EQ(0.99, std::get<0>(result_tp_sl_prices_short));
    ASSERT_DOUBLE_EQ(1.01, std::get<1>(result_tp_sl_prices_short));
}

// Test case for calculate_tp_sl_price function
TEST_F(TradingToolsTest, TestCalculateTpSlPriceWithPercent)
{
    double market_price = 1.00000;
    TakeProfitStopLossConfig config;

    config.type_take_profit = TypeTakeProfitStopLoss::PERCENT;
    config.take_profit_in_percent = 0.01;
    config.type_stop_loss = TypeTakeProfitStopLoss::PERCENT;
    config.stop_loss_in_percent = 0.01;
    auto result_tp_sl_prices_long = calculate_tp_sl_price(market_price, PositionSide::LONG, config, symbol_infos);
    auto result_tp_sl_prices_short = calculate_tp_sl_price(market_price, PositionSide::SHORT, config, symbol_infos);

    // Assert that the result matches the expected take profit and stop loss prices
    ASSERT_DOUBLE_EQ(1.01, std::get<0>(result_tp_sl_prices_long));
    ASSERT_DOUBLE_EQ(0.99, std::get<1>(result_tp_sl_prices_long));
    ASSERT_DOUBLE_EQ(0.99, std::get<0>(result_tp_sl_prices_short));
    ASSERT_DOUBLE_EQ(1.01, std::get<1>(result_tp_sl_prices_short));
}

// Test case for calculate_liquidation_price function
TEST_F(TradingToolsTest, TestCalculateLiquidationPrice)
{
    int leverage = 100;
    Position position;
    position.entry_price = 1.0000;

    position.side = PositionSide::LONG;
    ASSERT_FLOAT_EQ(0.99, calculate_liquidation_price(position, leverage));

    position.side = PositionSide::SHORT;
    ASSERT_FLOAT_EQ(1.01, calculate_liquidation_price(position, leverage));
}

// Test case for calculate_commission function
TEST_F(TradingToolsTest, TestCalculateCommission)
{
    double commission_per_lot = 2.0;
    double lot_size = 1.0;

    // Assert that the result matches the expected commission
    ASSERT_FLOAT_EQ(2.0, calculate_commission(commission_per_lot, lot_size, base_currency_conversion_rate));
}
