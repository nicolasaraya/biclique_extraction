# ==============================
#   Makefile — Biclique Extractor
# ==============================

# Source and header directories
SRC_DIRS := . Graph Utils test
SRCS := $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJS := $(patsubst %.cpp, build/%.o, $(SRCS))

OUT := biclique_extractor
OUT_DEBUG := biclique_extractor-g
CC := g++
CFLAGS := -std=c++20 -DBITS32 -I. -I./Graph -I./Utils
LFLAGS := -lm -lpthread -lz

debug_level ?= 3

# Flags depending on mode
ifdef debug
CFLAGS += -O0 -g -DDEBUG_LEVEL=$(debug_level)
else
CFLAGS += -O3
endif

# ==============================
#   Main Targets
# ==============================

.PHONY: all debug testGraphs clean gdb valgrind valgrind_leakcheck valgrind_extreme

all: $(OUT)

debug: debug=1
debug: $(OUT_DEBUG)

# Exclude test sources from main build
$(OUT): $(filter-out build/test/%.o, $(OBJS))
	$(CC) $^ -o $@ $(LFLAGS)

# Debug build (same object filtering)
$(OUT_DEBUG): $(filter-out build/test/%.o, $(OBJS))
	$(CC) $^ -o $@ $(LFLAGS)

# Separate target for testGraphs
testGraphs: $(filter-out build/main.o, $(OBJS))
	$(CC) $^ -o testGraph $(LFLAGS)

# ==============================
#   Generic Compilation Rule
# ==============================

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ==============================
#   Utilities
# ==============================

clean:
	rm -rf build $(OUT) $(OUT_DEBUG) testGraph

gdb: $(OUT_DEBUG)
	gdb ./$(OUT_DEBUG)

valgrind: $(OUT_DEBUG)
	valgrind ./$(OUT_DEBUG)

valgrind_leakcheck: $(OUT_DEBUG)
	valgrind --leak-check=full ./$(OUT_DEBUG)

valgrind_extreme: $(OUT_DEBUG)
	valgrind --leak-check=full --show-leak-kinds=all \
		--leak-resolution=high --track-origins=yes --vgdb=yes ./$(OUT_DEBUG)
