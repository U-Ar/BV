# BV
Implementation of Succinct Bit Vector BV 

## Implemented features
- BV 
	- rank(i) = (the number of 1s in B[0:i-1])
	- rank0(i) = (the number of 0s in B[0:i-1])
	- select(i) = (the position of i-th 1)
	- all these operations return unsigned long 
- PackedArray
	- Use an array of 64bit unsigned integer as an array of arbitrary length integer (shorter than or equal to 64bit)

## Usage

```c++
#include<BV.h>

BV bv("hoge.bin");
bv.build_rank();
bv.build_select(); //now building select before rank is not supported

bv.rank(10);
bv.select(5);
std::cout << "space complexity: " << bv.space() << " bits" << std::endl;
```

## Test files
BV_rank_test.cpp, BV_select_test.cpp and benchmark.cpp assume that you can use sdsl-lite library
(https://github.com/simongog/sdsl-lite). You can check if the results matches those of 
the external library and compare the processing time.
Also a file name "test/dna.200MB" is now hardcoded. Rewrite it according to the binary file
you want to test.

## Reference
Raman, R., Raman, V., & Satti, S. R. (2007). Succinct indexable dictionaries with applications to encoding k-ary trees, prefix sums and multisets. ACM Transactions on Algorithms (TALG), 3(4), 43-es.
