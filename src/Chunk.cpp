#include "Chunk.h"

#include <cerrno>
#include <cstring>
#include <iostream>
#include <unistd.h>

Chunk::Chunk(int fd, int starting_pos, bool is_double)
    : fd(fd),
      starting_pos(starting_pos),
      is_double(is_double)
{
    int number_written = -1;
    if (is_double)
    {
        doubleContent.reset(nullptr);
        double zeroes[CHUNK_SIZE];
        for (int i = 0; i < CHUNK_SIZE; i++)
        {
            zeroes[i] = 0.0;
        }
        number_written = pwrite(fd, &zeroes, CHUNK_SIZE * sizeof(double), starting_pos);
    }
    else
    {
        intContent.reset(nullptr);
        int zeroes[CHUNK_SIZE];
        for (int i = 0; i < CHUNK_SIZE; i++)
        {
            zeroes[i] = 0;
        }
        number_written = pwrite(fd, &zeroes, CHUNK_SIZE * sizeof(int), starting_pos);
    }

    if (number_written == -1)
    {
        std::cout << "Error creating a new chunk starting at " << starting_pos << " (" << std::strerror(errno) << ")" << std::endl;
    }
}

Chunk::Chunk(Chunk &&other)
    : fd(other.fd),
      starting_pos(other.starting_pos),
      is_double(other.is_double),
      nb_element(other.nb_element),
      intContent(std::move(other.intContent)),
      doubleContent(std::move(other.doubleContent))
{
}

Chunk &Chunk::operator=(Chunk &&other)
{
    fd = other.fd;
    starting_pos = other.starting_pos;
    is_double = other.is_double;
    nb_element = other.nb_element;
    intContent = std::move(other.intContent);
    doubleContent = std::move(other.doubleContent);

    return *this;
}

void Chunk::load()
{
    if (is_double)
    {
        doubleContent.reset(new double[CHUNK_SIZE]);
        pread(fd, &doubleContent[0], CHUNK_SIZE * sizeof(double), starting_pos);
    }
    else
    {
        intContent.reset(new int[CHUNK_SIZE]);
        pread(fd, &intContent[0], CHUNK_SIZE * sizeof(int), starting_pos);
    }
}

void Chunk::unload()
{
    if (is_double)
    {
        doubleContent.reset(nullptr);
    }
    else
    {
        intContent.reset(nullptr);
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

void Chunk::writeInts(unsigned int starting_chunk_pos, unsigned int number_of_values, int *attributes)
{
    unsigned int file_pos = starting_pos + starting_chunk_pos * sizeof(int);
    int number_written = pwrite(fd, attributes, number_of_values * sizeof(int), file_pos);
}

void Chunk::writeDoubles(unsigned int starting_chunk_pos, unsigned int number_of_values, double *values)
{
    unsigned int file_pos = starting_pos + starting_chunk_pos * sizeof(double);
    int number_written = pwrite(fd, values, number_of_values * sizeof(double), file_pos);
}