#include "gtest/gtest.h"
#include "Table.hpp"

TEST(TableReadWriteRowsTest, TableReadWriteRows)
{
    const unsigned long int number_of_rows = 1024 * 1024 * 1024;
    const unsigned long int write_block_size = 128 * 1024;
    const unsigned int attributes_count = 3;
    const unsigned int values_count = 2;

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

    for (unsigned long int i = 0; i < 3; i++)
    {
        EXPECT_EQ(table.readInt(i, 0), 1);
        EXPECT_EQ(table.readInt(i, 1), i);
        EXPECT_EQ(table.readInt(i, 2), 2 * i);
        EXPECT_EQ(table.readDouble(i, 3), i + 0.1);
        EXPECT_EQ(table.readDouble(i, 4), 1.0 * i);
    }
    for (unsigned long int i = number_of_rows - 3; i < number_of_rows; i++)
    {
        EXPECT_EQ(table.readInt(i, 0), 1);
        EXPECT_EQ(table.readInt(i, 1), i);
        EXPECT_EQ(table.readInt(i, 2), 2 * i);
        EXPECT_EQ(table.readDouble(i, 3), i + 0.1);
        EXPECT_EQ(table.readDouble(i, 4), 1.0 * i);
    }

    int row_to_find = 19542;
    int attribute_predicates[3] = {1, row_to_find, 2 * row_to_find};
    double value_predicates[2] = {0.1 + row_to_find, 1.0 * row_to_find};
    auto result = table.findRows(attribute_predicates, value_predicates);

    for (long unsigned int i = 0; i < result.size(); i++)
    {
        for (long unsigned int j = 0; j < CHUNK_SIZE; j++)
        {
            EXPECT_EQ(result[i][j], i * CHUNK_SIZE + j == row_to_find);
        }
    }
}

TEST(TableReadWriteRowsTest, TableReadWriteRowsWithOffset)
{
    const int offset = CHUNK_SIZE / 4;
    const int number_of_rows = 2 * CHUNK_SIZE;
    Table table{1, 1};
    unsigned int attributes[number_of_rows];
    double values[number_of_rows];
    for (int i = 0; i < number_of_rows - offset; i++)
    {
        attributes[i] = 1;
        values[i] = 0.1 + i;
    }
    table.writeRows(offset, number_of_rows, attributes, values);
}
