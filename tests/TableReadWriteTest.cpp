#include "gtest/gtest.h"
#include "Table.hpp"

TEST(TableReadWriteTest, TableReadWriteWithWriteRows)
{
    const int table_size = 3;
    const int attributes_count = 3;
    const int values_count = 2;
    Table table{attributes_count, values_count};

    unsigned int attributes[table_size * attributes_count];
    double values[table_size * values_count];

    for (unsigned int i = 0; i < table_size; i++)
    {
        attributes[table_size * 0 + i] = 1;
        attributes[table_size * 1 + i] = i;
        attributes[table_size * 2 + i] = 2 * i;
        values[table_size * 0 + i] = 0.1 + i;
        values[table_size * 1 + i] = 0.1 * i;
    }

    table.writeRows(0, table_size, attributes, values);

    for (int i = 0; i < table_size; i++)
    {
        EXPECT_EQ(table.readInt(i, 0), 1);
        EXPECT_EQ(table.readInt(i, 1), i);
        EXPECT_EQ(table.readInt(i, 2), 2 * i);
        EXPECT_EQ(table.readDouble(i, 3), i + 0.1);
        EXPECT_EQ(table.readDouble(i, 4), 0.1 * i);
    }
}
