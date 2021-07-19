#include<iostream>
#include<iomanip>
#include<vector>
#include<limits>
#include<cassert>
#include "PackedArray.h"

int main() 
{
    std::cout << "-- PackedArrayTest starts --" << std::endl;

    PackedArray pa1(100,64);
    std::cout << "Test for n=100, bits=64" << std::endl;
    for (uint64 i = 0; i < 100; i++) {
        pa1.set(i,i);
    }
    for (uint64 i = 0; i < 100; i++) {
        assert(pa1.get(i) == i);
    }
    std::cout << "Done" << std::endl << std::endl;


    PackedArray pa2(1,64);
    std::cout << "Test for n=1, bits=64 & set_bit" << std::endl;
    pa2.set_bit(0,0,1);
    pa2.set_bit(0,2,1);
    pa2.set_bit(0,4,1);
    assert(pa2.get(0) == 21);
    std::cout << "Done" << std::endl << std::endl;


    std::cout << "Test for n=10, bits=7" << std::endl;
    PackedArray pa21(10,7);
    for (uint64 i = 0; i < 10; i++) {
        pa21.set(i,i);
    }
    for (uint64 i = 0; i < 10; i++) {
        assert(pa21.get(i) == i);
    }
    std::cout << "Done" << std::endl << std::endl;


    std::cout << "Test for n=100, bits=7, array[i] = 13" << std::endl;
    PackedArray pa22(10,7);
    for (uint64 i = 0; i < 10; i++) {
        pa22.set(i,13);
    }
    for (uint64 i = 0; i < 10; i++) {
        assert(pa22.get(i) == 13);
    }
    std::cout << "Done" << std::endl << std::endl;


    std::cout << "Test for n=100, bits=7, array[i] = i" << std::endl;
    PackedArray pa3(100,7);
    for (uint64 i = 0; i < 100; i++) {
        pa3.set(i,i);
    }
    //std::cout << std::hex << pa3._get(2) << " " << pa3._get(3) << std::dec << std::endl;
    for (uint64 i = 0; i < 100; i++) {
        assert(pa3.get(i) == i%128);
    }
    std::cout << "Done" << std::endl << std::endl;


    std::cout << "Test for n=364, bits=5, array[i] = i%32" << std::endl;
    PackedArray pa4(364,5);
    for (uint64 i = 0; i < 364; i++) {
        pa4.set(i,i%32);
    }
    for (uint64 i = 0; i < 364; i++) {
        assert(pa4.get(i) == i%32);
    }
    std::cout << "Done" << std::endl << std::endl;



    std::cout << "-- PackedArrayTest  ends  --" << std::endl;

    
}