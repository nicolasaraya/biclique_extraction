CC=g++
CFLAGS=-g -O3 -fopenmp -lm s
EXECUTABLE=main.o 
OBJECTS = main.cpp AdjencyMatrix.hpp AdjencyMatrix.cpp

all: $(EXECUTABLE)

main.o: main.cpp AdjencyMatrix.cpp AdjencyMatrix.hpp
	$(CC) $(CFLAGS) -o biclique_extrator.o $(OBJECTS)
 
clear:
	rm -fr *.o biclique_extrator *~