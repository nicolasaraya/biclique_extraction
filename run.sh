 #!/bin/bash
./biclique_extractor    \
    --file ../data/dblp-2011.txt \
    --numSignatures 2   \
    --minClusterSize 10 \
    --bicliqueSize 500    \
    --minAdyNodes 2   \
    --shingleSize 1           \
    --bsDecrease 400    \
    --threshold 100     \
    --selfLoop 1 