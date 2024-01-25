#include "executor.h"
#include "data_frame.h"
#include <rapidjson/document.h>
#include <iostream>
#include <stdexcept>
#include <variant>

std::string test_data = R"({
  "type": "operation",
  "operation": "COUNT",
  "unit": 0.1,
  "initialValue": 0,
  "value": {
    "type": "operation",
    "operation": "DURATION",
    "minDuration": 0.1,
    "value": {
      "type": "operation",
      "operation": "AND",
      "operands": [
        {
          "type": "operation",
          "operation": "GT",
          "left": {
            "type": "operation",
            "operation": "ADD",
            "left": {
              "type": "operation",
              "operation": "SELECT",
              "value": "b"
            },
            "right": {
              "type": "operation",
              "operation": "SELECT",
              "value": "b"
            }
          },
          "right": {
            "type": "value",
            "value": 10
          }
        },
        {
          "type": "operation",
          "operation": "LT",
          "left": {
            "type": "operation",
            "operation": "SELECT",
            "value": "b"
          },
          "right": {
            "type": "value",
            "value": 20
          }
        }
      ]
    }
  }
})";


int main() {
    rapidjson::Document doc;
    doc.Parse(test_data.c_str());
    if (doc.HasParseError()) {
        throw std::runtime_error("Invalid json");
    }
    ComputeLib::Executor executor(1);
    try {
        auto start = std::chrono::high_resolution_clock::now();
        auto res = executor.run(doc);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "Time taken by code: " << duration.count() << " microseconds" << std::endl;
        std::cout << std::get<ComputeLib::NumericType>(res) << std::endl;
    } catch (const std::exception &e) {
        std::cout << e.what();
    }
}
