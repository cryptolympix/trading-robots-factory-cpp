#pragma once

#include <ctime>
#include "../types.hpp"
#include "../trading/schedule.hpp"
#include "../neat/config.hpp"

std::vector<bool> default_schedule_days = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
std::vector<bool> default_schedule_working_days = {false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false};
std::vector<bool> default_schedule_rest_days = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

TradingSchedule default_schedule = {
    .monday = default_schedule_days,
    .tuesday = default_schedule_days,
    .wednesday = default_schedule_days,
    .thursday = default_schedule_days,
    .friday = default_schedule_days,
    .saturday = default_schedule_days,
    .sunday = default_schedule_days,
};

TakeProfitStopLossConfig default_tpsl_config = {
    .type_stop_loss = TypeTakeProfitStopLoss::POINTS,
    .stop_loss_in_points = 20,
    .stop_loss_in_percent = 0.002,
    .type_take_profit = TypeTakeProfitStopLoss::POINTS,
    .take_profit_in_points = 20,
    .take_profit_in_percent = 0.002,
};

TrailingStopLossConfig default_tsl_config = {
    .type = TypeTrailingStopLoss::POINTS,
    .activation_level_in_points = 5,
    .trailing_stop_loss_in_points = 5,
};
