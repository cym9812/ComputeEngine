#ifndef DATA_FRAME_H
#define DATA_FRAME_H

#include <string>
#include <vector>
#include <variant>
#include <stdexcept>
#include <iostream>
#include <unordered_map>

using DataColumn = std::variant<std::vector<uint8_t>, std::vector<int32_t>, std::vector<uint32_t>, std::vector<double> >
;


class DataFrame {
public:
    DataFrame(const int64_t startTimestamp, const int64_t endTimestamp, const int64_t interval) {
        for (int64_t i = startTimestamp; i < endTimestamp; i += interval) {
            timestamps.emplace_back(i);
        }
    }

    std::size_t getRowCount() const {
        return timestamps.size();
    }

    void addColumn(const std::string &name, const DataColumn &column) {
        if (nameMap.contains(name)) {
            throw std::runtime_error("Column already exists");
        }
        if (std::holds_alternative<std::vector<uint8_t> >(column)) {
            auto &vec = std::get<std::vector<uint8_t> >(column);
            if (vec.size() != getRowCount()) {
                throw std::runtime_error("Column size does not match timestamps size");
            }
            columns.emplace_back(vec);
        }
    };

    [[nodiscard]] const DataColumn &getColumn(const std::string &name) const {
        if (!nameMap.contains(name)) {
            throw std::runtime_error("Column does not exist: " + name);
        }

        auto index = nameMap.at(name);
        return columns[index];
    }

    int64_t getSamplingIntervalNs() const {
        return 1e8; // 100ms
    }

private:
    std::unordered_map<std::string, uint32_t> nameMap{};
    std::vector<std::string> columnNames{};
    std::vector<int64_t> timestamps{};
    std::vector<DataColumn> columns{};
};


#endif //DATA_FRAME_H
