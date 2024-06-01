#ifndef CCB_HPP
#define CCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "memory.hpp"
#include "list.hpp"
#include "syscall_c.hpp"
#include "riscv.hpp"

class CCB
{
public:

    using Body = void (*)(void*);

    /*<---------DESTRUKTOR---------->*/

    ~CCB() {
        MemoryAlloc::mem_free(stack);
    }

    /*<----------GETTERI I SETTERI ZA FINISHED I BLOCKED--------->*/
    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }


    bool isBlocked() const { return blocked; }

    void setBlocked(bool value) { blocked = value; }


    /*<--------POZIVI KONSTRUKTORA--------->*/
    static CCB *create(Body body, void *arg);

    static CCB *createWithStack(Body body, void *arg, uint64* stack_space);

    static CCB *running;

    static void wrapper(void*);

    static void yield();

    static void join(thread_t pcbWait);

    static void dispatch();


    explicit CCB(Body body, void* arg) : body(body), finished(false),  blocked(false){
        stack = body != nullptr ? (uint64*)MemoryAlloc::mem_alloc(DEFAULT_STACK_SIZE*sizeof(uint64)) : nullptr;
        context.ra = (uint64) &wrapper;
        context.sp = stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0;
        this->arg = arg;
        if(body != nullptr) Scheduler::put(this);

    }

    explicit CCB(Body body, void* arg, uint64* stack_space) : body(body), finished(false),  blocked(false){
        stack = stack_space != nullptr ? stack_space : nullptr;
        context.ra = (uint64) &wrapper;
        context.sp = (uint64) &stack[DEFAULT_STACK_SIZE/sizeof(uint64)];
        this->arg = arg;
        if(body != nullptr) Scheduler::put(this);
    }

    /* <---------------------OPERATORI-------------------> */

    void *operator new(size_t size){
        return inbloks(size);
    }
    void operator delete(void* el) noexcept{
        MemoryAlloc::mem_free(el);
    }
    void *operator new[](size_t size){
        return inbloks(size);
    }
    void operator delete[](void* el) noexcept{
        MemoryAlloc::mem_free(el);
    }

    /* <----------------------!----------------------> */

private:

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void *arg;
    uint64 *stack;
    Context context;
    bool finished;
    bool blocked;

    friend class sem;
    static void contextSwitch(Context *oldContext, Context *runningContext);

    friend class Riscv;

    static void* inbloks(size_t size){
        size_t s;
        s = size/MEM_BLOCK_SIZE;
        if(size % MEM_BLOCK_SIZE != 0) s = s + 1;
        return MemoryAlloc::mem_alloc(s);
    }

};

#endif