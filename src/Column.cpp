#include "Column.h"
#define QUEUE_DEPTH 1

Column::Column(MemoryAllocator &memory_allocator, bool is_double)
    : memory_allocator(memory_allocator),
      is_double(is_double)
{
}

unsigned int Column::readInt(unsigned long int row_pos)
{
    // surely a more efficient way of doing this, look up java code
    // think of possible overflows or whatever
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
            chunks.push_back(memory_allocator.getChunk(false));
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
            chunks.push_back(memory_allocator.getChunk(true));
        }
        chunks[chunk_no].writeDoubles(starting_chunk_pos % CHUNK_SIZE, number_of_values, values + starting_chunk_pos - starting_row_pos);
        starting_chunk_pos += number_of_values;
    }
}


struct file_info {
    off_t file_sz;
    struct iovec iovecs[];      /* Referred by readv/writev */
};

int Column::submit_read_request(struct io_uring *ring) {
    struct file_info *fi;
    io_uring_queue_init(QUEUE_DEPTH, ring, 0);
    int fd = memory_allocator.fd;
    off_t bytes_remaining = chunks.size() * CHUNK_SIZE * sizeof(int);
    off_t offset = 0;
    int current_chunk = 0;
    while (bytes_remaining) {
        off_t bytes_to_read = bytes_remaining;
        if (bytes_to_read > CHUNK_SIZE)
            bytes_to_read = CHUNK_SIZE;
        offset += bytes_to_read;
        fi->iovecs[current_chunk].iov_len = bytes_to_read;
        void *buf;
        fi->iovecs[current_chunk].iov_base = buf;
        current_chunk++;
        bytes_remaining -= bytes_to_read;
    }
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);

    io_uring_prep_readv(sqe, fd, fi->iovecs, chunks.size(), 0);

    io_uring_sqe_set_data(sqe, fi);

    io_uring_submit(ring);

    return 0;
}

int Column::get_read(struct io_uring *ring) {
    struct io_uring_cqe *cqe;
    int ret = io_uring_wait_cqe(ring, &cqe);
    if (ret < 0) {
        perror("io_uring_wait_cqe");
        return 1;
    }
    struct file_info *fi = io_uring_cqe_get_data(cqe);

    // read data from fi adn return that?


    return 0;
}

std::vector<std::bitset<CHUNK_SIZE>> Column::findIntRows(int predicate)
{
    std::vector<std::bitset<CHUNK_SIZE>> result = std::vector<std::bitset<CHUNK_SIZE>>(chunks.size());
    struct io_uring ring;
    int ret = submit_read_request(&ring);
    

}

std::vector<std::bitset<CHUNK_SIZE>> Column::findDoubleRows(double predicate)
{
    std::vector<std::bitset<CHUNK_SIZE>> result = std::vector<std::bitset<CHUNK_SIZE>>(chunks.size());
    for (unsigned long int chunk_no = 0; chunk_no < chunks.size(); chunk_no++)
    {
        chunks[chunk_no].load();
        for (unsigned long int chunk_pos = 0; chunk_pos < CHUNK_SIZE; chunk_pos++)
        {
            result[chunk_no][chunk_pos] = chunks[chunk_no].readDouble(chunk_pos) == predicate;
        }
        chunks[chunk_no].unload();
    }
    return result;
}
