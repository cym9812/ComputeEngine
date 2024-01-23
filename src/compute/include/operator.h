#ifndef CPP_OPERATOR_H
#define CPP_OPERATOR_H

#include "rapidjson/document.h"
#include <vector>
#include <variant>
#include <string>
#include <stdexcept>

namespace ComputeLib {
    enum class OperatorEnum {
        EQ,
        NE,
        LT,
        LE,
        GT,
        GE,
        ADD,
        SUB,
        MUL,
        DIV,
        POW,
        ABS,
        AND,
        OR,
        COUNT,
        MAX,
        MIN,
        AVG,
        JUMP,
        BEFORE,
        AFTER,
        HOLD,
        DURATION,
        SELECT
    };

    enum class ValueType {
        BOOL_TYPE,
        NUMERIC_TYPE,
        BOOL_VECTOR_TYPE,
        NUMERIC_VECTOR_TYPE
    };

    using Query = rapidjson::Value;
    using BoolType = uint8_t;
    using NumericType = double;
    using BoolVectorType = std::vector<BoolType>;
    using NumericVectorType = std::vector<NumericType>;

    using GenericValue = std::variant<BoolType, NumericType, BoolVectorType, NumericVectorType>;

    OperatorEnum getOperatorEnum(const std::string &str);
}


#endif //CPP_OPERATOR_H
