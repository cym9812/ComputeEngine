#include "executor.h"
#include "rapidjson/document.h"
#include <gtest/gtest.h>
#include <variant>

TEST(TrendOpTest, Jump1) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"JUMP",
      "value":{"type":"value","value":[1,1,2,2,1,1,2,2]},
      "from":{"type":"value","value":[1]},
      "to":{"type":"value","value":[2]}
    })";
    ComputeLib::BoolVectorType expect = {0, 0, 1, 0, 0, 0, 1, 0};

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

TEST(TrendOpTest, Jump2) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"JUMP",
      "value":{"type":"value","value":[1,2,3,2,4,2,2,2]},
      "from":{"type":"value","value":[1,3]},
      "to":{"type":"value","value":[2]}
    })";
    ComputeLib::BoolVectorType expect = {0, 1, 0, 1, 0, 0, 0, 0};

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

TEST(TrendOpTest, Jump3) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"JUMP",
      "value":{"type":"value","value":[1,2,1,3,1,4,1,5]},
      "from":{"type":"value","value":[1]},
      "to":{"type":"value","value":[2,4]}
    })";
    ComputeLib::BoolVectorType expect = {0, 1, 0, 0, 0, 1, 0, 0};

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

TEST(TrendOpTest, Jump4) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"JUMP",
      "value":{"type":"value","value":[1,2,1,1,2,2,3,2]},
      "from":{"type":"value","value":[]},
      "to":{"type":"value","value":[2]}
    })";
    ComputeLib::BoolVectorType expect = {0, 1, 0, 0, 1, 0, 0, 1};

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

TEST(TrendOpTest, Jump5) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"JUMP",
      "value":{"type":"value","value":[1,1,1,2,3,1,3,2]},
      "from":{"type":"value","value":[1,3]},
      "to":{"type":"value","value":[]}
    })";
    ComputeLib::BoolVectorType expect = {0, 0, 0, 1, 0, 0, 0, 1};

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
