#include <ctime>
#include "../types.hpp"
#include "neat/config.hpp"
#include "indicators/indicator.hpp"
#include "indicators/candle.hpp"
#include "indicators/candle_patterns.hpp"
#include "indicators/candle_signals.hpp"
#include "indicators/momentum.hpp"
#include "indicators/time.hpp"
#include "indicators/trend.hpp"
#include "indicators/volatility.hpp"
#include "indicators/volume.hpp"

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
    .take_profit_in_percent = 0.001};

// 2023-01-01 00:00:00
std::tm start_training_date = {
    .tm_year = 2022 - 1900,
    .tm_mon = 0,
    .tm_mday = 1,
    .tm_hour = 0,
    .tm_min = 0,
    .tm_sec = 0};

// 2023-01-31 00:00:00
std::tm end_training_date = {
    .tm_year = 2022 - 1900,
    .tm_mon = 4,
    .tm_mday = 1,
    .tm_hour = 0,
    .tm_min = 0,
    .tm_sec = 0};

// 2023-01-01 00:00:00
std::tm start_test_date = {
    .tm_year = 2022 - 1900,
    .tm_mon = 0,
    .tm_mday = 1,
    .tm_hour = 0,
    .tm_min = 0,
    .tm_sec = 0};

// 2023-01-31 00:00:00
std::tm end_test_date = {
    .tm_year = 2022 - 1900,
    .tm_mon = 1,
    .tm_mday = 1,
    .tm_hour = 0,
    .tm_min = 0,
    .tm_sec = 0};

Config test_config = {
    .general{
        .name = "test",
        .version = "1.0",
        .symbol = "EURUSD",
        .initial_balance = 1000.0,
        .account_currency = "USD",
        .leverage = 100},
    .strategy{
        .timeframe = TimeFrame::M15,
        .maximum_risk = 0.01,
        .maximum_spread = 8,
        // .minimum_trade_duration = 0,
        // .maximum_trade_duration = 10,
        .minimum_duration_before_next_trade = 4,
        .maximum_trades_per_day = 2,
        .trading_schedule = schedule,
        .take_profit_stop_loss_config = tpsl_config},
    .training{
        .generations = 500,
        .bad_trader_threshold = 0.01,
        .inactive_trader_threshold = 500,
        .training_start_date = std::mktime(&start_training_date),
        .training_end_date = std::mktime(&end_training_date),
        .test_start_date = std::mktime(&start_test_date),
        .test_end_date = std::mktime(&end_test_date),
        .inputs = {
            .indicators = {
                {TimeFrame::M15, {
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
                {TimeFrame::H1, {
                                    new CandlePriceChange(),
                                    new CandleClose(),
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
        .minimum_profit_factor = 3,
        .minimum_winrate = 0.8,
        .maximum_drawdown = 0.05,
        .expected_return_per_day = 0.01,
        .expected_return_per_month = 0.2,
    },
    .neat = load_config("src/configs/neat_config_test.ini")};
