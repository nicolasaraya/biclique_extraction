OBJS	=   			main.o \
								Utils.o \
								Shingle.o \
								Trie.o \
								Cluster.o \
								GraphStd.o \
								Graph.o \
								GraphWeighted.o \
								Node.o \
								AttrMgr.o \

SOURCE	=       main.cpp \
								Graph/Graph.cpp \
								Graph/GraphWeighted.cpp \
								Graph/GraphStd.cpp \
								Graph/Node.cpp \
								Cluster.cpp \
								Shingle.cpp \
								Trie.cpp \
								Utils/Utils.cpp \
								AttrMgr.cpp \

HEADER	=       Graph/Graph.hpp \
								Graph/GraphStd.hpp \
								Graph/GraphWeighted.hpp \
								Graph/BicliqueExtracted.hpp \
								Graph/Node.hpp \
								Cluster.hpp \
								Shingle.hpp \
								Trie.hpp \
								Utils/Utils.hpp \
                Utils/DebugSystem.hpp \
								AttrMgr.hpp \

OBJ_FOLD = build

debug_level ?= 3

OUT	=           biclique_extractor
OUT_DEBUG = 		biclique_extractor-g
CC	 =          g++
FLAGS =         -c  -std=c++20 -DBITS32 -I. -I./Graph -I./Utils
DEBUG_FLAGS =		-O0 -g 
OPT = 					-O3
LFLAGS	=       -lm -lpthread
				

ifdef debug
FLAGS += $(DEBUG_FLAGS) -DDEBUG_LEVEL=$(debug_level)
else
FLAGS += $(OPT)
endif

all: 
	mkdir -p $(OBJ_FOLD)
	make $(OBJS)
	$(CC) $(OBJ_FOLD)/* -o $(OUT) $(LFLAGS)

debug:
	mkdir -p $(OBJ_FOLD)
	make debug=1 $(OBJS)
	$(CC) $(DEBUG_FLAGS) $(OBJ_FOLD)/* -o $(OUT_DEBUG) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp -o $(OBJ_FOLD)/main.o

checker.o: checker.cpp
	$(CC) $(FLAGS) checker.cpp -o $(OBJ_FOLD)/checker.o

Graph.o: Graph/Graph.cpp
	$(CC) $(FLAGS) Graph/Graph.cpp -o $(OBJ_FOLD)/Graph.o
	
GraphStd.o: Graph/GraphStd.cpp
	$(CC) $(FLAGS) Graph/GraphStd.cpp -o $(OBJ_FOLD)/GraphStd.o

Cluster.o: Cluster.cpp
	$(CC) $(FLAGS) Cluster.cpp -o $(OBJ_FOLD)/Cluster.o

GraphWeighted.o: Graph/GraphWeighted.cpp
	$(CC) $(FLAGS) Graph/GraphWeighted.cpp -o $(OBJ_FOLD)/GraphWeighted.o

Node.o: Graph/Node.cpp
	$(CC) $(FLAGS) Graph/Node.cpp -o $(OBJ_FOLD)/Node.o

NodeWeighted.o: Graph/NodeWeighted.cpp
	$(CC) $(FLAGS) Graph/NodeWeighted.cpp -o $(OBJ_FOLD)/NodeWeighted.o

NodeADT.o: Graph/NodeADT.hpp
	$(CC) $(FLAGS)Graph/NodeADT.hpp -o $(OBJ_FOLD)/NodeADT.o

Shingle.o: Shingle.cpp
	$(CC) $(FLAGS) Shingle.cpp -o $(OBJ_FOLD)/Shingle.o

Trie.o: Trie.cpp
	$(CC) $(FLAGS) Trie.cpp -o $(OBJ_FOLD)/Trie.o

Utils.o: Utils/Utils.cpp
	$(CC) $(FLAGS) Utils/Utils.cpp -o $(OBJ_FOLD)/Utils.o

AttrMgr.o: AttrMgr.cpp
	$(CC) $(FLAGS) AttrMgr.cpp -o $(OBJ_FOLD)/AttrMgr.o

clean:
	rm -rf $(OBJS) $(OBJS_CHECKER) $(OUT) $(OUT_DEBUG) $(OBJ_FOLD) 

gdb: $(OUT)
	gdb ./$(OBJ_FOLD)/$(OUT_DEBUG)

valgrind: $(OUT)
	valgrind ./$(OBJ_FOLD)/$(OUT_DEBUG)

valgrind_leakcheck: $(OUT)
	valgrind --leak-check=full ./$(OBJ_FOLD)/$(OUT_DEBUG)

valgrind_extreme: $(OUT)
	valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --vgdb=yes ./$(OBJ_FOLD)/$(OUT_DEBUG)
