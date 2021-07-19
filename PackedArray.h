#ifndef INCLUDED_PACKEDARRAY_H
#define INCLUDED_PACKEDARRAY_H

using uint64 = __UINT64_TYPE__;

class PackedArray
{
public:
    PackedArray(uint64 n_elems, uint64 n_bits);
    ~PackedArray();
    // array[i] = val
    bool set(uint64 i, uint64 val);
    // return array[i]
    uint64 get(uint64 i);
    bool set_bit(uint64 idx, uint64 pos, uint64 val);
    uint64 get_bit(uint64 idx, uint64 pos);
    bool _set(uint64 i, uint64 val);
    uint64 _get(uint64 i);
    // used space
    inline uint64 space() { return total_bits; }
private:
    uint64 size;
    uint64 total_bits;
    uint64 bits;
    uint64* array;
    inline void set_upper_bits(uint64& a, uint64 num_bits, uint64 val)
    {
        a = (a & (0xffffffffffffffff >> num_bits)) | ((val & ((1 << num_bits)-1)) << (64-num_bits));
    }
    inline void set_lower_bits(uint64& a, uint64 num_bits, uint64 val)
    {
        a = (a & (0xffffffffffffffff << (num_bits))) | (val >> (bits-num_bits));
    }
    inline void set_internal_bits(uint64& a, uint64 lb, uint64 rb, uint64 val)
    {
        a = (a & ((0xffffffffffffffff << (64-lb))|(0xffffffffffffffff >> (rb+1)))) | (val << (63-rb));
    }
    inline uint64 get_upper_bits(uint64 a, uint64 num_bits)
    {
        return a & (0xffffffffffffffff << (64-num_bits));
    }
    inline uint64 get_lower_bits(uint64 a, uint64 num_bits)
    {
        return a & (0xffffffffffffffff >> (64-num_bits));
    }
    inline uint64 get_internal_bits(uint64 a, uint64 lb, uint64 rb)
    {
        return (a & (0xffffffffffffffff >> (lb))) >> (63-rb);
    }
};

#endif 