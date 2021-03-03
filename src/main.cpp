#include <cassert>
#include <iostream>

#include "Table.h"

// TODO: implement benchmark

int main(int argc, char *argv[])
{
    const int table_size = 16 * 16;
    Table table{1, 1};
    for (int i = 0; i < table_size; i++)
    {
        table.writeInt(i, 0, i);
        table.writeDouble(i, 1, i + 0.1);
    }

    std::cout << "Testing data" << std::endl;
    for (int i = 0; i < table_size; i++)
    {
        assert(table.readInt(i, 0) == i);
        assert(table.readDouble(i, 1) == i + 0.1);
    }

    int attribute_predicates[1] = {2};
    double value_predicates[1] = {2.1};
    auto result = table.findRows(attribute_predicates, value_predicates);

    for (long unsigned int i = 0; i < result.size(); i++)
    {
        for (long unsigned int j = 0; j < CHUNK_SIZE; j++)
        {
            std::cout << result[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}