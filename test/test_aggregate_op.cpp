#include "executor.h"
#include "rapidjson/document.h"
#include <gtest/gtest.h>
#include <variant>


TEST(AggergateOpTest, Max) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"MAX",
      "value":{"type":"value","value":[0,1,2,3,4,5,6,7,8,9]}
    })";
    ComputeLib::NumericType expect = 9;

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumeric(result));
    auto &res = std::get<ComputeLib::NumericType>(result);

    EXPECT_EQ(expect, res);
}

TEST(AggergateOpTest, Min) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"MIN",
      "value":{"type":"value","value":[0,1,2,3,4,5,6,7,8,9]}
    })";
    ComputeLib::NumericType expect = 0;

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumeric(result));
    auto &res = std::get<ComputeLib::NumericType>(result);

    EXPECT_EQ(expect, res);
}

TEST(AggergateOpTest, Avg) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"AVG",
      "value":{"type":"value","value":[0,1,2,3,4,5,6,7,8,9]}
    })";
    ComputeLib::NumericType expect = 4.5;

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumeric(result));
    auto &res = std::get<ComputeLib::NumericType>(result);

    EXPECT_EQ(expect, res);
}
