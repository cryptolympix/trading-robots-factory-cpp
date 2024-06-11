#pragma once

#include <ctime>
#include "../types.hpp"
#include "../neat/config.hpp"
#include "../indicators/indicator.hpp"
#include "../indicators/candle.hpp"
#include "../indicators/candle_patterns.hpp"
#include "../indicators/candle_signals.hpp"
#include "../indicators/momentum.hpp"
#include "../indicators/time.hpp"
#include "../indicators/trend.hpp"
#include "../indicators/volatility.hpp"
#include "../indicators/volume.hpp"
#include "../indicators/momentum_signals.hpp"

std::vector<bool> schedule_working_days = {false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false};
std::vector<bool> schedule_rest_days = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

TradingSchedule schedule = {
    .monday = schedule_working_days,
    .tuesday = schedule_working_days,
    .wednesday = schedule_working_days,
    .thursday = schedule_working_days,
    .friday = schedule_working_days,
    .saturday = schedule_rest_days,
    .sunday = schedule_rest_days,
};

TakeProfitStopLossConfig tpsl_config = {
    .type_stop_loss = TypeTakeProfitStopLoss::POINTS,
    .stop_loss_in_points = 30,
    .stop_loss_in_percent = 0.002,
    .type_take_profit = TypeTakeProfitStopLoss::POINTS,
    .take_profit_in_points = 30,
    .take_profit_in_percent = 0.002,
};

std::tm start_training_date = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 1,
    .tm_mon = 0,
    .tm_year = 2023 - 1900,
};

std::tm end_training_date = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 1,
    .tm_mon = 10,
    .tm_year = 2023 - 1900,
};

std::tm start_test_date = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 1,
    .tm_mon = 10,
    .tm_year = 2023 - 1900,
};

std::tm end_test_date = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 1,
    .tm_mon = 4,
    .tm_year = 2024 - 1900,
};

Config __config__ = {
    .general{
        .name = "ProForexTrader",
        .version = "1.0",
        .symbol = "EURUSD",
        .initial_balance = 1000.0,
        .account_currency = "USD",
        .leverage = 30,
    },
    .strategy{
        .timeframe = TimeFrame::M15,
        .risk_per_trade = 0.05,
        .maximum_trades_per_day = 2,
        .maximum_spread = 8,
        .can_close_trade = false,
        .can_open_long_trade = true,
        .can_open_short_trade = false,
        .take_profit_stop_loss_config = tpsl_config,
        .trading_schedule = schedule,
    },
    .training{
        .generations = 1000,
        .bad_trader_threshold = 0.5,
        .inactive_trader_threshold = 500,
        .training_start_date = std::mktime(&start_training_date),
        .training_end_date = std::mktime(&end_training_date),
        .test_start_date = std::mktime(&start_test_date),
        .test_end_date = std::mktime(&end_test_date),
        .inputs = {
            .indicators = {
                {TimeFrame::M15, {
                                     new NFPWeek(),
                                     new Hour(),
                                     new MarketSession("new-york"),
                                     new MarketSession("london"),
                                     new MarketSession("tokyo"),
                                     new WeekDay("monday"),
                                     new WeekDay("tuesday"),
                                     new WeekDay("wednesday"),
                                     new WeekDay("thursday"),
                                     new WeekDay("friday"),
                                     new CandlePriceChange(),
                                     new CandleClose(),
                                     new CandleVolume(),
                                     new CandleBody(),
                                     new CandleShadowUpper(),
                                     new CandleShadowLower(),
                                     new PeakDistance(20, 0),
                                     new PeakCandleDistance(20, 0),
                                     new MFI(),
                                     new RSI(),
                                     new StochasticOscillator(),
                                     new InstitutionalBias(),
                                     new MACD(),
                                     new EMASlope(21, "close"),
                                     new ATR(),
                                     new StandardDeviation(),
                                     new CCI(),
                                     new ATR(),
                                     new StandardDeviation(),
                                     new AveragePriceChange(),
                                     new HighBreakSignal(),
                                     new NewHighSignal(),
                                     new RSISignal(),
                                     new MFISignal(),
                                 }},
                {TimeFrame::H1, {
                                    new CandlePriceChange(),
                                    new CandleClose(),
                                    new CandleVolume(),
                                    new CandleBody(),
                                    new CandleShadowUpper(),
                                    new CandleShadowLower(),
                                    new PeakDistance(20, 0),
                                    new PeakCandleDistance(20, 0),
                                    new MFI(),
                                    new RSI(),
                                    new StochasticOscillator(),
                                    new InstitutionalBias(),
                                    new MACD(),
                                    new EMASlope(21, "close"),
                                    new ATR(),
                                    new StandardDeviation(),
                                    new CCI(),
                                    new ATR(),
                                    new StandardDeviation(),
                                    new AveragePriceChange(),
                                    new HighBreakSignal(),
                                    new NewHighSignal(),
                                    new RSISignal(),
                                    new MFISignal(),
                                }}},
            .position = {
                // PositionInfo::TYPE,
                // PositionInfo::PNL,
            }}},
    .evaluation{
        .nb_trades_per_day = 1,
        .maximum_trade_duration = 5,
        .expected_return_per_day = 0.02,
        .expected_return_per_month = 0.5,
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
