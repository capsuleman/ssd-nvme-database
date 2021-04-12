#include <chrono>
#include <iostream>
#include <omp.h>

#include "Table.hpp"

int main(int argc, char **argv)
{
    std::cout << "Number of thread available: " << omp_get_max_threads() << std::endl;

    const unsigned long int number_of_rows = 1024 * 1024 * 1024;
    const unsigned long int write_block_size = 128 * 1024;
    const unsigned int attributes_count = 2;
    const unsigned int values_count = 2;

    unsigned long int gigabytes_handled = number_of_rows * (attributes_count * sizeof(int) + values_count * sizeof(double)) / (1024 * 1024 * 1024);
    std::cout << "Number of gigabytes handled: " << gigabytes_handled << std::endl;

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
    auto elapsed_seconds_write = std::chrono::duration_cast<std::chrono::duration<double>>(end_write - start_write);
    std::cout << "Written in " << elapsed_seconds_write.count() << "s (" << gigabytes_handled / elapsed_seconds_write.count() << " Go/s)" << std::endl;

    const int row_to_find = 19542;
    int attribute_predicates[2] = {row_to_find, 2 * row_to_find};
    double value_predicates[2] = {0.1 + row_to_find, 1.0 * row_to_find};

    auto start_find_sync = std::chrono::high_resolution_clock::now();
    auto result_sync = table.findRows(attribute_predicates, value_predicates, false, false);
    auto end_find_sync = std::chrono::high_resolution_clock::now();
    // Checking solution
    for (long unsigned int i = 0; i < result_sync.size(); i++)
    {
        for (long unsigned int j = 0; j < CHUNK_SIZE; j++)
        {
            if (result_sync[i][j] != (i * CHUNK_SIZE + j == row_to_find))
            {
                return 1;
            }
        }
    }
    auto elapsed_seconds_find_sync = std::chrono::duration_cast<std::chrono::duration<double>>(end_find_sync - start_find_sync);
    std::cout << "Sync find in " << elapsed_seconds_find_sync.count() << "s (" << gigabytes_handled / elapsed_seconds_find_sync.count() << " Go/s)" << std::endl;

    auto start_find_async = std::chrono::high_resolution_clock::now();
    auto result_async = table.findRows(attribute_predicates, value_predicates, true, false);
    auto end_find_async = std::chrono::high_resolution_clock::now();
    // Checking solution
    for (long unsigned int i = 0; i < result_async.size(); i++)
    {
        if (result_sync[i] != result_async[i])
        {
            return 1;
        }
    }
    auto elapsed_seconds_find_async = std::chrono::duration_cast<std::chrono::duration<double>>(end_find_async - start_find_async);
    std::cout << "Async find in " << elapsed_seconds_find_async.count() << "s (" << gigabytes_handled / elapsed_seconds_find_async.count() << " Go/s)" << std::endl;

    auto start_load = std::chrono::high_resolution_clock::now();
    table.loadEverything();
    auto end_load = std::chrono::high_resolution_clock::now();
    auto elapsed_seconds_load = std::chrono::duration_cast<std::chrono::duration<double>>(end_load - start_load);
    std::cout << "Loaded in " << elapsed_seconds_load.count() << "s (" << gigabytes_handled / elapsed_seconds_load.count() << " Go/s)" << std::endl;

    auto start_find_memory = std::chrono::high_resolution_clock::now();
    auto result_memory = table.findRows(attribute_predicates, value_predicates, true, true);
    auto end_find_memory = std::chrono::high_resolution_clock::now();
    // Checking solution
    for (long unsigned int i = 0; i < result_async.size(); i++)
    {
        if (result_sync[i] != result_memory[i])
        {
            return 1;
        }
    }
    auto elapsed_seconds_find_memory = std::chrono::duration_cast<std::chrono::duration<double>>(end_find_memory - start_find_memory);
    std::cout << "Memory find " << elapsed_seconds_find_memory.count() << "s (" << gigabytes_handled / elapsed_seconds_find_memory.count() << " Go/s)" << std::endl;

    table.unloadEverything();

    return 0;
}
