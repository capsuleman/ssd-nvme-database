#include "gtest/gtest.h"
#include "Table.h"

TEST(TableReadWriteRowsTest, TableReadWriteRows)
{
    const int number_of_rows = 1024 * 1024 * 1024;
    const int write_block_size = 128 * 1024;
    Table table{3, 2};
    for (int i = 0; i < number_of_rows; i += write_block_size)
    {
        int attributes[3][write_block_size];
        double values[2][write_block_size];
        for (int j = 0; j < write_block_size; j++)
        {
            attributes[0][j] = 1;
            attributes[1][j] = i + j;
            attributes[2][j] = 2 * (i + j);
            values[0][j] = 0.1 + i + j;
            values[1][j] = 1.0 * (i + j);
        }
        table.writeRows(i, write_block_size, *attributes, *values);
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
    int attributes[number_of_rows];
    double values[number_of_rows];
    for (int i = 0; i < number_of_rows - offset; i++)
    {
        attributes[i] = 1;
        values[i] = 0.1 + i;
    }
    table.writeRows(offset, number_of_rows, attributes, values);
}
