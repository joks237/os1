
#include "../inc/ccb.hpp"
#include "../inc/riscv.hpp"


extern "C" void pushRegisters();
extern "C" void popRegisters();


CCB *CCB::running = nullptr;


CCB *CCB::create(Body body, void* arg)
{
    return new CCB(body, arg);
}

CCB *CCB::createWithStack(CCB::Body body, void *arg, uint64 *stack_space) {
    return new CCB(body, arg, stack_space);
}


void CCB::yield() {
    pushRegisters();
    __asm__ volatile ("mv a0, %0" : : "r"(13));
    __asm__ volatile ("ecall");
    popRegisters();
}

void CCB::dispatch()
{
    CCB *old = running;

    if(!old->isFinished() && !old->isBlocked()) {

        Scheduler::put(old);

    }

    running = Scheduler::get();

    if(running != nullptr && running != old)
        CCB::contextSwitch(&old->context, &running->context);
}

void CCB::wrapper(void* b) {

    Riscv::popSppSpie(); //bitno da nije inline funkcija
    running->body(running->arg);
    thread_exit();
}

void CCB::join(thread_t threadJoin) {
    while(!threadJoin->isFinished()) {
        CCB::dispatch();
    }
}

