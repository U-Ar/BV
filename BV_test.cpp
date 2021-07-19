#include<iostream>
#include<vector>
#include<limits>
#include<cassert>
#include "BV.h"

int main() {

    std::cout << "build BV from std::vector<char>" << std::endl;
    std::vector<char> a1{0x10, 0x03, 0x87, 0x1f};
    BV b1(a1);

    std::cout << "try rank" << std::endl;

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

/*
    std::cout << "try select" << std::endl;

    assert(b1.select(0) == 0);
    assert(b1.select(1) == 4);
    assert(b1.select(2) == 8);
    assert(b1.select(3) == 9);
    assert(b1.select(4) == 16);
    assert(b1.select(5) == 17);
    assert(b1.select(6) == 18);
    assert(b1.select(7) == 23);
    assert(b1.select(8) == 24);
    assert(b1.select(9) == 25);
    assert(b1.select(10) == 26);
    assert(b1.select(11) == 27);
    assert(b1.select(12) == 28);
    assert(b1.select(13) == std::numeric_limits<unsigned long>::max());
    assert(b1.select(14) == std::numeric_limits<unsigned long>::max());*/



    std::cout << "build BV from char array" << std::endl;
    char[4] a2{0x10, 0x03, 0x87, 0x1f};
    BV b2(a2, 4);

    std::cout << "try rank" << std::endl;

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



    




}