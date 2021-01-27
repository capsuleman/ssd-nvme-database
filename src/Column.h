#ifndef __COLUMN_H
#define __COLUMN_H

#include <bitset>
#include <vector>
#include "main.h"
#include "Chunk.h"

// specify if double or single on creation
class Column
{
    int fd; // File descriptor where data is stored
    int isDouble;
    std::vector<Chunk> chunks; // Chunks constituting the column

public:
    Column(int fd, int isDouble);
    ~Column();

    int readInt(int row_pos);
    double readDouble(int row_pos);
    void writeInt(int row_pos, int value);
    void writeDouble(int row_pos, double value);
    std::bitset<CHUNK_SIZE> *findRows(int predicate);
};

#endif
