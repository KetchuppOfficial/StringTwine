# Twine

Implementation of a class for fast concatenation of strings

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

| Option      | Values         | Explanation                       | Default |
|-------------|----------------|-----------------------------------|---------|
| BUILD_TESTS | **ON**/**OFF** | Set to **ON** to build unit tests | **OFF** |
