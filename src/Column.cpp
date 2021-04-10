#include "Column.hpp"

Column::Column(ChunkAllocator &chunk_allocator, bool is_double)
    : chunk_allocator(chunk_allocator),
      is_double(is_double)
{
}

unsigned int Column::readInt(unsigned long int row_pos)
{
    // surely a more efficient way of doing this, look up java code
    // think of possible overflows or whatever
    unsigned long int chunk_no = row_pos / CHUNK_SIZE;
    unsigned long int chunk_pos = row_pos % CHUNK_SIZE;
    chunks[chunk_no].load();
    int value = chunks[chunk_no].readInt(chunk_pos);
    chunks[chunk_no].unload();
    return value;
}

double Column::readDouble(unsigned long int row_pos)
{
    long unsigned long int chunk_no = row_pos / CHUNK_SIZE;
    unsigned long int chunk_pos = row_pos % CHUNK_SIZE;
    chunks[chunk_no].load();
    double value = chunks[chunk_no].readDouble(chunk_pos);
    chunks[chunk_no].unload();
    return value;
}

void Column::writeInts(unsigned long int starting_row_pos, unsigned long int number_of_rows, unsigned int *attributes)
{
    unsigned long int starting_chunk_pos = starting_row_pos;
    while (starting_chunk_pos - starting_row_pos < number_of_rows)
    {
        unsigned int number_of_values = CHUNK_SIZE - starting_chunk_pos % CHUNK_SIZE;
        unsigned long int remaining_number_of_values = number_of_rows - (starting_chunk_pos - starting_row_pos);
        if (number_of_values > remaining_number_of_values)
        {
            number_of_values = remaining_number_of_values;
        }
        unsigned int chunk_no = starting_chunk_pos / CHUNK_SIZE;
        if (chunks.size() <= chunk_no)
        {
            chunks.reserve(chunk_no);
        }
        while (chunks.size() <= chunk_no)
        {
            chunks.push_back(chunk_allocator.getChunk(false));
        }
        chunks[chunk_no].writeInts(
            starting_chunk_pos % CHUNK_SIZE,
            number_of_values,
            attributes + starting_chunk_pos - starting_row_pos);
        starting_chunk_pos += number_of_values;
    }
}

void Column::writeDoubles(
    unsigned long int starting_row_pos,
    unsigned long int number_of_rows,
    double *values)
{
    unsigned long int starting_chunk_pos = starting_row_pos;
    while (starting_chunk_pos - starting_row_pos < number_of_rows)
    {
        unsigned int number_of_values = CHUNK_SIZE - starting_chunk_pos % CHUNK_SIZE;
        unsigned long int remaining_number_of_values = number_of_rows - (starting_chunk_pos - starting_row_pos);
        if (number_of_values > remaining_number_of_values)
        {
            number_of_values = remaining_number_of_values;
        }
        unsigned int chunk_no = starting_chunk_pos / CHUNK_SIZE;
        if (chunks.size() <= chunk_no)
        {
            chunks.reserve(chunk_no);
        }
        while (chunks.size() < chunk_no + 1)
        {
            chunks.push_back(chunk_allocator.getChunk(true));
        }
        chunks[chunk_no].writeDoubles(starting_chunk_pos % CHUNK_SIZE, number_of_values, values + starting_chunk_pos - starting_row_pos);
        starting_chunk_pos += number_of_values;
    }
}

std::vector<std::bitset<CHUNK_SIZE>> Column::findIntRows(int predicate)
{
    std::vector<std::bitset<CHUNK_SIZE>> result = std::vector<std::bitset<CHUNK_SIZE>>();
    result.reserve(chunks.size());
    for (Chunk &chunk : chunks)
    {
        chunk.load();
        result.push_back(chunk.findInt(predicate));
        chunk.unload();
    }
    return result;
}

std::vector<std::bitset<CHUNK_SIZE>> Column::findDoubleRows(double predicate)
{
    std::vector<std::bitset<CHUNK_SIZE>> result = std::vector<std::bitset<CHUNK_SIZE>>();
    result.reserve(chunks.size());
    for (Chunk &chunk : chunks)
    {
        chunk.load();
        result.push_back(chunk.findDouble(predicate));
        chunk.unload();
    }
    return result;
}
