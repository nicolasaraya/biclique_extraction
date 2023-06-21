nameFiles=(
    "dblp-2011"
#    "arabic-2005-hc"
#    "eu-2015-hc"
#    "eu-2015-host"
#    "indochina-2004"
#    "uk-2014-hc"
)

ext=".bin"

fileDir="../bin/"

params="
    --numSignatures 2
    --minClusterSize 20
    --bicliqueSize 5000
    --minAdyNodes 2
    --shingleSize 1
    --bsDecrease 1000
    --threshold 500
    --selfLoop 0 
    --iterations 10"

cd .. #in biclique_extracion
#make clean
make 

mkdir -p output
rm -f output/*

for name in ${nameFiles[@]}
    do
        file="${name}${ext}"
        exec="./biclique_extractor --file $fileDir$file $params"

        /usr/bin/time -ao output/time.log -f "${name}: %E (s)" $exec  | tee output/output_${name}.log

        cp log.txt output
        mv output/log.txt "output/log_${name}.log"

    done

