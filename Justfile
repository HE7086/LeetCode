alias b := build
alias t := test
alias c := check

build:
  cmake -B build -G Ninja
  cmake --build build

test *ARGS="": build
  ctest --test-dir build --output-on-failure {{ARGS}}

check ARG="": build
  #!/bin/bash
  ID=$(printf "%04d" {{ARG}})
  ctest --test-dir build --output-on-failure -R "test_$ID"

clean:
  rm -rf .cache build
