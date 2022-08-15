#!/bin/bash

if [ ! $# -eq 2 ] 
then 
    echo "Missing file, usage: 'filter.sh INPUT_FILENAME OUTPUT_FILENAME'"
    exit 0
fi

if [ -e $1 ]
then
    grep -E "^[a-z]{3,15}$" $1 | grep -v -E "([a-z])\1{2}" | sort -u > $2
else
    echo "Input file does not exist, usage: 'filter.sh INPUT_FILENAME OUTPUT_FILENAME'"
fi