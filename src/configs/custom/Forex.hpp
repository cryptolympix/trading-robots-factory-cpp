#ifndef FOREX_HPP
#define FOREX_HPP

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

Config Forex = {
    .general{
        .name = "Forex",
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
        .training_start_date = std::mktime(new std::tm({
            .tm_sec = 0,
            .tm_min = 0,
            .tm_hour = 0,
            .tm_mday = 1,
            .tm_mon = 0,
            .tm_year = 2020 - 1900,
        })),
        .training_end_date = std::mktime(new std::tm({
            .tm_sec = 0,
            .tm_min = 0,
            .tm_hour = 0,
            .tm_mday = 1,
            .tm_mon = 0,
            .tm_year = 2024 - 1900,
        })),
        .test_start_date = std::mktime(new std::tm({
            .tm_sec = 0,
            .tm_min = 0,
            .tm_hour = 0,
            .tm_mday = 1,
            .tm_mon = 0,
            .tm_year = 2024 - 1900,
        })),
        .test_end_date = std::mktime(new std::tm({
            .tm_sec = 0,
            .tm_min = 0,
            .tm_hour = 0,
            .tm_mday = 1,
            .tm_mon = 0,
            .tm_year = 2025 - 1900,
        })),
        .inputs = {
            .indicators = {
                {
                    TimeFrame::H1,
                    {
                        new NFPWeek(),
                        new MarketSession("new-york"),
                        new MarketSession("london"),
                        new MarketSession("tokyo"),
                        new CandlePriceChange(),
                        new RSI(14),
                        new StochasticOscillator(14, 3),
                        new TSI(13, 25),
                        new WPR(14),
                        new CCI(20),
                        new InstitutionalBias(9, 18),
                        new StandardDeviation(7),
                        new StandardDeviation(14),
                        new AveragePriceChange(5),
                        new AveragePriceChange(10),
                        new CMF(20),
                        new HighBreakSignal(10),
                        new LowBreakSignal(10),
                        new NewHighSignal(10),
                        new NewLowSignal(10),
                    },
                },
                {
                    TimeFrame::H4,
                    {
                        new CandlePriceChange(),
                        new RSI(14),
                        new StochasticOscillator(14, 3),
                        new TSI(13, 25),
                        new WPR(14),
                        new CCI(20),
                        new InstitutionalBias(9, 18),
                        new StandardDeviation(7),
                        new StandardDeviation(14),
                        new AveragePriceChange(5),
                        new AveragePriceChange(10),
                        new CMF(20),
                        new HighBreakSignal(10),
                        new LowBreakSignal(10),
                        new NewHighSignal(10),
                        new NewLowSignal(10),
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