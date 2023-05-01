 #!/bin/bash

./biclique_extractor    \
    --file ../data/dblp-2011.txt \
    --numSignatures 2   \
    --minClusterSize 10 \
    --bicliqueSize 2    \
    --minAdyNodes 500   \
    --hola 12           \
    --bsDecrease 400    \
    --threshold 150     \
    --selfLoop 1 