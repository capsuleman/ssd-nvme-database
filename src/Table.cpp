#include <iostream>
#include <cstdio>

#include "Table.h"

Table::Table(int attribute_count, int value_count)
{
    this->attribute_count = attribute_count;
    this->value_count = value_count;

    FILE *fp;
    fp = tmpfile();

    if (fp == NULL)
    {
        perror("Error creating temporary file");
        exit(1);
    }

    std::cout << "Created temporary file" << fp << std::endl;

    this->fd = fileno(fp);

    // create columns
    this->columns = new Column[this->attribute_count + this->value_count];

    for (int i = 0; i < this->attribute_count; i++)
    {
        this->columns[i] = new Column(this->fd);
    }

    for (int i = this->attribute_count; i < this->value_count; i++)
    {
        this->columns[i] = new Column(this->fd);
    }
}

Table::~Table()
{
}