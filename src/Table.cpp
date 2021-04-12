#include "Table.hpp"

Table::Table(unsigned int attribute_count, unsigned int value_count)
    : attribute_count(attribute_count),
      value_count(value_count)
{
    for (unsigned int i = 0; i < attribute_count + value_count; i++)
    {
        columns.emplace_back(chunk_allocator, i >= attribute_count);
    }
};

unsigned int Table::readInt(unsigned long int row_pos, unsigned int column)
{
    return columns[column].readInt(row_pos);
}

double Table::readDouble(unsigned long int row_pos, unsigned int column)
{
    return columns[column].readDouble(row_pos);
}

void Table::writeInt(unsigned long int row_pos, unsigned int column, unsigned int attribute)
{
    columns[column].writeInts(row_pos, 1, &attribute);
}

void Table::writeDouble(unsigned long int row_pos, unsigned int column, double value)
{
    columns[column].writeDoubles(row_pos, 1, &value);
}

void Table::writeRows(unsigned long int starting_row_pos, unsigned long int number_of_rows, unsigned int *attributes, double *values)
{
    for (unsigned int column = 0; column < attribute_count + value_count; column++)
    {
        if (column < attribute_count)
        {
            columns[column].writeInts(starting_row_pos, number_of_rows, attributes + column * number_of_rows);
        }
        else
        {
            columns[column].writeDoubles(starting_row_pos, number_of_rows, values + (column - attribute_count) * number_of_rows);
        }
    }
}

void Table::writeRow(unsigned long int row_pos, unsigned int *attributes, double *values)
{
    return writeRows(row_pos, 1, attributes, values);
};

std::vector<std::bitset<CHUNK_SIZE>> Table::findRows(int *attribute_predicates, double *value_predicates)
{
    std::vector<std::bitset<CHUNK_SIZE>> result;
    for (unsigned int i = 0; i < attribute_count; i++)
    {
        if (result.empty())
        {
            result = columns[i].findIntRows(attribute_predicates[i]);
        }
        else
        {
            std::vector<std::bitset<CHUNK_SIZE>> partial_result{columns[i].findIntRows(attribute_predicates[i])};
            for (long unsigned int j = 0; j < result.size(); j++)
            {
                result[j] &= partial_result[j];
            }
        }
    }

    for (unsigned int i = 0; i < value_count; i++)
    {
        if (result.empty())
        {
            result = columns[attribute_count + i].findDoubleRows(value_predicates[i]);
        }
        else
        {
            std::vector<std::bitset<CHUNK_SIZE>> partial_result{columns[attribute_count + i].findDoubleRows(value_predicates[i])};
            for (long unsigned int j = 0; j < result.size(); j++)
            {
                result[j] &= partial_result[j];
            }
        }
    }

    return result;
}

void Table::loadEverything()
{
    for (auto &column : columns)
    {
        column.loadEverything();
    }
}
