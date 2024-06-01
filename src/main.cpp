#include "../inc/ccb.hpp"
#include "../inc/riscv.hpp"
#include "../inc/syscall_cpp.hpp"
#include "../inc/sem.hpp"


extern void userMain();

void userMainWrapper(void*)
{
    userMain();
}

int main(){

    MemoryAlloc::head = nullptr;

    Riscv::w_stvec((uint64)&Riscv::supervisorTrap);

    userModeCall();

    CCB* dummy = new CCB(nullptr, nullptr, nullptr);

    CCB::running = dummy;

    CCB* something = new CCB(nullptr, nullptr, nullptr);

    CCB** userMainCorutine = &something;

    thread_create(userMainCorutine, &userMainWrapper, nullptr);



    while(!((*userMainCorutine)->isFinished())) {
        thread_dispatch();
    }

    delete *userMainCorutine;
    return 0;
}