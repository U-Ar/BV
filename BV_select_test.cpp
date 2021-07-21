#include<iostream>
#include<vector>
#include<limits>
#include<cassert>
#include<array>
#include<random>
#include<chrono>
#include<string>

#include <sdsl/int_vector.hpp>
//#include <sdsl/rank_support_v.hpp>
//#include <sdsl/rank_support_v5.hpp>
#include <sdsl/select_support_mcl.hpp>
using namespace sdsl;

#include "BV.h"

std::string testfile = "test/dna.50MB";

int main() {
    std::cout << std::endl <<  "--------------        SelectTest on BV         ----------------" << std::endl << std::endl;
    std::cout << "build BV from vector<uchar>{0x10, 0x03, 0x87, 0x1f}" << std::endl;
    BV b1(std::vector<uchar>{0x10, 0x03, 0x87, 0x1f});
    std::cout << "-building rank" << std::endl;
    b1.build_rank();
    std::cout << "-done" << std::endl << std::endl;
    std::cout << "-building select" << std::endl;
    b1.build_select();
    std::cout << "-done" << std::endl << std::endl;

    
    std::cout << "-select on BV{0x10, 0x03, 0x87, 0x1f}" << std::endl;
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
    std::cout << "-select done" << std::endl;


    std::cout << std::endl << std::endl << "---------  Test Using SDSL            ---------" << std::endl << std::endl;
    
    std::cout << "-building on " << testfile << std::endl;
    BV bobj(testfile);
    bobj.build_rank();
    bobj.build_select();
    std::cout << "-building done" << std::endl << std::endl;

    std::cout << testfile << " : " << std::endl;
    std::cout << "number of bytes : " << bobj.size() << std::endl;
    std::cout << "number of bits  : " << bobj.bit_size() << std::endl;
    std::cout << "number of ones  : " << bobj.ones() << std::endl;

    bit_vector btest(bobj.bit_size(),0,1);
    for (uint64 i = 0; i < bobj.bit_size(); i++)
    {
        btest[i] = (bobj[i/8] & (1 << (i%8))) > 0;
    }
    select_support_mcl<1,1> sls;
    util::init_support(sls,&btest);

    std::cout << "                    BV     SDSL" << std::endl;
    for (int i = 1; i < 20000; i += 1000) 
    {   
        std::cout << std::dec << "select(" << i << ") : " << std::setw(8) << bobj.select(i) << " " << std::setw(8) << sls(i) << std::endl;
    }


    std::cout << "--test at random i for 100000 times" << std::endl;
    std::uniform_int_distribution<> dist(1,bobj.ones());
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());

    
    std::vector<uint64> randoms(100000);
    
    for (size_t i = 0; i < randoms.size(); i++) randoms[i] = dist(engine);
    for (size_t i = 0; i < randoms.size(); i++) 
    {
        assert(bobj.select(randoms[i]) == sls(randoms[i]));
    }
    std::cout << "--test at random done" << std::endl << std::endl;
    



    std::cout << "--test for very sparse bitvector for 100000 times" << std::endl;
    std::cout << "length: 10000000, bits: 10000(approx)" << std::endl;
    uint64 n = 10000000, b = 10000;
    std::uniform_int_distribution<> bitdist(0,n-1);

    std::cout << "-building start" << std::endl;
    std::vector<unsigned char> vb((n+7)/8,0);
    bit_vector bsparsetest(n,0,1);

    for (uint64 i = 0; i < b; i++)
    {
        uint64 r = bitdist(engine);
        bsparsetest[r] = 1;
        vb[r/8] = vb[r/8] | (1 << (r%8));
    }

    BV bsparse(vb);
    bsparse.build_rank();
    bsparse.build_select();

    select_support_mcl<1,1> slsp;
    util::init_support(slsp,&bsparsetest);
    std::cout << "-building done" << std::endl;


    std::uniform_int_distribution<> distsparse(1,bsparse.ones());

    for (size_t i = 0; i < randoms.size(); i++) randoms[i] = distsparse(engine);
    for (size_t i = 0; i < randoms.size(); i++) 
    {
        assert(bsparse.select(randoms[i]) == slsp(randoms[i]));
    }

    std::cout << "--test at sparse done" << std::endl << std::endl;




    std::cout << std::endl << std::endl << "---------  Test Using SDSL Completed  ---------" << std::endl << std::endl;
    std::cout << "--------------      SelectTest Completed       ----------------" << std::endl << std::endl;
}