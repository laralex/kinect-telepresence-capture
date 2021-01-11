#include "nlohmann/json.hpp"
#include <gtest/gtest.h>

using json = nlohmann::json;

GTEST_TEST(NlohmannJson, Json)
{
    {
        json j;
        j["pi"] = 3.141;
        j["happy"] = true;
        j["name"] = "Niels";
        j["nothing"] = nullptr;
        j["answer"]["everything"] = 42;
        j["list"] = { 1, 0, 2 };
        j["object"] = { {"currency", "USD"}, {"value", 42.99} };
        ASSERT_TRUE(j["happy"]);
        ASSERT_EQ(j["answer"]["everything"], 42);
    }

    {
        json j = {
            {"pi", 3.141},
            {"happy", true},
            {"name", "Niels"},
            {"nothing", nullptr},
            {"answer", {
                {"everything", 42}
            }},
            {"list", {1, 0, 2}},
            {"object", {
                {"currency", "USD"},
                {"value", 42.99}
            }}
        };
        ASSERT_TRUE(j["happy"]);
        ASSERT_EQ(j["answer"]["everything"], 42);
    }
}