#include <cstdio>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "ChunkAllocator.h"

ChunkAllocator::ChunkAllocator()
{
    char nameBuff[32];
    memset(nameBuff, 0, sizeof(nameBuff));
    strncpy(nameBuff, "/tmp/ssd-bdd-XXXXXX", 21);
    fd = mkstemp(nameBuff);
    unlink(nameBuff);
    last_line = 0;
}

Chunk ChunkAllocator::getChunk(bool is_double)
{
    Chunk new_chunk{fd, last_line, is_double};
    last_line += CHUNK_SIZE * (is_double ? sizeof(double) : sizeof(int));
    return new_chunk;
}