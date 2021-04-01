#ifndef COLUMN_H
#define COLUMN_H

#include <bitset>
#include <vector>
#include <liburing.h>

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
    ~Column() = default;

    unsigned int readInt(unsigned long int row_pos);
    double readDouble(unsigned long int row_pos);
    void writeInts(unsigned long int starting_row_pos, unsigned long int number_of_rows, unsigned int *attributes);
    void writeDoubles(unsigned long int starting_row_pos, unsigned long int number_of_rows, double *values);

    std::vector<std::bitset<CHUNK_SIZE>> findIntRows(int predicate);
    std::vector<std::bitset<CHUNK_SIZE>> findDoubleRows(double predicate);

    int submit_read_request(struct io_uring *ring);
    int get_read(struct io_uring *ring);
};

#endif
