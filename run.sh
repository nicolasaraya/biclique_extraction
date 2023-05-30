 #!/bin/bash
./biclique_extractor    \
    --file ../eu-2015-host.txt \
    --numSignatures 2   \
    --minClusterSize 10 \
    --bicliqueSize 5000    \
    --minAdyNodes 2   \
    --shingleSize 1           \
    --bsDecrease 1000    \
    --threshold 500     \
<<<<<<< HEAD
    --selfLoop 1   \
    --debug 1
=======
    --selfLoop 1 
>>>>>>> 97abf47f72107c57e5dd0bdd0fab97cdec7f884d
