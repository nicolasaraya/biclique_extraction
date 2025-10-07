# Identification and extraction of Bicliques using MinHash and Prefix Tree

![Biclique](https://upload.wikimedia.org/wikipedia/commons/thumb/f/f3/Biclique_K_3_3.svg/2381px-Biclique_K_3_3.svg.png)

## Usage


## Compile

```bash
make
```
Flags:
* `-Dparallel` : enable parallelism
* `-DBITS32` : compile with 32 bits 
* `-DNUM_THREADS=value` : set number of threads
    

## Execute


```bash
./biclique_extractor params
```

## Parameters

* `--file`: file in `bin` or `txt`
* `--numSignature`: number of Min hashes
* `--minClusterSize`: minimum number of nodes in each cluster
* `--bicliqueSize`: minimum number of SxC per biclique 
* `--minAdyNodes`: minimum number of adyancy nodes per node to make cluster
* `--bsDecrease`: discount factor 
* `--shingleSize`: shingle size value
* `--selfLoop`: flag for selfloops
* `--threshold`: minimum bicliques founded per iteration
* `--debug`: debug flag
* `--iterations`: number of iterations
* `--useDelta`: set 1 to sae using delta16
* `--saveBin`: set 1 to save the result in binary, on by default
* `--saveTxt`: set 1 to save the result in txt, off by default
* `--optimize`: set 1 to run with optimized params, off by default


Execution example 
```bash
bash run.sh
```

## References

Hernández, C., & Navarro, G. (2014). Compressed representations for web and social graphs. Knowledge and information systems, 40(2), 279-313.
