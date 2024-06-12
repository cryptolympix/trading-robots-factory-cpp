#pragma once

#include <ctime>
#include "../types.hpp"
#include "../neat/config.hpp"
#include "../indicators/indicator.hpp"
#include "../indicators/candle.hpp"
#include "../indicators/candle_patterns.hpp"
#include "../indicators/candle_signals.hpp"
#include "../indicators/momentum.hpp"
#include "../indicators/momentum_signals.hpp"
#include "../indicators/time.hpp"
#include "../indicators/trend.hpp"
#include "../indicators/trend_signals.hpp"
#include "../indicators/volatility.hpp"
#include "../indicators/volatility_signals.hpp"
#include "../indicators/volume.hpp"
#include "../indicators/volume_signals.hpp"

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
    .stop_loss_in_percent = 0.005,
    .type_take_profit = TypeTakeProfitStopLoss::POINTS,
    .take_profit_in_points = 30,
    .take_profit_in_percent = 0.005,
};

std::tm start_training_date = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 1,
    .tm_mon = 1,
    .tm_year = 2022 - 1900,
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
        .timeframe = TimeFrame::H1,
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
                {TimeFrame::H1, {
                                    // Candle signals
                                    new DojiCandlePattern(),
                                    new SingleCandlePattern(),
                                    new DoubleCandlePattern(),
                                    new TripleCandlePattern(),
                                    new NewHighSignal(),
                                    new NewLowSignal(),
                                    new HighBreakSignal(),
                                    new LowBreakSignal(),
                                    // Candles
                                    new CandleOpen(),
                                    new CandleHigh(),
                                    new CandleLow(),
                                    new CandleClose(),
                                    new CandleVolume(),
                                    new CandleTickVolume(),
                                    new WhiteCandle(),
                                    new BlackCandle(),
                                    new CandleBody(),
                                    new CandleShadowUpper(),
                                    new CandleShadowLower(),
                                    new CandlePriceChange(),
                                    new PivotLowValue("low", 10, 10),
                                    new PivotHighValue("high", 10, 10),
                                    new PivotHighValue("high", 10, 10),
                                    new PivotLowValue("low", 10, 10),
                                    new HighestHigh(10, 10),
                                    new LowestLow(10, 10),
                                    new PeakDistance(20, 0),
                                    new PeakCandleDistance(20, 0),
                                    // Momentum signals
                                    new AwesomeOscillatorSignal(),
                                    new KAMASignal(),
                                    new MFISignal(),
                                    new PPOSignal(),
                                    new ROCSignal(),
                                    new RSISignal(),
                                    new StochasticRSISignal(),
                                    new StochasticOscillatorSignal(),
                                    new TSISignal(),
                                    new UOSignal(),
                                    new WilliamsRSignal(),
                                    // Momentum
                                    new AwesomeOscillator(),
                                    new KAMA(),
                                    new MFI(),
                                    new PPO(),
                                    new PVO(),
                                    new ROC(),
                                    new RSI(),
                                    new StochasticRSI(),
                                    new StochasticOscillator(),
                                    new TSI(),
                                    new UO(),
                                    new WilliamsPercentR(),
                                    // Time
                                    new Hour(),
                                    new Minute(),
                                    new NFPWeek(),
                                    new MarketSession("new-york"),
                                    new MarketSession("london"),
                                    new MarketSession("tokyo"),
                                    new WeekDay("monday"),
                                    new WeekDay("tuesday"),
                                    new WeekDay("wednesday"),
                                    new WeekDay("thursday"),
                                    new WeekDay("friday"),
                                    // Trend signals
                                    new ADXSignal(),
                                    new AroonSignal(),
                                    new CCISignal(),
                                    new DPOSignal(),
                                    new EMASignal(),
                                    new KSTSignal(),
                                    new MACDSignal(),
                                    new ParabolicSARSignal(),
                                    new SMASignal(),
                                    new TRIXSignal(),
                                    new VortexSignal(),
                                    new InstitutionalBiasSignal(),
                                    new IchimokuCloudSignal(),
                                    new IchimokuKijunTenkanSignal(),
                                    // Trend
                                    new ADX(),
                                    new AroonUp(),
                                    new AroonDown(),
                                    new CCI(),
                                    new DPO(),
                                    new EMA("close", 21),
                                    new KST(),
                                    new MACD(),
                                    new MI(),
                                    new ParabolicSAR(),
                                    new SMA("close", 21),
                                    new STC(),
                                    new TRIX(),
                                    new Vortex(),
                                    new InstitutionalBias(),
                                    new EMADifference(),
                                    new AroonTrend(),
                                    new IchimokuCloudTrend(),
                                    new IchimokuKijunTenkanTrend(),
                                    new SMASlope(21, "close"),
                                    new EMASlope(21, "close"),
                                    new Zigzag(),
                                    // Volatility signals
                                    new ATRSignal(),
                                    new BollingerChannelSignal(),
                                    new DonchianChannelSignal(),
                                    new KeltnerChannelSignal(),
                                    // Volatility
                                    new ATR(),
                                    new BollingerChannelHighBand(),
                                    new BollingerChannelLowBand(),
                                    new BollingerChannelMiddleBand(),
                                    new BollingerChannelPercentageBand(),
                                    new BollingerChannelWidthBand(),
                                    new DonchianChannelHighBand(),
                                    new DonchianChannelLowBand(),
                                    new DonchianChannelMiddleBand(),
                                    new KeltnerChannelHighBand(),
                                    new KeltnerChannelLowBand(),
                                    new KeltnerChannelMiddleBand(),
                                    new UI(),
                                    new StandardDeviation(),
                                    new AveragePriceChange(),
                                    // Volume signal
                                    new CMFSignal(),
                                    // Volume
                                    new ADL(),
                                    new CMF(),
                                    new FI(),
                                    new NVI(),
                                    new OBV(),
                                    new POC(),
                                    new PVI(),
                                    new VWAP(),
                                }}},
        }},
    .evaluation{
        .nb_trades_per_day = 1,
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
