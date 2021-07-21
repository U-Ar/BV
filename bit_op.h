#ifndef INCLUDED_BITOPERATIONS_H
#define INCLUDED_BITOPERATIONS_H

#include<string>
#include<iostream>

using uint64 = __UINT64_TYPE__;

inline uint64 popcount(uint64 n)
{
#ifdef __GNUC__
    return __builtin_popcountll(n); 
#else 
    uint64 c = 0;
    for (; n; ++c)
        n &= n - 1;
    return c;
#endif
}

#endif 