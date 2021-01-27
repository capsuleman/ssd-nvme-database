#ifndef __CHUNK_H
#define __CHUNK_H

class Chunk
{
    int fd;           // File descriptor where data is stored
    int starting_pos; // Starting position in file
    int nb_element;   // Current number of element in the chunk
    void *content;    // Buffer for reading data (could be nullptr when data is not loaded)

public:
    Chunk(int fd, int starting_pos);
    ~Chunk();

    void load();   // Load the content
    void unload(); // Unload the content
    int readInt(int chunk_pos);
    double readDouble(int chunk_pos);
    void writeInt(int chunk_pos, int value);
    void writeDouble(int chunk_pos, double value);
};

#endif
