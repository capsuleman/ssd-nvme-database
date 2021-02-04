#include "Column.h"

Column::Column(MemoryAllocator *memory_allocator, bool is_double) : memory_allocator(memory_allocator), is_double(is_double)
{

};

Column::~Column(){};

int Column::readInt(int row_pos)
{
    // surely a more efficient way of doing this, look up java code
    // think of possible overflows or whatever
    int chunk_no = (int)row_pos/CHUNK_SIZE;
    int chunk_pos = row_pos % CHUNK_SIZE;
    return this->chunks[chunk_no].readInt(chunk_pos);
}

double Column::readDouble(int row_pos)
{
    int chunk_no = (int)row_pos/CHUNK_SIZE;
    int chunk_pos = row_pos % CHUNK_SIZE;
    return this->chunks[chunk_no].readDouble(chunk_pos);
}

void Column::writeInt(int row_pos, int value)
{
    int chunk_no = (int)row_pos/CHUNK_SIZE;
    int chunk_pos = row_pos % CHUNK_SIZE;
    this->chunks[chunk_no].writeInt(chunk_pos, value);
}

void Column::writeDouble(int row_pos, double value)
{
    int chunk_no = (int)row_pos/CHUNK_SIZE;
    int chunk_pos = row_pos % CHUNK_SIZE;
    this->chunks[chunk_no].writeDouble(chunk_pos, value);
}
