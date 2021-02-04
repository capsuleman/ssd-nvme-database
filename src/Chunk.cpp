#include "Chunk.h"

Chunk::Chunk(int fd, int starting_pos) : fd(fd), starting_pos(starting_pos)
{
}

Chunk::~Chunk()
{
}


// load and unload should be private?
void Chunk::load()
{
}

void Chunk::unload()
{
}

int Chunk::readInt(int chunk_pos)
{
    // if chunk is not loaded, call load
    // read value
    return 0;
}

double Chunk::readDouble(int chunk_pos)
{
    return 0;
}

void Chunk::writeInt(int chunk_pos, int value)
{
}

void Chunk:: writeDouble(int chunk_pos, double value)
{
}