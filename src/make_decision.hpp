#ifndef MAKE_DECISION_HPP
#define MAKE_DECISION_HPP

#include "types.hpp"

struct MultiTimeFrameCandles
{
    CandleStick *m1;
    int m1_size;
    CandleStick *m5;
    int m5_size;
    CandleStick *m15;
    int m15_size;
    CandleStick *m30;
    int m30_size;
    CandleStick *h1;
    int h1_size;
    CandleStick *h4;
    int h4_size;
    CandleStick *d1;
    int d1_size;
};

struct PositionInfos
{
    double pnl;
    int duration;
    int type;
};

#ifdef _MSC_VER
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

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
    DLL_EXPORT int make_decision(MultiTimeFrameCandles *candles_data, PositionInfos &position_infos_data, double base_currency_conversion_rate, std::string &genome_save_file);

#ifdef __cplusplus
}
#endif

#endif
