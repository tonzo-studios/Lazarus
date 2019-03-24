#!/bin/bash

# Intended for Travis CI
# Compile and install SFML from its git repo

if [ "$TRAVIS_OS_NAME" == "linux" ]; then export CXX="g++"; fi
git clone https://github.com/SFML/SFML.git
cd SFML
mkdir install
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_INSTALL_FRAMEWORK_PREFIX=../install \
  -DSFML_BUILD_FRAMEWORKS=FALSE -DBUILD_SHARED_LIBS=TRUE ..
make
make install
cd ../..