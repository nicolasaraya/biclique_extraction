CC=g++
CFLAGS=-O3 -fopenmp -lm -std=c++17
CFLAGS_DEBUG=-O0 -g -Wall -lm -fopenmp -std=c++17
EXECUTABLE=main.o
CONFIGURE=clear mk download
OBJECTS = 	main.cpp \
			Utils.cpp \
		 	Utils.hpp \
			Node.hpp \
			Node.cpp \
			Define.hpp \
		 	AdjacencyMatrix.cpp \
		 	AdjacencyMatrix.hpp \
		 	BicliqueExtractor.hpp \
		 	BicliqueExtractor.cpp \
		 	Shingle.hpp \
		 	Shingle.cpp \
		 	Cluster.hpp \
		 	Cluster.cpp \
		 	Trie.hpp \
		 	Trie.cpp  
			
addV = addVertex

all: $(EXECUTABLE)

configure: $(CONFIGURE)

mk:
	mkdir -p ./data
	mkdir -p ./output

main.o: 
	$(CC) $(CFLAGS) -o biclique_extractor $(OBJECTS)

debug:
	$(CC)  $(CFLAGS_DEBUG) -o biclique_extractor-debug $(OBJECTS)

clear:
	rm -fr biclique_extrator biclique_extractor-debug

download:
	cd data && wget http://www.inf.udec.cl/~chernand/sources/kais2014/dsextract/dblp-2011.graph-txt.gz
	cd data && gzip -d dblp-2011.graph-txt.gz
	cd data && $(addV)
	

addVertex:
	awk -F " " '{if( NR==1 ){printf("%d\n",$1);}else{printf("%d %s\n", NR-2, $0);}}' dblp-2011.graph-txt > dblp-2011.net