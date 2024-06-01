#ifndef SYSCALL_C_HPP
#define SYSCALL_C_HPP

#include "../lib/hw.h"

class CCB;
typedef CCB* thread_t;

constexpr uint64 MEM_ALLOC_SYSCALL_C = 0x01;
constexpr uint64 MEM_FREE_SYSCALL_C = 0x02;
constexpr uint64 THREAD_CREATE_SYSCALL_C = 0x11;
constexpr uint64 THREAD_EXIT_SYSCALL_C = 0x12;
constexpr uint64 USER_MODE_SYSCALL_C = 0x50;
constexpr uint64 THREAD_DISPATCH_SYSCALL_C = 0x13;
constexpr uint64 GETC_SYSCALL_C = 0x41;
constexpr uint64 PUTC_SYSCALL_C = 0x42;
constexpr uint64 THREAD_JOIN_SYSCALL_C = 0x14;
constexpr uint64 SEM_OPEN_SYSCALL_C = 0x21;
constexpr uint64 SEM_CLOSE_SYSCALL_C = 0x22;
constexpr uint64 SEM_WAIT_SYSCALL_C = 0x23;
constexpr uint64 SEM_SIGNAL_SYSCALL_C = 0x24;


void* mem_alloc(size_t size);

int mem_free(void*);


int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
);

int thread_exit ();


void thread_dispatch ();

void thread_join (
        thread_t handle
);

class sem;
typedef sem* sem_t;
int sem_open (
        sem_t* handle,
        uint32 init
);

int sem_close (sem_t handle);

int sem_wait (sem_t id);

int sem_signal (sem_t id);

char getc();
void putc(char);

void userModeCall();

uint64 blks(size_t size);

int getId(thread_t handle);

int numOfBlocks(thread_t handle);

#endif
