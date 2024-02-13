#include "indicator.hpp"
#include "../types.hpp"

Indicator::Indicator(const std::string &label, const std::string &id, int offset)
    : label(label), id(id), offset(offset)
{
}

std::vector<double> Indicator::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return values;
}