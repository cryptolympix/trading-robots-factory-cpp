#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>
#include "../libs/json.hpp"
#include "../utils/time_frame.hpp"
#include "../types.hpp"
#include "../indicators/indicator.hpp"
#include "../indicators/builder.hpp"
#include "./serialization.hpp"

template <typename T>
void add_optional(nlohmann::json &json, const std::string &key, const std::optional<T> &opt)
{
    if (opt.has_value())
    {
        json[key] = opt.value();
    }
}

void check_key(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key))
    {
        throw std::runtime_error("Missing '" + key + "' key in the JSON object");
    }
}

void check_keys(const nlohmann::json &json, const std::vector<std::string> &keys)
{
    for (const auto &key : keys)
    {
        check_key(json, key);
    }
}

/**
 * @brief Create a JSON object from a Config object
 * @return JSON object
 */
nlohmann::json config_to_json(const Config &config)
{
    try
    {
        // General config
        nlohmann::json general_json = {
            {"name", config.general.name},
            {"version", config.general.version},
            {"symbol", config.general.symbol},
            {"initial_balance", config.general.initial_balance},
            {"leverage", config.general.leverage},
            {"account_currency", config.general.account_currency},
        };

        // Strategy config
        nlohmann::json strategy_json = {
            {"timeframe", time_frame_to_string(config.strategy.timeframe)},
            {"risk_per_trade", config.strategy.risk_per_trade},
        };
        add_optional(strategy_json, "maximum_trades_per_day", config.strategy.maximum_trades_per_day);
        add_optional(strategy_json, "maximum_spread", config.strategy.maximum_spread);
        add_optional(strategy_json, "minimum_trade_duration", config.strategy.minimum_trade_duration);
        add_optional(strategy_json, "maximum_trade_duration", config.strategy.maximum_trade_duration);
        add_optional(strategy_json, "minimum_duration_before_next_trade", config.strategy.minimum_duration_before_next_trade);
        add_optional(strategy_json, "can_close_trade", config.strategy.can_close_trade);
        add_optional(strategy_json, "can_open_long_trade", config.strategy.can_open_long_trade);
        add_optional(strategy_json, "can_open_short_trade", config.strategy.can_open_short_trade);

        // Take profit and stop loss config
        nlohmann::json tp_sl_json = {
            {"type_stop_loss", config.strategy.take_profit_stop_loss_config.type_stop_loss},
            {"type_take_profit", config.strategy.take_profit_stop_loss_config.type_take_profit},
        };
        add_optional(tp_sl_json, "stop_loss_in_points", config.strategy.take_profit_stop_loss_config.stop_loss_in_points);
        add_optional(tp_sl_json, "stop_loss_in_percent", config.strategy.take_profit_stop_loss_config.stop_loss_in_percent);
        add_optional(tp_sl_json, "stop_loss_extremum_period", config.strategy.take_profit_stop_loss_config.stop_loss_extremum_period);
        add_optional(tp_sl_json, "stop_loss_atr_period", config.strategy.take_profit_stop_loss_config.stop_loss_atr_period);
        add_optional(tp_sl_json, "stop_loss_atr_multiplier", config.strategy.take_profit_stop_loss_config.stop_loss_atr_multiplier);
        add_optional(tp_sl_json, "take_profit_in_points", config.strategy.take_profit_stop_loss_config.take_profit_in_points);
        add_optional(tp_sl_json, "take_profit_in_percent", config.strategy.take_profit_stop_loss_config.take_profit_in_percent);
        add_optional(tp_sl_json, "take_profit_extremum_period", config.strategy.take_profit_stop_loss_config.take_profit_extremum_period);
        add_optional(tp_sl_json, "take_profit_atr_period", config.strategy.take_profit_stop_loss_config.take_profit_atr_period);
        add_optional(tp_sl_json, "take_profit_atr_multiplier", config.strategy.take_profit_stop_loss_config.take_profit_atr_multiplier);

        strategy_json["take_profit_stop_loss_config"] = tp_sl_json;

        // Trading schedule
        if (config.strategy.trading_schedule.has_value())
        {
            const auto &schedule = config.strategy.trading_schedule.value();
            nlohmann::json schedule_json = {
                {"monday", schedule.monday},
                {"tuesday", schedule.tuesday},
                {"wednesday", schedule.wednesday},
                {"thursday", schedule.thursday},
                {"friday", schedule.friday},
                {"saturday", schedule.saturday},
                {"sunday", schedule.sunday},
            };
            strategy_json["trading_schedule"] = schedule_json;
        }

        // Trailing stop loss config
        if (config.strategy.trailing_stop_loss_config.has_value())
        {
            const auto &tsl_config = config.strategy.trailing_stop_loss_config.value();
            nlohmann::json tsl_json = {
                {"type", tsl_config.type},
            };
            add_optional(tsl_json, "activation_level_in_points", tsl_config.activation_level_in_points);
            add_optional(tsl_json, "activation_level_in_percent", tsl_config.activation_level_in_percent);
            add_optional(tsl_json, "trailing_stop_loss_in_points", tsl_config.trailing_stop_loss_in_points);
            add_optional(tsl_json, "trailing_stop_loss_in_percent", tsl_config.trailing_stop_loss_in_percent);

            strategy_json["trailing_stop_loss_config"] = tsl_json;
        }

        // Training config
        nlohmann::json training_json = {
            {"generations", config.training.generations},
            {"training_start_date", config.training.training_start_date},
            {"training_end_date", config.training.training_end_date},
            {"test_start_date", config.training.test_start_date},
            {"test_end_date", config.training.test_end_date},
        };
        add_optional(training_json, "bad_trader_threshold", config.training.bad_trader_threshold);
        add_optional(training_json, "inactive_trader_threshold", config.training.inactive_trader_threshold);
        add_optional(training_json, "decision_threshold", config.training.decision_threshold);

        // Training indicator inputs
        nlohmann::json indicators_json = nlohmann::json::object();
        for (const auto &[timeframe, indicators] : config.training.inputs.indicators)
        {
            nlohmann::json timeframe_json = nlohmann::json::array();
            for (const auto &indicator : indicators)
            {
                timeframe_json.push_back({
                    {"id_params", indicator->id_params},
                    {"id_params_pattern", indicator->id_params_pattern},
                });
            }
            indicators_json[time_frame_to_string(timeframe)] = timeframe_json;
        }

        // Training position inputs
        nlohmann::json position_json = nlohmann::json::array();
        for (const auto &pos : config.training.inputs.position)
        {
            if (pos == PositionInfo::TYPE)
            {
                position_json.push_back("TYPE");
            }
            else if (pos == PositionInfo::PNL)
            {
                position_json.push_back("PNL");
            }
            else if (pos == PositionInfo::DURATION)
            {
                position_json.push_back("DURATION");
            }
            else
            {
                throw std::runtime_error("Invalid position info");
            }
        }

        training_json["inputs"] = {
            {"indicators", indicators_json},
            {"position", position_json},
        };

        // Evaluation config
        nlohmann::json evaluation_json = nlohmann::json::object();
        add_optional(evaluation_json, "maximize_nb_trades", config.evaluation.maximize_nb_trades);
        add_optional(evaluation_json, "minimum_nb_trades", config.evaluation.minimum_nb_trades);
        add_optional(evaluation_json, "maximum_trade_duration", config.evaluation.maximum_trade_duration);
        add_optional(evaluation_json, "expected_return_per_day", config.evaluation.expected_return_per_day);
        add_optional(evaluation_json, "expected_return_per_month", config.evaluation.expected_return_per_month);
        add_optional(evaluation_json, "expected_return", config.evaluation.expected_return);
        add_optional(evaluation_json, "maximum_drawdown", config.evaluation.maximum_drawdown);
        add_optional(evaluation_json, "minimum_winrate", config.evaluation.minimum_winrate);
        add_optional(evaluation_json, "minimum_profit_factor", config.evaluation.minimum_profit_factor);

        // NEAT config
        nlohmann::json neat_json = {
            {"population_size", config.neat.population_size},
            {"fitness_threshold", config.neat.fitness_threshold},
            {"no_fitness_termination", config.neat.no_fitness_termination},
            {"reset_on_extinction", config.neat.reset_on_extinction},
            {"activation_default", config.neat.activation_default},
            {"activation_mutate_rate", config.neat.activation_mutate_rate},
            {"num_inputs", config.neat.num_inputs},
            {"num_outputs", config.neat.num_outputs},
            {"num_hidden_layers", config.neat.num_hidden_layers},
            {"compatibility_disjoint_coefficient", config.neat.compatibility_disjoint_coefficient},
            {"compatibility_weight_coefficient", config.neat.compatibility_weight_coefficient},
            {"conn_add_prob", config.neat.conn_add_prob},
            {"conn_delete_prob", config.neat.conn_delete_prob},
            {"enabled_default", config.neat.enabled_default},
            {"enabled_mutate_rate", config.neat.enabled_mutate_rate},
            {"initial_connections", config.neat.initial_connections},
            {"node_add_prob", config.neat.node_add_prob},
            {"node_delete_prob", config.neat.node_delete_prob},
            {"weight_init_mean", config.neat.weight_init_mean},
            {"weight_init_stdev", config.neat.weight_init_stdev},
            {"weight_init_type", config.neat.weight_init_type},
            {"weight_max_value", config.neat.weight_max_value},
            {"weight_min_value", config.neat.weight_min_value},
            {"weight_mutate_rate", config.neat.weight_mutate_rate},
            {"weight_replace_rate", config.neat.weight_replace_rate},
            {"max_stagnation", config.neat.max_stagnation},
            {"species_elitism", config.neat.species_elitism},
            {"elitism", config.neat.elitism},
            {"survival_threshold", config.neat.survival_threshold},
            {"min_species_size", config.neat.min_species_size},
            {"compatibility_threshold", config.neat.compatibility_threshold},
            {"bad_species_threshold", config.neat.bad_species_threshold},
        };

        nlohmann::json config_json = {
            {"general", general_json},
            {"strategy", strategy_json},
            {"training", training_json},
            {"evaluation", evaluation_json},
            {"neat", neat_json},
        };

        return config_json;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Error creating JSON: ") + e.what());
    }
}

/**
 * @brief Create a Config object from a JSON object
 * @return Config object
 */
Config config_from_json(const nlohmann::json &json)
{
    check_keys(json, {
                         "general",
                         "strategy",
                         "training",
                         "evaluation",
                         "neat",
                     });
    check_keys(json["general"], {
                                    "name",
                                    "version",
                                    "symbol",
                                    "initial_balance",
                                    "leverage",
                                    "account_currency",
                                });
    check_keys(json["strategy"], {
                                     "timeframe",
                                     "risk_per_trade",
                                     "take_profit_stop_loss_config",
                                 });
    check_keys(json["training"], {
                                     "generations",
                                     "training_start_date",
                                     "training_end_date",
                                     "test_start_date",
                                     "test_end_date",
                                     "inputs",
                                 });
    check_keys(json["neat"], {
                                 "population_size",
                                 "fitness_threshold",
                                 "no_fitness_termination",
                                 "reset_on_extinction",
                                 "activation_default",
                                 "activation_mutate_rate",
                                 "num_inputs",
                                 "num_outputs",
                                 "num_hidden_layers",
                                 "compatibility_disjoint_coefficient",
                                 "compatibility_weight_coefficient",
                                 "conn_add_prob",
                                 "conn_delete_prob",
                                 "enabled_default",
                                 "enabled_mutate_rate",
                                 "initial_connections",
                                 "node_add_prob",
                                 "node_delete_prob",
                                 "weight_init_mean",
                                 "weight_init_stdev",
                                 "weight_init_type",
                                 "weight_max_value",
                                 "weight_min_value",
                                 "weight_mutate_rate",
                                 "weight_replace_rate",
                                 "max_stagnation",
                                 "species_elitism",
                                 "elitism",
                                 "survival_threshold",
                                 "min_species_size",
                                 "compatibility_threshold",
                                 "bad_species_threshold",
                             });

    Config config;

    // Parse general config data
    config.general.name = json["general"]["name"];
    config.general.version = json["general"]["version"];
    config.general.symbol = json["general"]["symbol"];
    config.general.initial_balance = json["general"]["initial_balance"];
    config.general.leverage = json["general"]["leverage"];
    config.general.account_currency = json["general"]["account_currency"];

    // Parse strategy config data
    config.strategy.timeframe = time_frame_from_string(json["strategy"]["timeframe"]);
    config.strategy.risk_per_trade = json["strategy"]["risk_per_trade"];
    if (json["strategy"].contains("maximum_trades_per_day"))
    {
        config.strategy.maximum_trades_per_day = json["strategy"]["maximum_trades_per_day"];
    }
    if (json["strategy"].contains("maximum_spread"))
    {
        config.strategy.maximum_spread = json["strategy"]["maximum_spread"];
    }
    if (json["strategy"].contains("minimum_trade_duration"))
    {
        config.strategy.minimum_trade_duration = json["strategy"]["minimum_trade_duration"];
    }
    if (json["strategy"].contains("maximum_trade_duration"))
    {
        config.strategy.maximum_trade_duration = json["strategy"]["maximum_trade_duration"];
    }
    if (json["strategy"].contains("minimum_duration_before_next_trade"))
    {
        config.strategy.minimum_duration_before_next_trade = json["strategy"]["minimum_duration_before_next_trade"];
    }
    if (json["strategy"].contains("can_close_trade"))
    {
        config.strategy.can_close_trade = json["strategy"]["can_close_trade"];
    }
    if (json["strategy"].contains("can_open_long_trade"))
    {
        config.strategy.can_open_long_trade = json["strategy"]["can_open_long_trade"];
    }
    if (json["strategy"].contains("can_open_short_trade"))
    {
        config.strategy.can_open_short_trade = json["strategy"]["can_open_short_trade"];
    }

    // Parse take profit and stop loss config data
    config.strategy.take_profit_stop_loss_config.type_stop_loss = json["strategy"]["take_profit_stop_loss_config"]["type_stop_loss"];
    config.strategy.take_profit_stop_loss_config.type_take_profit = json["strategy"]["take_profit_stop_loss_config"]["type_take_profit"];
    if (json["strategy"]["take_profit_stop_loss_config"].contains("stop_loss_in_points"))
    {
        config.strategy.take_profit_stop_loss_config.stop_loss_in_points = json["strategy"]["take_profit_stop_loss_config"]["stop_loss_in_points"];
    }
    if (json["strategy"]["take_profit_stop_loss_config"].contains("stop_loss_in_percent"))
    {
        config.strategy.take_profit_stop_loss_config.stop_loss_in_percent = json["strategy"]["take_profit_stop_loss_config"]["stop_loss_in_percent"];
    }
    if (json["strategy"]["take_profit_stop_loss_config"].contains("stop_loss_extremum_period"))
    {
        config.strategy.take_profit_stop_loss_config.stop_loss_extremum_period = json["strategy"]["take_profit_stop_loss_config"]["stop_loss_extremum_period"];
    }
    if (json["strategy"]["take_profit_stop_loss_config"].contains("stop_loss_atr_period"))
    {
        config.strategy.take_profit_stop_loss_config.stop_loss_atr_period = json["strategy"]["take_profit_stop_loss_config"]["stop_loss_atr_period"];
    }
    if (json["strategy"]["take_profit_stop_loss_config"].contains("stop_loss_atr_multiplier"))
    {
        config.strategy.take_profit_stop_loss_config.stop_loss_atr_multiplier = json["strategy"]["take_profit_stop_loss_config"]["stop_loss_atr_multiplier"];
    }
    if (json["strategy"]["take_profit_stop_loss_config"].contains("take_profit_in_points"))
    {
        config.strategy.take_profit_stop_loss_config.take_profit_in_points = json["strategy"]["take_profit_stop_loss_config"]["take_profit_in_points"];
    }
    if (json["strategy"]["take_profit_stop_loss_config"].contains("take_profit_in_percent"))
    {
        config.strategy.take_profit_stop_loss_config.take_profit_in_percent = json["strategy"]["take_profit_stop_loss_config"]["take_profit_in_percent"];
    }
    if (json["strategy"]["take_profit_stop_loss_config"].contains("take_profit_extremum_period"))
    {
        config.strategy.take_profit_stop_loss_config.take_profit_extremum_period = json["strategy"]["take_profit_stop_loss_config"]["take_profit_extremum_period"];
    }
    if (json["strategy"]["take_profit_stop_loss_config"].contains("take_profit_atr_period"))
    {
        config.strategy.take_profit_stop_loss_config.take_profit_atr_period = json["strategy"]["take_profit_stop_loss_config"]["take_profit_atr_period"];
    }
    if (json["strategy"]["take_profit_stop_loss_config"].contains("take_profit_atr_multiplier"))
    {
        config.strategy.take_profit_stop_loss_config.take_profit_atr_multiplier = json["strategy"]["take_profit_stop_loss_config"]["take_profit_atr_multiplier"];
    }

    // Parse trading schedule data
    if (json["strategy"].contains("trading_schedule"))
    {
        const auto &schedule_json = json["strategy"]["trading_schedule"];
        config.strategy.trading_schedule = TradingSchedule{
            .monday = schedule_json["monday"],
            .tuesday = schedule_json["tuesday"],
            .wednesday = schedule_json["wednesday"],
            .thursday = schedule_json["thursday"],
            .friday = schedule_json["friday"],
            .saturday = schedule_json["saturday"],
            .sunday = schedule_json["sunday"]};
    }

    // Parse trailing stop loss config data
    if (json["strategy"].contains("trailing_stop_loss_config"))
    {
        const auto &tsl_json = json["strategy"]["trailing_stop_loss_config"];
        config.strategy.trailing_stop_loss_config = TrailingStopLossConfig{
            .type = tsl_json["type"]};
        if (tsl_json.contains("activation_level_in_points"))
        {
            config.strategy.trailing_stop_loss_config->activation_level_in_points = tsl_json["activation_level_in_points"];
        }
        if (tsl_json.contains("activation_level_in_percent"))
        {
            config.strategy.trailing_stop_loss_config->activation_level_in_percent = tsl_json["activation_level_in_percent"];
        }
        if (tsl_json.contains("trailing_stop_loss_in_points"))
        {
            config.strategy.trailing_stop_loss_config->trailing_stop_loss_in_points = tsl_json["trailing_stop_loss_in_points"];
        }
        if (tsl_json.contains("trailing_stop_loss_in_percent"))
        {
            config.strategy.trailing_stop_loss_config->trailing_stop_loss_in_percent = tsl_json["trailing_stop_loss_in_percent"];
        }
    }

    // Parse training config data
    config.training.generations = json["training"]["generations"];
    config.training.training_start_date = json["training"]["training_start_date"];
    config.training.training_end_date = json["training"]["training_end_date"];
    config.training.test_start_date = json["training"]["test_start_date"];
    config.training.test_end_date = json["training"]["test_end_date"];
    if (json["training"].contains("bad_trader_threshold"))
    {
        config.training.bad_trader_threshold = json["training"]["bad_trader_threshold"];
    }
    if (json["training"].contains("inactive_trader_threshold"))
    {
        config.training.inactive_trader_threshold = json["training"]["inactive_trader_threshold"];
    }
    if (json["training"].contains("decision_threshold"))
    {
        config.training.decision_threshold = json["training"]["decision_threshold"];
    }

    // Parse training inputs data
    const auto &inputs_json = json["training"]["inputs"];
    const auto &indicators_json = inputs_json["indicators"];
    const auto &position_json = inputs_json["position"];

    std::map<TimeFrame, std::vector<Indicator *>> indicators;
    for (const auto &[timeframe, indicators_id] : indicators_json.items())
    {
        std::vector<Indicator *> indicators_list;
        for (const auto &indicator : indicators_id)
        {
            if (!indicator.contains("id_params"))
            {
                throw std::runtime_error("Missing 'id_params' key in the JSON object");
            }

            if (!indicator.contains("id_params_pattern"))
            {
                throw std::runtime_error("Missing 'id_params_pattern' key in the JSON object");
            }

            const std::string id_params = indicator["id_params"];
            const std::string id_params_pattern = indicator["id_params_pattern"];
            const std::vector<IndicatorParam> params = extract_parameters(id_params, id_params_pattern);
            indicators_list.push_back(create_indicator_from_id(id_params, params));
        }
        indicators[time_frame_from_string(timeframe)] = indicators_list;
    }

    std::vector<PositionInfo> position_info;
    for (const auto &pos : position_json)
    {
        if (pos == "TYPE")
        {
            position_info.push_back(PositionInfo::TYPE);
        }
        else if (pos == "PNL")
        {
            position_info.push_back(PositionInfo::PNL);
        }
        else if (pos == "DURATION")
        {
            position_info.push_back(PositionInfo::DURATION);
        }
        else
        {
            throw std::runtime_error("Invalid position info");
        }
    }

    config.training.inputs = NeuralNetworkInputs{
        .indicators = indicators,
        .position = position_info,
    };

    // Parse evaluation config data
    if (json["evaluation"].contains("maximize_nb_trades"))
    {
        config.evaluation.maximize_nb_trades = json["evaluation"]["maximize_nb_trades"];
    }
    if (json["evaluation"].contains("minimum_nb_trades"))
    {
        config.evaluation.minimum_nb_trades = json["evaluation"]["minimum_nb_trades"];
    }
    if (json["evaluation"].contains("maximum_trade_duration"))
    {
        config.evaluation.maximum_trade_duration = json["evaluation"]["maximum_trade_duration"];
    }
    if (json["evaluation"].contains("expected_return_per_day"))
    {
        config.evaluation.expected_return_per_day = json["evaluation"]["expected_return_per_day"];
    }
    if (json["evaluation"].contains("expected_return_per_month"))
    {
        config.evaluation.expected_return_per_month = json["evaluation"]["expected_return_per_month"];
    }
    if (json["evaluation"].contains("expected_return"))
    {
        config.evaluation.expected_return = json["evaluation"]["expected_return"];
    }
    if (json["evaluation"].contains("maximum_drawdown"))
    {
        config.evaluation.maximum_drawdown = json["evaluation"]["maximum_drawdown"];
    }
    if (json["evaluation"].contains("minimum_winrate"))
    {
        config.evaluation.minimum_winrate = json["evaluation"]["minimum_winrate"];
    }
    if (json["evaluation"].contains("minimum_profit_factor"))
    {
        config.evaluation.minimum_profit_factor = json["evaluation"]["minimum_profit_factor"];
    }

    // Parse NEAT config data
    config.neat.population_size = json["neat"]["population_size"];
    config.neat.fitness_threshold = json["neat"]["fitness_threshold"];
    config.neat.no_fitness_termination = json["neat"]["no_fitness_termination"];
    config.neat.reset_on_extinction = json["neat"]["reset_on_extinction"];
    config.neat.activation_default = json["neat"]["activation_default"];
    config.neat.activation_mutate_rate = json["neat"]["activation_mutate_rate"];
    config.neat.num_inputs = json["neat"]["num_inputs"];
    config.neat.num_outputs = json["neat"]["num_outputs"];
    config.neat.num_hidden_layers = json["neat"]["num_hidden_layers"];
    config.neat.compatibility_disjoint_coefficient = json["neat"]["compatibility_disjoint_coefficient"];
    config.neat.compatibility_weight_coefficient = json["neat"]["compatibility_weight_coefficient"];
    config.neat.conn_add_prob = json["neat"]["conn_add_prob"];
    config.neat.conn_delete_prob = json["neat"]["conn_delete_prob"];
    config.neat.enabled_default = json["neat"]["enabled_default"];
    config.neat.enabled_mutate_rate = json["neat"]["enabled_mutate_rate"];
    config.neat.initial_connections = json["neat"]["initial_connections"];
    config.neat.node_add_prob = json["neat"]["node_add_prob"];
    config.neat.node_delete_prob = json["neat"]["node_delete_prob"];
    config.neat.weight_init_mean = json["neat"]["weight_init_mean"];
    config.neat.weight_init_stdev = json["neat"]["weight_init_stdev"];
    config.neat.weight_init_type = json["neat"]["weight_init_type"];
    config.neat.weight_max_value = json["neat"]["weight_max_value"];
    config.neat.weight_min_value = json["neat"]["weight_min_value"];
    config.neat.weight_mutate_rate = json["neat"]["weight_mutate_rate"];
    config.neat.weight_replace_rate = json["neat"]["weight_replace_rate"];
    config.neat.max_stagnation = json["neat"]["max_stagnation"];
    config.neat.species_elitism = json["neat"]["species_elitism"];
    config.neat.elitism = json["neat"]["elitism"];
    config.neat.survival_threshold = json["neat"]["survival_threshold"];
    config.neat.min_species_size = json["neat"]["min_species_size"];

    return config;
}

/**
 * @brief Check if two Config objects are the same
 * @return True if the Config objects are the same, false otherwise
 */
bool is_same_config(const Config &config1, const Config &config2)
{
    bool is_same_general_config = config1.general.account_currency == config2.general.account_currency &&
                                  config1.general.initial_balance == config2.general.initial_balance &&
                                  config1.general.leverage == config2.general.leverage &&
                                  config1.general.name == config2.general.name &&
                                  config1.general.symbol == config2.general.symbol &&
                                  config1.general.version == config2.general.version;

    if (!is_same_general_config)
    {
        std::cout << "General config is different" << std::endl;
        return false;
    }

    bool is_same_strategy_config = config1.strategy.timeframe == config2.strategy.timeframe &&
                                   config1.strategy.risk_per_trade == config2.strategy.risk_per_trade &&
                                   config1.strategy.maximum_trades_per_day.value_or(0) == config2.strategy.maximum_trades_per_day.value_or(0) &&
                                   config1.strategy.maximum_spread.value_or(0) == config2.strategy.maximum_spread.value_or(0) &&
                                   config1.strategy.minimum_trade_duration.value_or(0) == config2.strategy.minimum_trade_duration.value_or(0) &&
                                   config1.strategy.maximum_trade_duration.value_or(0) == config2.strategy.maximum_trade_duration.value_or(0) &&
                                   config1.strategy.minimum_duration_before_next_trade.value_or(0) == config2.strategy.minimum_duration_before_next_trade.value_or(0) &&
                                   config1.strategy.can_close_trade.value_or(true) == config2.strategy.can_close_trade.value_or(true) &&
                                   config1.strategy.can_open_long_trade.value_or(true) == config2.strategy.can_open_long_trade.value_or(true) &&
                                   config1.strategy.can_open_short_trade.value_or(true) == config2.strategy.can_open_short_trade.value_or(true) &&
                                   config1.strategy.take_profit_stop_loss_config.type_stop_loss == config2.strategy.take_profit_stop_loss_config.type_stop_loss &&
                                   config1.strategy.take_profit_stop_loss_config.type_take_profit == config2.strategy.take_profit_stop_loss_config.type_take_profit &&
                                   config1.strategy.take_profit_stop_loss_config.stop_loss_in_points.value_or(0) == config2.strategy.take_profit_stop_loss_config.stop_loss_in_points.value_or(0) &&
                                   config1.strategy.take_profit_stop_loss_config.stop_loss_in_percent.value_or(0) == config2.strategy.take_profit_stop_loss_config.stop_loss_in_percent.value_or(0) &&
                                   config1.strategy.take_profit_stop_loss_config.stop_loss_extremum_period.value_or(0) == config2.strategy.take_profit_stop_loss_config.stop_loss_extremum_period.value_or(0) &&
                                   config1.strategy.take_profit_stop_loss_config.stop_loss_atr_period.value_or(0) == config2.strategy.take_profit_stop_loss_config.stop_loss_atr_period.value_or(0) &&
                                   config1.strategy.take_profit_stop_loss_config.stop_loss_atr_multiplier.value_or(0) == config2.strategy.take_profit_stop_loss_config.stop_loss_atr_multiplier.value_or(0) &&
                                   config1.strategy.take_profit_stop_loss_config.take_profit_in_points.value_or(0) == config2.strategy.take_profit_stop_loss_config.take_profit_in_points.value_or(0) &&
                                   config1.strategy.take_profit_stop_loss_config.take_profit_in_percent.value_or(0) == config2.strategy.take_profit_stop_loss_config.take_profit_in_percent.value_or(0) &&
                                   config1.strategy.take_profit_stop_loss_config.take_profit_extremum_period.value_or(0) == config2.strategy.take_profit_stop_loss_config.take_profit_extremum_period.value_or(0) &&
                                   config1.strategy.take_profit_stop_loss_config.take_profit_atr_period.value_or(0) == config2.strategy.take_profit_stop_loss_config.take_profit_atr_period.value_or(0) &&
                                   config1.strategy.take_profit_stop_loss_config.take_profit_atr_multiplier.value_or(0) == config2.strategy.take_profit_stop_loss_config.take_profit_atr_multiplier.value_or(0) &&
                                   config1.strategy.trading_schedule.has_value() == config2.strategy.trading_schedule.has_value() &&
                                   config1.strategy.trading_schedule.value().monday == config2.strategy.trading_schedule.value().monday &&
                                   config1.strategy.trading_schedule.value().tuesday == config2.strategy.trading_schedule.value().tuesday &&
                                   config1.strategy.trading_schedule.value().wednesday == config2.strategy.trading_schedule.value().wednesday &&
                                   config1.strategy.trading_schedule.value().thursday == config2.strategy.trading_schedule.value().thursday &&
                                   config1.strategy.trading_schedule.value().friday == config2.strategy.trading_schedule.value().friday &&
                                   config1.strategy.trading_schedule.value().saturday == config2.strategy.trading_schedule.value().saturday &&
                                   config1.strategy.trading_schedule.value().sunday == config2.strategy.trading_schedule.value().sunday &&
                                   config1.strategy.trailing_stop_loss_config.has_value() == config2.strategy.trailing_stop_loss_config.has_value() &&
                                   config1.strategy.trailing_stop_loss_config.value().type == config2.strategy.trailing_stop_loss_config.value().type &&
                                   config1.strategy.trailing_stop_loss_config.value().activation_level_in_points.value_or(0) == config2.strategy.trailing_stop_loss_config.value().activation_level_in_points.value_or(0) &&
                                   config1.strategy.trailing_stop_loss_config.value().activation_level_in_percent.value_or(0) == config2.strategy.trailing_stop_loss_config.value().activation_level_in_percent.value_or(0) &&
                                   config1.strategy.trailing_stop_loss_config.value().trailing_stop_loss_in_points.value_or(0) == config2.strategy.trailing_stop_loss_config.value().trailing_stop_loss_in_points.value_or(0) &&
                                   config1.strategy.trailing_stop_loss_config.value().trailing_stop_loss_in_percent.value_or(0) == config2.strategy.trailing_stop_loss_config.value().trailing_stop_loss_in_percent.value_or(0);

    return true;

    if (!is_same_strategy_config)
    {
        std::cout << "Strategy config is different" << std::endl;
        return false;
    }

    bool is_same_training_config = config1.training.generations == config2.training.generations &&
                                   config1.training.training_start_date == config2.training.training_start_date &&
                                   config1.training.training_end_date == config2.training.training_end_date &&
                                   config1.training.test_start_date == config2.training.test_start_date &&
                                   config1.training.test_end_date == config2.training.test_end_date &&
                                   config1.training.bad_trader_threshold.value_or(0) == config2.training.bad_trader_threshold.value_or(0) &&
                                   config1.training.inactive_trader_threshold.value_or(0) == config2.training.inactive_trader_threshold.value_or(0) &&
                                   config1.training.decision_threshold.value_or(0) == config2.training.decision_threshold.value_or(0) &&
                                   config1.training.inputs.indicators.size() == config2.training.inputs.indicators.size() &&
                                   config1.training.inputs.position.size() == config2.training.inputs.position.size() &&
                                   std::equal(config1.training.inputs.position.begin(), config1.training.inputs.position.end(), config2.training.inputs.position.begin()) &&
                                   std::equal(config1.training.inputs.indicators.begin(), config1.training.inputs.indicators.end(), config2.training.inputs.indicators.begin());

    if (!is_same_training_config)
    {
        std::cout << "Training config is different" << std::endl;
        return false;
    }

    bool is_same_evaluation_config = config1.evaluation.maximize_nb_trades.value_or(0) == config2.evaluation.maximize_nb_trades.value_or(0) &&
                                     config1.evaluation.minimum_nb_trades.value_or(0) == config2.evaluation.minimum_nb_trades.value_or(0) &&
                                     config1.evaluation.maximum_trade_duration.value_or(0) == config2.evaluation.maximum_trade_duration.value_or(0) &&
                                     config1.evaluation.expected_return_per_day.value_or(0) == config2.evaluation.expected_return_per_day.value_or(0) &&
                                     config1.evaluation.expected_return_per_month.value_or(0) == config2.evaluation.expected_return_per_month.value_or(0) &&
                                     config1.evaluation.expected_return.value_or(0) == config2.evaluation.expected_return.value_or(0) &&
                                     config1.evaluation.maximum_drawdown.value_or(0) == config2.evaluation.maximum_drawdown.value_or(0) &&
                                     config1.evaluation.minimum_winrate.value_or(0) == config2.evaluation.minimum_winrate.value_or(0) &&
                                     config1.evaluation.minimum_profit_factor.value_or(0) == config2.evaluation.minimum_profit_factor.value_or(0);

    if (!is_same_evaluation_config)
    {
        std::cout << "Evaluation config is different" << std::endl;
        return false;
    }

    bool is_same_neat_config = config1.neat.population_size == config2.neat.population_size &&
                               config1.neat.fitness_threshold == config2.neat.fitness_threshold &&
                               config1.neat.no_fitness_termination == config2.neat.no_fitness_termination &&
                               config1.neat.reset_on_extinction == config2.neat.reset_on_extinction &&
                               config1.neat.activation_default == config2.neat.activation_default &&
                               config1.neat.activation_mutate_rate == config2.neat.activation_mutate_rate &&
                               config1.neat.num_inputs == config2.neat.num_inputs &&
                               config1.neat.num_outputs == config2.neat.num_outputs &&
                               config1.neat.num_hidden_layers == config2.neat.num_hidden_layers &&
                               config1.neat.compatibility_disjoint_coefficient == config2.neat.compatibility_disjoint_coefficient &&
                               config1.neat.compatibility_weight_coefficient == config2.neat.compatibility_weight_coefficient &&
                               config1.neat.conn_add_prob == config2.neat.conn_add_prob &&
                               config1.neat.conn_delete_prob == config2.neat.conn_delete_prob &&
                               config1.neat.enabled_default == config2.neat.enabled_default &&
                               config1.neat.enabled_mutate_rate == config2.neat.enabled_mutate_rate &&
                               config1.neat.initial_connections == config2.neat.initial_connections &&
                               config1.neat.node_add_prob == config2.neat.node_add_prob &&
                               config1.neat.node_delete_prob == config2.neat.node_delete_prob &&
                               config1.neat.weight_init_mean == config2.neat.weight_init_mean &&
                               config1.neat.weight_init_stdev == config2.neat.weight_init_stdev &&
                               config1.neat.weight_init_type == config2.neat.weight_init_type &&
                               config1.neat.weight_max_value == config2.neat.weight_max_value &&
                               config1.neat.weight_min_value == config2.neat.weight_min_value &&
                               config1.neat.weight_mutate_rate == config2.neat.weight_mutate_rate &&
                               config1.neat.weight_replace_rate == config2.neat.weight_replace_rate &&
                               config1.neat.max_stagnation == config2.neat.max_stagnation &&
                               config1.neat.species_elitism == config2.neat.species_elitism &&
                               config1.neat.elitism == config2.neat.elitism &&
                               config1.neat.survival_threshold == config2.neat.survival_threshold &&
                               config1.neat.min_species_size == config2.neat.min_species_size &&
                               config1.neat.compatibility_threshold == config2.neat.compatibility_threshold &&
                               config1.neat.bad_species_threshold == config2.neat.bad_species_threshold;

    if (!is_same_neat_config)
    {
        std::cout << "NEAT config is different" << std::endl;
        return false;
    }

    return true;
}
