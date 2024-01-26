#include "executor.h"
#include "rapidjson/document.h"
#include <gtest/gtest.h>
#include <variant>

TEST(MathOpTest, Addition1) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "ADD",
                                   "left": {
                                     "type": "value",
                                     "value": [1,2,3,4,5]
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": [1,2,3,4,5]
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumericVector(result));
    auto &vec = std::get<ComputeLib::NumericVectorType>(result);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 2);
    EXPECT_EQ(vec[1], 4);
    EXPECT_EQ(vec[2], 6);
    EXPECT_EQ(vec[3], 8);
    EXPECT_EQ(vec[4], 10);
}

TEST(MathOpTest, Addition2) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "ADD",
                                   "left": {
                                     "type": "value",
                                     "value": 1
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": 1
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumeric(result));
    auto resultVal = std::get<ComputeLib::NumericType>(result);
    EXPECT_EQ(resultVal, 2);
}

TEST(MathOpTest, Addition3) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "ADD",
                                   "left": {
                                     "type": "value",
                                     "value": [1,2,3,4,5]
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": 1
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumericVector(result));
    auto &vec = std::get<ComputeLib::NumericVectorType>(result);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 2);
    EXPECT_EQ(vec[1], 3);
    EXPECT_EQ(vec[2], 4);
    EXPECT_EQ(vec[3], 5);
    EXPECT_EQ(vec[4], 6);
}

TEST(MathOpTest, Subtraction1) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "SUB",
                                   "left": {
                                     "type": "value",
                                     "value": [1,2,3,4,5]
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": [1,1,1,1,1]
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumericVector(result));
    auto &vec = std::get<ComputeLib::NumericVectorType>(result);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[1], 1);
    EXPECT_EQ(vec[2], 2);
    EXPECT_EQ(vec[3], 3);
    EXPECT_EQ(vec[4], 4);
}

TEST(MathOpTest, Subtraction2) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "SUB",
                                   "left": {
                                     "type": "value",
                                     "value": 1
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": 1
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumeric(result));
    auto resultVal = std::get<ComputeLib::NumericType>(result);
    EXPECT_EQ(resultVal, 0);
}

TEST(MathOpTest, Subtraction3) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "SUB",
                                   "left": {
                                     "type": "value",
                                     "value": [1,2,3,4,5]
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": 1
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumericVector(result));
    auto &vec = std::get<ComputeLib::NumericVectorType>(result);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[1], 1);
    EXPECT_EQ(vec[2], 2);
    EXPECT_EQ(vec[3], 3);
    EXPECT_EQ(vec[4], 4);
}

TEST(MathOpTest, Multiplication1) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "MUL",
                                   "left": {
                                     "type": "value",
                                     "value": [1,2,3,4,5]
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": [1,2,3,4,5]
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumericVector(result));
    auto &vec = std::get<ComputeLib::NumericVectorType>(result);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 4);
    EXPECT_EQ(vec[2], 9);
    EXPECT_EQ(vec[3], 16);
    EXPECT_EQ(vec[4], 25);
}

TEST(MathOpTest, Multiplication2) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "MUL",
                                   "left": {
                                     "type": "value",
                                     "value": 2
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": 3
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumeric(result));
    auto resultVal = std::get<ComputeLib::NumericType>(result);
    EXPECT_EQ(resultVal, 6);
}

TEST(MathOpTest, Multiplication3) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "MUL",
                                   "left": {
                                     "type": "value",
                                     "value": [1,2,3,4,5]
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": 2
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumericVector(result));
    auto &vec = std::get<ComputeLib::NumericVectorType>(result);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 2);
    EXPECT_EQ(vec[1], 4);
    EXPECT_EQ(vec[2], 6);
    EXPECT_EQ(vec[3], 8);
    EXPECT_EQ(vec[4], 10);
}

TEST(MathOpTest, Division1){
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "DIV",
                                   "left": {
                                     "type": "value",
                                     "value": [1,2,3,4,5]
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": [2,2,2,2,2]
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumericVector(result));
    auto &vec = std::get<ComputeLib::NumericVectorType>(result);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 0.5);
    EXPECT_EQ(vec[1], 1);
    EXPECT_EQ(vec[2], 1.5);
    EXPECT_EQ(vec[3], 2);
    EXPECT_EQ(vec[4], 2.5);
}

TEST(MathOpTest, Division2) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "DIV",
                                   "left": {
                                     "type": "value",
                                     "value": 10
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": 5
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumeric(result));
    auto resultVal = std::get<ComputeLib::NumericType>(result);
    EXPECT_EQ(resultVal, 2);
}

TEST(MathOpTest, Division3) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "DIV",
                                   "left": {
                                     "type": "value",
                                     "value": [1,2,3,4,5]
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": 2
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumericVector(result));
    auto &vec = std::get<ComputeLib::NumericVectorType>(result);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 0.5);
    EXPECT_EQ(vec[1], 1);
    EXPECT_EQ(vec[2], 1.5);
    EXPECT_EQ(vec[3], 2);
    EXPECT_EQ(vec[4], 2.5);
}

TEST(MathOpTest, power1){
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "POW",
                                   "left": {
                                     "type": "value",
                                     "value": [1,2,3,4,5]
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": [1,2,3,4,5]
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumericVector(result));
    auto &vec = std::get<ComputeLib::NumericVectorType>(result);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 4);
    EXPECT_EQ(vec[2], 27);
    EXPECT_EQ(vec[3], 256);
    EXPECT_EQ(vec[4], 3125);
}

TEST(MathOpTest, Power2) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "POW",
                                   "left": {
                                     "type": "value",
                                     "value": 2
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": 3
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumeric(result));
    auto resultVal = std::get<ComputeLib::NumericType>(result);
    EXPECT_EQ(resultVal, 8);
}

TEST(MathOpTest, Power3) {
    ComputeLib::Executor executor(1);
    const std::string task = R"({
                                   "type": "operation",
                                   "operation": "POW",
                                   "left": {
                                     "type": "value",
                                     "value": [1,2,3,4,5]
                                   },
                                   "right": {
                                     "type": "value",
                                     "value": 2
                                   }
                                })";

    rapidjson::Document doc;
    doc.Parse(task.c_str());
    ComputeLib::GenericValue result = executor.run(doc);
    EXPECT_TRUE(executor.holdsNumericVector(result));
    auto &vec = std::get<ComputeLib::NumericVectorType>(result);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 4);
    EXPECT_EQ(vec[2], 9);
    EXPECT_EQ(vec[3], 16);
    EXPECT_EQ(vec[4], 25);
}
