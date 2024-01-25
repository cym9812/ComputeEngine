cmake_minimum_required(VERSION 3.22)
project(ComputeEngine CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CompilerWarnings.cmake)
include(cmake/StandardSettings.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/Utils.cmake)
include(cmake/Vcpkg.cmake)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.\n")
endif()

include_directories(include)
include_directories(src/compute/include)
include_directories(third_party/include)

add_executable(ComputeEngine
        src/main.cpp
        src/compute/operator.cpp
        src/compute/executor.cpp
)

set_project_warnings(${PROJECT_NAME})

if(${PROJECT_NAME}_ENABLE_UNIT_TESTING)
    add_subdirectory(test)
endif()