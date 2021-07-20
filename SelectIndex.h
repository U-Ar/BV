#ifndef INCLUDED_SELECTINDEX_H
#define INCLUDED_SELECTINDEX_H

#include<iostream>
#include<iomanip>
#include "PackedArray.h"
#include "bit_op.h"

using uint64 = __UINT64_TYPE__;

class SelectBlock
{
public:
    SelectBlock();
    virtual ~SelectBlock();
    virtual uint64 select(uint64 i);
    virtual uint64 space();
};

class SparseBlock : public SelectBlock
{
public:
    SparseBlock(BV& const bv, uint64 left, uint64 right, uint64 o);
    ~SparseBlock();
    uint64 select(uint64 i) override;
    uint64 space() override;
private:
    uint64 ones;
    uint64* positions;
};

class DenseBlock : public SelectBlock
{
public:
    DenseBlock(BV& const bv, uint64 left, uint64 right, uint64 o);
    ~DenseBlock();
    uint64 select(uint64 i) override;
    uint64 space() override;
private:
    uint64 ones;
};


#endif