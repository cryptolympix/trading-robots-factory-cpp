#include "gtest/gtest.h"
#include <string>
#include <vector>
#include "../builder.hpp"

TEST(Builder, ExtractParameters)
{
    std::string id_params = "id-14-source";
    std::string id_params_pattern = R"(id-(\d+)-(\w+))";

    std::vector<IndicatorParam> parameters = extract_parameters(id_params, id_params_pattern);

    ASSERT_EQ(parameters.size(), 2);
    ASSERT_EQ(std::get<int>(parameters[0]), 14);
    ASSERT_EQ(std::get<std::string>(parameters[1]), "source");
}

TEST(Builder, CreateIndicatorFromId)
{
    std::string id_params = "rsi-14-0";
    std::vector<IndicatorParam> parameters = {14, 0};

    Indicator *indicator = create_indicator_from_id(id_params, parameters);

    ASSERT_EQ(indicator->id, "rsi");
    ASSERT_EQ(indicator->params.size(), 2);
    ASSERT_EQ(std::get<int>(indicator->params["period"]), 14);
    ASSERT_EQ(std::get<int>(indicator->params["offset"]), 0);
}