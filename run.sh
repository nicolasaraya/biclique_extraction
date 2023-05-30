 #!/bin/bash
./biclique_extractor    \
    --file ../data/dblp-2011.txt \
    --numSignatures 2   \
    --minClusterSize 10 \
    --bicliqueSize 5000    \
    --minAdyNodes 2   \
    --shingleSize 1           \
    --bsDecrease 1000    \
    --threshold 500     \
    --selfLoop 1 
