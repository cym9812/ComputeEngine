find_package(GTest REQUIRED)

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
