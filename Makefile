CC=g++
CFLAGS=-g -O3 -fopenmp -lm
EXECUTABLE=main.o
CONFIGURE=clear mk
OBJECTS = main.cpp AdjencyMatrix.cpp AdjencyMatrix.hpp

all: $(EXECUTABLE)

configure: $(CONFIGURE)

mk:
	mkdir -p ./data
	mkdir -p ./output

main.o: main.cpp AdjencyMatrix.cpp AdjencyMatrix.hpp
	$(CC) $(CFLAGS) -o biclique_extractor $(OBJECTS)
 
clear:
	rm -fr biclique_extrator