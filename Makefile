CC=g++
CFLAGS=-g -O3 -fopenmp -lm
EXECUTABLE=main.o 
OBJECTS = main.cpp AdjencyMatrix.cpp

all: $(EXECUTABLE)

main.o: main.cpp AdjencyMatrix.cpp AdjencyMatrix.hpp
	$(CC) $(CFLAGS) -o biclique_extrator.o $(OBJECTS)
 
clear:
	rm -fr *.o biclique_extrator *~