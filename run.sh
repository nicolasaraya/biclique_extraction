#!/bin/bash

./biclique_extractor \
    --numSignatures 2 \
    --minClusterSize 4 \
    --bicliqueSize 1000 \
    --minAdyNodes 2 \
    --shingleSize 1 \
    --bsDecrease 400 \
    --threshold 100 \
    --selfLoop 0 \
    --debug 0 \
    --iterations 100 \
    --file ../txt/cnr-2000-hc/cnr-2000-hc.txt