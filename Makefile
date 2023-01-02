CC=g++
CFLAGS=-O3 -fopenmp -lm -Wall
EXECUTABLE=main.o
CONFIGURE=clear mk download
OBJECTS = main.cpp AdjacencyMatrix.cpp AdjacencyMatrix.hpp BicliqueExtractor.hpp BicliqueExtractor.cpp Shingle.hpp Shingle.cpp Cluster.hpp Cluster.cpp Trie.hpp Trie.cpp Utils.hpp
addV = addVertex

all: $(EXECUTABLE)

configure: $(CONFIGURE)

mk:
	mkdir -p ./data
	mkdir -p ./output

main.o: 
	$(CC) $(CFLAGS) -o biclique_extractor $(OBJECTS)
 
clear:
	rm -fr biclique_extrator

download:
	cd data && wget http://www.inf.udec.cl/~chernand/sources/kais2014/dsextract/dblp-2011.graph-txt.gz
	cd data && gzip -d dblp-2011.graph-txt.gz
	cd data && $(addV)
	

addVertex:
	awk -F " " '{if( NR==1 ){printf("%d\n",$1);}else{printf("%d %s\n", NR-2, $0);}}' dblp-2011.graph-txt > dblp-2011.net