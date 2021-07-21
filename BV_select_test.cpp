#include<iostream>
#include<vector>
#include<limits>
#include<cassert>
#include<array>
#include<random>
#include<chrono>

#include <sdsl/int_vector.hpp>
#include <sdsl/rank_support_v.hpp>
#include <sdsl/rank_support_v5.hpp>
using namespace sdsl;

#include "BV.h"

int main() {
    std::cout << "--------------        Select Test on BV        ----------------" << std::endl;
    std::cout << "build BV from vector<uchar>" << std::endl;
    BV b1(std::vector<uchar>{0x10, 0x03, 0x87, 0x1f});
    b1.build_rank();
    b1.build_select();
    std::cout << "done" << std::endl;

    
    std::cout << "select on BV{0x10, 0x03, 0x87, 0x1f}" << std::endl;
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
    std::cout << "select done" << std::endl;


    std::cout << "---------------------------------------------------------------" << std::endl;


    /*
    std::cout << "------test using sdsl library------" << std::endl;

    std::cout << "building on dna.50MB" << std::endl;
    BV bobj("test/dna.50MB");
    std::cout << "read" << std::endl;
    bobj.build_rank();
    std::cout << "done" << std::endl;

    bit_vector btest(bobj.bit_size(),0,1);

    for (uint64 i = 0; i < bobj.bit_size(); i++)
    {
        btest[i] = (bobj[i/8] & (1 << (i%8))) > 0;
    }

    rank_support_v rs;
    util::init_support(rs,&btest);

    for (int i = 127; i < 2000; i+=128) 
    {
        std::cout << std::dec << "rank(" << i << "): " << bobj.rank(i) << " " << rs(i+1) << std::endl;
    }


    std::cout << "test at random i for 100 times" << std::endl;
    std::uniform_int_distribution<> dist(0,bobj.bit_size()-1);
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());

    std::vector<uint64> randoms(100);
    for (size_t i = 0; i < 100; i++) randoms[i] = dist(engine);
    for (size_t i = 0; i < randoms.size(); i++) 
    {
        assert(bobj.rank(randoms[i]) == rs(randoms[i]+1));
    }
    std::cout << "done" << std::endl;




    uint64 myrank_time = 0, sdslrank_time = 0;

    std::vector<uint64> vec(1000), vec2(1000);
    for (int i = 0; i < 100; i++)
    {
        for (size_t i = 0; i < 1000; i++) {
            vec[i] = dist(engine); vec2[i] = vec[i]+1;
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < 1000; i++) bobj.rank(vec[i]);
        auto end = std::chrono::high_resolution_clock::now();
        myrank_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();

        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < 1000; i++) rs(vec2[i]);
        end = std::chrono::high_resolution_clock::now();
        sdslrank_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    }
    std::cout << "avg time for   myrank : " << myrank_time / 100 << std::endl;
    std::cout << "avg time for sdslrank : " << sdslrank_time / 100 << std::endl;*/

}