CXX      ?= clang++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
SRC_DIR  := src
OBJ_DIR  := build

COMMON_OBJS := $(OBJ_DIR)/teacher.o $(OBJ_DIR)/csv.o
HW1_OBJS    := $(COMMON_OBJS) $(OBJ_DIR)/main.o
GEN_OBJS    := $(COMMON_OBJS) $(OBJ_DIR)/data_gen.o

.PHONY: all clean docs gen-data bench plots

all: hw1 data_gen

hw1: $(HW1_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

data_gen: $(GEN_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

# Генерирует 12 наборов данных от 100 до 100000.
gen-data: data_gen
	mkdir -p data
	for n in 100 500 1000 2000 5000 10000 20000 30000 50000 70000 100000; do \
		./data_gen data/teachers_$$n.csv $$n; \
	done

bench: hw1 gen-data
	mkdir -p results
	./hw1 bench data results/timings.csv

plots: bench
	python3 plot.py

docs:
	doxygen Doxyfile

clean:
	rm -rf $(OBJ_DIR) hw1 data_gen results/*.csv results/*.png docs/html
