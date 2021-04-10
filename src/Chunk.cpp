#include "Chunk.hpp"

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
        doubleCache.reset(nullptr);
        double zeroes[CHUNK_SIZE] = {};
        number_written = pwrite(fd, &zeroes, CHUNK_SIZE * sizeof(double), starting_pos);
    }
    else
    {
        intCache.reset(nullptr);
        int zeroes[CHUNK_SIZE] = {};
        number_written = pwrite(fd, &zeroes, CHUNK_SIZE * sizeof(int), starting_pos);
    }

    if (number_written == -1)
    {
        std::cout << "Error creating a new chunk starting at " << starting_pos << ": " << std::strerror(errno) << std::endl;
    }
}

Chunk::Chunk(Chunk &&other)
    : fd(other.fd),
      starting_pos(other.starting_pos),
      is_double(other.is_double),
      nb_element(other.nb_element),
      intCache(std::move(other.intCache)),
      doubleCache(std::move(other.doubleCache))
{
}

Chunk &Chunk::operator=(Chunk &&other)
{
    fd = other.fd;
    starting_pos = other.starting_pos;
    is_double = other.is_double;
    nb_element = other.nb_element;
    intCache = std::move(other.intCache);
    doubleCache = std::move(other.doubleCache);

    return *this;
}

void Chunk::load()
{
    int number_written = -1;
    if (is_double)
    {
        doubleCache.reset(new double[CHUNK_SIZE]);
        number_written = pread(fd, doubleCache.get(), CHUNK_SIZE * sizeof(double), starting_pos);
    }
    else
    {
        intCache.reset(new unsigned int[CHUNK_SIZE]);
        number_written = pread(fd, intCache.get(), CHUNK_SIZE * sizeof(int), starting_pos);
    }

    if (number_written == -1)
    {
        std::cout << "Error loading a chunk starting at " << starting_pos << ": " << std::strerror(errno) << std::endl;
    }
}

void Chunk::aload(io_uring *ring, unsigned long int chunk_no)
{
    struct io_uring_sqe *sqe;
    struct io_data *data = (io_data *)malloc(sizeof(*data));

    sqe = io_uring_get_sqe(ring);
    if (is_double)
    {
        doubleCache.reset(new double[CHUNK_SIZE]);
        io_uring_prep_read(sqe, fd, doubleCache.get(), CHUNK_SIZE * sizeof(double), starting_pos);
    }
    else
    {
        intCache.reset(new unsigned int[CHUNK_SIZE]);
        io_uring_prep_read(sqe, fd, intCache.get(), CHUNK_SIZE * sizeof(int), starting_pos);
    }
    data->chunk_no = chunk_no;
    io_uring_sqe_set_data(sqe, data);
    io_uring_submit(ring);
}

void Chunk::unload()
{
    if (is_double)
    {
        doubleCache.reset(nullptr);
    }
    else
    {
        intCache.reset(nullptr);
    }
}

unsigned int Chunk::readInt(unsigned long int chunk_pos) const
{
    return intCache[chunk_pos];
}

double Chunk::readDouble(unsigned long int chunk_pos) const
{
    return doubleCache[chunk_pos];
}

std::bitset<CHUNK_SIZE> Chunk::findInt(unsigned int predicate) const
{
    std::bitset<CHUNK_SIZE> result;
    unsigned int *intContent = intCache.get();
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
    double *doubleContent = doubleCache.get();
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
    int number_written = pwrite(fd, attributes, number_of_values * sizeof(int), file_pos);
    if (number_written == -1)
    {
        std::cout << "Error writing on chunk starting at " << starting_pos << ": " << std::strerror(errno) << std::endl;
    }
}

void Chunk::writeDoubles(unsigned int starting_chunk_pos, unsigned int number_of_values, double *values) const
{
    unsigned long int file_pos = starting_pos + starting_chunk_pos * sizeof(double);
    int number_written = pwrite(fd, values, number_of_values * sizeof(double), file_pos);
    if (number_written == -1)
    {
        std::cout << "Error writing on chunk starting at " << starting_pos << ": " << std::strerror(errno) << std::endl;
    }
}