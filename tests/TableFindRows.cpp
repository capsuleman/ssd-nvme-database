#include "gtest/gtest.h"
#include "Table.h"

TEST(TableFindRowsTest, TableFindRows)
{
    const int table_size = 16 * 16;
    Table table{3, 2};
    for (int i = 0; i < table_size; i++)
    {
        table.writeInt(i, 0, 1);
        table.writeInt(i, 1, i);
        table.writeInt(i, 2, 2 * i);
        table.writeDouble(i, 3, i + 0.1);
        table.writeDouble(i, 4, i * 1.0);
    }
    int attribute_predicates[3] = {1, 2, 4};
    double value_predicates[2] = {2.1, 2.0};
    auto result = table.findRows(attribute_predicates, value_predicates);

    for (long unsigned int i = 0; i < result.size(); i++)
    {
        for (long unsigned int j = 0; j < CHUNK_SIZE; j++)
        {
            EXPECT_EQ(result[i][j], i * CHUNK_SIZE + j == 2);
        }
    }
}

TEST(TableFindRowsTest, TableFindRowsWithWriteRows)
{
    const int table_size = 16 * 16;
    Table table{3, 2};
    for (int i = 0; i < table_size; i++)
    {
        int attributes[3] = {1, i, 2 * i};
        double values[2] = {i + 0.1, 1.0 * i};
        table.writeRow(i, attributes, values);
    }

    int attribute_predicates[3] = {1, 2, 4};
    double value_predicates[2] = {2.1, 2.0};
    auto result = table.findRows(attribute_predicates, value_predicates);

    for (long unsigned int i = 0; i < result.size(); i++)
    {
        for (long unsigned int j = 0; j < CHUNK_SIZE; j++)
        {
            EXPECT_EQ(result[i][j], i * CHUNK_SIZE + j == 2);
        }
    }
}
