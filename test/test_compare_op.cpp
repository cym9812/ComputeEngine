#include "executor.h"
#include "rapidjson/document.h"
#include <gtest/gtest.h>
#include <variant>


TEST(CompareOpTest, EQ) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"EQ",
      "left":{"type":"value","value":[0,1,2,3,4,5,6,7,8,9]},
      "right":{"type":"value","value":5}
    })";
    ComputeLib::BoolVectorType expect = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0};

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

TEST(CompareOpTest, NE) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"NE",
      "left":{"type":"value","value":[0,1,2,3,4,5,6,7,8,9]},
      "right":{"type":"value","value":5}
    })";
    ComputeLib::BoolVectorType expect = {1, 1, 1, 1, 1, 0, 1, 1, 1, 1};

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

TEST(CompareOpTest, LT) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"LT",
      "left":{"type":"value","value":[0,1,2,3,4,5,6,7,8,9]},
      "right":{"type":"value","value":5}
    })";
    ComputeLib::BoolVectorType expect = {1, 1, 1, 1, 1, 0, 0, 0, 0, 0};

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

TEST(CompareOpTest, LE) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"LE",
      "left":{"type":"value","value":[0,1,2,3,4,5,6,7,8,9]},
      "right":{"type":"value","value":5}
    })";
    ComputeLib::BoolVectorType expect = {1, 1, 1, 1, 1, 1, 0, 0, 0, 0};

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

TEST(CompareOpTest, GT) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"GT",
      "left":{"type":"value","value":[0,1,2,3,4,5,6,7,8,9]},
      "right":{"type":"value","value":5}
    })";
    ComputeLib::BoolVectorType expect = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1};

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

TEST(CompareOpTest, GE) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"GE",
      "left":{"type":"value","value":[0,1,2,3,4,5,6,7,8,9]},
      "right":{"type":"value","value":5}
    })";
    ComputeLib::BoolVectorType expect = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1};

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
