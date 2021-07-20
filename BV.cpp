#include "BV.h"

const uint64 BV::rank_table[] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

BV::BV(size_t size)
    : rank_enabled(false), select_enabled(false)
{
    array = new uint64[size];
    N = (uint64)size;
    B = 64 * N;
}

BV::BV(std::string const& filename)
    : rank_enabled(false), select_enabled(false)
{
    std::ifstream ifs(filename, std::ios::binary);

    ifs.seekg(0,std::ios::end);
    long long int size = ifs.tellg();
    ifs.seekg(0);

    N = (uint64) (size + 7) / 8;
    B = 64 * N;

    array = new uint64[N];

    ifs.read(array,size);
    uint64 i = 0;
    char a, b, c, d, e, f, g, h;
    while (!ifs.eof())
    {
        ifs >> a >> b >> c >> d >> e >> f >> g >> h;
        array[i++] = (uint64) ( 
            (uchar) a | ((uchar) b) << 8 | ((uchar) c) << 16 | ((uchar) d) << 24 |
            ((uchar) e) << 32 | ((uchar) f) << 40 | ((uchar) g) << 48 | ((uchar) h) << 56
        );
    }

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

    block_rank = new PackedArray(num_block, BV::block_bits);
    chunk_rank = new PackedArray(num_chunk, BV::chunk_bits);

    uint64 acc_b = 0, acc_c;
    for (uint64 i = 0; i < num_block; i++)
    {
        block_rank->set(i,acc_b);
        acc_c = 0;
        for (uint64 j = 0; j < BV::block_size / BV::chunk_size; j++)
        {
            chunk_rank->set(i * BV::block_size / BV::chunk_size + j, acc_c);

            if (i == num_block-1 && j == num_chunk-1) break;
            for (uint64 k = 0; k < BV::word_per_chunk; k++)
            {
                acc_c += popcount(array[i*BV::word_per_block+j*BV::word_per_chunk+k]);
            }
        }
        acc_b += acc_c;
    }

    rank_enabled = true;
}

void BV::build_select()
{
    select_enabled = true;
}

uint64 BV::rank(uint64 i)
{
    //std::cout << block_rank->get(i/BV::block_size) << " " << chunk_rank->get(i/BV::chunk_size) << " ";
    return block_rank->get(i/BV::block_size) + chunk_rank->get(i/BV::chunk_size) + rem_rank(i);
}

uint64 BV::rem_rank(uint64 i)
{
    uint64 res = 0;
    for (uint64 j = (i/BV::chunk_size) * BV::word_per_chunk; j < ((i+8)>>3)-1; j++)
    {
        res += BV::rank_table[(uchar)(*this)[j]];
    }
    return res + BV::rank_table[(uchar)((*this)[(i>>3)] << (7 - (i%8)))];
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

void BV::report_detail()
{
    std::cout << "BV::report_detail()" << std::endl;
    std::cout << "length: " << N << std::endl;
    std::cout << "total_bits: " << B << std::endl;
    std::cout << "first word: " << std::hex << +(*this)[0] << std::dec << std::endl;
    std::cout << "report_detail done" << std::endl;
}