# Twine

Implementation of a class template for fast concatenation of strings.

## How to clone the repository

```bash
git clone https://github.com/KetchuppOfficial/StringTwine.git
```

## How to build with conan package manager

### 0) Requirements

The following applications and libraries have to be installed:

- CMake of version 3.20 (or higher)
- python3
- python3-venv

### 1) Install conan and other python modules this project depends on

```bash
python3 -m venv .venv
.venv/bin/pip3 install -r requirements.txt
```

### 2) Run conan

If you do not have conan profile, you may create a default one with the command

```bash
.venv/bin/conan profile detect
```

or specify your own profile by adding option `--profile:build=<your_profile>` to the following
command that installs all libraries the project depends on.

```bash
.venv/bin/conan install . --output-folder=third_party --build=missing -s compiler.cppstd=23
```

### 2) Build the project with cmake

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=./third_party/conan_toolchain.cmake
cmake --build build
```

## List of cmake options defined by the project

| Option          | Values         | Explanation                           | Default |
|-----------------|----------------|---------------------------------------|---------|
| BUILD_TESTS     | **ON**/**OFF** | Set to **ON** to build unit tests     | **OFF** |
| BUILD_BENCHMARK | **ON**/**OFF** | Set to **ON** to build the benchmark  | **OFF** |

## Benchmark

`cool::Twine` was compared against `std::string` in concatenation of a long sequence of strings.
*Google Benchmark* library was used for that purpose. Use can see the source code for the benchmark
[here](/benchmark/src/twine.cpp). The results are as follows:

```text
2025-09-22T10:40:08+03:00
Running build/benchmark/twine-benchmark
Run on (12 X 3000 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 512 KiB (x6)
  L3 Unified 4096 KiB (x2)
Load Average: 0.80, 0.60, 0.54
-------------------------------------------------------
Benchmark             Time             CPU   Iterations
-------------------------------------------------------
cool_bench_1        581 ns          581 ns      1208561
cool_bench_2        579 ns          579 ns      1207771
cool_bench_4        310 ns          310 ns      2246739
std_bench_1         409 ns          409 ns      1718082
std_bench_2         537 ns          537 ns      1311375
std_bench_4         794 ns          794 ns       888537
```

It can be observed that putting parenthesis at the right places when using `cool::Twine` is likely
to improve the performance comparing to using `std::string`.
