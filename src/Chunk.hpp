#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <bitset>
#include <cstddef>
#include <liburing.h>
#include <memory>
#include <vector>

#include "main.hpp"

struct io_data
{
    unsigned long int chunk_no;
};

class Chunk
{
    int fd;                                   // File descriptor where data is stored
    unsigned long int starting_pos;           // Starting position in file
    bool is_double;                           // Is storing int or double
    unsigned int nb_element;                  // Current number of element in the chunk
    std::unique_ptr<unsigned int[]> intCache; // Cache for reading data (could be nullptr when data is not loaded)
    std::unique_ptr<double[]> doubleCache;    // Same for double

public:
    Chunk(int fd, unsigned long int starting_pos, bool is_double);
    Chunk(Chunk &&);
    Chunk &operator=(Chunk &&);
    ~Chunk() = default;

    void load();                                            // Load the content
    void aload(io_uring *ring, unsigned long int chunk_no); // Asynchronous load()
    void unload();                                          // Unload the content
    unsigned int readInt(unsigned long int chunk_pos) const;
    double readDouble(unsigned long int chunk_pos) const;
    std::bitset<CHUNK_SIZE> findInt(unsigned int predicate) const;
    std::bitset<CHUNK_SIZE> findDouble(double predicate) const;
    void writeInts(unsigned starting_chunk_pos, unsigned int number_of_values, unsigned int *attributes) const;
    void writeDoubles(unsigned starting_chunk_pos, unsigned int number_of_values, double *values) const;
};

#endif
