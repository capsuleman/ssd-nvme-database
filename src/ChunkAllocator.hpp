#ifndef CHUNK_ALLOCATOR_HPP
#define CHUNK_ALLOCATOR_HPP

#include "main.hpp"
#include "Chunk.hpp"

class ChunkAllocator
{
    int fd; // File descriptor where data is stored
    unsigned long int last_line;

public:
    ChunkAllocator();
    ~ChunkAllocator() = default;

    Chunk getChunk(bool is_double);
};

#endif