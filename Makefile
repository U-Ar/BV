SHELL=/bin/bash
CC = g++
FLAGS = -std=c++17 -Wall -Wextra -DNDEBUG  -O3 -ffast-math -funroll-loops -msse4.2 -march=native -msse4.2 -march=native -DHAVE_CXA_DEMANGLE
#IFLAG = -I$(HOME)/sdsl/include
#LFLAG = -L$(HOME)/sdsl/lib
#LIBS =#-lsdsl -ldivsufsort -ldivsufsort64
PROGRAMS = lcptest report_size show_runs
SHOW_LOG = show_log.txt
REPORT_LOG = report_log.txt
BIG_CORPUS = $(CORPUS_PATH)/big 
SMALL_CORPUS = $(CORPUS_PATH)/small

all:	build show report

build:	lcptest report_size show_runs

lcptest:
	$(CC) $(FLAGS) -o lcptest lcptest.cpp $(LIBS)

show:
	find $(SMALL_CORPUS) -type f -exec ./show_runs {} > $(SHOW_LOG) \;

report:
	find $(BIG_CORPUS) -type f -exec ./report_size {} > $(REPORT_LOG) \;

clean:
	rm -rf $(PROGRAMS) $(SHOW_LOG) $(REPORT_LOG)