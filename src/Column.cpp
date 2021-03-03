#include "Column.h"

Column::Column(MemoryAllocator &memory_allocator, bool is_double)
    : memory_allocator(memory_allocator), is_double(is_double) {}

int Column::readInt(int row_pos)
{
    // surely a more efficient way of doing this, look up java code
    // think of possible overflows or whatever
    int chunk_no = row_pos / CHUNK_SIZE;
    int chunk_pos = row_pos % CHUNK_SIZE;
    chunks[chunk_no].load();
    int value = chunks[chunk_no].readInt(chunk_pos);
    chunks[chunk_no].unload();
    return value;
}

double Column::readDouble(int row_pos)
{
    int chunk_no = row_pos / CHUNK_SIZE;
    int chunk_pos = row_pos % CHUNK_SIZE;
    chunks[chunk_no].load();
    double value = chunks[chunk_no].readDouble(chunk_pos);
    chunks[chunk_no].unload();
    return value;
}

void Column::writeInt(int row_pos, int value)
{
    unsigned int chunk_no = row_pos / CHUNK_SIZE;
    unsigned int chunk_pos = row_pos % CHUNK_SIZE;

    if (chunks.size() < chunk_no + 1)
    {
        chunks.push_back(memory_allocator.getChunk(false));
    }

    chunks[chunk_no].writeInt(chunk_pos, value);
}

void Column::writeDouble(int row_pos, double value)
{
    unsigned int chunk_no = row_pos / CHUNK_SIZE;
    unsigned int chunk_pos = row_pos % CHUNK_SIZE;

    if (chunks.size() < chunk_no + 1)
    {
        chunks.push_back(memory_allocator.getChunk(true));
    }

    chunks[chunk_no].writeDouble(chunk_pos, value);
}
