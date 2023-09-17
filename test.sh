#!/bin/bash

if [[ $# -eq 0 ]]
then
    echo "./test.sh short|short-memory|long|long-memory n_repetitions"
    echo -e "\tshort: run tests that are fast without check of memory leaks for n_repetitions times"
    echo -e "\tshort-memory: run tests that are fast with check of memory leaks for n_repetitions times"
    echo -e "\tlong: run tests that are slow without check of memory leaks for n_repetitions times"
    echo -e "\tlong-memory: run tests that are slow with check of memory leaks for n_repetitions times"
    echo -e "\tn_repetitions=1 by default"
    exit 1
fi

if [[ "$1" = "ls" ]]
then
    array=(main main-memory list list-memory parse parse-memory init init-memory assert assert-memory print print-memory static static-memory short short-memory long long-memory)
    for i in "${array[@]}"
    do
        ./$0 $i name
    done
    exit 1
fi

n_repetitions=1
if [[ $# -eq 2 ]]
then
    re='^[0-9]+$'
    if ! [[ $2 =~ $re ]] && ! [[ "$2" = "name" ]] && ! [[ "$2" = "name-all" ]]
    then
        echo "error: n_repetitions is not a number" >&2
        exit 0
    fi
    n_repetitions=$2
fi

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

mkdir tests/fp_poly/input_test
mkdir tests/fp_integer/input_test

ctest_cmd=""
if [[ "$1" = "main" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"main\" -LE \"memory\""
elif [[ "$1" = "main-memory" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"main\" -L \"memory\""
elif [[ "$1" = "list" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"list\" -LE \"memory|mock\""
elif [[ "$1" = "list-memory" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"list\" -L \"memory|mock\""
elif [[ "$1" = "parse" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"parse\" -LE \"memory\""
elif [[ "$1" = "parse-memory" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"parse\" -L \"memory\""
elif [[ "$1" = "init" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"init\" -LE \"memory\""
elif [[ "$1" = "init-memory" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"init\" -L \"memory\""
elif [[ "$1" = "assert" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"assert\" -LE \"memory\""
elif [[ "$1" = "assert-memory" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"assert\" -L \"memory\""
elif [[ "$1" = "print" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"print\" -LE \"memory\""
elif [[ "$1" = "print-memory" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"print\" -L \"memory\""
elif [[ "$1" = "static" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"static\" -LE \"memory\""
elif [[ "$1" = "static-memory" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"static\" -L \"memory\""
elif [[ "$1" = "short" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"nominal\" -LE \"memory|long-integer|long-irreducible\""
elif [[ "$1" = "short-memory" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"nominal\" -L \"memory\" -LE \"long-integer|long-irreducible\""
elif [[ "$1" = "long" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"nominal\" -LE \"memory|short-integer|short-irreducible\""
elif [[ "$1" = "long-memory" ]]
then
    ctest_cmd="ctest --output-on-failure -L \"nominal\" -L \"memory\" -LE \"short-integer|short-irreducible\""
fi

if [[ $2 = "name" ]]
then
    cd build
    echo "Test label: $1"
    ctest_cmd=$ctest_cmd" -N"
    # remove last line
    eval $ctest_cmd | head -n -1
    cd ../
    exit 1
fi

cd build
make
cd ../
for ((i=1; i<=$n_repetitions; i++))
do
    echo "Round $i of test is running..."
    if [[ "$1" -eq "short" || "$1" -eq "short-memory" || "$1" -eq "long" || "$1" -eq "long-memory" ]]
    then
        cd tests/fp_poly/
        touch arg_pari_gp.gp
        if [[ "$1" -eq "short" || "$1" -eq "short-memory" ]]
        then
            echo -n short_test=1 > arg_pari_gp.gp
        else
            echo -n short_test=0 > arg_pari_gp.gp
        fi
        gp -q arg_pari_gp.gp generate_input.gp
        rm arg_pari_gp.gp
        cd ../../tests/fp_integer/
        gp -q generate_input_integer.gp
        cd ../../
    fi
    cd build
    eval $ctest_cmd > ctest_results.txt
    cat ctest_results.txt
    if ! grep -q "100% tests passed" ctest_results.txt
    then
        echo -e "${RED}Round $i of tests failed${NC}"
        rm ctest_results.txt
        exit 1
    fi
    rm ctest_results.txt
    echo -e "${GREEN}Round $i of tests succeed${NC}"
    cd ../
done
echo -e "${GREEN}All round tests succeed!${NC}"

<<COMMENT1

if [[ "$1" = "parse-memory" ]]
then
    cd build
    make
    ctest --output-on-failure -L "parse" -L "memory" > ctest_results.txt
    cat ctest_results.txt
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
    cd build
    make
    cd ../
    for ((i=1; i<=$2; i++))
    do
        echo "Round $i of test is running..."
        cd tests/fp_poly/
        touch arg_pari_gp.gp
        echo -n short_test=1 > arg_pari_gp.gp
        gp -q arg_pari_gp.gp generate_input.gp
        rm arg_pari_gp.gp
        cd ../../tests/fp_integer/
        gp -q generate_input_integer.gp
        cd ../../build
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
        cd ../
    done
    echo "All round tests succeed!"
fi

if [[ "$1" = "short-memory" ]]
then
    cd build
    make
    cd ../
    for ((i=1; i<=$2; i++))
    do
        echo "Round $i of test is running..."
        cd tests/fp_poly/
        touch arg_pari_gp.gp
        echo -n short_test=1 > arg_pari_gp.gp
        gp -q arg_pari_gp.gp generate_input.gp
        rm arg_pari_gp.gp
        cd ../../tests/fp_integer/
        gp -q generate_input_integer.gp
        cd ../../build
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
        cd ../
    done
fi

if [[ "$1" = "long" ]]
then
    cd build
    make
    cd ../
    for ((i=1; i<=$2; i++))
    do
        echo "Round $i of test is running..."
        cd tests/fp_poly/
        touch arg_pari_gp.gp
        echo -n short_test=0 > arg_pari_gp.gp
        gp -q arg_pari_gp.gp generate_input.gp
        rm arg_pari_gp.gp
        cd ../../tests/fp_integer/
        gp -q generate_input_integer.gp
        cd ../../build
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
        cd ../
    done
fi

if [[ "$1" = "long-memory" ]]
then
    cd build
    make
    cd ../!
    for ((i=1; i<=$2; i++))
    do
        echo "Round $i of test is running..."
        cd tests/fp_poly/
        touch arg_pari_gp.gp
        echo -n short_test=0 > arg_pari_gp.gp
        gp -q arg_pari_gp.gp generate_input.gp
        rm arg_pari_gp.gp
        cd ../../tests/fp_integer/
        gp -q generate_input_integer.gp
        cd ../../build
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
        cd ../
    done
fi
COMMENT1