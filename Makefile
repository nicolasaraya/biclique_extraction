CC=g++
CFLAGS=-g -O3 -fopenmp -lm
EXECUTABLE=main.o
CONFIGURE=clear mk
OBJECTS = main.cpp AdjencyMatrix.cpp AdjencyMatrix.hpp BicliqueExtractor.hpp BicliqueExtractor.cpp Shingle.hpp Shingle.cpp Cluster.hpp Cluster.cpp

all: $(EXECUTABLE)

configure: $(CONFIGURE)

mk:
	mkdir -p ./data
	mkdir -p ./output

main.o: 
	$(CC) $(CFLAGS) -o biclique_extractor $(OBJECTS)
 
clear:
	rm -fr biclique_extrator