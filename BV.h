#ifndef INCLUDED_BV_H
#define INCLUDED_BV_H

#include<vector>
#include<string>
#include<fstream>
#include "PackedArray.h"
#include "bit_op.h"

using uint64 = __UINT64_TYPE__;


class BV
{
    static uint64 block_size;
    static uint64 chunk_size;
    static uint64 block_bits;
    static uint64 chunk_bits;
    static uint64 byte_per_block;
    static uint64 byte_per_chunk;
public:
    //explicit BV(char* arr, uint64 length);
    //explicit BV(std::vector<char> const& arr);
    explicit BV(std::string const& filename);
    ~BV();
    void build_rank();
    void build_select();
    uint64 rank(uint64 i);
    uint64 select(uint64 i);

    uint64 space();
    uint64 index_space();
    uint64 rank_space();
    uint64 select_space();
private:

    char* array;
    uint64 N;
    uint64 B;
    bool rank_enabled, select_enabled;

    uint64 num_block, num_chunk;
    PackedArray* block_rank;
    PackedArray* chunk_rank;
};

uint64 BV::block_size = 2 << 16;
uint64 BV::chunk_size = 256;

//number of bits necessary for representing rank(block)
uint64 BV::block_bits = 16;

//number of bits necessary for representing rank(chunk)
uint64 BV::chunk_bits = 8;

//number of bytes per block
uint64 BV::byte_per_block = BV::block_size / 8;

//number of bytes per chunk
uint64 BV::byte_per_chunk = BV::chunk_size / 8;


#endif