#!/bin/bash

if [[ $# -eq 0 ]] || [[ $# -eq 1 ]]
then
    echo "./test.sh short|short-memory|long|long-memory n_repetitions"
    echo -e "\tshort: run tests that are fast without check of memory leaks for n_repetitions times"
    echo -e "\tshort-memory: run tests that are fast with check of memory leaks for n_repetitions times"
    echo -e "\tlong: run tests that are slow without check of memory leaks for n_repetitions times"
    echo -e "\tlong-memory: run tests that are slow with check of memory leaks for n_repetitions times"
    exit 1
fi

if [[ "$1" = "parse" ]]
then
    cd ../../build
    make
    ctest --output-on-failure -L normal short-irreducible -LE long-irreducible > ctest_results.txt
    if ! grep -q "100% tests passed" ctest_results.txt
    then
        echo "Parse test failed"
        rm ctest_results.txt
        exit 1
    fi
    rm ctest_results.txt
    echo "Parse test succeed"
fi

if [[ "$1" = "short" ]]
then
    cd tests/fp_poly/
    touch arg_pari_gp.gp
    echo -n short_test=1 > arg_pari_gp.gp
    gp -q arg_pari_gp.gp generate_input.gp
    rm arg_pari_gp.gp
    cd ../../tests/fp_integer/
    gp -q generate_input_integer.gp
    cd ../../build
    make
    for ((i=1; i<=$2; i++))
    do
        echo "Round $i of test is running..."
        ctest --output-on-failure -L "nominal|short-polynom|short-integer" -LE "memory|long-integer|long-polynom" > ctest_results.txt
        cat ctest_results.txt
        if ! grep -q "100% tests passed" ctest_results.txt
        then
            echo "Round $i of tests failed"
            rm ctest_results.txt
            exit 1
        fi
        rm ctest_results.txt
        echo "Round $i of tests succeed"
    done
    echo "All round tests succeed!"
fi

if [[ "$1" = "short-memory" ]]
then
    cd tests/fp_poly/
    touch arg_pari_gp.gp
    echo -n short_test=1 > arg_pari_gp.gp
    gp -q arg_pari_gp.gp generate_input.gp
    rm arg_pari_gp.gp
    cd ../../tests/fp_integer/
    gp -q generate_input_integer.gp
    cd ../../build
    make
    for ((i=1; i<=$2; i++))
    do
        echo "Round $i of test is running..."
        ctest --output-on-failure -L "memory|short-polynom|short-integer" -LE "nominal|long-integer|long-polynom" > ctest_results.txt
        cat ctest_results.txt
        if ! grep -q "100% tests passed" ctest_results.txt
        then
            echo "Round $i of tests failed"
            rm ctest_results.txt
            exit 1
        fi
        rm ctest_results.txt
        echo "Round $i of tests succeed"
    done
fi

if [[ "$1" = "long" ]]
then
    cd tests/fp_poly/
    touch arg_pari_gp.gp
    echo -n short_test=0 > arg_pari_gp.gp
    gp -q arg_pari_gp.gp generate_input.gp
    rm arg_pari_gp.gp
    cd ../../tests/fp_integer/
    gp -q generate_input_integer.gp
    cd ../../build
    make
    for ((i=1; i<=$2; i++))
    do
        echo "Round $i of test is running..."
        ctest --output-on-failure -L "nominal|long-polynom|long-integer" -LE "memory|short-integer|short-polynom" > ctest_results.txt
        cat ctest_results.txt
        if ! grep -q "100% tests passed" ctest_results.txt
        then
            echo "Round $i of tests failed"
            rm ctest_results.txt
            exit 1
        fi
        rm ctest_results.txt
        echo "Round $i of tests succeed"
    done
fi

if [[ "$1" = "long-memory" ]]
then
    cd tests/fp_poly/
    touch arg_pari_gp.gp
    echo -n short_test=0 > arg_pari_gp.gp
    gp -q arg_pari_gp.gp generate_input.gp
    rm arg_pari_gp.gp
    cd ../../tests/fp_integer/
    gp -q generate_input_integer.gp
    cd ../../build
    make
    for ((i=1; i<=$2; i++))
    do
        echo "Round $i of test is running..."
        ctest --output-on-failure -L "memory|long-polynom|long-integer" -LE "nominal|short-integer|short-polynom" > ctest_results.txt
        cat ctest_results.txt
        if ! grep -q "100% tests passed" ctest_results.txt
        then
            echo "Round $i of tests failed"
            rm ctest_results.txt
            exit 1
        fi
        rm ctest_results.txt
        echo "Round $i of tests succeed"
    done
fi