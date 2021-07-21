#ifndef INCLUDED_BV_H
#define INCLUDED_BV_H

#include<algorithm>
#include<fstream>
#include<iomanip>
#include<iostream>
#include<limits>
#include<memory>
#include<string>
#include<vector>
#include "PackedArray.h"
#include "bit_op.h"

using uint64 = __UINT64_TYPE__;
using uint32 = __UINT32_TYPE__;
using uchar = unsigned char;

class SelectBlock;

class BV
{
public:
    //------------ RANK -------------
    static uint64 block_size;
    static uint64 chunk_size;
    //number of bits necessary for representing rank(block)
    static uint64 block_bits;
    //number of bits necessary for representing rank(chunk)
    static uint64 chunk_bits;
    //number of bytes per block
    static uint64 byte_per_block;
    //number of bytes per chunk
    static uint64 byte_per_chunk;
    static const uint64 rank_table[];
    //-------------------------------

    //----------- SELECT ------------
    static uint64 area_ones;
    static uint64 boundary_size;
    //-------------------------------


    explicit BV(std::string const& filename);
    explicit BV(size_t size);
    // constructor for std::vector<uchar>
    BV(std::vector<uchar> const& il) : BV(il.size())
    {
        size_t idx = 0;
        for (; idx < il.size(); idx++)
        {
            O += popcount(il[idx]);
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
    inline uint64 rank0(uint64 i) { return i+1 - rank(i); }
    uint64 select(uint64 i);

    uint64 space();
    uint64 index_space();
    uint64 rank_space();
    uint64 select_space();

    void report_detail();

    inline uint64 size() { return N; }
    inline uint64 bit_size() { return B; }
    inline uint64 ones() { return O; }

private:
    uint64 rem_rank(uint64 i);

    char* array;
    uint64 N; // number of bytes
    uint64 B; // number of bits
    uint64 O; // number of ones
    bool rank_enabled, select_enabled;

    //------------ RANK -------------
    uint64 num_block, num_chunk;
    PackedArray* block_rank; // larger block for rank
    PackedArray* chunk_rank; // smaller block for rank
    //-------------------------------

    //----------- SELECT ------------
    uint64 num_area;
    SelectBlock** area_rank;
    //-------------------------------
};




class SelectBlock
{
public:
    SelectBlock() {};
    virtual ~SelectBlock() {};
    virtual uint64 select(uint64 i) { return i; };
    virtual uint64 space() { return 0; };
};

class SparseBlock : public SelectBlock
{
public:
    SparseBlock(const BV* bv, uint64 left, uint64 right, uint64 o);
    ~SparseBlock();
    uint64 select(uint64 i) override;
    uint64 space() override;
private:
    uint64 ones;
    uint64* positions;
};

class DenseBlock : public SelectBlock
{
    static uint64 chunk_size;
    static uint64 arity;
public:
    DenseBlock(BV* ptr, uint64 l, uint64 r, uint64 o);
    ~DenseBlock();
    uint64 select(uint64 i) override;
    uint64 space() override;
private:
    uint64 leaf_select(uint64 i, uint64 leaf);
    
    BV* bv;
    uint64 ones;
    uint64 left, right;
    uint64 length;
    uint64 num_leaf;
    uint64 num_node;
    uint32* tree;
};




#endif