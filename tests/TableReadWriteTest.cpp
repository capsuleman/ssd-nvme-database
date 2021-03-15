#include "gtest/gtest.h"
#include "Table.h"

TEST(TableReadWriteTest, TableReadWrite)
{
    const int table_size = 16 * 16;
    Table table{1, 1};
    for (int i = 0; i < table_size; i++)
    {
        table.writeInt(i, 0, i);
        table.writeDouble(i, 1, i + 0.1);
    }

    for (int i = 0; i < table_size; i++)
    {
        EXPECT_EQ(table.readInt(i, 0), i);
        EXPECT_EQ(table.readDouble(i, 1), i + 0.1);
    }
}

TEST(TableReadWriteTest, TableReadWriteWithWriteRow)
{
    const int table_size = 1024;
    Table table{3, 2};
    for (int i = 0; i < table_size; i++)
    {
        int attributes[3] = {1, i, 2 * i};
        double values[2] = {i + 0.1, 1.0 * i};
        table.writeRow(i, attributes, values);
    }

    for (int i = 0; i < table_size; i++)
    {
        EXPECT_EQ(table.readInt(i, 0), 1);
        EXPECT_EQ(table.readInt(i, 1), i);
        EXPECT_EQ(table.readInt(i, 2), 2 * i);
        EXPECT_EQ(table.readDouble(i, 3), i + 0.1);
        EXPECT_EQ(table.readDouble(i, 4), 1.0 * i);
    }
}
