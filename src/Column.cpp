#include "Column.h"

Column::Column(MemoryAllocator *memory_allocator, bool is_double) : memory_allocator(memory_allocator), is_double(is_double){};

Column::~Column(){};
