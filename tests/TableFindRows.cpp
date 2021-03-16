#include "gtest/gtest.h"
#include "Table.h"

TEST(TableFindRowsTest, TableFindRowsWithWriteRows)
{
    const int table_size = 16 * 16;
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
        values[table_size * 1 + i] = 1.0 * i;
    }
    table.writeRows(0, table_size, attributes, values);

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
