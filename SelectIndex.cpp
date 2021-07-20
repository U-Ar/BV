#include "SelectIndex.h"

// [left,right]
SparseBlock::SparseBlock(BV& const bv, uint64 left, uint64 right, uint64 o)
    : ones(o)
{
    positions = new uint64[ones];
    uint64 idx = 0;
    for (uint pos = left; pos <= right; pos++)
    {
        if (((uchar)bv[pos/8]) & (1 << (pos%8))) positions[idx++] = pos;
    }
}

SparseBlock::~SparseBlock()
{
    delete[] positions;
}

uint64 SparseBlock::select(uint64 i)
{
    return positions[i];
}

uint64 SparseBlock::space()
{
    return 64 * ones;
}



DenseBlock::DenseBlock(BV& const bv, uint64 left, uint64 right, uint64 o)
    : ones(o), length(right-left+1), num_leaf((length+DenseBlock::chunk_size-1)/DenseBlock::chunk_size)
{

}

DenseBlock::~DenseBlock()
{
    
}

uint64 DenseBlock::select(uint64 i)
{

}

uint64 DenseBlock::space()
{

}