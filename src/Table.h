#ifndef TABLE_H
#define TABLE_H

#include <vector>

#include "Column.h"
#include "MemoryAllocator.h"

class Table
{
    MemoryAllocator memory_allocator; // Memory Allocator to generate new chunk
    int attribute_count;              // Number of int columns
    int value_count;                  // Number of double columns
    std::vector<Column> columns;      // List of table's column

public:
    Table(int attribute_count, int value_count);
    ~Table() = default;

    int readInt(int row_pos, int column);
    double readDouble(int row_pos, int column);
    void writeInt(int row_pos, int column, int value);
    void writeDouble(int row_pos, int column, double value);
    void writeRow(int row_pos, int *attributes, double *values);

    std::vector<std::bitset<CHUNK_SIZE>> findRows(int *attribute_predicates, double *value_predicates);
};

#endif
