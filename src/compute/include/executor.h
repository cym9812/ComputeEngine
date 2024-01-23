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
    class Executor {
    public:
        explicit Executor(uint32_t num_threads);

        ~Executor() = default;

        GenericValue run(const Query &plan);

    private:
        uint32_t num_threads_;
        std::vector<std::thread> threads;
        std::queue<std::function<void()>> tasks;
        std::mutex mutex;
        std::condition_variable cv;
        bool stop = false;

        std::unordered_map<std::string, std::function<GenericValue(const Query &)>> operationMap_;

        GenericValue eqOp(const Query &query);

        GenericValue neOp(const Query &query);

        GenericValue ltOp(const Query &query);

        GenericValue leOp(const Query &query);

        GenericValue gtOp(const Query &query);

        GenericValue geOp(const Query &query);

        GenericValue addOp(const Query &query);

        GenericValue subOp(const Query &query);

        GenericValue mulOp(const Query &query);

        GenericValue divOp(const Query &query);

        GenericValue powOp(const Query &query);

        GenericValue absOp(const Query &query);

        GenericValue andOp(const Query &query);

        GenericValue orOp(const Query &query);

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
    };
}

#endif //CPP_EXECUTOR_H
