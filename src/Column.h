#include <bitset>
#include "main.h"
#include "Chunk.h"

class Column
{
    int fd;                 // File descriptor where data is stored
    IntegerChunk *chunks[]; // Chunks constituting the column

public:
    Column(int fd);
    ~Column();

    int readInt(int row_pos);
    double readDouble(int row_pos);
    void writeInt(int row_pos, int value);
    void writeDouble(int row_pos, double value);
    std::bitset<CHUNK_SIZE> *findRows(int predicate);
};
