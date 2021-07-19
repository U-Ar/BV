#ifndef INCLUDED_BV_H
#define INCLUDED_BV_H

#include<vector>
#include<string>
#include<fstream>
#include<memory>
#include<iostream>
#include<iomanip>
#include "PackedArray.h"
#include "bit_op.h"

using uint64 = __UINT64_TYPE__;
using uchar = unsigned char;


class BV
{
    static const uint64 block_size = 2 << 16;
    static const uint64 chunk_size = 256;

    //number of bits necessary for representing rank(block)
    static const uint64 block_bits = 16;

    //number of bits necessary for representing rank(chunk)
    static const uint64 chunk_bits = 8;

    //number of bytes per block
    static const uint64 byte_per_block = BV::block_size / 8;

    //number of bytes per chunk
    static const uint64 byte_per_chunk = BV::chunk_size / 8;

    static const uint64 rank_table[];

public:

    explicit BV(std::string const& filename);
    explicit BV(size_t size);
    // constructor for std::vector<uchar>
    BV(std::vector<uchar> const& il) : BV(il.size())
    {
        size_t idx = 0;
        std::cout << il.size() << std::endl;
        for (; idx < il.size(); idx++)
        {
            std::cout << "elem at " << idx << std::endl; 
            std::cout << il[idx] << std::endl;
        }
        for (; idx < il.size(); idx++)
        {
            std::cout << "elem: " << std::endl; 
            std::cout << il[idx] << std::endl;
            (*this)[idx] = il[idx];
        }
    }
    // move constructor
    //BV(BV&& other);
    // copy constructor
    //BV(const BV& other);
    ~BV();

    char& operator[](size_t i)
    {
        return array[i];
    }
    const char& operator[](size_t i) const
    {
        return array[i];
    }

    void build_rank();
    void build_select();
    uint64 rank(uint64 i);
    uint64 select(uint64 i);

    uint64 space();
    uint64 index_space();
    uint64 rank_space();
    uint64 select_space();

    void report_detail();

private:
    uint64 rem_rank(uint64 i);

    char* array;
    uint64 N;
    uint64 B;
    bool rank_enabled, select_enabled;

    uint64 num_block, num_chunk;
    PackedArray* block_rank;
    PackedArray* chunk_rank;
};




#endif