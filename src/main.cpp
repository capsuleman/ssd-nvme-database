#include <iostream>
#include "Table.h"

int main(int argc, char *argv[])
{
    Table table{1, 1};
    for (int i = 0; i < 16; i++)
        table.writeInt(i, 0, i);

    std::cout << "Reading data on column 0 : ";
    for (int i = 0; i < 16; i++)
        std::cout << table.readInt(i, 0) << " ";
    std::cout << std::endl;
    return 0;
}