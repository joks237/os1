#include "../inc/memory.hpp"
MemHeader *MemoryAlloc::head = nullptr;

void *MemoryAlloc::mem_alloc(size_t size) {

    if (head == nullptr) {
        head = (MemHeader*)((uint64) HEAP_START_ADDR);
        head->size = (uint64) HEAP_END_ADDR - (uint64) HEAP_START_ADDR - 1;
        head->isFree = true;
        head->next = nullptr;
        head->prev = nullptr;
    }

    size *= MEM_BLOCK_SIZE;

    MemHeader *curr = head;
    while (true) {
        if (curr->size >= (size + sizeof(MemHeader)) && curr->isFree) {


            auto *newSeg = (MemHeader *) ((uint64) curr + sizeof (MemHeader) + size);


            curr->next = newSeg;
            curr->size = size;
            newSeg->prev = curr;
            newSeg->next = nullptr;

            newSeg->size = ((uint64) curr->size) - (sizeof(MemHeader) + size);

            newSeg->isFree = true;

            if (curr == head) {
                head = curr->next;
            }
            curr->isFree = false;

            return curr + 1;
        }
        if (!curr->next) {
            return nullptr;
        }
        curr = curr->next;
    }
}

void MemoryAlloc::merge(MemHeader *first, MemHeader *second) {

    if (!first || !second)return;


    if(second < first) {
        second->next = first->next;
        second->size += first->size + sizeof(MemHeader);


        if (first->next != nullptr)
            first->next->prev = second;

    } else {
        first->next = second->next;
        first->size += second->size + sizeof(MemHeader);

        if (second->next != nullptr)
            second->next->prev = first;

    }

}


int MemoryAlloc::mem_free(void *p) {

    if (!p) return -1;

    MemHeader *current = ((MemHeader *) p) - sizeof(MemHeader);
    current->isFree = true;

    if(!current->prev) head = current;

    if (!current->next && current->next->isFree) {
        merge(current, current->next);
    }

    if (!current->prev && current->prev->isFree)
        merge(current, current->prev);

    current = nullptr;
    return 0;
}