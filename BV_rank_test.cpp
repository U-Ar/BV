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

std::string testfile = "test/dna.50MB";

int main() {
    std::cout << std::endl <<  "--------------         RankTest on BV          ----------------" << std::endl;
    std::cout << "build BV from vector<uchar>" << std::endl;
    BV b1(std::vector<uchar>{0x10, 0x03, 0x87, 0x1f});
    b1.build_rank();
    std::cout << "done" << std::endl;

    std::cout << "rank on BV{0x10, 0x03, 0x87, 0x1f}" << std::endl;

    assert(b1.rank(1) == 0);
    assert(b1.rank(2) == 0);
    assert(b1.rank(3) == 0);
    assert(b1.rank(4) == 0);
    assert(b1.rank(5) == 1);
    assert(b1.rank(6) == 1);
    assert(b1.rank(7) == 1);
    assert(b1.rank(8) == 1);
    assert(b1.rank(9) == 2);
    assert(b1.rank(10) == 3);
    assert(b1.rank(11) == 3);
    assert(b1.rank(12) == 3);
    assert(b1.rank(13) == 3);
    assert(b1.rank(14) == 3);
    assert(b1.rank(15) == 3);
    assert(b1.rank(16) == 3);
    assert(b1.rank(17) == 4);
    assert(b1.rank(18) == 5);
    assert(b1.rank(19) == 6);
    assert(b1.rank(20) == 6);
    assert(b1.rank(21) == 6);
    assert(b1.rank(22) == 6);
    assert(b1.rank(23) == 6);
    assert(b1.rank(24) == 7);
    assert(b1.rank(25) == 8);
    assert(b1.rank(26) == 9);
    assert(b1.rank(27) == 10);
    assert(b1.rank(28) == 11);
    assert(b1.rank(29) == 12);
    assert(b1.rank(30) == 12);
    assert(b1.rank(31) == 12);
    assert(b1.rank(32) == 12);
    std::cout << "rank done" << std::endl;

    std::cout << "build BV using BV(size) and operator[]" << std::endl;
    BV b2((size_t)4);
    b2[0] = 0x10; b2[1] = 0x03; b2[2] = 0x87; b2[3] = 0x1f;
    b2.build_rank();

    std::cout << "rank on BV{0x10, 0x03, 0x87, 0x1f}" << std::endl;

    assert(b2.rank(1) == 0);
    assert(b2.rank(2) == 0);
    assert(b2.rank(3) == 0);
    assert(b2.rank(4) == 0);
    assert(b2.rank(5) == 1);
    assert(b2.rank(6) == 1);
    assert(b2.rank(7) == 1);
    assert(b2.rank(8) == 1);
    assert(b2.rank(9) == 2);
    assert(b2.rank(10) == 3);
    assert(b2.rank(11) == 3);
    assert(b2.rank(12) == 3);
    assert(b2.rank(13) == 3);
    assert(b2.rank(14) == 3);
    assert(b2.rank(15) == 3);
    assert(b2.rank(16) == 3);
    assert(b2.rank(17) == 4);
    assert(b2.rank(18) == 5);
    assert(b2.rank(19) == 6);
    assert(b2.rank(20) == 6);
    assert(b2.rank(21) == 6);
    assert(b2.rank(22) == 6);
    assert(b2.rank(23) == 6);
    assert(b2.rank(24) == 7);
    assert(b2.rank(25) == 8);
    assert(b2.rank(26) == 9);
    assert(b2.rank(27) == 10);
    assert(b2.rank(28) == 11);
    assert(b2.rank(29) == 12);
    assert(b2.rank(30) == 12);
    assert(b2.rank(31) == 12);
    assert(b2.rank(32) == 12);

    std::cout << "done" << std::endl;




    std::cout << "------test using sdsl library------" << std::endl;

    std::cout << "building on " << testfile << std::endl;
    BV bobj(testfile);
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

    std::cout << "test at random i for 100000 times" << std::endl;
    std::uniform_int_distribution<> dist(1,bobj.bit_size());
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());

    std::vector<uint64> randoms(100000);
    for (size_t i = 0; i < 100000; i++) randoms[i] = dist(engine);
    for (size_t i = 0; i < randoms.size(); i++) 
    {
        assert(bobj.rank(randoms[i]) == rs(randoms[i]));
    }
    std::cout << "done" << std::endl;

    std::cout << "-----------------------------------" << std::endl;

    std::cout << "--------------       RankTest Completed        ----------------" << std::endl << std::endl;
}