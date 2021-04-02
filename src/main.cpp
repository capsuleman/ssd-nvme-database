#include <iostream>

#include "Table.h"

int main(int argc, char **argv)
{
    const unsigned long int number_of_rows = 1024 * 1024;
    const unsigned long int write_block_size = 128 * 1024;
    const unsigned int attributes_count = 3;
    const unsigned int values_count = 2;
    bool is_good = true;

    Table table{attributes_count, values_count};
    for (unsigned long int i = 0; i < number_of_rows; i += write_block_size)
    {
        unsigned int attributes[write_block_size * attributes_count];
        double values[write_block_size * values_count];
        for (unsigned long int j = 0; j < write_block_size; j++)
        {
            attributes[write_block_size * 0 + j] = 1;
            attributes[write_block_size * 1 + j] = i + j;
            attributes[write_block_size * 2 + j] = 2 * (i + j);
            values[write_block_size * 0 + j] = 0.1 + i + j;
            values[write_block_size * 1 + j] = 1.0 * (i + j);
        }
        table.writeRows(i, write_block_size, attributes, values);
    }

    int row_to_find = 19542;
    int attribute_predicates[3] = {1, row_to_find, 2 * row_to_find};
    double value_predicates[2] = {0.1 + row_to_find, 1.0 * row_to_find};
    auto result = table.findRows(attribute_predicates, value_predicates);

    for (long unsigned int i = 0; i < result.size(); i++)
    {
        for (long unsigned int j = 0; j < CHUNK_SIZE; j++)
        {
            is_good = is_good && (result[i][j] == (i * CHUNK_SIZE + j == row_to_find));
        }
    }

    return !is_good;
}
