#include <iostream>
#include "neat/genome.hpp"
#include "utils/time_frame.hpp"
#include "utils/optional.cpp"
#include "trader.hpp"
#include "make_decision.hpp"
#include "types.hpp"

// Choose the configuration file to use
#include "configs/test.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Make a decision based on the genome
     *
     * @param candles_data The candles data
     * @param position_infos_data The position infos
     * @param base_currency_conversion_rate The base currency conversion rate
     * @param genome_save_file The genome save file
     *
     * @return The decision (0: wait, 1: long, 2: short, 3: close)
     */
    int DLL_EXPORT make_decision(MultiTimeFrameCandles *candles_data, PositionInfos &position_infos_data, double base_currency_conversion_rate, std::string &genome_save_file)
    {
        // Get the config
        Config config = test_config;

        // Load the genome from the file
        Genome *genome = Genome::load(genome_save_file);

        std::vector<TimeFrame> candles_timeframes = {};
        CandleSticksData candles = {};
        IndicatorsData indicators = {};

        if (candles_data->m1_size > 0)
        {
            candles_timeframes.push_back(TimeFrame::M1);
            candles[TimeFrame::M1] = std::vector<CandleStick>(candles_data->m1, candles_data->m1 + candles_data->m1_size);
        }
        if (candles_data->m5_size > 0)
        {
            candles_timeframes.push_back(TimeFrame::M5);
            candles[TimeFrame::M5] = std::vector<CandleStick>(candles_data->m5, candles_data->m5 + candles_data->m5_size);
        }
        if (candles_data->m15_size > 0)
        {
            candles_timeframes.push_back(TimeFrame::M15);
            candles[TimeFrame::M15] = std::vector<CandleStick>(candles_data->m15, candles_data->m15 + candles_data->m15_size);
        }
        if (candles_data->m30_size > 0)
        {
            candles_timeframes.push_back(TimeFrame::M30);
            candles[TimeFrame::M30] = std::vector<CandleStick>(candles_data->m30, candles_data->m30 + candles_data->m30_size);
        }
        if (candles_data->h1_size > 0)
        {
            candles_timeframes.push_back(TimeFrame::H1);
            candles[TimeFrame::H1] = std::vector<CandleStick>(candles_data->h1, candles_data->h1 + candles_data->h1_size);
        }
        if (candles_data->h4_size > 0)
        {
            candles_timeframes.push_back(TimeFrame::H4);
            candles[TimeFrame::H4] = std::vector<CandleStick>(candles_data->h4, candles_data->h4 + candles_data->h4_size);
        }
        if (candles_data->d1_size > 0)
        {
            candles_timeframes.push_back(TimeFrame::D1);
            candles[TimeFrame::D1] = std::vector<CandleStick>(candles_data->d1, candles_data->d1 + candles_data->d1_size);
        }

        // Prepare the inputs of the genome
        std::vector<double> inputs = {};

        // Calculate the indicators and add them to the inputs
        for (const auto &[timeframe, indicators] : config.training.inputs.indicators)
        {
            if (std::find(candles_timeframes.begin(), candles_timeframes.end(), timeframe) == candles_timeframes.end())
            {
                std::cerr << "One of the timeframe is not available in the input data" << std::endl;
                std::exit(1);
            }

            for (const auto &indicator : indicators)
            {
                inputs.push_back(indicator->calculate(candles[timeframe]).back());
            }
        }

        // Get the position infos from the config
        std::vector<PositionInfo> position_infos = {};
        for (const auto &position_info : config.training.inputs.position)
        {
            position_infos.push_back(position_info);
        }

        // Add the position info to the inputs
        for (const auto &position_info : position_infos)
        {
            if (position_info == PositionInfo::TYPE)
            {
                inputs.push_back(position_infos_data.type);
            }
            if (position_info == PositionInfo::PNL)
            {
                inputs.push_back(position_infos_data.pnl);
            }
            if (position_info == PositionInfo::DURATION)
            {
                inputs.push_back(position_infos_data.duration);
            }
        }

        // Create the trader and make the decision
        Trader *trader = new Trader(genome, config);
        trader->look(candles, indicators, base_currency_conversion_rate, position_infos);
        trader->think();

        int loop_interval_minutes = get_time_frame_value(config.strategy.timeframe);
        CandleStick last_candle = trader->candles[config.strategy.timeframe].back();

        // Position information
        bool has_position = position_infos_data.type != 0;
        bool has_long_position = has_position && position_infos_data.type == 1;
        bool has_short_position = has_position && position_infos_data.type == -1;

        // Decision taken
        double maximum = *std::max_element(trader->decisions.begin(), trader->decisions.end());
        bool want_long = maximum == trader->decisions[0];
        bool want_short = maximum == trader->decisions[1];
        bool wait = maximum == trader->decisions[2];

        if (!wait)
        {
            bool can_trade_now = trader->can_trade();

            // Check if the trader can close a trade
            bool can_close_position = true;
            if (!has_position)
            {
                can_close_position = false;
            }
            else if (config.strategy.minimum_trade_duration.has_value())
            {
                can_close_position = position_infos_data.duration >= config.strategy.minimum_trade_duration.value();
            }

            if (can_trade_now)
            {
                if (has_position)
                {
                    if (has_long_position && want_short && can_close_position)
                    {
                        return 3; // Close
                    }
                    else if (has_short_position && want_long && can_close_position)
                    {
                        return 3; // Close
                    }
                }
                else
                {
                    if (want_long)
                    {
                        return 1; // Long
                    }
                    else if (want_short)
                    {
                        return 2; // Short
                    }
                }
            }
            else if (has_position)
            {
                if (has_long_position && want_short && can_close_position)
                {
                    return 3; // Close
                }
                else if (has_short_position && want_long && can_close_position)
                {
                    return 3; // Close
                }
            }
        }

        // Check the duration of the current trade
        if (has_position && config.strategy.maximum_trade_duration.has_value())
        {
            // Check if the position has reached the maximum trade duration
            if (position_infos_data.duration >= config.strategy.maximum_trade_duration.value())
            {
                return 3; // Close
            }
        }

        return 0; // Wait
    }

#ifdef __cplusplus
}
#endif
