SHELL=/bin/bash
CC = g++
RELEASEFLAGS = -std=c++17 -Wall -Wextra -DNDEBUG  -O3 -ffast-math -funroll-loops -msse4.2 -march=native -msse4.2 -march=native -DHAVE_CXA_DEMANGLE
FLAGS        = -std=c++17 -Wall -Wextra -O3 -ffast-math -funroll-loops -msse4.2 -march=native -msse4.2 -march=native -DHAVE_CXA_DEMANGLE
IFLAG = -I$(HOME)/sdsl/include
LFLAG = -L$(HOME)/sdsl/lib
LIBS = -lsdsl -ldivsufsort -ldivsufsort64
PROGRAMS = PackedArray_test


#all:	build show report

#build:	lcptest report_size show_runs
run_test: packedarraytest
	./PackedArray_test

packedarraytest:
	$(CC) $(FLAGS) $(IFLAG) $(LFLAG) -o PackedArray_test PackedArray_test.cpp PackedArray.cpp $(LIBS)

clean:
	rm -rf $(PROGRAMS) 

