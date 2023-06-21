#!/bin/bash
./biclique_extractor \
    --numSignatures 2 \
    --minClusterSize 10 \
    --bicliqueSize 500 \
    --minAdyNodes 2 \
    --shingleSize 1 \
    --bsDecrease 100 \
    --threshold 1 \
    --selfLoop 0 \
    --debug 0 \
    --iterations 10 \ 
    --file ../data/dblp-2011.txt
