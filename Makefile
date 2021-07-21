SHELL=/bin/bash
CC = g++
RELEASEFLAGS = -std=c++17 -Wall -Wextra -DNDEBUG  -O3 -ffast-math -funroll-loops -msse4.2 -march=native -msse4.2 -march=native -DHAVE_CXA_DEMANGLE
FLAGS        = -std=c++17 -Wall -Wextra -O3 -ffast-math -funroll-loops -msse4.2 -march=native -msse4.2 -march=native -DHAVE_CXA_DEMANGLE
IFLAG = -I$(HOME)/sdsl/include
LFLAG = -L$(HOME)/sdsl/lib
LIBS = -lsdsl -ldivsufsort -ldivsufsort64
PROGRAMS = PackedArray_test BV_rank_test BV_select_test benchmark


run_benchmark: benchmark 
	./benchmark

test_rank: BV_rank_test
	./BV_rank_test

test_select: BV_select_test
	./BV_select_test

test_packed: PackedArray_test
	./PackedArray_test

test_all: PackedArray_test BV_rank_test BV_select_test
	./PackedArray_test
	./BV_rank_test
	./BV_select_test

PackedArray_test: PackedArray_test.cpp PackedArray.h PackedArray.cpp
	$(CC) $(FLAGS) $(IFLAG) $(LFLAG) -o PackedArray_test PackedArray_test.cpp PackedArray.cpp $(LIBS)

BV_rank_test: BV_rank_test.cpp BV.h BV.cpp PackedArray.h PackedArray.cpp
	$(CC) $(FLAGS) $(IFLAG) $(LFLAG) -o BV_rank_test BV_rank_test.cpp BV.cpp PackedArray.cpp $(LIBS)

BV_select_test: BV_select_test.cpp BV.h BV.cpp PackedArray.h PackedArray.cpp
	$(CC) $(FLAGS) $(IFLAG) $(LFLAG) -o BV_select_test BV_select_test.cpp BV.cpp PackedArray.cpp $(LIBS)

benchmark: benchmark.cpp BV.h BV.cpp PackedArray.h PackedArray.cpp
	$(CC) $(FLAGS) $(IFLAG) $(LFLAG) -o benchmark benchmark.cpp BV.cpp PackedArray.cpp $(LIBS)

clean:
	rm -rf $(PROGRAMS) 

