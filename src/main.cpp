#include "executor.h"
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
            "operation": "SELECT",
            "value": "a"
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
      auto res = executor.run(doc);
      std::cout << std::get<ComputeLib::NumericType>(res) << std::endl;

    }catch (const std::exception &e) {
      std::cout << e.what();
    }
}
