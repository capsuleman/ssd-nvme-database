#include "Column.h"
#include "main.h"
#include <vector>

class Table
{
    int fd;              // File descriptor where data is stored
    int attribute_count; // Number of int columns
    int value_count;     // Number of double columns
    std::vector<Column> columns;   // List of table's column

public:
    Table(int attribute_count, int value_count);
    ~Table();

    int readInt(int row_pos, int column);
    double readDouble(int row_pos, int column);
    void writeInt(int row_pos, int column, int value);
    void writeDouble(int row_pos, int column, double value);

    std::bitset<CHUNK_SIZE> *findRows(int *predicates);
};