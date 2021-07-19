#ifndef INCLUDED_BV_H
#define INCLUDED_BV_H

#include<vector>
#include<string>
#include<fstream>
using uint64 = __UINT64_TYPE__;


class BV
{
    static uint64 block_size;
    static uint64 chunk_size;
    static uint64 block_bits;
    static uint64 chunk_bits;
public:
    //explicit BV(char* arr, uint64 length);
    //explicit BV(std::vector<char> const& arr);
    explicit BV(std::string const& filename);
    ~BV();
    uint64 rank(uint64 i);
    //uint64 space();
    uint64 rank_space();
    //uint64 select_size();
private:
    char* array;
    uint64 N;
    uint64 B;
};

uint64 BV::block_size = 2 << 16;
uint64 BV::chunk_size = 256;

//number of bits necessary for representing rank(block)
uint64 BV::block_bits = 16;

//number of bits necessary for representing rank(chunk)
uint64 BV::chunk_bits = 8;


#endif