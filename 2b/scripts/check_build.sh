#!/usr/bin/env bash

set -e
set -u
set -x

test -d src/
test -d temp/

# Compiler for the normal builds. Override with: COMPILER=clang++-21 ./scripts/check_build.sh
COMPILER="${COMPILER:-clang++-20}"

if [ "$(uname)" = "Darwin" ]; then
  # macOS: Homebrew clang's TSan crashes at startup, so use Apple's clang for TSan.
  # Apple's clang enables extra warnings under -Weverything that the course code trips.
  TSAN_COMPILER=/usr/bin/clang++
  TSAN_FLAGS="-Wno-poison-system-directories -Wno-thread-safety -Wno-thread-safety-negative"
else
  TSAN_COMPILER="$COMPILER"
  TSAN_FLAGS=""
fi

cd temp

# build <dir> <build type> <compiler> <extra flags> <sanitizer>
build() {
  mkdir -p "$1"
  pushd "$1"
  cmake -G "Unix Makefiles" ../.. \
    -DCMAKE_BUILD_TYPE="$2" \
    -DCMAKE_CXX_COMPILER="$3" \
    -DCMAKE_CXX_FLAGS="$4" \
    -DUSE_SANITIZER="$5" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
  cmake --build . --config "$2" --parallel
  popd
}

build build-tsan    Debug   "$TSAN_COMPILER" "$TSAN_FLAGS" tsan
build build-asan    Debug   "$COMPILER"      ""            asan
build build-release Release "$COMPILER"      ""            ""
build build-debug   Debug   "$COMPILER"      ""            ""

# Let clangd find the compile flags
ln -sf temp/build-debug/compile_commands.json ../compile_commands.json