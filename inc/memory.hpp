#ifndef MEMORY_ALLOCATOR_HPP
#define MEMORY_ALLOCATOR_HPP
#include "../lib/hw.h"

struct MemHeader{
    size_t size;
    MemHeader *next, *prev;
    bool isFree;
};


class MemoryAlloc{
private:

    MemoryAlloc() {}
public:

    static void *mem_alloc(size_t size);
    static int mem_free(void *p);
    static void merge(MemHeader* first, MemHeader* second);
    static struct MemHeader *head;
};

#endif