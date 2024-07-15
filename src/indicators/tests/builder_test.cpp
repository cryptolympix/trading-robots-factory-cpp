#include "gtest/gtest.h"
#include <string>
#include <unordered_map>
#include <typeindex>
#include "../builder.hpp"

TEST(BuilderTest, ExtractParameters)
{
    std::string id_params = "id-period=14-source=close";
    std::string id_params_pattern = R"(id-(\w+)=(\d+)-(\w+)=(\w+))";

    std::unordered_map<std::string, IndicatorParam> parameters = extract_parameters(id_params, id_params_pattern);

    ASSERT_EQ(parameters.size(), 2);
    ASSERT_EQ(std::get<int>(parameters["period"]), 14);
    ASSERT_EQ(std::get<std::string>(parameters["source"]), "close");
}

TEST(BuilderTest, CheckParams)
{
    std::unordered_map<std::string, IndicatorParam> parameters = {{"period", 14}, {"source", "close"}};
    std::unordered_map<std::string, std::type_index> required_params = {{"period", typeid(int)}, {"source", typeid(std::string)}};

    ASSERT_TRUE(check_params(parameters, required_params));

    parameters = {{"period", 14}};
    required_params = {{"period", typeid(int)}, {"source", typeid(std::string)}};

    ASSERT_THROW(check_params(parameters, required_params), std::invalid_argument);
}

TEST(BuilderTest, CreateIndicatorFromId)
{
    std::string id = "rsi";
    std::unordered_map<std::string, IndicatorParam> parameters = {{"period", 14}, {"offset", 0}};

    Indicator *indicator = create_indicator_from_id(id, parameters);

    ASSERT_EQ(indicator->id, "rsi");
    ASSERT_EQ(indicator->params.size(), 2);
    ASSERT_EQ(std::get<int>(indicator->params["period"]), 14);
    ASSERT_EQ(std::get<int>(indicator->params["offset"]), 0);
}