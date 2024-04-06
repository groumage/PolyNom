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
rm -rf coverage
mkdir coverage
# check gcovr is installed
if ! command -v gcovr &> /dev/null
then
    echo "gcovr could not be found"
    exit
fi
gcovr -r . --exclude tests/ -j 8 --html --html-details -o coverage/coverage.html
firefox coverage/coverage.html