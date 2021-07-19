#include<iostream>
#include<vector>
#include<limits>
#include<cassert>
#include "BV.h"

int main() {

    std::cout << "-- BVTest starts --" << std::endl;
    std::cout << "build BV from vector<uchar>" << std::endl;
    BV b1(std::vector<uchar>{0x10, 0x03, 0x87, 0x1f});
    b1.build_rank();
    std::cout << "done" << std::endl;

    std::cout << "rank on BV{0x10, 0x03, 0x87, 0x1f}" << std::endl;
    b1.report_detail();

    assert(b1.rank(0) == 0);
    assert(b1.rank(1) == 0);
    assert(b1.rank(2) == 0);
    assert(b1.rank(3) == 0);
    assert(b1.rank(4) == 1);
    assert(b1.rank(5) == 1);
    assert(b1.rank(6) == 1);
    assert(b1.rank(7) == 1);
    assert(b1.rank(8) == 2);
    assert(b1.rank(9) == 3);
    assert(b1.rank(10) == 3);
    assert(b1.rank(11) == 3);
    assert(b1.rank(12) == 3);
    assert(b1.rank(13) == 3);
    assert(b1.rank(14) == 3);
    assert(b1.rank(15) == 3);
    assert(b1.rank(16) == 4);
    assert(b1.rank(17) == 5);
    assert(b1.rank(18) == 6);
    assert(b1.rank(19) == 6);
    assert(b1.rank(20) == 6);
    assert(b1.rank(21) == 6);
    assert(b1.rank(22) == 6);
    assert(b1.rank(23) == 7);
    assert(b1.rank(24) == 8);
    assert(b1.rank(25) == 9);
    assert(b1.rank(26) == 10);
    assert(b1.rank(27) == 11);
    assert(b1.rank(28) == 12);
    assert(b1.rank(29) == 12);
    assert(b1.rank(30) == 12);
    assert(b1.rank(31) == 12);
    std::cout << "done" << std::endl;


    std::cout << "build BV using BV(size) and operator[]" << std::endl;
    BV b2((size_t)4);
    b2[0] = 0x10; b2[1] = 0x03; b2[2] = 0x87; b2[3] = 0x1f;
    b2.build_rank();

    std::cout << "rank on BV{0x10, 0x03, 0x87, 0x1f}" << std::endl;

    assert(b2.rank(0) == 0);
    assert(b2.rank(1) == 0);
    assert(b2.rank(2) == 0);
    assert(b2.rank(3) == 0);
    assert(b2.rank(4) == 1);
    assert(b2.rank(5) == 1);
    assert(b2.rank(6) == 1);
    assert(b2.rank(7) == 1);
    assert(b2.rank(8) == 2);
    assert(b2.rank(9) == 3);
    assert(b2.rank(10) == 3);
    assert(b2.rank(11) == 3);
    assert(b2.rank(12) == 3);
    assert(b2.rank(13) == 3);
    assert(b2.rank(14) == 3);
    assert(b2.rank(15) == 3);
    assert(b2.rank(16) == 4);
    assert(b2.rank(17) == 5);
    assert(b2.rank(18) == 6);
    assert(b2.rank(19) == 6);
    assert(b2.rank(20) == 6);
    assert(b2.rank(21) == 6);
    assert(b2.rank(22) == 6);
    assert(b2.rank(23) == 7);
    assert(b2.rank(24) == 8);
    assert(b2.rank(25) == 9);
    assert(b2.rank(26) == 10);
    assert(b2.rank(27) == 11);
    assert(b2.rank(28) == 12);
    assert(b2.rank(29) == 12);
    assert(b2.rank(30) == 12);
    assert(b2.rank(31) == 12);

    std::cout << "done" << std::endl;

/*
    std::cout << "try select" << std::endl;

    assert(b2.select(0) == 0);
    assert(b2.select(1) == 4);
    assert(b2.select(2) == 8);
    assert(b2.select(3) == 9);
    assert(b2.select(4) == 16);
    assert(b2.select(5) == 17);
    assert(b2.select(6) == 18);
    assert(b2.select(7) == 23);
    assert(b2.select(8) == 24);
    assert(b2.select(9) == 25);
    assert(b2.select(10) == 26);
    assert(b2.select(11) == 27);
    assert(b2.select(12) == 28);
    assert(b2.select(13) == std::numeric_limits<unsigned long>::max());
    assert(b2.select(14) == std::numeric_limits<unsigned long>::max());*/



    



    std::cout << "-- BVTest  ends  --" << std::endl;
}