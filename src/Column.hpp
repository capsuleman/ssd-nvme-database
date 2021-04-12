#ifndef COLUMN_HPP
#define COLUMN_HPP

#include <bitset>
#include <iostream>
#include <liburing.h>
#include <vector>

#include "main.hpp"
#include "Chunk.hpp"
#include "ChunkAllocator.hpp"

// specify if double or single on creation
class Column
{
    ChunkAllocator &chunk_allocator; // Memory Allocator to generate new chunk
    bool is_double;                  // Integer or double column
    std::vector<Chunk> chunks;       // Chunks constituting the column
    struct io_uring _ring;           // IO_uring ring

    unsigned long int collectOne(bool is_blocking);
    std::vector<std::bitset<CHUNK_SIZE>> findIntRowsAsync(int predicate);
    std::vector<std::bitset<CHUNK_SIZE>> findIntRowsSync(int predicate, bool without_loading);
    std::vector<std::bitset<CHUNK_SIZE>> findDoubleRowsAsync(double predicate);
    std::vector<std::bitset<CHUNK_SIZE>> findDoubleRowsSync(double predicate, bool without_loading);

public:
    Column(ChunkAllocator &chunk_allocator, bool is_double);
    Column(Column &&) = default;
    Column &operator=(Column &&) = default;
    ~Column() = default;

    unsigned int readInt(unsigned long int row_pos);
    double readDouble(unsigned long int row_pos);
    void writeInts(unsigned long int starting_row_pos, unsigned long int number_of_rows, unsigned int *attributes);
    void writeDoubles(unsigned long int starting_row_pos, unsigned long int number_of_rows, double *values);

    std::vector<std::bitset<CHUNK_SIZE>> findIntRows(int predicate, bool use_async, bool without_loading);
    std::vector<std::bitset<CHUNK_SIZE>> findDoubleRows(double predicate, bool use_async, bool without_loading);

    void loadEverything();
    void unloadEverything();
};

#endif
