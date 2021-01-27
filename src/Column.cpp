#include "Column.h"


Column::Column(int fd, int isDouble)
{
    this->fd = fd;
    this->isDouble = isDouble;
};

Column::~Column(){};
