#include <iostream>

#include "Table.h"

// TODO: implement benchmark

int main(int argc, char *argv[])
{
    const int table_size = 16;
    Table table{1, 1};
    for (int i = 0; i < table_size; i++)
    {
        table.writeInt(i, 0, i);
        table.writeDouble(i, 1, i + 0.1);
    }

    std::cout << "Reading data on column 0 : ";
    for (int i = 0; i < table_size; i++)
    {
        std::cout << table.readInt(i, 0) << " ";
    }
    std::cout << std::endl;

    std::cout << "Reading data on column 1 : ";
    for (int i = 0; i < table_size; i++)
    {
        std::cout << table.readDouble(i, 1) << " ";
    }
    std::cout << std::endl;
    return 0;
}