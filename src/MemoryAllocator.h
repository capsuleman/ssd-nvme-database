#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include "main.h"
#include "Chunk.h"

class MemoryAllocator
{
    unsigned long int last_line;

public:
    int fd; // File descriptor where data is stored
    MemoryAllocator();
    ~MemoryAllocator() = default;

    Chunk getChunk(bool is_double);
};

#endif