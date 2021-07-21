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
    array = new char[size];
    N = (uint64)size;
    B = 8 * N;
}

BV::BV(std::string const& filename)
    : rank_enabled(false), select_enabled(false)
{
    std::ifstream ifs(filename, std::ios::binary);

    ifs.seekg(0,std::ios::end);
    long long int size = ifs.tellg();
    ifs.seekg(0);

    array = new char[size];
    ifs.read(array,size);

    N = (uint64)size;
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
        for (uint64 i = 0; i < num_area; i++) delete area_rank[i];
        delete[] area_rank;
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
            for (uint64 k = 0; k < BV::byte_per_chunk; k++)
            {
                acc_c += popcount(array[i*BV::byte_per_block+j*BV::byte_per_chunk+k]);
            }
        }
        acc_b += acc_c;
    }

    rank_enabled = true;
}

void BV::build_select()
{
    O = 0;
    for (size_t i = 0; i < N; i++) 
    {
        //O += popcount((uchar)(*this)[i]);
        O += rank_table[(uchar)(*this)[i]];
    }
    num_area = (O + area_ones - 1) / area_ones;
    area_rank = new SelectBlock*[num_area];
    
    uint64 left = 0, right = 0, idx = 0, ones = 0, pre_ones = 0;
    for (size_t i = 0; i < N; i++) 
    {
        pre_ones = ones;
        //ones += popcount((uchar)(*this)[i]);
        ones += rank_table[(uchar)(*this)[i]];
        if (ones >= area_ones)
        {
            for (size_t j = 0; j < 8; j++)
            {
                if (((uchar)(*this)[i]) & (1 << j)) {
                    pre_ones++;
                    if (pre_ones == area_ones) {
                        //ones = popcount(((uchar)(*this)[i]) >> (j+1));
                        ones = rank_table[((uchar)(*this)[i]) >> (j+1)];
                        right = i << 3 + j;
                        if (right - left >= boundary_size)
                        {
                            area_rank[idx++] = new SparseBlock(this,left,right,area_ones);
                        } else 
                        {
                            area_rank[idx++] = new DenseBlock(this,left,right,area_ones);
                        }
                        left = right + 1;
                        break;
                    }
                }
            }
        }
    }
    if (ones > 0) {
        right = B - 1;
        if (right - left >= boundary_size)
        {
            area_rank[idx++] = new SparseBlock(this,left,right,ones);
        } else 
        {
            area_rank[idx++] = new DenseBlock(this,left,right,ones);
        }
    }

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
    for (uint64 j = (i/BV::chunk_size) * BV::byte_per_chunk; j < ((i+8)>>3)-1; j++)
    {
        res += BV::rank_table[(uchar)(*this)[j]];
    }
    return res + BV::rank_table[(uchar)((*this)[(i>>3)] << (7 - (i%8)))];
}

uint64 BV::select(uint64 i)
{
    return area_rank[(i-1)/area_ones]->select(i%area_ones ? i%area_ones : area_ones);
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
    uint64 res = sizeof(SelectBlock*) * 8 * num_area;
    for (uint64 i = 0; i < num_area; i++) {
        res += area_rank[i]->space();
    }
    return res;
}

void BV::report_detail()
{
    std::cout << "BV::report_detail()" << std::endl;
    std::cout << "length: " << N << std::endl;
    std::cout << "total_bits: " << B << std::endl;
    std::cout << "first 4 chars: " << std::hex << +(*this)[0] << " " << +(*this)[1] << " " <<  +(*this)[2] 
    << " " << +(*this)[3] << std::dec << std::endl;
    std::cout << "report_detail done" << std::endl;
}








//---- SparseBlock ----

// [left,right]
SparseBlock::SparseBlock(const BV* bv, uint64 left, uint64 right, uint64 o)
    : ones(o)
{
    positions = new uint64[ones+1];
    uint64 idx = 1;
    positions[0] = 0;
    for (uint64 pos = left; pos <= right; pos++)
    {
        if (((uchar)(*bv)[pos>>3]) & (1 << (pos%8))) positions[idx++] = pos;
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






//---- DenseBlock ----

DenseBlock::DenseBlock(BV* ptr, uint64 l, uint64 r, uint64 o)
    : bv(ptr), ones(o), left(l), right(r), length(r-l+1), num_leaf((length+chunk_size-1)/chunk_size)
{
    num_node = 1;
    uint64 width = 1;
    while (width * arity < num_leaf)
    {
        width *= arity;
        // number of internal nodes & shallower leaves
        num_node += width;
    }
    // number of deeper leaves
    uint64 deeper = (r*(num_leaf-width)+r-2)/(r-1);
    num_node += deeper;

    tree = new uint32[num_node+1];


    //initialize for deeper leaves
    for (uint64 i = 0; i < deeper; i++)
    {
        // now using rank implicitly for building select index
        if (left == 0 && i == 0) 
            tree[num_node-deeper+1] = bv->rank(left + chunk_size - 1);
        else if (left + (i+1) * chunk_size - 1 > right)
            tree[i+num_node-deeper+1] = bv->rank(right) - bv->rank(left + i * chunk_size - 1);
        else 
            tree[i+num_node-deeper+1] = bv->rank(left + (i+1) * chunk_size - 1) - bv->rank(left + i * chunk_size - 1);
    }

    //initialize for shallower leaves
    for (uint64 i = 0; i < num_leaf - deeper; i++)
    {
        // now using rank implicitly for building select index
        if (left + (i+deeper+1) * chunk_size - 1 > right)
            tree[i+num_node-num_leaf+1] = bv->rank(right) - bv->rank(left + (i+deeper) * chunk_size - 1);
        else 
            tree[i+num_node-num_leaf+1] = bv->rank(left + (i+deeper+1) * chunk_size - 1) - bv->rank(left + (i+deeper) * chunk_size - 1);
    }

    //initialize for internal nodes
    for (uint64 i = num_node-num_leaf; i != 0; i--)
    {
        tree[i] = 0;
        for (uint64 j = 1; j <= arity; j++)
        {
            // children
            uint64 k = arity * i - arity + 1 + j;
            if (k > num_node) break;
            tree[i] += tree[k];
        }
    }
}

DenseBlock::~DenseBlock()
{
    delete[] tree;
}

uint64 DenseBlock::select(uint64 i)
{
    if (i == 0) return 0;
    uint64 p = 1, acc = 1, width = DenseBlock::arity;
    while (true)
    {
        if (DenseBlock::arity * (p-1) + 2 > num_node) {
            if (acc + width >= num_node) // deeper leaf
            {
                return leaf_select(i,i-acc);
            } 
            else // shallower leaf 
            {
                uint64 x = num_node - (acc + width);
                return leaf_select(i,x+i-(x+DenseBlock::arity-1)/DenseBlock::arity);
            }
        }
        // down to children
        for (uint64 q = DenseBlock::arity * (p-1) + 2; q < DenseBlock::arity * p + 2; q++)
        {
            if (i <= tree[q]) {
                p = q;
                break;
            }
            i -= tree[q];
        }
        acc += width;
        width *= DenseBlock::arity;
    }
    return std::numeric_limits<uint64>::max();
}

uint64 DenseBlock::leaf_select(uint64 i, uint64 leaf)
{
    uint64 end = left + leaf * DenseBlock::chunk_size;
    for (uint64 pos = left + (leaf-1) * DenseBlock::chunk_size; pos < end; pos++)
    {
        if (((uchar)(*bv)[pos>>3]) & (1 << (pos%8)))
            if (!(--i)) return pos;
    }
}

uint64 DenseBlock::space()
{
    return 32 * (num_node+1);
}