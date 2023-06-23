OBJS	=   	main.o \
				Graph.o \
				GraphWeighted.o \
				BicliqueExtractor.o \
				Cluster.o \
				Node.o \
				NodeWeighted.o \
				Shingle.o \
				Trie.o \
				Utils.o

OBJS_CHECKER = 	checker.o \
				Graph.o \
				Node.o \
				Utils.o

SOURCE	=       main.cpp \
				Graph/Graph.cpp \
				Graph/GraphWeighted.cpp \
				Graph/Node.cpp \
				Graph/NodeWeighted.cpp \
				Graph/NodeADT.hpp \
				BicliqueExtractor.cpp \
				Cluster.cpp \
				Shingle.cpp \
				Trie.cpp \
				Utils/Utils.cpp
HEADER	=       Graph/Graph.hpp \
				Graph/GraphADT.hpp \
				Graph/GraphWeighted.hpp \
				Graph/Node.hpp \
				Graph/NodeWeighted.hpp \
				Graph/NodeADT.hpp \
				BicliqueExtractor.hpp \
				Cluster.hpp \
				Shingle.hpp \
				Trie.hpp \
				Utils/Utils.hpp \
				Utils/define.hpp
OUT	=           biclique_extractor
OUT_CHECKER =   checker
CC	 =          g++
FLAGS =         -c  -std=c++20 -O0 -DBITS32 -g
LFLAGS	=       -lm -lpthread
				

all: $(OBJS) $(OBJS_CHECKER)
	$(CC) $(OBJS) -o $(OUT) $(LFLAGS)
	$(CC) $(OBJS_CHECKER) -o $(OUT_CHECKER)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 

checker.o: checker.cpp
	$(CC) $(FLAGS) checker.cpp

Graph.o: Graph/Graph.cpp
	$(CC) $(FLAGS) Graph/Graph.cpp 

BicliqueExtractor.o: BicliqueExtractor.cpp
	$(CC) $(FLAGS) BicliqueExtractor.cpp 

Cluster.o: Cluster.cpp
	$(CC) $(FLAGS) Cluster.cpp 

GraphWeighted.o: Graph/GraphWeighted.cpp
	$(CC) $(FLAGS) Graph/GraphWeighted.cpp 

Node.o: Graph/Node.cpp
	$(CC) $(FLAGS) Graph/Node.cpp 

NodeWeighted.o: Graph/NodeWeighted.cpp
	$(CC) $(FLAGS) Graph/NodeWeighted.cpp 

NodeADT.o: Graph/NodeADT.hpp
	$(CC) $(FLAGS)Graph/NodeADT.hpp

Shingle.o: Shingle.cpp
	$(CC) $(FLAGS) Shingle.cpp 

Trie.o: Trie.cpp
	$(CC) $(FLAGS) Trie.cpp 

Utils.o: Utils/Utils.cpp
	$(CC) $(FLAGS) Utils/Utils.cpp 

clean:
	rm -f $(OBJS) $(OBJS_CHECKER) $(OUT) $(OUT_CHECKER)

debug: $(OUT)
	gdb ./$(OUT)

valgrind: $(OUT)
	valgrind ./$(OUT)

valgrind_leakcheck: $(OUT)
	valgrind --leak-check=full ./$(OUT)

valgrind_extreme: $(OUT)
	valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --vgdb=yes ./$(OUT)
