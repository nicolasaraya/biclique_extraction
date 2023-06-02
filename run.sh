 #!/bin/bash
./biclique_extractor    \
    --file ../bin32/dblp-2011.bin \
    --numSignatures 2   \
    --minClusterSize 4 \
    --bicliqueSize 5000    \
    --minAdyNodes 2   \
    --shingleSize 1           \
    --bsDecrease 1000    \
    --threshold 500     \
    --selfLoop 1      \
    --debug 0       \
    --iterations 10
