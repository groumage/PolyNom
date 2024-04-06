#!/bin/bash

print_usage() {
    echo "usage: ./test.sh arg [-n_repetitions]"
    echo -e "\targ             \t: the execution mode of the script"
    echo -e "\t\tshort         \t: execute fast tests without memory leaks checks"
    echo -e "\t\tshort-memory  \t: execute fast tests with memory leaks checks"
    echo -e "\t\tlong          \t: execute slow tests without memory leaks checks"
    echo -e "\t\tlong-memory   \t: execute fast tests with memory leaks checks"
    echo -e "\t\tother         \t: execute all test which are not related to polynom manipulation"
    echo -e "\t\tls            \t: list the unit test performed depending on the label used"
    echo -e "\tn_repetitions   \t: the number of repetitions of the tests (default is 1)"
}

if [[ $# -eq 0 ]] || [[ $1 = "help" ]]
then
    print_usage
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
    if ! [[ $2 =~ $re ]] && ! [[ "$2" = "name" ]]
    then
        echo "error: n_repetitions is not a number" >&2
        exit 0
    fi
    n_repetitions=$2
fi

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

mkdir -p tests/fp_poly/input_test
mkdir -p tests/fp_integer/input_test

# select the tests (with the use of labels) depending on the arg value
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

# list the tests performed depending on the label used
if [[ $2 = "name" ]]
then
    cd build
    echo "Test label: $1"
    ctest_cmd=$ctest_cmd" -N"
    eval $ctest_cmd | tail -n +2 | head -n -1
    cd ../
    exit 1
fi

perform_tests() {
    cd build
    eval $1 > ctest_results.txt
    cat ctest_results.txt
    if ! grep -q "100% tests passed" ctest_results.txt
    then
        rm ctest_results.txt
        return 0
    fi
    rm ctest_results.txt
    cd ../
    return 1
}

if [[ "$1" = "other" ]]
then
    for ((i=1; i<=$n_repetitions; i++))
    do
        echo "Round $i of test is running..."
        perform_tests "ctest --output-on-failure -LE \"nominal\""
        if [[ $? -eq 0 ]]
        then
            echo -e "${RED}Tests failed${NC}"
            exit 1
        fi
        echo -e "${GREEN}Round $i of tests succeed${NC}"
    done
    echo -e "${GREEN}All round tests succeed!${NC}"
fi

generate_input_test() {
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
}

if [[ "$1" -eq "short" || "$1" -eq "short-memory" || "$1" -eq "long" || "$1" -eq "long-memory" ]]
then
    for ((i=1; i<=$n_repetitions; i++))
    do
        echo "Round $i of test is running..."
        generate_input_test
        perform_tests "$ctest_cmd"
        if [[ $? -eq 0 ]]
        then
            echo -e "${RED}Tests failed${NC}"
            exit 1
        fi
        echo -e "${GREEN}Round $i of tests succeed${NC}"
    done
    echo -e "${GREEN}All round tests succeed!${NC}"
fi