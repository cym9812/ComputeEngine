cmake_policy(SET CMP0135 NEW)
include(FetchContent)
FetchContent_Declare(gtest
        QUIET
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
# configure build of googletest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

enable_testing()

file(GLOB TEST_SOURCES test*.cpp)

add_executable(ut
        ${TEST_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/compute/executor.cpp
        ${CMAKE_SOURCE_DIR}/src/compute/operator.cpp
)

target_link_libraries(ut
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ut)
