#include "Table.h"

Table::Table(int attribute_count, int value_count) : attribute_count(attribute_count), value_count(value_count), columns(std::vector<Column>())
{
    // create columns
    for (int i = 0; i < attribute_count; i++)
    {
        columns.emplace_back(memory_allocator, false);
    }

    for (int i = attribute_count; i < (attribute_count + value_count); i++)
    {
        columns.emplace_back(memory_allocator, true);
    }
};

Table::~Table()
{
}

int Table::readInt(int row_pos, int column)
{
    return this->columns[column].readInt(row_pos);
}

double Table::readDouble(int row_pos, int column)
{
    return this->columns[column].readDouble(row_pos);
}

// for write, how do we get newest row_pos?
// does write also work for updating values?

void Table::writeInt(int row_pos, int column, int value)
{
    this->columns[column].writeInt(row_pos, value);
}

void Table::writeDouble(int row_pos, int column, double value)
{
    this->columns[column].writeDouble(row_pos, value);
}
