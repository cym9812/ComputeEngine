#include "executor.h"
#include "operator.h"
#include <rapidjson/document.h>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace ComputeLib;

static constexpr uint8_t TRUE = 1;
static constexpr uint8_t FALSE = 0;

#define REGISTER_OPERATION(opName, opFunc) \
    operationMap_[opName] = [this](const Query &query) { return opFunc(query); }

#define GET_BOOL(var) get<BoolType>(var)
#define GET_NUMERIC(var) get<NumericType>(var)
#define GET_BOOL_VECTOR(var) get<BoolVectorType>(var)
#define GET_NUMERIC_VECTOR(var) get<NumericVectorType>(var)

Executor::Executor(uint32_t num_threads) : num_threads_(num_threads) {
    REGISTER_OPERATION("EQ", compareOp);
    REGISTER_OPERATION("NE", compareOp);
    REGISTER_OPERATION("LT", compareOp);
    REGISTER_OPERATION("LE", compareOp);
    REGISTER_OPERATION("GT", compareOp);
    REGISTER_OPERATION("GE", compareOp);
    REGISTER_OPERATION("ADD", addOp);
    REGISTER_OPERATION("SUB", subOp);
    REGISTER_OPERATION("MUL", mulOp);
    REGISTER_OPERATION("DIV", divOp);
    REGISTER_OPERATION("POW", powOp);
    REGISTER_OPERATION("ABS", absOp);
    REGISTER_OPERATION("AND", logicalOp);
    REGISTER_OPERATION("OR", logicalOp);
    REGISTER_OPERATION("COUNT", countOp);
    REGISTER_OPERATION("MAX", maxOp);
    REGISTER_OPERATION("MIN", minOp);
    REGISTER_OPERATION("AVG", avgOp);
    REGISTER_OPERATION("JUMP", jumpOp);
    REGISTER_OPERATION("BEFORE", beforeOp);
    REGISTER_OPERATION("AFTER", afterOp);
    REGISTER_OPERATION("HOLD", holdOp);
    REGISTER_OPERATION("DURATION", durationOp);
    REGISTER_OPERATION("SELECT", selectOp);
}

bool Executor::isSameType(const std::vector<GenericValue> &vector, const ValueType type) {
    switch (type) {
        case ValueType::BOOL_TYPE:
            return std::ranges::all_of(vector, [](const GenericValue &val) {
                return holdsBool(val);
            });
        case ValueType::NUMERIC_TYPE:
            return std::ranges::all_of(vector, [](const GenericValue &val) {
                return holdsNumeric(val);
            });
        case ValueType::BOOL_VECTOR_TYPE:
            return std::ranges::all_of(vector, [](const GenericValue &val) {
                return holdsBoolVector(val);
            });
        case ValueType::NUMERIC_VECTOR_TYPE:
            return std::ranges::all_of(vector, [](const GenericValue &val) {
                return holdsNumericVector(val);
            });
        default:
            throw std::runtime_error("Unknown ValueType");
    }
}

bool Executor::isSameLength(const std::vector<GenericValue> &vector) {
    if (vector.size() <= 1) {
        return true;
    }
    bool result = true;
    if (isSameType(vector, ValueType::BOOL_VECTOR_TYPE)) {
        const std::size_t size = GET_BOOL_VECTOR(vector[0]).size();
        for (const auto &elem: vector) {
            if (GET_BOOL_VECTOR(elem).size() != size) {
                result = false;
                break;
            }
        }
    } else if (isSameType(vector, ValueType::NUMERIC_VECTOR_TYPE)) {
        const std::size_t size = GET_NUMERIC_VECTOR(vector[0]).size();
        for (const auto &elem: vector) {
            if (GET_NUMERIC_VECTOR(elem).size() != size) {
                result = false;
                break;
            }
        }
    } else {
        throw std::runtime_error("Invalid type inside vector");
    }
    return result;
}

GenericValue Executor::run(const Query &query) {
    const std::string type = query["type"].GetString();

    if (type == std::string("operation")) {
        const std::string operatorString = getQueryOperation(query);
        if (!operationMap_.contains(operatorString)) {
            throw std::runtime_error("Unknown operation: " + operatorString);
        }
        GenericValue returnValue = operationMap_[operatorString](query);
        return returnValue;
    } else if (type == std::string("value")) {
        if (query["value"].IsNumber()) {
            return query["value"].GetDouble();
        } else {
            throw std::runtime_error("Unknown value type: " + std::string(query["value"].GetString()));
        }
    } else {
        throw std::runtime_error("Unknown type: " + type);
    }
}


GenericValue Executor::compareOp(const Query &query) {
    static const std::unordered_map<std::string, CompareFunction> functionMap = {
        {"EQ", &compareEqual<NumericType>},
        {"NE", &compareNotEqual<NumericType>},
        {"LT", &compareLess<NumericType>},
        {"GT", &compareGreater<NumericType>},
        {"LE", &compareLessEqual<NumericType>},
        {"GE", &compareGreaterEqual<NumericType>}
    };

    const std::string op = getQueryOperation(query);
    if (!functionMap.contains(op)) {
        throw std::runtime_error("Unknown operator: " + op);
    }
    auto &cmp = functionMap.at(op);
    const GenericValue left = run(query["left"]);
    const GenericValue right = run(query["right"]);

    if (holdsNumeric(left) && holdsNumeric(right)) {
        BoolType result = cmp(GET_NUMERIC(left), GET_NUMERIC(right)) ? TRUE : FALSE;
        return result;
    }

    if (holdsNumericVector(left) && holdsNumeric(right)) {
        const auto &leftVector = GET_NUMERIC_VECTOR(left);
        const auto rightValue = GET_NUMERIC(right);
        std::vector<BoolType> result(leftVector.size(), FALSE);
        for (std::size_t i = 0; i < leftVector.size(); ++i) {
            result[i] = cmp(leftVector[i], rightValue) ? TRUE : FALSE;
        }
        return result;
    }

    if (holdsNumericVector(left) && holdsNumericVector(right)) {
        const auto &leftVector = GET_NUMERIC_VECTOR(left);
        const auto &rightVector = GET_NUMERIC_VECTOR(right);
        if (leftVector.size() != rightVector.size()) {
            throw std::runtime_error("Vector size mismatch");
        }
        std::vector<BoolType> result(leftVector.size(), FALSE);
        for (std::size_t i = 0; i < leftVector.size(); ++i) {
            result[i] = cmp(leftVector[i], rightVector[i]) ? TRUE : FALSE;
        }
        return result;
    }

    throw std::runtime_error("Unsupported type for compare operator");
}

GenericValue Executor::addOp(const Query &query) {
    BoolType result = 1;
    return result;
}

GenericValue Executor::subOp(const Query &query) {
    BoolType result = 1;
    return result;
}

GenericValue Executor::mulOp(const Query &query) {
    BoolType result = 1;
    return result;
}

GenericValue Executor::divOp(const Query &query) {
    BoolType result = 1;
    return result;
}

GenericValue Executor::powOp(const Query &query) {
    BoolType result = 1;
    return result;
}

GenericValue Executor::absOp(const Query &query) {
    BoolType result = 1;
    return result;
}


GenericValue Executor::logicalOp(const Query &query) {
    static const std::unordered_map<std::string, LogicalFunction> functionMap = {
        {"AND", &logicalAnd<BoolType>},
        {"OR", &logicalOr<BoolType>}
    };

    const auto op = getQueryOperation(query);
    if (!functionMap.contains(op)) {
        throw std::runtime_error("Unknown operator: " + op);
    }
    auto &func = functionMap.at(op);

    std::vector<GenericValue> operands;
    for (const auto &elem: query["operands"].GetArray()) {
        operands.emplace_back(run(elem));
    }

    if (operands.empty()) {
        throw std::runtime_error("No operands for logical operator");
    }

    if (isSameType(operands, ValueType::BOOL_TYPE)) {
        BoolType result = TRUE;
        for (const auto &operand: operands) {
            result = func(result, GET_BOOL(operand));
        }
        return result;
    }

    if (isSameType(operands, ValueType::BOOL_VECTOR_TYPE)) {
        if (!isSameLength(operands)) {
            throw std::runtime_error("Operands must have the same length");
        }
        const std::size_t size = GET_BOOL_VECTOR(operands[0]).size();
        BoolVectorType result(size, TRUE);
        for (const auto &elem: operands) {
            const auto &vec = GET_BOOL_VECTOR(elem);
            for (std::size_t i = 0; i < size; ++i) {
                result[i] = func(result[i], vec[i]);
            }
        }
        return result;
    }

    throw std::runtime_error("Operands of logical operators must be of type bool or bool[]");
}

GenericValue Executor::countOp(const Query &query) {
    GenericValue value = run(query["value"]);

    if (holdsBoolVector(value)) {
        const auto &vec = GET_BOOL_VECTOR(value);
        auto result = static_cast<NumericType>(std::ranges::count(vec, TRUE));
        return result;
    }
    return value;
}

GenericValue Executor::maxOp(const Query &query) {
    BoolType result = 1;
    return result;
}

GenericValue Executor::minOp(const Query &query) {
    BoolType result = 1;
    return result;
}

GenericValue Executor::avgOp(const Query &query) {
    BoolType result = 1;
    return result;
}

GenericValue Executor::jumpOp(const Query &query) {
    BoolType result = 1;
    return result;
}

GenericValue Executor::beforeOp(const Query &query) {
    BoolType result = 1;
    return result;
}

GenericValue Executor::afterOp(const Query &query) {
    BoolType result = 1;
    return result;
}

GenericValue Executor::holdOp(const Query &query) {
    BoolType result = 1;
    return result;
}

GenericValue Executor::durationOp(const Query &query) {
    GenericValue value = run(query["value"]);

    return value;
}

GenericValue Executor::selectOp(const Query &query) {
    const std::string name = query["value"].GetString();
    std::cout << "selectOp: " << name << std::endl;
    std::vector<double> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 1};
    return data;
}
