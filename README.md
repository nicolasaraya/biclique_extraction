# Identification and extraction of Bicliques using MinHash and Prefix Tree

![Biclique](https://upload.wikimedia.org/wikipedia/commons/thumb/f/f3/Biclique_K_3_3.svg/2381px-Biclique_K_3_3.svg.png)

## Usage

 Clear and create directories

```bash
make configure
```

Load dataset in ```Data``` folder

Compile

```bash
make
```

Execute


```bash
./biclique_extractor filename num_sign min_cluster_size min_adyacency_nodes biclique_size biclique_size_decrease
```


* `filename`: file in Data folder
* `num_sign`: number of Min hashes
* `min_cluster_size`: minimum number of nodes in each cluster
* `min_adyency_nodes`: minimum number of adyancy nodes per node to make cluster
* `biclique_size`: minimum number of SxC per biclique
* `biclique_size_decrease`: discount factor 

Execution example 
```bash
bash run.sh
```

## References

Hernández, C., & Navarro, G. (2014). Compressed representations for web and social graphs. Knowledge and information systems, 40(2), 279-313.
