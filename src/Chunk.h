#ifndef __CHUNK_H
#define __CHUNK_H

#include <cstddef>
#include <vector>

#include "main.h"

class Chunk
{
    int fd;                            // File descriptor where data is stored
    int starting_pos;                  // Starting position in file
    bool is_double;                    // Is storing int or double
    int nb_element;                    // Current number of element in the chunk
    std::vector<int> intContent;       // Cache vector for reading data (could be empty when data is not loaded)
    std::vector<double> doubleContent; // Same for double

public:
    Chunk(int fd, int starting_pos, bool is_double);
    ~Chunk();

    void load();   // Load the content
    void unload(); // Unload the content
    int readInt(int chunk_pos);
    double readDouble(int chunk_pos);
    void writeInt(int chunk_pos, int value);
    void writeDouble(int chunk_pos, double value);
};

#endif
