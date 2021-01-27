#include "Column.h"
#include "Chunk.h"

Column::Column(int fd, int isDouble)
{
    this->fd = fd;
    this->isDouble = isDouble;
};

Column::~Column(){};
