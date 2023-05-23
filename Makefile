OBJS	= main.o AdjacencyMatrix.o BicliqueExtractor.o Cluster.o Node.o Shingle.o Trie.o Utils.o
SOURCE	= main.cpp AdjacencyMatrix.cpp BicliqueExtractor.cpp Cluster.cpp Node.cpp Shingle.cpp Trie.cpp Utils.cpp
HEADER	= AdjacencyMatrix.hpp BicliqueExtractor.hpp Cluster.hpp Node.hpp Shingle.hpp Trie.hpp Utils.hpp define.hpp
OUT	= biclique_extractor
CC	 = g++
FLAGS = -c  -std=c++17 \
		-O3
LFLAGS	 = -lm \
		#-fopenmp

all: $(OBJS)
	$(CC) $(OBJS) -o $(OUT) $(LFLAGS)


main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 

AdjacencyMatrix.o: AdjacencyMatrix.cpp
	$(CC) $(FLAGS) AdjacencyMatrix.cpp 

BicliqueExtractor.o: BicliqueExtractor.cpp
	$(CC) $(FLAGS) BicliqueExtractor.cpp 

Cluster.o: Cluster.cpp
	$(CC) $(FLAGS) Cluster.cpp 

Node.o: Node.cpp
	$(CC) $(FLAGS) Node.cpp 

Shingle.o: Shingle.cpp
	$(CC) $(FLAGS) Shingle.cpp 

Trie.o: Trie.cpp
	$(CC) $(FLAGS) Trie.cpp 

Utils.o: Utils.cpp
	$(CC) $(FLAGS) Utils.cpp 

clean:
	rm -f $(OBJS) $(OUT)

debug: $(OUT)
	gdb ./$(OUT)

valgrind: $(OUT)
	valgrind ./$(OUT)

valgrind_leakcheck: $(OUT)
	valgrind --leak-check=full ./$(OUT)

valgrind_extreme: $(OUT)
	valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --vgdb=yes ./$(OUT)

download:
	cd data && wget http://www.inf.udec.cl/~chernand/sources/kais2014/dsextract/dblp-2011.graph-txt.gz
	cd data && gzip -d dblp-2011.graph-txt.gz
	cd data && $(addV)

mk:
	mkdir -p ./data
	mkdir -p ./output

