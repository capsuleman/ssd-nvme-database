#include "MemoryAllocator.h"

MemoryAllocator::MemoryAllocator(int fd)
{
    this->fd = fd;
}

MemoryAllocator::~MemoryAllocator()
{
}
