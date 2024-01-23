#include "executor.h"
#include <rapidjson/document.h>
#include <iostream>
#include <stdexcept>

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
            "value": 10
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
    executor.run(doc);
    std::cout << "123" << std::endl;
}