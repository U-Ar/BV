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

std::string testfile = "test/dna.200MB";

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
    BV::block_size = 1 << 15;
    BV::chunk_size = 256;
    //BV::block_bits = 64;
    //BV::chunk_bits = 16;
    BV::byte_per_block = BV::block_size / 8;
    BV::byte_per_chunk = BV::chunk_size / 8;
    BV::area_ones = 1 << 9;
    BV::boundary_size = 1 << 18;
    std::cout << std::endl << "Performance Benchmark of BV and SDSL on " << testfile << std::endl;


    std::cout << "- BV building on " << testfile << std::endl;
    BV bobj(testfile);
    auto start = std::chrono::high_resolution_clock::now();
    bobj.build_rank();
    bobj.build_select();
    auto end = std::chrono::high_resolution_clock::now();
    uint64 mybuild_time = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
    std::cout << "- BV building done" << std::endl << std::endl;
    std::cout << "- BV building time = " << mybuild_time << "us" << std::endl << std::endl;

    std::cout << "-- BV rank space = " << bobj.rank_space() << " bits " 
        << (double)bobj.rank_space() / bobj.bit_size() << " per bit" << std::endl;
    std::cout << "-- BV select space = " << bobj.select_space() << " bits "
        << (double)bobj.select_space() / bobj.bit_size() << " per bit" << std::endl;

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
    start = std::chrono::high_resolution_clock::now();
    rank_support_v rs;
    util::init_support(rs,&btest);
    select_support_mcl<1,1> sls;
    util::init_support(sls,&btest);
    end = std::chrono::high_resolution_clock::now();
    uint64 sdslbuild_time = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();

    std::cout << "- SDSL building done" << std::endl << std::endl;
    std::cout << "- SDSL building time = " << sdslbuild_time << "us" << std::endl << std::endl;



    std::uniform_int_distribution<> dist(1,bobj.bit_size());
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());

    uint64 myrank_time = 0, sdslrank_time = 0;

    std::vector<uint64> vec(10000);
    for (int j = 0; j < 100; j++)
    {
        for (size_t i = 0; i < 10000; i++) {
            vec[i] = dist(engine); 
        }
        for (size_t i = 0; i < 10000; i++) {
            assert(bobj.rank(vec[i]) == rs(vec[i])); 
        }

        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < 10000; i++) bobj.rank(vec[i]);
        end = std::chrono::high_resolution_clock::now();
        myrank_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();

        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < 10000; i++) rs(vec[i]);
        end = std::chrono::high_resolution_clock::now();
        sdslrank_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    }
    std::cout << "avg time for   myrank : " << myrank_time / 100  << " ns"<< std::endl;
    std::cout << "avg time for sdslrank : " << sdslrank_time / 100 << " ns" << std::endl;



    std::uniform_int_distribution<> bitdist(1,bobj.ones());
    uint64 myselect_time = 0, sdslselect_time = 0;

    for (int j = 0; j < 100; j++)
    {
        for (size_t i = 0; i < 10000; i++) {
            vec[i] = bitdist(engine); 
        }
        for (size_t i = 0; i < 10000; i++) {
            assert(bobj.select(vec[i]) == sls(vec[i])); 
        }

        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < 10000; i++) bobj.select(vec[i]);
        end = std::chrono::high_resolution_clock::now();
        myselect_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();

        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < 10000; i++) sls(vec[i]);
        end = std::chrono::high_resolution_clock::now();
        sdslselect_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    }
    std::cout << "avg time for   myselect : " << myselect_time / 100 << " ns" << std::endl;
    std::cout << "avg time for sdslselect : " << sdslselect_time / 100 << " ns" << std::endl;



    


    std::cout << std::endl << "benchmark for sparse bitvector" << std::endl;
    std::cout << "length: 1677721600, bits: 100000(approx)" << std::endl;
    uint64 n = 1677721600, b = 100000;
    std::uniform_int_distribution<> bitdist2(0,n-1);

    std::vector<unsigned char> vb((n+7)/8,0);
    bit_vector bsparsetest(n,0,1);
    for (uint64 i = 0; i < b; i++)
    {
        uint64 r = bitdist2(engine);
        bsparsetest[r] = 1;
        vb[r/8] = vb[r/8] | (1 << (r%8));
    }
    BV bsparse(vb);
    bsparse.build_rank();
    bsparse.build_select();
    rank_support_v rsp;
    util::init_support(rsp,&bsparsetest);
    select_support_mcl<1,1> slsp;
    util::init_support(slsp,&bsparsetest);



    std::uniform_int_distribution<> dist2(1,bsparse.bit_size());
    myrank_time = 0, sdslrank_time = 0;
    for (int j = 0; j < 100; j++)
    {
        for (size_t i = 0; i < 10000; i++) {
            vec[i] = dist2(engine); 
        }
        for (size_t i = 0; i < 10000; i++) {
            assert(bsparse.rank(vec[i]) == rsp(vec[i])); 
        }

        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < 10000; i++) bsparse.rank(vec[i]);
        end = std::chrono::high_resolution_clock::now();
        myrank_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();

        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < 10000; i++) rsp(vec[i]);
        end = std::chrono::high_resolution_clock::now();
        sdslrank_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    }
    std::cout << "(sparse)avg time for   myrank : " << myrank_time / 100 << " ns" << std::endl;
    std::cout << "(sparse)avg time for sdslrank : " << sdslrank_time / 100 << " ns" << std::endl;





    std::uniform_int_distribution<> bitdist3(1,bsparse.ones());
    myselect_time = 0, sdslselect_time = 0;
    for (int j = 0; j < 100; j++)
    {
        for (size_t i = 0; i < 10000; i++) {
            vec[i] = bitdist3(engine); 
        }
        for (size_t i = 0; i < 10000; i++) {
            assert(bsparse.select(vec[i]) == slsp(vec[i])); 
        }

        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < 10000; i++) bsparse.select(vec[i]);
        end = std::chrono::high_resolution_clock::now();
        myselect_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();

        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < 10000; i++) slsp(vec[i]);
        end = std::chrono::high_resolution_clock::now();
        sdslselect_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    }
    std::cout << "(sparse)index size              : " << (double)bsparse.select_space() / bsparse.bit_size() << " per bit" << std::endl;
    std::cout << "(sparse)avg time for   myselect : " << myselect_time / 100 << " ns" << std::endl;
    std::cout << "(sparse)avg time for sdslselect : " << sdslselect_time / 100 << " ns" << std::endl;
    


/*
    std::cout << std::endl << "cross validation for rank" << std::endl;
    uint64 BV::block_size = 1 << 15;
    uint64 BV::chunk_size = 256;
    uint64 BV::block_bits = 64;
    uint64 BV::chunk_bits = 16;
    uint64 BV::byte_per_block = BV::block_size / 8;
    uint64 BV::byte_per_chunk = BV::chunk_size / 8;
    uint64 BV::area_ones = 1 << 9;
    uint64 BV::boundary_size = 1 << 18;*/

}