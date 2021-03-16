#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include "main.h"
#include "Chunk.h"

class MemoryAllocator
{
    int fd; // File descriptor where data is stored
    unsigned long int last_line;

public:
    MemoryAllocator();
    ~MemoryAllocator() = default;

    Chunk getChunk(bool is_double);
};

#endif