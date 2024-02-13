#ifndef INDICATOR_H
#define INDICATOR_H

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "../types.hpp"

struct Candle; // Forward declaration

class Indicator
{
public:
    Indicator(const std::string &label, const std::string &id, int offset = 0);
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const;

private:
    std::string label;
    std::vector<double> values;
    std::string id;
    int offset;
};

#endif // INDICATOR_H