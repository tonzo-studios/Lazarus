#!/bin/bash

# Build script intended for use in Travis CI

set -ex

# Build and execute tess
build_type=Debug
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE="$build_type" -DSFML_DIR=./SFML/install/lib/cmake/SFML \
    -DSFML_INCLUDE_DIR=./SFML/install/include ..
make -j3
cd tests
[ -f lazarus_test ] && sh -c ./lazarus_test

# Build documentation
cd ../..
doxygen Doxyfile