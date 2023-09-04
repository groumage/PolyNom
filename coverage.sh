#!/bin/bash

rm -rf build
mkdir build
cd build/
cmake ..
make
cd ../
array=(list parse init assert print static short)
for i in "${array[@]}"
do
    ./test.sh $i
done
cd build/
make coverage