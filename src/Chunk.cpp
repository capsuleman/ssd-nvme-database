#include "Chunk.h"
#include <iostream>
#include <unistd.h>
#include <assert.h>

Chunk::Chunk(int fd, int starting_pos, bool is_double)
    : fd(fd), starting_pos(starting_pos), is_double(is_double)
{
    int zeroes[CHUNK_SIZE];
    for (int i = 0; i < CHUNK_SIZE; i++)
        zeroes[i] = 0;
    int number_written = pwrite(fd, &zeroes, CHUNK_SIZE * sizeof(int), starting_pos);
    if (number_written == -1)
        std::cout << "Error creating a new chunk" << std::endl;

    std::cout << "Created empty chunk starting position " << starting_pos << std::endl;
}

Chunk::~Chunk()
{
}

// load and unload should be private?
void Chunk::load()
{
    if (is_double)
        pread(fd, &doubleContent, CHUNK_SIZE * sizeof(double), starting_pos);
    else
        pread(fd, &intContent, CHUNK_SIZE * sizeof(int), starting_pos);
}

void Chunk::unload()
{
}

int Chunk::readInt(int chunk_pos)
{
    return intContent[chunk_pos];
}

double Chunk::readDouble(int chunk_pos)
{
    return 0;
}

void Chunk::writeInt(int chunk_pos, int value)
{
    int file_pos = starting_pos + chunk_pos * sizeof(int);
    pwrite(fd, &value, sizeof(int), file_pos);
}

void Chunk::writeDouble(int chunk_pos, double value)
{
}