#include "executor.h"
#include "operator.h"
#include "rapidjson/document.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

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
    REGISTER_OPERATION("ADD", mathOp);
    REGISTER_OPERATION("SUB", mathOp);
    REGISTER_OPERATION("MUL", mathOp);
    REGISTER_OPERATION("DIV", mathOp);
    REGISTER_OPERATION("POW", mathOp);
    REGISTER_OPERATION("ABS", absOp);
    REGISTER_OPERATION("AND", logicalOp);
    REGISTER_OPERATION("OR", logicalOp);
    REGISTER_OPERATION("COUNT", countOp);
    REGISTER_OPERATION("MAX", aggregateOp);
    REGISTER_OPERATION("MIN", aggregateOp);
    REGISTER_OPERATION("AVG", aggregateOp);
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
    const std::string type = getQueryType(query);

    if (type == "operation") {
        const std::string operatorString = getQueryOperation(query);
        if (!operationMap_.contains(operatorString)) {
            throw std::runtime_error("Unknown operation: " + operatorString);
        }
        GenericValue returnValue = operationMap_[operatorString](query);
        return returnValue;
    }

    if (type == "value") {
        if (query["value"].IsNumber()) {
            NumericType returnValue = query["value"].GetDouble();
            return returnValue;
        }
        if (query["value"].IsArray()) {
            NumericVectorType returnValue;
            for (const auto &elem: query["value"].GetArray()) {
                returnValue.emplace_back(elem.GetDouble());
            }
            return returnValue;
        }

        throw std::runtime_error("Invalid type inside value");
    }

    throw std::runtime_error("Unknown type: " + type);
}


GenericValue Executor::compareOp(const Query &query) {
    /*
     * Query format:
     * "type": "operation"
     * "operation": "EQ"/"NE"/etc.
     * "left": <left operand>
     * "right": <right operand>
     */
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
        BoolVectorType result(leftVector.size(), FALSE);
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
        BoolVectorType result(leftVector.size(), FALSE);
        for (std::size_t i = 0; i < leftVector.size(); ++i) {
            result[i] = cmp(leftVector[i], rightVector[i]) ? TRUE : FALSE;
        }
        return result;
    }

    throw std::runtime_error("Unsupported type for compare operator");
}

GenericValue Executor::mathOp(const Query &query) {
    /*
     * Query format:
     * "type": "operation"
     * "operation": "ADD"/"SUB"/etc.
     * "left": <left operand>
     * "right": <right operand>
     */
    static const std::unordered_map<std::string, MathFunction> functionMap = {
        {"ADD", &mathAdd<NumericType>},
        {"SUB", &mathSub<NumericType>},
        {"MUL", &mathMul<NumericType>},
        {"DIV", &mathDiv<NumericType>},
        {"POW", &mathPow<NumericType>},
    };

    const std::string op = getQueryOperation(query);
    if (!functionMap.contains(op)) {
        throw std::runtime_error("Unknown operator: " + op);
    }
    auto &func = functionMap.at(op);

    const GenericValue left = run(query["left"]);
    const GenericValue right = run(query["right"]);

    if (holdsNumeric(left) && holdsNumeric(right)) {
        NumericType result = func(GET_NUMERIC(left), GET_NUMERIC(right));
        return result;
    }

    if (holdsNumericVector(left) && holdsNumeric(right)) {
        const auto &leftVector = GET_NUMERIC_VECTOR(left);
        const auto rightValue = GET_NUMERIC(right);
        NumericVectorType result(leftVector.size(), FALSE);
        for (std::size_t i = 0; i < leftVector.size(); ++i) {
            result[i] = func(leftVector[i], rightValue);
        }
        return result;
    }

    if (holdsNumericVector(left) && holdsNumericVector(right)) {
        const auto &leftVector = GET_NUMERIC_VECTOR(left);
        const auto &rightVector = GET_NUMERIC_VECTOR(right);
        if (leftVector.size() != rightVector.size()) {
            throw std::runtime_error("Vector size mismatch");
        }
        NumericVectorType result(leftVector.size(), FALSE);
        for (std::size_t i = 0; i < leftVector.size(); ++i) {
            result[i] = func(leftVector[i], rightVector[i]);
        }
        return result;
    }

    throw std::runtime_error("Unsupported type for numeric operation");
}

GenericValue Executor::absOp(const Query &query) {
    /*
     * Query format:
     * "type": "operation"
     * "operation": "ABS"
     * "value": <operand>
     */

    const auto value = run(query["value"]);

    if (holdsNumeric(value)) {
        NumericType result = std::abs(GET_NUMERIC(value));
        return result;
    }

    if (holdsNumericVector(value)) {
        const auto &vec = GET_NUMERIC_VECTOR(value);
        NumericVectorType result(vec.size(), 0);
        for (std::size_t i = 0; i < vec.size(); ++i) {
            result[i] = std::abs(vec[i]);
        }
        return result;
    }

    throw std::runtime_error("Unsupported type for ABS operation");
}


GenericValue Executor::logicalOp(const Query &query) {
    /*
     * Query format:
     * "type": "operation"
     * "operation": "AND"/"OR".
     * "operands": [<operand>, <operand>, ...]
     */
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
    /*
     * Query format:
     * "type": "operation"
     * "operation": "COUNT"
     * "value": <operand>
     * "initialValue": <value>
     * "unit": <value>
     */
    const GenericValue value = run(query["value"]);
    const GenericValue initialValue = run(query["initialValue"]);
    const GenericValue unit = run(query["unit"]);

    if (holdsBoolVector(value) && holdsNumeric(initialValue) && holdsNumeric(unit)) {
        const auto &vec = GET_BOOL_VECTOR(value);
        const auto iVal = GET_NUMERIC(initialValue);
        const auto uVal = GET_NUMERIC(unit);
        auto result = static_cast<NumericType>(std::ranges::count(vec, TRUE)) * iVal + uVal;
        return result;
    }

    throw std::runtime_error("Operand of COUNT must be of type bool[]");
}

GenericValue Executor::aggregateOp(const Query &query) {
    /*
     * Query format:
     * "type": "operation"
     * "operation": "MAX"/"MIN"/"AVG"
     * "value": <operand>
     */
    static const std::unordered_map<std::string, AggregateFunction> functionMap = {
        {"MAX", &aggregateMax<NumericType>},
        {"MIN", &aggregateMin<NumericType>},
        {"AVG", &aggregateAvg<NumericType>},
    };

    const std::string op = getQueryOperation(query);
    if (!functionMap.contains(op)) {
        throw std::runtime_error("Unknown operator: " + op);
    }
    auto &func = functionMap.at(op);

    const GenericValue value = run(query["value"]);
    if (holdsNumericVector(value)) {
        NumericType result = func(GET_NUMERIC_VECTOR(value));
        return result;
    }

    throw std::runtime_error("Operand of aggregate functions must be of type numeric[]");
}

GenericValue Executor::jumpOp(const Query &query) {
    /*
     * Query format:
     * "type": "operation"
     * "operation": "JUMP"
     * "value": <operand>
     * "from": [<value>, <value>, ...]
     * "to": <value>
     */
    const GenericValue value = run(query["value"]);
    const GenericValue from = run(query["from"]);
    const GenericValue to = run(query["to"]);

    if (holdsNumericVector(value) && holdsNumericVector(from) && holdsNumeric(to)) {
        const auto &valueVec = GET_NUMERIC_VECTOR(value);
        const auto &fromVec = GET_NUMERIC_VECTOR(from);
        const auto toVal = GET_NUMERIC(to);

        BoolVectorType result(valueVec.size(), FALSE);

        if (fromVec.empty()) {
            // Any -> toVal
            for (std::size_t i = 1; i < valueVec.size(); ++i) {
                if (valueVec[i - 1] != toVal && valueVec[i] == toVal) {
                    result[i] = TRUE;
                }
            }
        } else {
            // fromVal -> toVal
            for (std::size_t i = 1; i < valueVec.size(); ++i) {
                if (valueVec[i] == toVal) {
                    for (const auto &fromVal: fromVec) {
                        if (valueVec[i - 1] == fromVal) {
                            result[i] = TRUE;
                        }
                    }
                }
            }
        }

        return result;
    }

    throw std::runtime_error("Operand of jump functions must be of type numeric[]");
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
    /*
     * Query format:
     * "type": "operation"
     * "operation": "DURATION"
     * "value": <operand>
     * "minDuration": <value>
     */

    const GenericValue value = run(query["value"]);
    const GenericValue minDuration = run(query["minDuration"]);

    if (holdsBoolVector(value) && holdsNumeric(minDuration)) {
        const auto &valueVec = GET_BOOL_VECTOR(value);
        const auto minDurationVal = GET_NUMERIC(minDuration);
        BoolVectorType result(valueVec);

        const auto cntThreshold = static_cast<uint32_t>(minDurationVal / 0.1);
        uint32_t cnt = 0;
        bool inSequence = false;
        for (std::size_t i = 0; i < result.size(); i++) {
            if (inSequence) {
                if (result[i] == FALSE) {
                    if (cnt < cntThreshold) {
                        std::fill_n(result.begin() + i - cnt, cnt, FALSE);
                    }
                    cnt = 0;
                    inSequence = false;
                }
            } else {
                if (result[i] == TRUE) {
                    inSequence = true;
                    cnt++;
                }
            }
        }
    }

    return value;
}

GenericValue Executor::selectOp(const Query &query) {
    /*
     * Query format:
     * "type": "operation"
     * "operation": "SELECT"
     * "value": string
     */
    const std::string name = query["value"].GetString();
    std::cout << "selectOp: " << name << std::endl;
    std::vector<double> data(100000);
    return data;
}
