# LeetCode

[![Tests](https://github.com/HE7086/LeetCode/actions/workflows/test.yml/badge.svg)](https://github.com/HE7086/LeetCode/actions/workflows/test.yml)

My solution to LeetCode algorithm problems in C++. Together with a question fetcher(template generator).

## Usage
* requirements
  - Compiler that supports c++23
  - Relatively new CMake
  - [nlohmann-json](https://github.com/nlohmann/json)
  - [cpr](https://github.com/libcpr/cpr)
  - [gtest](https://github.com/google/googletest)
  - [ninja](https://github.com/ninja-build/ninja)
  - optional: [just](https://github.com/casey/just)

* building
  - `just build` would compile all the initialized solutions in `Debug` mode.
  - `just build Release` same thing but in `Release` mode.
  - By default all solutions are built in `Debug` mode and the fetcher in `Release` mode.

* fetcher
  - `just fetch <id>` would initialize a code template at `src/solution/s<id>_<name>.cpp`. E.g. `just f 1` creates `src/solution/s0001_two_sum.cpp`.

* testing
  - `just test` would run tests for all initialized questions.
  - `just check` would run tests for the last modified solution(likely what you are working on).
  - `just check <id>` would run tests of the corresponding question. E.g. `just c 1` runs tests from `src/solution/s0001_two_sum.cpp`.
  - All solutions are also tested in GitHub actions.

* debugging
  - `just debug` would launch `gdb` with the last modified solution and set a breakpoint at the function.
  - `just debug <id>` same with `check`.
  - This script is not very robust. It relies on the structure of the code template. Writing code only between two comment sections (`//======`) and keeping all the helper functions above solution should allow the script to work.

## Note
* Only tested on Linux.
* Really wished to make things working with C++20 modules. However too many things broke and `clangd` didn't even support modules yet.
* No submission support.
