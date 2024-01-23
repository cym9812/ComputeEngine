#include "executor.h"
#include "operator.h"
#include <rapidjson/document.h>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace ComputeLib;

#define TRUE 1
#define FALSE 0

#define REGISTER_OPERATION(opName, opFunc) \
    operationMap_[opName] = [this](const Query &query) { return opFunc(query); }

Executor::Executor(uint32_t num_threads) : num_threads_(num_threads) {
    REGISTER_OPERATION("EQ", eqOp);
    REGISTER_OPERATION("NE", neOp);
    REGISTER_OPERATION("LT", ltOp);
    REGISTER_OPERATION("LE", leOp);
    REGISTER_OPERATION("GT", gtOp);
    REGISTER_OPERATION("GE", geOp);
    REGISTER_OPERATION("ADD", addOp);
    REGISTER_OPERATION("SUB", subOp);
    REGISTER_OPERATION("MUL", mulOp);
    REGISTER_OPERATION("DIV", divOp);
    REGISTER_OPERATION("POW", powOp);
    REGISTER_OPERATION("ABS", absOp);
    REGISTER_OPERATION("AND", andOp);
    REGISTER_OPERATION("OR", orOp);
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

GenericValue Executor::run(const Query &query) {
    const std::string type = query["type"].GetString();

    if (type == std::string("operation")) {
        std::string operatorString = query["operation"].GetString();
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
    }
}

GenericValue Executor::eqOp(const Query &query) {

}

GenericValue Executor::neOp(const Query &query) {

}

GenericValue Executor::ltOp(const Query &query) {

}

GenericValue Executor::leOp(const Query &query) {

}

GenericValue Executor::gtOp(const Query &query) {
    GenericValue left = run(query["left"]);
    GenericValue right = run(query["right"]);

    if (holds_alternative<double>(left) && holds_alternative<double>(right)) {
        return get<double>(left) > get<double>(right);
    } else if (holds_alternative<std::vector<double>>(left) && holds_alternative<double>(right)) {
        auto &leftVector = get<std::vector<double>>(left);
        auto rightValue = get<double>(right);
        std::vector<uint8_t> result(leftVector.size(), FALSE);
        for (uint32_t i = 0; i < leftVector.size(); ++i) {
            result[i] = leftVector[i] > rightValue;
        }
        return result;
    } else if (holds_alternative<double>(left) && holds_alternative<std::vector<double>>(right)) {
        auto &leftVector = get<std::vector<double>>(left);
        auto &rightVector = get<std::vector<double>>(right);
        if (leftVector.size() != rightVector.size()) {
            throw std::runtime_error("Vector size mismatch");
        }
        std::vector<uint8_t> result(leftVector.size(), FALSE);
        for (uint32_t i = 0; i < leftVector.size(); ++i) {
            result[i] = leftVector[i] > rightVector[i];
        }
        return result;
    } else {
        throw std::runtime_error("Unsupported type for > operator");
    }
}

GenericValue Executor::geOp(const Query &query) {

}

GenericValue Executor::addOp(const Query &query) {

}

GenericValue Executor::subOp(const Query &query) {

}

GenericValue Executor::mulOp(const Query &query) {

}

GenericValue Executor::divOp(const Query &query) {

}

GenericValue Executor::powOp(const Query &query) {

}

GenericValue Executor::absOp(const Query &query) {

}

GenericValue Executor::andOp(const Query &query) {

}

GenericValue Executor::orOp(const Query &query) {

}

GenericValue Executor::countOp(const Query &query) {

}

GenericValue Executor::maxOp(const Query &query) {

}

GenericValue Executor::minOp(const Query &query) {

}

GenericValue Executor::avgOp(const Query &query) {

}

GenericValue Executor::jumpOp(const Query &query) {

}

GenericValue Executor::beforeOp(const Query &query) {

}

GenericValue Executor::afterOp(const Query &query) {

}

GenericValue Executor::holdOp(const Query &query) {

}

GenericValue Executor::durationOp(const Query &query) {

}

GenericValue Executor::selectOp(const Query &query) {
    const std::string name = query["value"].GetString();
    std::cout << "selectOp: " << name << std::endl;
    std::vector<double> data = {1, 2, 3, 4, 5, 6};
    return data;
}
