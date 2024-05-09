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

TradingSchedule schedule = {
    .monday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false},
    .tuesday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false},
    .wednesday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false},
    .thursday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false},
    .friday = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false},
    .saturday = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
    .sunday = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}};

TakeProfitStopLossConfig tpsl_config = {
    .type_stop_loss = TypeTakeProfitStopLoss::PERCENT,
    .stop_loss_in_points = 300,
    .stop_loss_in_percent = 0.001,
    .type_take_profit = TypeTakeProfitStopLoss::PERCENT,
    .take_profit_in_points = 300,
    .take_profit_in_percent = 0.002,
};

std::tm start_training_date = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 1,
    .tm_mon = 0,
    .tm_year = 2023 - 1900};

std::tm end_training_date = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 1,
    .tm_mon = 3,
    .tm_year = 2023 - 1900};

std::tm start_test_date = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 1,
    .tm_mon = 3,
    .tm_year = 2023 - 1900};

std::tm end_test_date = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 31,
    .tm_mon = 4,
    .tm_year = 2023 - 1900};

Config __config__ = {
    .general{
        .name = "test",
        .version = "1.0",
        .symbol = "EURUSD",
        .initial_balance = 1000.0,
        .account_currency = "USD",
        .leverage = 100},
    .strategy{
        .timeframe = TimeFrame::M5,
        .maximum_risk = 0.05,
        .maximum_trades_per_day = 2,
        .maximum_spread = 8,
        // .minimum_trade_duration = 0,
        // .maximum_trade_duration = 10,
        .minimum_duration_before_next_trade = 10,
        .take_profit_stop_loss_config = tpsl_config,
        .trading_schedule = schedule},
    .training{
        .generations = 100,
        .bad_trader_threshold = 0.01,
        .inactive_trader_threshold = 500,
        .training_start_date = std::mktime(&start_training_date),
        .training_end_date = std::mktime(&end_training_date),
        .test_start_date = std::mktime(&start_test_date),
        .test_end_date = std::mktime(&end_test_date),
        .inputs = {
            .indicators = {
                {TimeFrame::M5, {
                                    new Hour(),
                                    new NFPWeek(),
                                    new MarketSession("london"),
                                    new MarketSession("new-york"),
                                    new MarketSession("tokyo"),
                                    new WeekDay("sunday"),
                                    new WeekDay("monday"),
                                    new WeekDay("tuesday"),
                                    new WeekDay("wednesday"),
                                    new WeekDay("thursday"),
                                    new WeekDay("friday"),
                                    new WeekDay("saturday"),
                                    new CandlePriceChange(),
                                    new CandleClose(),
                                    new CandleVolume(),
                                    new CandleBody(),
                                    new CandleShadowUpper(),
                                    new CandleShadowLower(),
                                    new PeakDistance(20, 0),
                                    new PeakCandleDistance(20, 0),
                                    new NewHighSignal(20, 0),
                                    new NewLowSignal(20, 0),
                                    new HighBreakSignal(20, 0),
                                    new LowBreakSignal(20, 0),
                                    new MFI(),
                                    new RSI(),
                                    new StochasticOscillator(),
                                    new InstitutionalBias(),
                                    new EMADifference(),
                                    new AroonTrend(),
                                    new MACD(),
                                    new IchimokuCloudTrend(),
                                    new IchimokuKijunTenkanTrend(),
                                    new EMASlope(21, "close"),
                                    new ATR(),
                                    new StandardDeviation(),
                                    new AveragePriceChange(),
                                    new CMF(),
                                    new OBV(),
                                    new VWAP(),
                                }},
                {TimeFrame::M30, {
                                     new CandlePriceChange(),
                                     new CandleClose(),
                                     new CandleVolume(),
                                     new CandleBody(),
                                     new CandleShadowUpper(),
                                     new CandleShadowLower(),
                                     new PeakDistance(20, 0),
                                     new PeakCandleDistance(20, 0),
                                     new NewHighSignal(20, 0),
                                     new NewLowSignal(20, 0),
                                     new HighBreakSignal(20, 0),
                                     new LowBreakSignal(20, 0),
                                     new MFI(),
                                     new RSI(),
                                     new StochasticOscillator(),
                                     new InstitutionalBias(),
                                     new EMADifference(),
                                     new AroonTrend(),
                                     new MACD(),
                                     new IchimokuCloudTrend(),
                                     new IchimokuKijunTenkanTrend(),
                                     new EMASlope(21, "close"),
                                     new ATR(),
                                     new StandardDeviation(),
                                     new AveragePriceChange(),
                                     new CMF(),
                                     new OBV(),
                                     new VWAP(),
                                 }}},
            .position = {
                PositionInfo::TYPE,
                PositionInfo::PNL,
                PositionInfo::DURATION,
            }}},
    .evaluation{
        .expected_return_per_day = 0.02,
        .expected_return_per_month = 0.5,
        .maximum_drawdown = 0.05,
        .minimum_winrate = 0.8,
        .minimum_profit_factor = 3,
    },
    .neat = load_config("src/configs/neat_config.txt")
    //.neat = load_config("C:\\Users\\Maxime\\AppData\\Roaming\\MetaQuotes\\Terminal\\D0E8209F77C8CF37AD8BF550E51FF075\\MQL5\\Libraries\\neat_config.txt")
};
