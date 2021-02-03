#include <iostream>
#include <cstdio>

#include "Table.h"

Table::Table(int attribute_count, int value_count) : attribute_count(attribute_count), value_count(value_count), columns(std::vector<Column>())
{
    FILE *fp;
    fp = tmpfile();

    if (fp == NULL)
    {
        perror("Error creating temporary file");
        exit(1);
    }

    std::cout << "Created temporary file " << fp << std::endl;

    int fd = fileno(fp);
    this->memory_allocator = new MemoryAllocator(fd);

    // create columns
    for (int i = 0; i < this->attribute_count; i++)
    {
        this->columns.emplace_back(this->memory_allocator, false);
    }

    for (int i = this->attribute_count; i < this->value_count; i++)
    {
        this->columns.emplace_back(this->memory_allocator, true);
    }
};

Table::~Table()
{
    delete this->memory_allocator;
}
