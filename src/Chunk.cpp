#include "Chunk.h"
#include <iostream>
#include <unistd.h>
#include <assert.h>

Chunk::Chunk(int fd, int starting_pos, bool is_double)
    : fd(fd), starting_pos(starting_pos), is_double(is_double)
{
    int number_written;
    if (is_double)
    {
        doubleContent = std::vector<double>();
        double zeroes[CHUNK_SIZE];
        for (int i = 0; i < CHUNK_SIZE; i++)
            zeroes[i] = 0.0;
        number_written = pwrite(fd, &zeroes, CHUNK_SIZE * sizeof(double), starting_pos);
    }
    else
    {
        intContent = std::vector<int>();
        int zeroes[CHUNK_SIZE];
        for (int i = 0; i < CHUNK_SIZE; i++)
            zeroes[i] = 0;
        number_written = pwrite(fd, &zeroes, CHUNK_SIZE * sizeof(int), starting_pos);
    }

    if (number_written == -1)
        std::cout << "Error creating a new chunk" << std::endl;

    std::cout << "Created empty chunk starting position " << starting_pos << std::endl;
}

Chunk::~Chunk()
{
}

void Chunk::load()
{
    if (is_double)
    {
        doubleContent.reserve(CHUNK_SIZE);
        pread(fd, &doubleContent[0], CHUNK_SIZE * sizeof(double), starting_pos);
    }
    else
    {
        intContent.reserve(CHUNK_SIZE);
        pread(fd, &intContent[0], CHUNK_SIZE * sizeof(int), starting_pos);
    }
}

void Chunk::unload()
{
    if (is_double)
    {
        doubleContent.clear();
        doubleContent.shrink_to_fit();
    }
    else
    {
        intContent.clear();
        intContent.shrink_to_fit();
    }
}

int Chunk::readInt(int chunk_pos)
{
    return intContent[chunk_pos];
}

double Chunk::readDouble(int chunk_pos)
{
    return doubleContent[chunk_pos];
}

void Chunk::writeInt(int chunk_pos, int value)
{
    int file_pos = starting_pos + chunk_pos * sizeof(int);
    pwrite(fd, &value, sizeof(int), file_pos);
}

void Chunk::writeDouble(int chunk_pos, double value)
{
    int file_pos = starting_pos + chunk_pos * sizeof(double);
    pwrite(fd, &value, sizeof(double), file_pos);
}
