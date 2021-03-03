#include "Table.h"

Table::Table(int attribute_count, int value_count)
    : attribute_count(attribute_count),
      value_count(value_count)
{
    // create columns
    for (int i = 0; i < attribute_count + value_count; i++)
    {
        columns.emplace_back(memory_allocator, i >= attribute_count);
    }
};

int Table::readInt(int row_pos, int column)
{
    return columns[column].readInt(row_pos);
}

double Table::readDouble(int row_pos, int column)
{
    return columns[column].readDouble(row_pos);
}

// for write, how do we get newest row_pos?
// does write also work for updating values?

void Table::writeInt(int row_pos, int column, int value)
{
    columns[column].writeInt(row_pos, value);
}

void Table::writeDouble(int row_pos, int column, double value)
{
    columns[column].writeDouble(row_pos, value);
}

std::vector<std::bitset<CHUNK_SIZE>> Table::findRows(int *attribute_predicates, double *value_predicates)
{
    std::vector<std::bitset<CHUNK_SIZE>> result;
    for (int i = 0; i < attribute_count; i++)
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

    for (int i = 0; i < value_count; i++)
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