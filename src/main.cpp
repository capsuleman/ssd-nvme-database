#include <chrono>
#include <iostream>

#include "Table.hpp"

int main(int argc, char **argv)
{
    const unsigned long int number_of_rows = 1024 * 1024;
    const unsigned long int write_block_size = 128 * 1024;
    const unsigned int attributes_count = 2;
    const unsigned int values_count = 2;

    Table table{attributes_count, values_count};
    auto start_write = std::chrono::high_resolution_clock::now();
    for (unsigned long int i = 0; i < number_of_rows; i += write_block_size)
    {
        unsigned int attributes[write_block_size * attributes_count];
        double values[write_block_size * values_count];
        for (unsigned long int j = 0; j < write_block_size; j++)
        {
            attributes[write_block_size * 0 + j] = i + j;
            attributes[write_block_size * 1 + j] = 2 * (i + j);
            values[write_block_size * 0 + j] = 0.1 + i + j;
            values[write_block_size * 1 + j] = 1.0 * (i + j);
        }
        table.writeRows(i, write_block_size, attributes, values);
    }
    auto end_write = std::chrono::high_resolution_clock::now();

    int row_to_find = 19542;
    int attribute_predicates[2] = {row_to_find, 2 * row_to_find};
    double value_predicates[2] = {0.1 + row_to_find, 1.0 * row_to_find};
    auto start_read = std::chrono::high_resolution_clock::now();
    auto result = table.findRows(attribute_predicates, value_predicates);

    auto end_read = std::chrono::high_resolution_clock::now();

    for (long unsigned int i = 0; i < result.size(); i++)
    {
        for (long unsigned int j = 0; j < CHUNK_SIZE; j++)
        {
            if (result[i][j] != (i * CHUNK_SIZE + j == row_to_find))
            {
                return 1;
            }
        }
    }

    auto elapsed_seconds_read = std::chrono::duration_cast<std::chrono::duration<double>>(end_read - start_read);
    auto elapsed_seconds_write = std::chrono::duration_cast<std::chrono::duration<double>>(end_write - start_write);
    unsigned long int bytes_handled = number_of_rows * (attributes_count * sizeof(int) + values_count * sizeof(double));

    std::cout << "Number of bytes handled: " << bytes_handled << std::endl;
    std::cout << "Read in " << elapsed_seconds_read.count() << "s (" << bytes_handled / elapsed_seconds_read.count() / 1e9 << " Go/s)" << std::endl;
    std::cout << "Written in " << elapsed_seconds_write.count() << "s (" << bytes_handled / elapsed_seconds_write.count() / 1e9 << " Go/s)" << std::endl;

    return 0;
}
