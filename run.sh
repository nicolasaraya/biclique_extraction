 #!/bin/bash
./biclique_extractor    \
--file ../wgraphs/bin/arabic-2005-hc.bin \
--numSignatures 2   \
--minClusterSize 10 \
--bicliqueSize 500    \
--minAdyNodes 2   \
--shingleSize 1           \
--bsDecrease 400    \
--threshold 100     \
--selfLoop 1      \
--debug 0       \
--iterations 10
