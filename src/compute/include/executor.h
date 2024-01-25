#ifndef CPP_EXECUTOR_H
#define CPP_EXECUTOR_H

#include "operator.h"
#include "rapidjson/document.h"
#include <algorithm>
#include <cmath>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <numeric>
#include <queue>
#include <thread>
#include <variant>
#include <vector>

namespace ComputeLib {
    using CompareFunction = std::function<bool(const NumericType &, const NumericType &)>;
    using LogicalFunction = std::function<BoolType(const BoolType &, const BoolType &)>;
    using MathFunction = std::function<NumericType(const NumericType &, const NumericType &)>;
    using AggregateFunction = std::function<NumericType(const std::vector<NumericType> &)>;

    // CompareFunction
    template<typename T>
    bool compareEqual(const T &left, const T &right) {
        return left == right;
    }

    template<typename T>
    bool compareNotEqual(const T &left, const T &right) {
        return left != right;
    }

    template<typename T>
    bool compareLess(const T &left, const T &right) {
        return left < right;
    }

    template<typename T>
    bool compareGreater(const T &left, const T &right) {
        return left > right;
    }

    template<typename T>
    bool compareLessEqual(const T &left, const T &right) {
        return left <= right;
    }

    template<typename T>
    bool compareGreaterEqual(const T &left, const T &right) {
        return left >= right;
    }

    // LogicalFunction
    template<typename T>
    BoolType logicalAnd(const T &left, const T &right) {
        return left & right;
    }

    template<typename T>
    BoolType logicalOr(const T &left, const T &right) {
        return left | right;
    }

    // MathFunction
    template<typename T>
    T mathAdd(const T &left, const T &right) {
        return left + right;
    }

    template<typename T>
    T mathSub(const T &left, const T &right) {
        return left - right;
    }

    template<typename T>
    T mathMul(const T &left, const T &right) {
        return left * right;
    }

    template<typename T>
    T mathDiv(const T &left, const T &right) {
        return left / right;
    }

    template<typename T>
    T mathPow(const T &left, const T &right) {
        return std::pow(left, right);
    }

    // AggregateFunction
    template<typename T>
    T aggregateMax(const std::vector<T> &vec) {
        return std::ranges::max(vec);
    }

    template<typename T>
    T aggregateMin(const std::vector<T> &vec) {
        return std::ranges::min(vec);
    }

    template<typename T>
    T aggregateSum(const std::vector<T> &vec) {
        return std::accumulate(vec.begin(), vec.end(), 0);
    }

    template<typename T>
    T aggregateAvg(const std::vector<T> &vec) {
        double sum = aggregateSum<T>(vec);
        return sum / vec.size();
    }

    class Executor {
    public:
        explicit Executor(uint32_t num_threads);

        ~Executor() = default;

        GenericValue run(const Query &query);

        GenericValue compareOp(const Query &query);

        GenericValue mathOp(const Query &query);

        GenericValue absOp(const Query &query);

        GenericValue logicalOp(const Query &query);

        GenericValue countOp(const Query &query);

        GenericValue aggregateOp(const Query &query);

        GenericValue minOp(const Query &query);

        GenericValue avgOp(const Query &query);

        GenericValue jumpOp(const Query &query);

        GenericValue beforeOp(const Query &query);

        GenericValue afterOp(const Query &query);

        GenericValue holdOp(const Query &query);

        GenericValue durationOp(const Query &query);

        GenericValue selectOp(const Query &query);

        static bool holdsBool(const GenericValue &value) {
            return std::holds_alternative<BoolType>(value);
        }

        static bool holdsNumeric(const GenericValue &value) {
            return std::holds_alternative<NumericType>(value);
        }

        static bool holdsBoolVector(const GenericValue &value) {
            return std::holds_alternative<BoolVectorType>(value);
        }

        static bool holdsNumericVector(const GenericValue &value) {
            return std::holds_alternative<NumericVectorType>(value);
        }

        static std::string getQueryType(const Query &query) {
            if (!query.IsObject() || !query.HasMember("type") || !query["type"].IsString()) {
                throw std::runtime_error("Could not find query type, invalid query format");
            }
            return query["type"].GetString();
        }

        static std::string getQueryOperation(const Query &query) {
            if (!query.IsObject() || !query.HasMember("operation") || !query["operation"].IsString()) {
                throw std::runtime_error("Could not find query operation, invalid query format");
            }
            return query["operation"].GetString();
        }

        static bool isSameType(const std::vector<GenericValue> &vector, ValueType type);

        static bool isSameLength(const std::vector<GenericValue> &vector);

    private:
        uint32_t num_threads_;
        std::vector<std::thread> threads;
        std::queue<std::function<void()> > tasks;
        std::mutex mutex;
        std::condition_variable cv;
        bool stop = false;

        std::unordered_map<std::string, std::function<GenericValue(const Query &)> > operationMap_;
    };
}

#endif //CPP_EXECUTOR_H
