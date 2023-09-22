#!/bin/bash

./biclique_extractor \
    --numSignatures 3 \
    --minClusterSize 10 \
    --bicliqueSize 1000 \
    --minAdyNodes 2 \
    --shingleSize 1 \
    --bsDecrease 500 \
    --threshold 100 \
    --selfLoop 0 \
    --debug 0 \
    --iterations 100 \
    --file ../txt/cnr-2000-hc/cnr-2000-hc.txt