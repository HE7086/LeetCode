alias b := build
alias c := check
alias d := debug
alias f := fetch
alias t := test

build MODE="Debug":
  cmake -B build -G "Ninja Multi-Config"
  cmake --build build --config {{MODE}}

test *ARGS="": build
  GTEST_COLOR=1 ctest --test-dir build --output-on-failure {{ARGS}}

check ARG="": build
  #!/bin/bash
  if [ -z "{{ARG}}" ]; then
    TARGET=$(ls -t1 src/solution | head -n 1 | grep -Po '(?<=^s)([0-9]+)')
  else
    TARGET="{{ARG}}"
  fi
  GTEST_COLOR=1 ctest --test-dir build --output-on-failure -R $(printf "Test.s%04d" "$((10#$TARGET))")

build-fetcher:
  cmake -B build -G "Ninja Multi-Config"
  cmake --build build --config "Release" --target fetcher

fetch ARG="": build-fetcher
  build/src/Release/fetcher {{ARG}}

debug ARG="": build
  #!/bin/bash
  if [ -z "{{ARG}}" ]; then
    TARGET=$(ls -t1 src/solution | head -n 1 | grep -Po '(?<=^s)([0-9]+)')
  else
    TARGET="{{ARG}}"
  fi
  ID=$(printf "s%04d" "$((10#$TARGET))")
  FUNC=$(find build/src/Debug -name "$ID*" -type f -exec nm -C {} \; | grep -Po '(?<=W Solution::)[a-zA-Z_]+' | tail -n 1)
  echo "Breakpoint: $FUNC"
  find build/src/Debug -name "$ID*" -exec gdb {} -ex "b $FUNC" \;

clean:
  rm -rf .cache build
