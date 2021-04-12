#include <cstring>

#include "Column.hpp"

Column::Column(ChunkAllocator &chunk_allocator, bool is_double)
    : chunk_allocator(chunk_allocator),
      is_double(is_double)
{
    int ret = io_uring_queue_init(QUEUE_DEPTH, &_ring, 0);
    if (ret < 0)
    {
        std::cout << "Issue on queue init: " << std::strerror(-ret) << std::endl;
    }
}

unsigned int Column::readInt(unsigned long int row_pos)
{
    unsigned long int chunk_no = row_pos / CHUNK_SIZE;
    unsigned long int chunk_pos = row_pos % CHUNK_SIZE;
    chunks[chunk_no].load();
    int value = chunks[chunk_no].readInt(chunk_pos);
    chunks[chunk_no].unload();
    return value;
}

double Column::readDouble(unsigned long int row_pos)
{
    long unsigned long int chunk_no = row_pos / CHUNK_SIZE;
    unsigned long int chunk_pos = row_pos % CHUNK_SIZE;
    chunks[chunk_no].load();
    double value = chunks[chunk_no].readDouble(chunk_pos);
    chunks[chunk_no].unload();
    return value;
}

void Column::writeInts(unsigned long int starting_row_pos, unsigned long int number_of_rows, unsigned int *attributes)
{
    unsigned long int starting_chunk_pos = starting_row_pos;
    while (starting_chunk_pos - starting_row_pos < number_of_rows)
    {
        unsigned int number_of_values = CHUNK_SIZE - starting_chunk_pos % CHUNK_SIZE;
        unsigned long int remaining_number_of_values = number_of_rows - (starting_chunk_pos - starting_row_pos);
        if (number_of_values > remaining_number_of_values)
        {
            number_of_values = remaining_number_of_values;
        }
        unsigned int chunk_no = starting_chunk_pos / CHUNK_SIZE;
        if (chunks.size() <= chunk_no)
        {
            chunks.reserve(chunk_no);
        }
        while (chunks.size() <= chunk_no)
        {
            chunks.push_back(chunk_allocator.getChunk(false));
        }
        chunks[chunk_no].writeInts(
            starting_chunk_pos % CHUNK_SIZE,
            number_of_values,
            attributes + starting_chunk_pos - starting_row_pos);
        starting_chunk_pos += number_of_values;
    }
}

void Column::writeDoubles(
    unsigned long int starting_row_pos,
    unsigned long int number_of_rows,
    double *values)
{
    unsigned long int starting_chunk_pos = starting_row_pos;
    while (starting_chunk_pos - starting_row_pos < number_of_rows)
    {
        unsigned int number_of_values = CHUNK_SIZE - starting_chunk_pos % CHUNK_SIZE;
        unsigned long int remaining_number_of_values = number_of_rows - (starting_chunk_pos - starting_row_pos);
        if (number_of_values > remaining_number_of_values)
        {
            number_of_values = remaining_number_of_values;
        }
        unsigned int chunk_no = starting_chunk_pos / CHUNK_SIZE;
        if (chunks.size() <= chunk_no)
        {
            chunks.reserve(chunk_no);
        }
        while (chunks.size() < chunk_no + 1)
        {
            chunks.push_back(chunk_allocator.getChunk(true));
        }
        chunks[chunk_no].writeDoubles(starting_chunk_pos % CHUNK_SIZE, number_of_values, values + starting_chunk_pos - starting_row_pos);
        starting_chunk_pos += number_of_values;
    }
}

unsigned long int Column::collectOne(bool is_blocking)
{
    struct io_uring_cqe *cqe;

    io_uring_wait_cqe(&_ring, &cqe);
    if (cqe->res < 0)
    {
        std::cout << "IO issue: " << std::strerror(-cqe->res) << std::endl;
    }

    struct io_data *data = (io_data *)io_uring_cqe_get_data(cqe);
    io_uring_cqe_seen(&_ring, cqe);

    unsigned long int chunk_no = data->chunk_no;
    free(data);

    return chunk_no;
}

std::vector<std::bitset<CHUNK_SIZE>> Column::findIntRowsSync(int predicate, bool without_loading)
{
    const unsigned long int number_of_chunks = chunks.size();
    std::vector<std::bitset<CHUNK_SIZE>> result = std::vector<std::bitset<CHUNK_SIZE>>();
    result.reserve(number_of_chunks);
    for (unsigned long int i = 0; i < number_of_chunks; i++)
    {
        if (without_loading)
        {
            result[i] = chunks[i].findInt(predicate);
        }
        else
        {
            chunks[i].load();
            result[i] = chunks[i].findInt(predicate);
            chunks[i].unload();
        }
    }
    return result;
}

std::vector<std::bitset<CHUNK_SIZE>> Column::findIntRowsAsync(int predicate)
{
    const unsigned long int number_of_chunks = chunks.size();
    std::vector<std::bitset<CHUNK_SIZE>> result = std::vector<std::bitset<CHUNK_SIZE>>();
    result.reserve(number_of_chunks);
    unsigned int current_queue_length = 0;
    unsigned long int i = 0;

    while ((i < number_of_chunks) || (current_queue_length > 0))
    {
        while ((current_queue_length < QUEUE_DEPTH) && (i < number_of_chunks))
        {
            chunks[i].aload(&_ring, i);
            current_queue_length++;
            i++;
        }
        unsigned long int chunk_no = collectOne(true);
        result[chunk_no] = chunks[chunk_no].findInt(predicate);
        chunks[chunk_no].unload();
        current_queue_length--;
    }
    return result;
}

std::vector<std::bitset<CHUNK_SIZE>> Column::findIntRows(int predicate, bool use_async, bool without_loading)
{
    if (use_async)
    {
        return findIntRowsAsync(predicate);
    }
    return findIntRowsSync(predicate, without_loading);
}

std::vector<std::bitset<CHUNK_SIZE>> Column::findDoubleRowsSync(double predicate, bool without_loading)
{
    const unsigned long int number_of_chunks = chunks.size();
    std::vector<std::bitset<CHUNK_SIZE>> result = std::vector<std::bitset<CHUNK_SIZE>>();
    result.reserve(number_of_chunks);
    for (unsigned long int i = 0; i < number_of_chunks; i++)
    {
        if (without_loading)
        {
            result[i] = chunks[i].findInt(predicate);
        }
        else
        {
            chunks[i].load();
            result[i] = chunks[i].findInt(predicate);
            chunks[i].unload();
        }
    }
    return result;
}

std::vector<std::bitset<CHUNK_SIZE>> Column::findDoubleRowsAsync(double predicate)
{
    const unsigned long int number_of_chunks = chunks.size();
    std::vector<std::bitset<CHUNK_SIZE>> result = std::vector<std::bitset<CHUNK_SIZE>>();
    result.reserve(number_of_chunks);
    unsigned int current_queue_length = 0;
    unsigned long int i = 0;

    while ((i < number_of_chunks) || (current_queue_length > 0))
    {
        while ((current_queue_length < QUEUE_DEPTH) && (i < number_of_chunks))
        {
            chunks[i].aload(&_ring, i);
            current_queue_length++;
            i++;
        }
        unsigned long int chunk_no = collectOne(true);
        result[chunk_no] = chunks[chunk_no].findDouble(predicate);
        chunks[chunk_no].unload();
        current_queue_length--;
    }
    return result;
}

std::vector<std::bitset<CHUNK_SIZE>> Column::findDoubleRows(double predicate, bool use_async, bool without_loading)
{
    if (use_async)
    {
        return findDoubleRowsAsync(predicate);
    }
    return findDoubleRowsSync(predicate, without_loading);
}

void Column::loadEverything()
{
    for (auto &chunk : chunks)
    {
        chunk.load();
    }
}

void Column::unloadEverything()
{
    for (auto &chunk : chunks)
    {
        chunk.load();
    }
}