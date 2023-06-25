#!/bin/bash

# Repeat the tests
cd build
for ((i=1; i<=10; i++))
do
    echo "Test $i"
    ctest -L "short" > ctest_results.txt
    if ! grep -q "100% tests passed" ctest_results.txt
    then
        cat ctest_results.txt
        exit 1
    fi
done
rm ctest_results.txt
echo "All tests passed"