#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <cstring>

#include "MemoryAllocator.h"

MemoryAllocator::MemoryAllocator()
{
    char nameBuff[32];
    memset(nameBuff, 0, sizeof(nameBuff));
    strncpy(nameBuff, "/tmp/ssd-bdd-XXXXXX", 21);
    int fd = mkstemp(nameBuff);
    unlink(nameBuff);

    this->fd = fd;
    this->last_line = 0;

    char fname[FILENAME_MAX], link[FILENAME_MAX] = {0};
    sprintf(fname, "/proc/self/fd/%d", fd);
    if (readlink(fname, link, sizeof link - 1) > 0)
        printf("Temporary file created : %s\n", link);
}

MemoryAllocator::~MemoryAllocator()
{
}
