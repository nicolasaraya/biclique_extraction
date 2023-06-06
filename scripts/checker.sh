#!/bin/bash

nameFiles=(
#    "dblp-2011"
#    "arabic-2005-hc"
#    "eu-2015-hc"
#    "eu-2015-host"
    "indochina-2004"
#    "uk-2014-hc"
)

format="bin"
path="../../bin/"
rm -rf ../output/diff.log

for name in ${nameFiles[@]}
    do
        compress_file="$path${name}_compressed.txt"
        biclique_file="$path${name}_bicliques.txt"

        original_file="../../txt/$name.txt"
        final_file="$path${name}_compressed_rebuild.txt"

        exec="../checker --final $compress_file --biclique $biclique_file" 

        $exec

        echo $name | tee -a ../output/diff.log
        if diff "$original_file" "$final_file" &> /dev/null ; then
            echo "Files are the same" | tee -a ../output/diff.log
        else
            echo "Files differ"  | tee -a ../output/diff.log
fi


    done

