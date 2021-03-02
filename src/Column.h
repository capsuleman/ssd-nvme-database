#ifndef __COLUMN_H
#define __COLUMN_H

#include <bitset>
#include <vector>
#include "main.h"
#include "Chunk.h"
#include "MemoryAllocator.h"

// specify if double or single on creation
class Column
{
    MemoryAllocator &memory_allocator; // Memory Allocator to generate new chunk
    bool is_double;                    // Integer or double column
    std::vector<Chunk> chunks;         // Chunks constituting the column

public:
    Column(MemoryAllocator &memory_allocator, bool is_double);
    Column(Column &&) = default;
    Column &operator=(Column &&) = default;

    ~Column();

    int readInt(int row_pos);
    double readDouble(int row_pos);
    void writeInt(int row_pos, int value);
    void writeDouble(int row_pos, double value);
    std::bitset<CHUNK_SIZE> *findRows(int predicate);
};

#endif
