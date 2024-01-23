#include "operator.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>

using namespace ComputeLib;

OperatorEnum ComputeLib::getOperatorEnum(const std::string &str) {
    static const std::unordered_map<std::string, OperatorEnum> typeMap = {
            {"EQ", OperatorEnum::EQ},
            {"NE", OperatorEnum::NE},
            {"LT", OperatorEnum::LT},
            {"LE", OperatorEnum::LE},
            {"GT", OperatorEnum::GT},
            {"GE", OperatorEnum::GE},
            {"ADD", OperatorEnum::ADD},
            {"SUB", OperatorEnum::SUB},
            {"MUL", OperatorEnum::MUL},
            {"DIV", OperatorEnum::DIV},
            {"POW", OperatorEnum::POW},
            {"ABS", OperatorEnum::ABS},
            {"AND", OperatorEnum::AND},
            {"OR", OperatorEnum::OR},
            {"COUNT", OperatorEnum::COUNT},
            {"MAX", OperatorEnum::MAX},
            {"MIN", OperatorEnum::MIN},
            {"AVG", OperatorEnum::AVG},
            {"JUMP", OperatorEnum::JUMP},
            {"BEFORE", OperatorEnum::BEFORE},
            {"AFTER", OperatorEnum::AFTER},
            {"HOLD", OperatorEnum::HOLD},
            {"DURATION", OperatorEnum::DURATION},
            {"SELECT", OperatorEnum::SELECT}
    };
    if (!typeMap.contains(str)) {
        throw std::runtime_error("Unknown operator: " + str);
    }
    return typeMap.at(str);
}
