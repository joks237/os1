#ifndef SYSCALL_CPP_HPP
#define SYSCALL_CPP_HPP

#include "syscall_c.hpp"
#include "memory.hpp"

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    void join();
    static void dispatch ();
    static void threadRoutine(void* t);

    thread_t myHandle;
protected:
    Thread ();
    virtual void run () {}
private:

    void (*body)(void*); void* arg;
    bool created = false;
};

class Semaphore {
public:
    explicit Semaphore (unsigned init = 1); //mozda treba bez explicit
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};

class Console {
public:
    static char getc ();
    static void putc (char);
};

#endif