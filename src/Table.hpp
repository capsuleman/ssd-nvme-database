#ifndef TABLE_HPP
#define TABLE_HPP

#include "Column.hpp"
#include "ChunkAllocator.hpp"

class Table
{
    ChunkAllocator chunk_allocator; // Chunk Allocator to generate new chunk
    unsigned int attribute_count;   // Number of int columns
    unsigned int value_count;       // Number of double columns
    std::vector<Column> columns;    // List of table's column

public:
    Table(unsigned int attribute_count, unsigned int value_count);
    ~Table() = default;

    unsigned int readInt(unsigned long int row_pos, unsigned int column);
    double readDouble(unsigned long int row_pos, unsigned int column);
    void writeInt(unsigned long int row_pos, unsigned int column, unsigned int value);
    void writeDouble(unsigned long int row_pos, unsigned int column, double value);
    void writeRow(unsigned long int row_pos, unsigned int *attributes, double *values);
    void writeRows(unsigned long int starting_row_pos, unsigned long int number_of_rows, unsigned int *attributes, double *values);

    std::vector<std::bitset<CHUNK_SIZE>> findRows(int *attribute_predicates, double *value_predicates);
};

#endif
