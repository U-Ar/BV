SHELL=/bin/bash
CC = g++
RELEASEFLAGS = -std=c++17 -Wall -Wextra -DNDEBUG  -O3 -ffast-math -funroll-loops -msse4.2 -march=native -msse4.2 -march=native -DHAVE_CXA_DEMANGLE
FLAGS        = -std=c++17 -Wall -Wextra -O3 -ffast-math -funroll-loops -msse4.2 -march=native -msse4.2 -march=native -DHAVE_CXA_DEMANGLE
IFLAG = -I$(HOME)/sdsl/include
LFLAG = -L$(HOME)/sdsl/lib
LIBS = -lsdsl -ldivsufsort -ldivsufsort64
PROGRAMS = PackedArray_test BV_test


run_select: bvselecttest
	./BV_select_test

run_test: packedarraytest bvranktest bvselecttest
	./PackedArray_test
	./BV_rank_test
	./BV_select_test

packedarraytest:
	$(CC) $(FLAGS) $(IFLAG) $(LFLAG) -o PackedArray_test PackedArray_test.cpp PackedArray.cpp $(LIBS)

bvranktest:
	$(CC) $(FLAGS) $(IFLAG) $(LFLAG) -o BV_rank_test BV_rank_test.cpp BV.cpp PackedArray.cpp $(LIBS)

bvselecttest:
	$(CC) $(FLAGS) $(IFLAG) $(LFLAG) -o BV_select_test BV_select_test.cpp BV.cpp PackedArray.cpp $(LIBS)

clean:
	rm -rf $(PROGRAMS) 

