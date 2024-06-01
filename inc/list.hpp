#ifndef LIST_HPP
#define LIST_HPP

#include "memory.hpp"
template <typename T>

class List{
    struct Elem
    {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}

        void *operator new(size_t size){
            return inbloks(size);
        }
        void operator delete(void* el){
            MemoryAlloc::mem_free(el);
        }
        void *operator new[](size_t size){
            return inbloks(size);
        }
        void operator delete[](void* el){
            MemoryAlloc::mem_free(el);
        }

    };



    static void* inbloks(size_t size){
        size_t s;
        s = size/MEM_BLOCK_SIZE;
        if(size % MEM_BLOCK_SIZE != 0) s = s + 1;
        return MemoryAlloc::mem_alloc(s);
    }

public:

    Elem *head, *tail;

    List() : head(0), tail(0) {}

    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    void addLast(T *data)
    {
        Elem *elem = new Elem(data, 0);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    T *removeFirst()
    {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    int getSize(){
        int sizeList = 0;
        for(Elem* pom = head; pom != nullptr; pom = pom->next) {
            sizeList++;
        }
        return sizeList;
    }

};

#endif