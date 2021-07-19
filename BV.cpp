#include "BV.h"
/*
class BV
{
    static uint64 block_size;
    static uint64 chunk_size;
    static uint64 block_bits;
    static uint64 chunk_bits;
public:
    explicit BV(char* array, uint64 length);
    explicit BV(std::vector<char> const& array);
    ~BV();
    uint64 rank_size();
    //uint64 select_size();
private:
    char* array;
    uint64 N;
    uint64 B;
};*/

explicit BV::BV(std::string const& filename)
    : rank_enabled(false), select_enabled(false)
{
    std::ifstream ifs(filename, std::ios::binary);

    ifs.seekg(0,std::ios::end);
    long long int size = ifs.tellg();
    ifs.seekg(0);

    array = new char[size];
    ifs.read(array,size);

    N = size;
    B = 8 * N;
}

BV::~BV()
{
    delete[] array;
    if (rank_enabled) {
        delete block_rank;
        delete chunk_rank;
    }
    if (select_enabled) {
        ;
    }
}

void BV::build_rank()
{
    num_block = (B + BV::block_size - 1)/BV::block_size;
    num_chunk = (B + BV::chunk_size - 1)/BV::chunk_size;

    block_rank = new PackedArray((B + BV::block_size - 1)/BV::block_size, BV::block_bits);
    chunk_rank = new PackedArray((B + BV::chunk_size - 1)/BV::chunk_size, BV::chunk_bits);

    uint64 acc = 0;
    for (uint64 i = 0; i < num_block; i++)
    {
        block_rank->set(i,acc);
        for (uint64 j = 0; j < num_chunk; j++)
        {
            chunk_rank->set(i * BV::block_size / BV::chunk_size + j, acc);
            for (uint64 k = 0; k < BV::byte_per_chunk)
            {
                acc += popcount
            }
        }
    }



    rank_enabled = true;
}

void BV::build_select()
{
    select_enabled = true;
}

uint64 BV::rank(uint64 i)
{

}

uint64 BV::select(uint64 i)
{
    //not yet implemented
    return 0;
}

uint64 BV::space()
{
    return B + index_space();
}

uint64 BV::index_space()
{
    return rank_space() + select_space();
}

uint64 BV::rank_space()
{
    return block_rank->space() + chunk_rank->space();
}

uint64 BV::select_space()
{
    //not yet implemented
    return 0;
}