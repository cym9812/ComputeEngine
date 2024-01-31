#include "executor.h"
#include "rapidjson/document.h"
#include <gtest/gtest.h>
#include <variant>

TEST(MathOpTest, After1) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"AFTER",
      "value":{"type":"value","value":[1,2,3,4,5,5,5,1]},
      "from":{"type":"value","value":2},
      "to":{"type":"value","value":4},
      "duration":{"type":"value","value":0.2}
    })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsBoolVector(result));
    auto &vec = std::get<ComputeLib::BoolVectorType>(result);
    ComputeLib::BoolVectorType expect = {0, 0, 0, 0, 1, 1, 1, 0};
    EXPECT_EQ(vec.size(), expect.size());
    for (size_t i = 0; i < expect.size(); ++i) {
        EXPECT_EQ(vec[i], expect[i]) << "Mismatch at index " << i;
    }
}

TEST(MathOpTest, After2) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"AFTER",
      "value":{"type":"value","value":[5,4,3,2,1,1,1,5]},
      "from":{"type":"value","value":4},
      "to":{"type":"value","value":2},
      "duration":{"type":"value","value":0.2}
    })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsBoolVector(result));
    auto &vec = std::get<ComputeLib::BoolVectorType>(result);
    ComputeLib::BoolVectorType expect = {0, 0, 0, 0, 1, 1, 1, 0};
    EXPECT_EQ(vec.size(), expect.size());
    for (size_t i = 0; i < expect.size(); ++i) {
        EXPECT_EQ(vec[i], expect[i]) << "Mismatch at index " << i;
    }
}

TEST(MathOpTest, After3) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"AFTER",
      "value":{"type":"value","value":[1,2,3,4,5,5,1,1]},
      "from":{"type":"value","value":2},
      "to":{"type":"value","value":4},
      "duration":{"type":"value","value":0.4}
    })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsBoolVector(result));
    auto &vec = std::get<ComputeLib::BoolVectorType>(result);
    ComputeLib::BoolVectorType expect = {0, 0, 0, 0, 0, 0, 0, 0};
    EXPECT_EQ(vec.size(), expect.size());
    for (size_t i = 0; i < expect.size(); ++i) {
        EXPECT_EQ(vec[i], expect[i]) << "Mismatch at index " << i;
    }
}


TEST(MathOpTest, After4) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"AFTER",
      "value":{"type":"value","value":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,5,6,7,8,9,10,11,12]},
      "from":{"type":"value","value":3.8},
      "to":{"type":"value","value":8},
      "duration":{"type":"value","value":0.2}
    })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsBoolVector(result));
    auto &vec = std::get<ComputeLib::BoolVectorType>(result);
    ComputeLib::BoolVectorType expect = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    EXPECT_EQ(vec.size(), expect.size());
    for (size_t i = 0; i < expect.size(); ++i) {
        EXPECT_EQ(vec[i], expect[i]) << "Mismatch at index " << i;
    }
}

TEST(MathOpTest, After5) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"AFTER",
      "value":{"type":"value","value":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,3,6,7,8,9,10,11,12]},
      "from":{"type":"value","value":3.8},
      "to":{"type":"value","value":8},
      "duration":{"type":"value","value":0.2}
    })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsBoolVector(result));
    auto &vec = std::get<ComputeLib::BoolVectorType>(result);
    ComputeLib::BoolVectorType expect = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1};
    EXPECT_EQ(vec.size(), expect.size());
    for (size_t i = 0; i < expect.size(); ++i) {
        EXPECT_EQ(vec[i], expect[i]) << "Mismatch at index " << i;
    }
}

TEST(MathOpTest, After6) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"AFTER",
      "value":{"type":"value","value":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,6,8,7,3,6,7,8,9,10,11,12]},
      "from":{"type":"value","value":3.8},
      "to":{"type":"value","value":8},
      "duration":{"type":"value","value":0.2}
    })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsBoolVector(result));
    auto &vec = std::get<ComputeLib::BoolVectorType>(result);
    ComputeLib::BoolVectorType expect = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1};
    EXPECT_EQ(vec.size(), expect.size());
    for (size_t i = 0; i < expect.size(); ++i) {
        EXPECT_EQ(vec[i], expect[i]) << "Mismatch at index " << i;
    }
}

TEST(MathOpTest, After7) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
      "type":"operation",
      "operation":"AFTER",
      "value":{"type":"value","value":[1,2,3,6,2,3,4,5,6,7,5,4,6,7,7,7,8,8,6,5,4,3,2,1,2,3]},
      "from":{"type":"value","value":2.5},
      "to":{"type":"value","value":6},
      "duration":{"type":"value","value":0.3}
    })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsBoolVector(result));
    auto &vec = std::get<ComputeLib::BoolVectorType>(result);
    ComputeLib::BoolVectorType expect = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    EXPECT_EQ(vec.size(), expect.size());
    for (size_t i = 0; i < expect.size(); ++i) {
        EXPECT_EQ(vec[i], expect[i]) << "Mismatch at index " << i;
    }
}
