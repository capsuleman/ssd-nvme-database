#include "gtest/gtest.h"
#include "Table.h"

TEST(TableReadWriteRowsTest, TableReadWriteRows)
{
    const int number_of_rows = 1024 * 1024 * 1024;
    const int write_block_size = 128 * 1024;
    const int attributes_count = 3;
    const int values_count = 2;

    Table table{attributes_count, values_count};
    for (int i = 0; i < number_of_rows; i += write_block_size)
    {
        unsigned int attributes[write_block_size * attributes_count];
        double values[write_block_size * values_count];
        for (int j = 0; j < write_block_size; j++)
        {
            attributes[write_block_size * 0 + j] = 1;
            attributes[write_block_size * 1 + j] = i + j;
            attributes[write_block_size * 2 + j] = 2 * (i + j);
            values[write_block_size * 0 + j] = 0.1 + i + j;
            values[write_block_size * 1 + j] = 1.0 * (i + j);
        }
        table.writeRows(i, write_block_size, attributes, values);
    }

    for (int i = 0; i < 3; i++)
    {
        EXPECT_EQ(table.readInt(i, 0), 1);
        EXPECT_EQ(table.readInt(i, 1), i);
        EXPECT_EQ(table.readInt(i, 2), 2 * i);
        EXPECT_EQ(table.readDouble(i, 3), i + 0.1);
        EXPECT_EQ(table.readDouble(i, 4), 1.0 * i);
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
