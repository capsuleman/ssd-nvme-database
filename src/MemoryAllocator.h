#ifndef __MEMORY_ALLOCATOR_H
#define __MEMORY_ALLOCATOR_H

#include "main.h"
#include "Chunk.h"

class MemoryAllocator
{
    int fd; // File descriptor where data is stored
    int last_line;

public:
    MemoryAllocator();
    ~MemoryAllocator();

    Chunk getChunk(bool is_double);
};

#endif