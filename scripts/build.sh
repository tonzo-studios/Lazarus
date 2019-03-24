#!/bin/bash

# Build script intended for use in Travis CI

set -ex

# Build and execute tess
build_type=Debug
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE="$build_type" ..
make -j3
cd tests
[ -f lazarus_tests ] && sh -c ./lazarus_tests

# Build documentation
cd ../..
doxygen Doxyfile