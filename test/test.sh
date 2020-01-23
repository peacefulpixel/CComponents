#!/bin/bash

ESC_ENABLED=1
FAILS=0
SUCCS=0

function invalid_usage
{
    printf "Usage: ./test.sh path/to/binaries [--noesc]"
    exit 1
}

function invalid_dir
{
    printf "Directory $1 does not exists"
    exit 1
}

function print_result
{
    result_type=$1
    info=
    succ=
    fail=
    end=

    if [ $ESC_ENABLED = 1 ]
    then
        info="\033[1;37m"
        succ="\033[1;32m"
        fail="\033[1;31m"
        end="\033[0m"
    fi

    case "$result_type" in
        "INFO")
            printf "$info[  INFO  ]$end"
        ;;
        "SUCCESS")
            printf "$succ[SUCCESS!]$end"
        ;;
        "FAIL")
            printf "$fail[ FAILED ]$end"
        ;;
    esac
}

function print
{
    result_type=$1
    message=$2

    print_result $result_type
    
    printf " $message\n"
}

function test_one
{
    path=$1

    ./$path
    result=$?

    if [ $result = 0 ]
    then
        print SUCCESS "Test $path was successfully completed"
        ((SUCCS=SUCCS+1))
    else
        print FAIL "Test $path was failed"
        ((FAILS=FAILS+1))
    fi  
}

function test_all
{
    tests_path=$1

    IFS=$'\n'; set -f
    for test_file in `find $tests_path -name *.o`
    do
        test_one "$test_file"
    done
    unset IFS; set +f

    print INFO "$((FAILS+SUCCS)) tests was processed. Fails: $FAILS"
}

[[ "" = "$1" ]] && invalid_usage
[[ ! -d "$1" ]] && invalid_dir $1

for arg in "$@"
do
    case "$arg" in
        "--noesc")
            ESC_ENABLED=0
        ;;
    esac
done

test_all $1