#ifndef CPP_EXECUTOR_H
#define CPP_EXECUTOR_H

#include "operator.h"
#include <rapidjson/document.h>
#include <variant>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace ComputeLib {
    template<typename T>
    using CompareFunctionTemplate = std::function<bool(const T &, const T &)>;

    using CompareFunction = CompareFunctionTemplate<NumericType>;

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

    using LogicalFunction = std::function<BoolType(const BoolType &, const BoolType &)>;

    template<typename T>
    BoolType logicalAnd(const T &left, const T &right) {
        return left & right;
    }

    template<typename T>
    BoolType logicalOr(const T &left, const T &right) {
        return left | right;
    }


    class Executor {
    public:
        explicit Executor(uint32_t num_threads);

        ~Executor() = default;

        GenericValue run(const Query &query);

        GenericValue compareOp(const Query &query);

        GenericValue addOp(const Query &query);

        GenericValue subOp(const Query &query);

        GenericValue mulOp(const Query &query);

        GenericValue divOp(const Query &query);

        GenericValue powOp(const Query &query);

        GenericValue absOp(const Query &query);

        GenericValue logicalOp(const Query &query);

        GenericValue countOp(const Query &query);

        GenericValue maxOp(const Query &query);

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
            if (!query.IsObject() || !query["type"].IsString()) {
                throw std::runtime_error("Could not find query type, invalid query format");
            }
            return query["type"].GetString();
        }

        static std::string getQueryOperation(const Query &query) {
            if (!query.IsObject() | !query["operation"].IsString()) {
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
