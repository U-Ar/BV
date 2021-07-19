#ifndef INCLUDED_BITOP_H
#define INCLUDED_BITOP_H

using uint64 = __UINT64_TYPE__;
/*
inline void set_upper_bits(uint64& a, uint64 num_bits, uint64 val)
{
    a = (a & (0xffffffffffffffff >> num_bits)) | ((val & ((1 << num_bits)-1)) << (64-num_bits));
}

inline void set_lower_bits(uint64& a, uint64 num_bits, uint64 val)
{
    a = (a & (0xffffffffffffffff << (num_bits))) | (val >> (64-num_bits));
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

inline uint64 get_internal_bits(uint64& a, uint64 lb, uint64 rb)
{
    return (a & (0xffffffffffffffff >> (lb))) >> (63-rb);
}*/

inline uint64 popcount(uint64 n)
{
#ifdef __GNUC__
    return __builin_popcountll(n);
#else 
    uint64 c = 0;
    for (; n; ++c)
        n &= n - 1;
    return c;
#endif
}




#endif 