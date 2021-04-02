#include "Chunk.h"

#include <cerrno>
#include <cstring>
#include <iostream>
#include <unistd.h>

Chunk::Chunk(int fd, unsigned long int starting_pos, bool is_double)
    : fd(fd),
      starting_pos(starting_pos),
      is_double(is_double)
{
    int number_written = -1;
    if (is_double)
    {
        doubleContent.reset(nullptr);
        double zeroes[CHUNK_SIZE] = {};
        number_written = pwrite(fd, &zeroes, CHUNK_SIZE * sizeof(double), starting_pos);
    }
    else
    {
        intContent.reset(nullptr);
        int zeroes[CHUNK_SIZE] = {};
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
        intContent.reset(new unsigned int[CHUNK_SIZE]);
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

unsigned int Chunk::readInt(unsigned long int chunk_pos) const
{
    return intContent[chunk_pos];
}

double Chunk::readDouble(unsigned long int chunk_pos) const
{
    return doubleContent[chunk_pos];
}

std::bitset<CHUNK_SIZE> Chunk::findInt(unsigned int predicate) const
{
    std::bitset<CHUNK_SIZE> result;
    for (unsigned long int i = 0; i < CHUNK_SIZE; i++)
    {
        if (predicate == intContent[i])
        {
            result.set(i);
        }
    }
    return result;
}

std::bitset<CHUNK_SIZE> Chunk::findDouble(double predicate) const
{
    std::bitset<CHUNK_SIZE> result;
    for (unsigned long int i = 0; i < CHUNK_SIZE; i++)
    {
        if (predicate == doubleContent[i])
        {
            result.set(i);
        }
    }
    return result;
}

void Chunk::writeInts(unsigned int starting_chunk_pos, unsigned int number_of_values, unsigned int *attributes) const
{
    unsigned long int file_pos = starting_pos + starting_chunk_pos * sizeof(int);
    pwrite(fd, attributes, number_of_values * sizeof(int), file_pos);
}

void Chunk::writeDoubles(unsigned int starting_chunk_pos, unsigned int number_of_values, double *values) const
{
    unsigned long int file_pos = starting_pos + starting_chunk_pos * sizeof(double);
    pwrite(fd, values, number_of_values * sizeof(double), file_pos);
}