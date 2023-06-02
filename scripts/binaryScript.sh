#!/bin/bash

nameFiles=(
    "dblp-2011"
    "arabic-2005-hc"
#    "eu-2015-hc"
    "eu-2015-host"
#    "indochina-2004"
#    "uk-2014-hc"
)
dir="../../txt/"

cd ../binary_conv 
g++ *.cpp


for name in ${nameFiles[@]}
    do
        file="${name}.txt"
        echo $file
        exec="./a.out 1 $dir$file"
        $exec
    done

mv -f ../../txt/*.bin ../../bin
