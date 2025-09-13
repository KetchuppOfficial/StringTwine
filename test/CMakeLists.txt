find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(twine-unit-tests src/some-test.cpp)
target_link_libraries(twine-unit-tests
PRIVATE
    cool::twine
    GTest::GTest
    GTest::gtest_main
    Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(twine-unit-tests)
