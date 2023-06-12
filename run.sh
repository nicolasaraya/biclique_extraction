 #!/bin/bash
./biclique_extractor    \
--file ../data/ej1.txt \
--numSignatures 2   \
--minClusterSize 1 \
--bicliqueSize 5    \
--minAdyNodes 2   \
--shingleSize 1           \
--bsDecrease 400    \
--threshold 1     \
--selfLoop 0      \
--debug 0       \
--iterations 10
