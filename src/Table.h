#include "Column.h"
#include "main.h"

class Table
{
    int fd;            // File descriptor where data is stored
    int nb_of_column;  // Number of columns / attributes
    Column *columns[]; // List of table's column

public:
    Table();
    ~Table();

    int readInt(int row_pos, int column);
    double readDouble(int row_pos, int column);
    void writeInt(int row_pos, int column, int value);
    void writeDouble(int row_pos, int column, double value);

    std::bitset<CHUNK_SIZE> *findRows(int *predicates);
};