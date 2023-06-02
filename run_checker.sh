#!/bin/bash
dataset="dblp-2011"
format="txt"

compress_file=../$format/"$dataset"\_compressed."$format"
biclique_file=../$format/"$dataset"\_bicliques."$format"

original_file=../$format/"$dataset"."$format"
final_file=../$format/"$dataset"\_compressed_rebuild."$format"

./checker    \
    --final $compress_file  \
    --biclique $biclique_file

if diff "$original_file" "$final_file" &> /dev/null ; then
    echo "Files are the same"
else
    echo "Files differ"
fi