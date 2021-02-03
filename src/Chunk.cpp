#include "Chunk.h"

Chunk::Chunk(int fd, int starting_pos) : fd(fd), starting_pos(starting_pos)
{
}

Chunk::~Chunk()
{
}
