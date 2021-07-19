#include "PackedArray.h"

PackedArray::PackedArray(uint64 n_elems, uint64 n_bits)
    : size(n_elems), bits(n_bits), total_bits(n_elems*n_bits)
{
    array = new uint64[(total_bits+63)/64];
}

PackedArray::~PackedArray()
{
    delete[] array;
}

bool PackedArray::set(uint64 i, uint64 val)
{
    uint64 lb = i * bits;
    uint64 rb = lb + bits - 1;
    uint64 li = lb / 64, ri = rb / 64;
    uint64 lr = lb % 64, rr = rb % 64;

    if (li != ri)
    {
        set_lower_bits(array[li],64-lr,val);
        set_upper_bits(array[ri],rr+1,val);
        //array[li] = (array[li] & (0xffffffffffffffff << (64-lr))) | (val >> lr);
        //array[ri] = (array[ri] & (0xffffffffffffffff >> rr)) | ((val & ((1 << rr)-1) << (64 - rr)));
    } else {
        set_internal_bits(array[li],lr,rr,val);
    }
    return true;
}

uint64 PackedArray::get(uint64 i)
{
    uint64 lb = i * bits;
    uint64 rb = lb + bits - 1;
    uint64 li = lb / 64, ri = rb / 64;
    uint64 lr = lb % 64, rr = rb % 64;

    if (li != ri)
    {
        return (get_lower_bits(array[li],64-lr) << (rr+1)) | (get_upper_bits(array[ri],rr+1) >> (63-rr));
    } else {
        return get_internal_bits(array[li],lr,rr);
    }
}

bool PackedArray::set_bit(uint64 idx, uint64 pos, uint64 val)
{
    uint64 b = idx * bits + (bits-pos-1), i = b / 64, r = b % 64;
    set_internal_bits(array[i],r,r,val);
    return true;
}

uint64 PackedArray::get_bit(uint64 idx, uint64 pos)
{
    uint64 b = idx * bits + (bits-pos-1), i = b / 64, r = b % 64;
    return get_internal_bits(array[i],r,r);
}

bool PackedArray::_set(uint64 i, uint64 val)
{
    array[i] = val;
    return true;
}

uint64 PackedArray::_get(uint64 i)
{
    return array[i];
}