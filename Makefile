OBJS	= main.o AdjacencyMatrix.o BicliqueExtractor.o Cluster.o Node.o Shingle.o Trie.o Utils.o
SOURCE	= main.cpp AdjacencyMatrix.cpp BicliqueExtractor.cpp Cluster.cpp Node.cpp Shingle.cpp Trie.cpp Utils.cpp
HEADER	= AdjacencyMatrix.hpp BicliqueExtractor.hpp Cluster.hpp Node.hpp Shingle.hpp Trie.hpp Utils.hpp define.hpp
OUT	= biclique_extractor
CC	 = g++
FLAGS	 = -c -Wall -O0 -g
LFLAGS	 = -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp -std=c++17

AdjacencyMatrix.o: AdjacencyMatrix.cpp
	$(CC) $(FLAGS) AdjacencyMatrix.cpp -std=c++17

BicliqueExtractor.o: BicliqueExtractor.cpp
	$(CC) $(FLAGS) BicliqueExtractor.cpp -std=c++17

Cluster.o: Cluster.cpp
	$(CC) $(FLAGS) Cluster.cpp -std=c++17

Node.o: Node.cpp
	$(CC) $(FLAGS) Node.cpp -std=c++17

Shingle.o: Shingle.cpp
	$(CC) $(FLAGS) Shingle.cpp -std=c++17

Trie.o: Trie.cpp
	$(CC) $(FLAGS) Trie.cpp -std=c++17

Utils.o: Utils.cpp
	$(CC) $(FLAGS) Utils.cpp -std=c++17

clean:
	rm -f $(OBJS) $(OUT)

debug: $(OUT)
	valgrind $(OUT)

valgrind: $(OUT)
	valgrind $(OUT)

valgrind_leakcheck: $(OUT)
	valgrind --leak-check=full $(OUT)

valgrind_extreme: $(OUT)
	valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --vgdb=yes $(OUT)

download:
	cd data && wget http://www.inf.udec.cl/~chernand/sources/kais2014/dsextract/dblp-2011.graph-txt.gz
	cd data && gzip -d dblp-2011.graph-txt.gz
	cd data && $(addV)

mk:
	mkdir -p ./data
	mkdir -p ./output