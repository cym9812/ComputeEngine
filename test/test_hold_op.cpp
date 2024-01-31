#include "executor.h"
#include "rapidjson/document.h"
#include <gtest/gtest.h>
#include <variant>

TEST(MathOpTest, holdValueToValueForward) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"HOLD",
      "value":{"type":"value","value":[1,2,1,1,2,2,2,1]},
      "from":{"type":"value","value":[1]},
      "to":{"type":"value","value":[2]},
      "duration":{"type":"value","value":0.2}
    })";
    ComputeLib::BoolVectorType expect = {0, 0, 0, 0, 0, 1, 1, 0};

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsBoolVector(result));
    auto &vec = std::get<ComputeLib::BoolVectorType>(result);
    EXPECT_EQ(vec.size(), expect.size());
    for (size_t i = 0; i < expect.size(); ++i) {
        EXPECT_EQ(vec[i], expect[i]) << "Mismatch at index " << i;
    }
}

TEST(MathOpTest, holdValueToValueBackword) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"HOLD",
      "value":{"type":"value","value":[1,2,1,1,2,2,2,1]},
      "from":{"type":"value","value":[1]},
      "to":{"type":"value","value":[2]},
      "duration":{"type":"value","value":-0.2}
    })";
    ComputeLib::BoolVectorType expect = {0, 0, 0, 0, 1, 1, 0, 0};

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsBoolVector(result));
    auto &vec = std::get<ComputeLib::BoolVectorType>(result);
    EXPECT_EQ(vec.size(), expect.size());
    for (size_t i = 0; i < expect.size(); ++i) {
        EXPECT_EQ(vec[i], expect[i]) << "Mismatch at index " << i;
    }
}

TEST(MathOpTest, holdValueToAnyForward) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"HOLD",
      "value":{"type":"value","value":[1,2,2,1,3,3,1,4,5,6]},
      "from":{"type":"value","value":[1]},
      "to":{"type":"value","value":[]},
      "duration":{"type":"value","value":0.2}
    })";
    ComputeLib::BoolVectorType expect = {0, 0, 1, 0, 0, 1, 0, 0, 1, 1};

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsBoolVector(result));
    auto &vec = std::get<ComputeLib::BoolVectorType>(result);
    EXPECT_EQ(vec.size(), expect.size());
    for (size_t i = 0; i < expect.size(); ++i) {
        EXPECT_EQ(vec[i], expect[i]) << "Mismatch at index " << i;
    }
}

TEST(MathOpTest, holdValueToAnyBackword) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"HOLD",
      "value":{"type":"value","value":[6,5,4,1,3,3,1,2,2,1]},
      "from":{"type":"value","value":[1]},
      "to":{"type":"value","value":[]},
      "duration":{"type":"value","value":-0.2}
    })";
    ComputeLib::BoolVectorType expect = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0};

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsBoolVector(result));
    auto &vec = std::get<ComputeLib::BoolVectorType>(result);
    EXPECT_EQ(vec.size(), expect.size());
    for (size_t i = 0; i < expect.size(); ++i) {
        EXPECT_EQ(vec[i], expect[i]) << "Mismatch at index " << i;
    }
}

TEST(MathOpTest, holdAnyToValueForward) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"HOLD",
      "value":{"type":"value","value":[1,1,1,2,1,1,3,1,1,1]},
      "from":{"type":"value","value":[]},
      "to":{"type":"value","value":[1]},
      "duration":{"type":"value","value":0.2}
    })";
    ComputeLib::BoolVectorType expect = {0, 0, 0, 0, 0, 1, 0, 0, 1, 1};

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsBoolVector(result));
    auto &vec = std::get<ComputeLib::BoolVectorType>(result);
    EXPECT_EQ(vec.size(), expect.size());
    for (size_t i = 0; i < expect.size(); ++i) {
        EXPECT_EQ(vec[i], expect[i]) << "Mismatch at index " << i;
    }
}

TEST(MathOpTest, holdAnyToValueBackword) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"HOLD",
      "value":{"type":"value","value":[1,1,1,3,1,1,2,1,1,1]},
      "from":{"type":"value","value":[]},
      "to":{"type":"value","value":[1]},
      "duration":{"type":"value","value":-0.2}
    })";
    ComputeLib::BoolVectorType expect = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0};

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsBoolVector(result));
    auto &vec = std::get<ComputeLib::BoolVectorType>(result);
    EXPECT_EQ(vec.size(), expect.size());
    for (size_t i = 0; i < expect.size(); ++i) {
        EXPECT_EQ(vec[i], expect[i]) << "Mismatch at index " << i;
    }
}
