#!/bin/bash

if [ ! $# -eq 1 ] 
then 
    echo "Missing file, usage: 'filter.sh FILENAME'"
    exit 0
fi

if [ -e $1 ]
then
    cat $1 | grep -o -w '\w\{3,15\}' | sort -u > output.txt
else
    echo "File does not exist, usage 'filter.sh FILENAME'"
fi