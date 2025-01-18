#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include <ctime>
#include <vector>
#include <string>
#include "../../types.hpp"
#include "../../trading/schedule.hpp"
#include "../../neat/config.hpp"
#include "../default.hpp"

// Indicators
#include "../../indicators/indicator.hpp"
#include "../../indicators/candle.hpp"
#include "../../indicators/candle_patterns.hpp"
#include "../../indicators/candle_signals.hpp"
#include "../../indicators/momentum.hpp"
#include "../../indicators/momentum_signals.hpp"
#include "../../indicators/time.hpp"
#include "../../indicators/trend.hpp"
#include "../../indicators/trend_signals.hpp"
#include "../../indicators/volatility.hpp"
#include "../../indicators/volatility_signals.hpp"
#include "../../indicators/volume.hpp"
#include "../../indicators/volume_signals.hpp"

std::tm start_training_date = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 1,
    .tm_mon = 0,
    .tm_year = 2020 - 1900,
};

std::tm end_training_date = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 31,
    .tm_mon = 11,
    .tm_year = 2023 - 1900,
};

std::tm start_test_date = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 1,
    .tm_mon = 0,
    .tm_year = 2024 - 1900,
};

std::tm end_test_date = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 1,
    .tm_mon = 0,
    .tm_year = 2025 - 1900,
};

Config Example = {
    .general{
        .name = "Example",
        .version = "1.0",
        .symbol = "EURUSD",
        .initial_balance = 1000.0,
        .account_currency = "USD",
        .leverage = 10,
    },
    .strategy{
        .timeframe = TimeFrame::H1,
        .risk_per_trade = 0.02,
        .maximum_trades_per_day = 2,
        .can_close_trade = false,
        .can_open_long_trade = true,
        .can_open_short_trade = true,
        .take_profit_stop_loss_config = default_tpsl_config,
        .trading_schedule = default_schedule,
        // .trailing_stop_loss_config = default_tsl_config,
    },
    .training{
        .generations = 1000,
        .bad_trader_threshold = 0.3, // If the trader loose more than 50% of its initial balance, it's a bad trader
        .inactive_trader_threshold = 200,
        .training_start_date = std::mktime(&start_training_date),
        .training_end_date = std::mktime(&end_training_date),
        .test_start_date = std::mktime(&start_test_date),
        .test_end_date = std::mktime(&end_test_date),
        .inputs = {
            .indicators = {
                {
                    TimeFrame::H1,
                    {
                        new NFPWeek(),
                        new MarketSession("new-york"),
                        new MarketSession("london"),
                        new MarketSession("tokyo"),
                        new CandleVolume(0),
                        new CandleVolume(1),
                        new CandleVolume(2),
                        new CandleVolume(3),
                        new CandleVolume(4),
                        new CandleVolume(5),
                        new CandleVolume(6),
                        new CandleVolume(7),
                        new CandleVolume(8),
                        new CandleVolume(9),
                        new CandlePriceChange(0),
                        new CandlePriceChange(1),
                        new CandlePriceChange(2),
                        new CandlePriceChange(3),
                        new CandlePriceChange(4),
                        new CandlePriceChange(5),
                        new CandlePriceChange(6),
                        new CandlePriceChange(7),
                        new CandlePriceChange(8),
                        new CandlePriceChange(9),
                        new CandleBody(0),
                        new CandleBody(1),
                        new CandleBody(2),
                        new CandleBody(3),
                        new CandleBody(4),
                        new CandleBody(5),
                        new CandleBody(6),
                        new CandleBody(7),
                        new CandleBody(8),
                        new CandleBody(9),
                        new CandleShadowUpper(0),
                        new CandleShadowUpper(1),
                        new CandleShadowUpper(2),
                        new CandleShadowUpper(3),
                        new CandleShadowUpper(4),
                        new CandleShadowUpper(5),
                        new CandleShadowUpper(6),
                        new CandleShadowUpper(7),
                        new CandleShadowUpper(8),
                        new CandleShadowUpper(9),
                        new CandleShadowLower(0),
                        new CandleShadowLower(1),
                        new CandleShadowLower(2),
                        new CandleShadowLower(3),
                        new CandleShadowLower(4),
                        new CandleShadowLower(5),
                        new CandleShadowLower(6),
                        new CandleShadowLower(7),
                        new CandleShadowLower(8),
                        new CandleShadowLower(9),
                        new AveragePriceChange(5),
                        new AveragePriceChange(10),
                        new AveragePriceChange(15),
                        new StandardDeviation(7),
                        new StandardDeviation(14),
                        new StandardDeviation(21),
                        new InstitutionalBias(9, 18),
                        new HighBreakSignal(10),
                        new LowBreakSignal(10),
                        new NewHighSignal(10),
                        new NewLowSignal(10),
                        new HighBreakSignal(20),
                        new LowBreakSignal(20),
                        new NewHighSignal(20),
                        new NewLowSignal(20),
                    },
                },
                {
                    TimeFrame::H4,
                    {
                        new CandleVolume(0),
                        new CandleVolume(1),
                        new CandleVolume(2),
                        new CandleVolume(3),
                        new CandleVolume(4),
                        new CandleVolume(5),
                        new CandleVolume(6),
                        new CandleVolume(7),
                        new CandleVolume(8),
                        new CandleVolume(9),
                        new CandlePriceChange(0),
                        new CandlePriceChange(1),
                        new CandlePriceChange(2),
                        new CandlePriceChange(3),
                        new CandlePriceChange(4),
                        new CandlePriceChange(5),
                        new CandlePriceChange(6),
                        new CandlePriceChange(7),
                        new CandlePriceChange(8),
                        new CandlePriceChange(9),
                        new CandleBody(0),
                        new CandleBody(1),
                        new CandleBody(2),
                        new CandleBody(3),
                        new CandleBody(4),
                        new CandleBody(5),
                        new CandleBody(6),
                        new CandleBody(7),
                        new CandleBody(8),
                        new CandleBody(9),
                        new CandleShadowUpper(0),
                        new CandleShadowUpper(1),
                        new CandleShadowUpper(2),
                        new CandleShadowUpper(3),
                        new CandleShadowUpper(4),
                        new CandleShadowUpper(5),
                        new CandleShadowUpper(6),
                        new CandleShadowUpper(7),
                        new CandleShadowUpper(8),
                        new CandleShadowUpper(9),
                        new CandleShadowLower(0),
                        new CandleShadowLower(1),
                        new CandleShadowLower(2),
                        new CandleShadowLower(3),
                        new CandleShadowLower(4),
                        new CandleShadowLower(5),
                        new CandleShadowLower(6),
                        new CandleShadowLower(7),
                        new CandleShadowLower(8),
                        new CandleShadowLower(9),
                        new AveragePriceChange(5),
                        new AveragePriceChange(10),
                        new AveragePriceChange(15),
                        new StandardDeviation(7),
                        new StandardDeviation(14),
                        new StandardDeviation(21),
                        new InstitutionalBias(9, 18),
                        new HighBreakSignal(10),
                        new LowBreakSignal(10),
                        new NewHighSignal(10),
                        new NewLowSignal(10),
                        new HighBreakSignal(20),
                        new LowBreakSignal(20),
                        new NewHighSignal(20),
                        new NewLowSignal(20),
                    },
                },
            },
            .position = {},
        },
    },
    .evaluation{
        .maximize_nb_trades = false,
        .minimum_nb_trades = 2000,
        .maximum_trade_duration = 5,
        // .expected_return_per_day = 0.02,
        // .expected_return_per_month = 0.5,
        .maximum_drawdown = 0.05,
        .minimum_winrate = 0.8,
        .minimum_profit_factor = 3,
    },
#ifdef _WIN32
    .neat = neat::load_config("C:\\Users\\Maxime\\AppData\\Roaming\\MetaQuotes\\Terminal\\D0E8209F77C8CF37AD8BF550E51FF075\\MQL5\\Libraries\\neat_config.txt")
#else
    .neat = neat::load_config("src/configs/neat_config.txt")
#endif
};

#endif