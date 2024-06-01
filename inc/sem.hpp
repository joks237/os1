#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP


#include "list.hpp"
#include "ccb.hpp"
#include "syscall_c.hpp"
#include "memory.hpp"

class sem{
public:
    sem(sem_t* handle, unsigned v);
    ~sem();

    int wait();
    int signal();
    int close();


    List<CCB> blockedList;
private:
    int val;
    friend class Riscv;
    bool mutexClosed;
};

#endif