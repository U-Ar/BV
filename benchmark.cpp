#include<iostream>
#include<vector>
#include<limits>
#include<cassert>
#include<array>
#include<random>
#include<chrono>
#include<string>

#include <sdsl/int_vector.hpp>
#include <sdsl/rank_support_v.hpp>
#include <sdsl/select_support_mcl.hpp>

using namespace sdsl;

#include "BV.h"

std::string testfile = "test/dna.50MB";

inline void println(std::string const& str)
{
    std::cout << str << std::endl;
}

inline void println(uint64 const a)
{
    std::cout << a << std::endl;
}



int main()
{
    std::cout << std::endl << "Performance Benchmark of BV and SDSL on " << testfile << std::endl;


    std::cout << "- BV building on " << testfile << std::endl;
    BV bobj(testfile);
    bobj.build_rank();
    bobj.build_select();
    std::cout << "- BV building done" << std::endl << std::endl;

    std::cout << testfile << " file info " << std::endl;
    std::cout << "number of bytes : " << bobj.size() << std::endl;
    std::cout << "number of bits  : " << bobj.bit_size() << std::endl;
    std::cout << "number of ones  : " << bobj.ones() << std::endl << std::endl;

    std::cout << "- SDSL building on " << testfile << std::endl;
    bit_vector btest(bobj.bit_size(),0,1);
    for (uint64 i = 0; i < bobj.bit_size(); i++)
    {
        btest[i] = (bobj[i/8] & (1 << (i%8))) > 0;
    }
    rank_support_v rs;
    util::init_support(rs,&btest);
    select_support_mcl<1,1> sls;
    util::init_support(sls,&btest);
    std::cout << "- SDSL building done" << std::endl << std::endl;


    std::cout << "--test at random i for 10000 times" << std::endl;
    std::uniform_int_distribution<> dist(1,bobj.ones());
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());

    /*std::vector<uint64> randoms(10000);
    for (size_t i = 0; i < 10000; i++) randoms[i] = dist(engine);
    for (size_t i = 0; i < randoms.size(); i++) 
    {
        assert(bobj.rank(randoms[i]) == rs(randoms[i]));
        assert(bobj.select(randoms[i]) == sls(randoms[i]));
    }*/
    std::vector<uint64> randoms(100);
    for (size_t i = 0; i < 100; i++) randoms[i] = dist(engine);
    for (size_t i = 0; i < randoms.size(); i++) 
    {
        std::cout << "rank: " << bobj.rank(randoms[i]) << " "<< rs(randoms[i]) << std::endl;
        std::cout << "select: " << bobj.select(randoms[i])  << " " << sls(randoms[i]) << std::endl;
    }
    std::cout << "--test at random done" << std::endl;



    /*
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