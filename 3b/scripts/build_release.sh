#!/usr/bin/env bash

set -e
set -u
set -x

test -d src/
test -d temp/

cd temp

mkdir -p build-release
pushd build-release

cmake -G "Unix Makefiles" ../.. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++-20
cmake --build . --config Release --parallel

popd

