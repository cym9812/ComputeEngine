find_package(GTest REQUIRED)

enable_testing()

file(GLOB TEST_SOURCES test*.cpp)

add_executable(ut ${TEST_SOURCES})

target_link_libraries(ut
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ut)
