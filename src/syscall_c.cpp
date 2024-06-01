#include "../inc/syscall_c.hpp"
#include "../inc/printing.hpp"


void* mem_alloc(size_t size){
    uint64* address;

    size_t newSize = blks(size);

    __asm__ volatile("mv a1, %0"::"r"(newSize));
    __asm__ volatile("mv a0, %0"::"r"(MEM_ALLOC_SYSCALL_C));

    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a0":"=r"(address));

    return address;
}

int mem_free(void* addr){

    uint64 ret = 0;
    __asm__ volatile("mv a1, %0":: "r"(addr));

    __asm__ volatile("mv a0, %0":: "r"(MEM_FREE_SYSCALL_C));

    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a0":"=r"(ret));

    return ret;
}


int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg){

    CCB** ret = handle;
    auto* stack_space = (uint64*) mem_alloc(DEFAULT_STACK_SIZE*sizeof(uint64));

    __asm__ volatile("mv a4, %0":: "r"(stack_space));
    __asm__ volatile("mv a3, %0":: "r"(arg));
    __asm__ volatile("mv a2, %0":: "r"(start_routine));
    __asm__ volatile("mv a1, %0":: "r"(handle));
    __asm__ volatile("mv a0, %0":: "r"(THREAD_CREATE_SYSCALL_C));

    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a0":"=r"(ret));

    if(!ret) return -1;
    else return 0;
}

int thread_exit (){

    int ret = 0;

    __asm__ volatile("mv a1, %0" : : "r" (ret));

    __asm__ volatile("mv a0, %0":: "r"(THREAD_EXIT_SYSCALL_C));

    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

void thread_dispatch (){

    __asm__ volatile("mv a0, %0":: "r"(THREAD_DISPATCH_SYSCALL_C));


    __asm__ volatile("ecall");

}

void thread_join (thread_t handle){

    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0":: "r"(THREAD_JOIN_SYSCALL_C));

    __asm__ volatile("ecall");
}


int sem_open (sem_t* handle, unsigned init){
    sem_t* ret;

    __asm__ volatile("mv a2, %0":: "r"(init));
    __asm__ volatile("mv a1, %0":: "r"(handle));

    __asm__ volatile("mv a0, %0"::  "r"(SEM_OPEN_SYSCALL_C));

    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a0":"=r"(ret));

    if(!ret) return -1;
    else return 0;
}

int sem_close (sem_t handle){
    uint64 ret = 0;
    __asm__ volatile("mv a1, %0" :: "r"(handle)) ;
    __asm__ volatile("mv a0, %0":: "r"(SEM_CLOSE_SYSCALL_C));

    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

int sem_wait (sem_t id){

    uint64 ret = 0;
    __asm__ volatile("mv a1, %0" :: "r"(id)) ;
    __asm__ volatile("mv a0, %0":: "r"(SEM_WAIT_SYSCALL_C));

    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

int sem_signal (sem_t id){
    uint64 ret = 0;
    __asm__ volatile("mv a1, %0" :: "r"(id)) ;
    __asm__ volatile("mv a0, %0":: "r"(SEM_SIGNAL_SYSCALL_C));

    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

char getc(){

    char ret;
    __asm__ volatile("mv a0, %0"::"r"(GETC_SYSCALL_C));
    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a0":"=r"(ret));
    return ret;
}

void putc(char c){
    __asm__ volatile("mv a1, %0"::"r"(c));
    __asm__ volatile("mv a0, %0"::"r"(PUTC_SYSCALL_C));
    __asm__ volatile("ecall");
}

void userModeCall() {
    __asm__ volatile("mv a0, %0" : : "r" (USER_MODE_SYSCALL_C));
    __asm__ volatile("ecall");

}

uint64 blks(size_t size) {
    uint64 newSize;

    if(size < MEM_BLOCK_SIZE) { newSize = 1;} else {
        size /= MEM_BLOCK_SIZE;
        newSize = (size % MEM_BLOCK_SIZE != 0) ? size + 1 : size;
    }
    return newSize;
}


int numOfBlocks(thread_t handle) {
    __asm__ volatile("mv a1, %0" :: "r"(handle));
    __asm__ volatile ("mv a0, %0" :: "r"(0x70));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile("mv %0, a0":"=r"(ret));
    return ret;
}