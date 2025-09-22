find_package(benchmark REQUIRED)

add_executable(twine-benchmark src/twine.cpp)
target_link_libraries(twine-benchmark
PRIVATE
    cool::twine
    benchmark::benchmark
)
