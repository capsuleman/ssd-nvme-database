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

    return 0;
}