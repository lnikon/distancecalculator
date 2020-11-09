#!/bin bash

echo "Environment: `uname -a`"
echo "Compiler: `$CXX --verions`"

rm -rf build || exit 1
mkdir build || exit 1
cd build || exit 1
cmake ..  || exit 1
make -j8 || exit 1
