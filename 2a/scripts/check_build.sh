#!/usr/bin/env bash

set -e
set -u
set -x

test -d src/
test -d temp/

cd temp

mkdir -p build-tsan
pushd build-tsan

cmake -G "Unix Makefiles" ../.. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DUSE_SANITIZER=tsan -DCMAKE_CXX_FLAGS="-Wno-poison-system-directories -Wno-thread-safety -Wno-thread-safety-negative"
cmake --build . --config Debug --parallel

popd


mkdir -p build-asan
pushd build-asan

cmake -G "Unix Makefiles" ../.. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++-20 -DUSE_SANITIZER=asan
cmake --build . --config Debug --parallel

popd


mkdir -p build-release
pushd build-release

cmake -G "Unix Makefiles" ../.. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++-20
cmake --build . --config Release --parallel

popd

mkdir -p build-debug
pushd build-debug

cmake -G "Unix Makefiles" ../.. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++-20
cmake --build . --config Debug --parallel

popd

