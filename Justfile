alias b := build
alias t := test
alias c := check
alias f := fetch

build MODE="Debug":
  cmake -B build -G "Ninja Multi-Config"
  cmake --build build --config {{MODE}}

test *ARGS="": build
  GTEST_COLOR=1 ctest --test-dir build/src --output-on-failure {{ARGS}}

check ARG="": build
  #!/bin/bash
  if [ -z "{{ARG}}" ]; then
    TARGET=$(ls -t1 src/solution | head -n 1 | grep -Po '(?<=^s)([0-9]+)')
  else
    TARGET="{{ARG}}"
  fi
  GTEST_COLOR=1 ctest --test-dir build/src --output-on-failure -R $(printf "Test.s%04d" "$TARGET")

build-fetcher:
  cmake -B build -G "Ninja Multi-Config"
  cmake --build build --config "Release" --target fetcher

fetch ARG="": build-fetcher
  build/src/Release/fetcher {{ARG}}

clean:
  rm -rf .cache build
