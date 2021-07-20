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
    static const uint64 block_size = 1 << 15;
    static const uint64 chunk_size = 256;

    //number of bits necessary for representing rank(block)
    static const uint64 block_bits = 64;

    //number of bits necessary for representing rank(chunk)
    static const uint64 chunk_bits = 16;

    //number of words per block
    static const uint64 word_per_block = BV::block_size / 64;

    //number of words per chunk
    static const uint64 word_per_chunk = BV::chunk_size / 64;

    static const uint64 rank_table[];

public:

    explicit BV(std::string const& filename);
    explicit BV(size_t size);
    // constructor for std::vector<uchar>
    BV(std::vector<uint64> const& il) : BV(il.size())
    {
        size_t idx = 0;
        for (; idx < il.size(); idx++)
        {
            (*this)[idx] = il[idx];
        }
    }
    // move constructor
    //BV(BV&& other);
    // copy constructor
    //BV(const BV& other);
    ~BV();

    uint64& operator[](size_t i)
    {
        return array[i];
    }
    const uint64& operator[](size_t i) const
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

    inline uint64 size() { return N; }
    inline uint64 bit_size() { return B; }

private:
    uint64 rem_rank(uint64 i);

    uint64* array;
    uint64 N;
    uint64 B;
    bool rank_enabled, select_enabled;

    uint64 num_block, num_chunk;
    PackedArray* block_rank;
    PackedArray* chunk_rank;
};




#endif