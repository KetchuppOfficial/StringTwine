#include <cool/twine.hpp>

#include "benchmark/benchmark.h"

static constexpr const char *kStr = "lorem ipsum dolor sit amet, consectetur adipiscing";

static void cool_bench_1(benchmark::State &state) {
    for (auto _ : state) {
        auto str = (cool::Twine{kStr} + kStr + kStr + kStr + kStr + kStr + kStr + kStr + kStr +
                    kStr + kStr + kStr + kStr + kStr + kStr + kStr + kStr + kStr + kStr + kStr +
                    kStr + kStr + kStr + kStr)
                       .str();
        benchmark::DoNotOptimize(str);
    }
}

static void cool_bench_2(benchmark::State &state) {
    for (auto _ : state) {
        auto str = ((cool::Twine{kStr} + kStr + kStr + kStr + kStr + kStr + kStr + kStr + kStr +
                     kStr + kStr + kStr) +
                    (cool::Twine{kStr} + kStr + kStr + kStr + kStr + kStr + kStr + kStr + kStr +
                     kStr + kStr + kStr))
                       .str();
        benchmark::DoNotOptimize(str);
    }
}

static void cool_bench_4(benchmark::State &state) {
    for (auto _ : state) {
        auto str = ((cool::Twine{kStr} + kStr + kStr + kStr + kStr + kStr) +
                    (cool::Twine{kStr} + kStr + kStr + kStr + kStr + kStr)) +
                   ((cool::Twine{kStr} + kStr + kStr + kStr + kStr + kStr) +
                    (cool::Twine{kStr} + kStr + kStr + kStr + kStr + kStr))
                       .str();
        benchmark::DoNotOptimize(str);
    }
}

static void std_bench_1(benchmark::State &state) {
    for (auto _ : state) {
        auto str = std::string{kStr} + kStr + kStr + kStr + kStr + kStr + kStr + kStr + kStr +
                   kStr + kStr + kStr + kStr + kStr + kStr + kStr + kStr + kStr + kStr + kStr +
                   kStr + kStr + kStr + kStr;
        benchmark::DoNotOptimize(str);
    }
}

static void std_bench_2(benchmark::State &state) {
    for (auto _ : state) {
        auto str = ((std::string{kStr} + kStr + kStr + kStr + kStr + kStr + kStr + kStr + kStr +
                     kStr + kStr + kStr) +
                    (std::string{kStr} + kStr + kStr + kStr + kStr + kStr + kStr + kStr + kStr +
                     kStr + kStr + kStr));
        benchmark::DoNotOptimize(str);
    }
}

static void std_bench_4(benchmark::State &state) {
    for (auto _ : state) {
        auto str = ((std::string{kStr} + kStr + kStr + kStr + kStr + kStr) +
                    (std::string{kStr} + kStr + kStr + kStr + kStr + kStr)) +
                   ((std::string{kStr} + kStr + kStr + kStr + kStr + kStr) +
                    (std::string{kStr} + kStr + kStr + kStr + kStr + kStr));
        benchmark::DoNotOptimize(str);
    }
}

BENCHMARK(cool_bench_1);
BENCHMARK(cool_bench_2);
BENCHMARK(cool_bench_4);

BENCHMARK(std_bench_1);
BENCHMARK(std_bench_2);
BENCHMARK(std_bench_4);

BENCHMARK_MAIN();
