#include <gtest/gtest.h>
#include "../trading_tools.hpp"
#include "../../types.hpp"

// Test fixture for trading tools
class TradingToolsTest : public ::testing::Test
{
protected:
    SymbolInfo symbol_infos;
    double base_currency_conversion_rate;

    void SetUp() override
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
TEST_F(TradingToolsTest, CalculatePips)
{
    double entry_price = 1.12000;
    double exit_price = 1.12050;
    double expected_pips = 5.0; // Expected number of pips

    double result_pips = calculate_pips(entry_price, exit_price, symbol_infos);

    // Assert that the result matches the expected number of pips
    ASSERT_DOUBLE_EQ(expected_pips, result_pips);
}

// Test case for calculate_pip_value function
TEST_F(TradingToolsTest, CalculatePipValue)
{
    double market_price = 1.00000;
    double expected_pip_value = 10.0;

    double result_pip_value = calculate_pip_value(market_price, symbol_infos);

    // Assert that the result matches the expected pip value
    ASSERT_DOUBLE_EQ(expected_pip_value, result_pip_value);
}

// Test case for calculate_profit_loss function
TEST_F(TradingToolsTest, CalculateProfitLoss)
{
    Position position;
    int leverage = 100;

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
TEST_F(TradingToolsTest, CalculatePositionSize)
{
    double market_price = 1.0000;
    double account_equity = 10000.0;
    double risk_percentage = 0.01;
    double stop_loss_pips = 50.0;

    // Assert that the result matches the expected position size
    ASSERT_DOUBLE_EQ(0.2, calculate_position_size(market_price, account_equity, risk_percentage, stop_loss_pips, symbol_infos, base_currency_conversion_rate));
}

// Test case for calculate_initial_margin function
TEST_F(TradingToolsTest, CalculateInitialMargin)
{
    double market_price = 1.00000;
    int leverage = 100;

    // Assert that the result matches the expected initial margin
    ASSERT_DOUBLE_EQ(1000.0, calculate_initial_margin(market_price, leverage, symbol_infos, base_currency_conversion_rate));
}

// Test case for calculate_tp_sl_price function
TEST_F(TradingToolsTest, CalculateTpSlPriceWithPoints)
{
    double market_price = 1.00000;
    TakeProfitStopLossConfig config;

    config.type_take_profit = TypeTakeProfitStopLoss::POINTS;
    config.take_profit_in_points = 100.0;
    config.type_stop_loss = TypeTakeProfitStopLoss::POINTS;
    config.stop_loss_in_points = 100.0;
    auto result_tp_sl_prices_long = calculate_tp_sl_price(market_price, std::vector<Candle>(), PositionSide::LONG, config, symbol_infos);
    auto result_tp_sl_prices_short = calculate_tp_sl_price(market_price, std::vector<Candle>(), PositionSide::SHORT, config, symbol_infos);

    // Assert that the result matches the expected take profit and stop loss prices
    ASSERT_DOUBLE_EQ(1.01, std::get<0>(result_tp_sl_prices_long));
    ASSERT_DOUBLE_EQ(0.99, std::get<1>(result_tp_sl_prices_long));
    ASSERT_DOUBLE_EQ(0.99, std::get<0>(result_tp_sl_prices_short));
    ASSERT_DOUBLE_EQ(1.01, std::get<1>(result_tp_sl_prices_short));
}

// Test case for calculate_tp_sl_price function
TEST_F(TradingToolsTest, CalculateTpSlPriceWithPercent)
{
    double market_price = 1.00000;
    TakeProfitStopLossConfig config;
    config.type_take_profit = TypeTakeProfitStopLoss::PERCENT;
    config.take_profit_in_percent = 0.01;
    config.type_stop_loss = TypeTakeProfitStopLoss::PERCENT;
    config.stop_loss_in_percent = 0.01;

    auto result_tp_sl_prices_long = calculate_tp_sl_price(market_price, std::vector<Candle>(), PositionSide::LONG, config, symbol_infos);
    auto result_tp_sl_prices_short = calculate_tp_sl_price(market_price, std::vector<Candle>(), PositionSide::SHORT, config, symbol_infos);

    // Assert that the result matches the expected take profit and stop loss prices
    ASSERT_DOUBLE_EQ(1.01, std::get<0>(result_tp_sl_prices_long));
    ASSERT_DOUBLE_EQ(0.99, std::get<1>(result_tp_sl_prices_long));
    ASSERT_DOUBLE_EQ(0.99, std::get<0>(result_tp_sl_prices_short));
    ASSERT_DOUBLE_EQ(1.01, std::get<1>(result_tp_sl_prices_short));
}

TEST_F(TradingToolsTest, CalculateTpSlPriceWithExtremum)
{
    double market_price = 1.020000;
    TakeProfitStopLossConfig config;
    config.type_take_profit = TypeTakeProfitStopLoss::EXTREMUM;
    config.take_profit_extremum_period = 5;
    config.type_stop_loss = TypeTakeProfitStopLoss::EXTREMUM;
    config.stop_loss_extremum_period = 5;

    time_t now = std::time(nullptr);
    std::vector<Candle> candles = {
        Candle{now, 1.01400, 1.01650, 1.01350, 1.015000, 0.0, 0.0, 0.0},
        Candle{now, 1.01500, 1.02000, 1.01350, 1.018000, 0.0, 0.0, 0.0},
        Candle{now, 1.01800, 1.02200, 1.01750, 1.021000, 0.0, 0.0, 0.0},
        Candle{now, 1.02100, 1.02500, 1.02000, 1.021500, 0.0, 0.0, 0.0},
        Candle{now, 1.02100, 1.02200, 1.01850, 1.020000, 0.0, 0.0, 0.0},
    };

    auto result_tp_sl_prices_long = calculate_tp_sl_price(market_price, candles, PositionSide::LONG, config, symbol_infos);
    auto result_tp_sl_prices_short = calculate_tp_sl_price(market_price, candles, PositionSide::SHORT, config, symbol_infos);

    // Assert that the result matches the expected take profit and stop loss prices
    ASSERT_DOUBLE_EQ(1.02500, std::get<0>(result_tp_sl_prices_long));
    ASSERT_DOUBLE_EQ(1.01350, std::get<1>(result_tp_sl_prices_long));
    ASSERT_DOUBLE_EQ(1.01350, std::get<0>(result_tp_sl_prices_short));
    ASSERT_DOUBLE_EQ(1.02500, std::get<1>(result_tp_sl_prices_short));
}

TEST_F(TradingToolsTest, CalculateTpSlPriceWithATR)
{
    double market_price = 1.020000;
    TakeProfitStopLossConfig config;
    config.type_take_profit = TypeTakeProfitStopLoss::ATR;
    config.take_profit_atr_period = 5;
    config.take_profit_atr_multiplier = 1.0;
    config.type_stop_loss = TypeTakeProfitStopLoss::ATR;
    config.stop_loss_atr_period = 5;
    config.stop_loss_atr_multiplier = 1.0;

    time_t now = std::time(nullptr);
    std::vector<Candle> candles = {
        Candle{now, 1.01400, 1.01650, 1.01350, 1.015000, 0.0, 0.0, 0.0},
        Candle{now, 1.01500, 1.02000, 1.01350, 1.018000, 0.0, 0.0, 0.0},
        Candle{now, 1.01800, 1.02200, 1.01750, 1.021000, 0.0, 0.0, 0.0},
        Candle{now, 1.02100, 1.02500, 1.02000, 1.021500, 0.0, 0.0, 0.0},
        Candle{now, 1.02100, 1.02200, 1.01850, 1.020000, 0.0, 0.0, 0.0},
    };

    auto result_tp_sl_prices_long = calculate_tp_sl_price(market_price, candles, PositionSide::LONG, config, symbol_infos);
    auto result_tp_sl_prices_short = calculate_tp_sl_price(market_price, candles, PositionSide::SHORT, config, symbol_infos);

    double tp_long = std::get<0>(result_tp_sl_prices_long);
    double sl_long = std::get<1>(result_tp_sl_prices_long);
    double tp_short = std::get<0>(result_tp_sl_prices_short);
    double sl_short = std::get<1>(result_tp_sl_prices_short);

    // Assert that the result matches the expected take profit and stop loss prices
    ASSERT_TRUE(tp_long > 1.020000 && tp_long < 1.02500);
    ASSERT_TRUE(sl_long > 1.01350 && sl_long < 1.020000);
    ASSERT_TRUE(tp_short > 1.01350 && tp_short < 1.020000);
    ASSERT_TRUE(sl_short > 1.020000 && sl_short < 1.02500);
}

// Test case for calculate_liquidation_price function
TEST_F(TradingToolsTest, CalculateLiquidationPrice)
{
    int leverage = 100;
    Position *long_position = new Position{PositionSide::LONG, 0.01, 1.0000, std::time(nullptr), 0.0};
    ASSERT_FLOAT_EQ(0.99, calculate_liquidation_price(long_position, leverage, symbol_infos));

    Position *short_position = new Position{PositionSide::SHORT, 0.01, 1.0000, std::time(nullptr), 0.0};
    ASSERT_FLOAT_EQ(1.01, calculate_liquidation_price(short_position, leverage, symbol_infos));
}

// Test case for calculate_commission function
TEST_F(TradingToolsTest, CalculateCommission)
{
    double commission_per_lot = 2.0;
    double lot_size = 1.0;

    // Assert that the result matches the expected commission
    ASSERT_FLOAT_EQ(2.0, calculate_commission(commission_per_lot, lot_size, base_currency_conversion_rate));
}
