#include "../inc/riscv.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

inline void Riscv::handleSyscall() {
    uint64 volatile a1;
    uint64 volatile a2;
    uint64 volatile a3;
    uint64 volatile a4;

    __asm__ volatile ("mv %0, a4" : "=r"(a4));
    __asm__ volatile("mv %0, a3" : "=r"(a3));
    __asm__ volatile("mv %0, a2" : "=r"(a2));
    __asm__ volatile("mv %0, a1" : "=r"(a1));

    uint64 volatile sstatus = r_sstatus();

    uint64 volatile sepc = r_sepc();
    uint64 volatile opCode;

    __asm__ volatile("mv %0, a0": "=r"(opCode));

        switch(opCode) {
            case MEM_ALLOC_RISCV: {
                auto size = (size_t) a1;
                uint64* address;
                address = (uint64*)MemoryAlloc::mem_alloc(size);
                __asm__ volatile("mv a0, %0":: "r"(address));
                __asm__ volatile("sd a0, 10*8(fp)");
                break;
            }
            case MEM_FREE_RISCV: {
                void* addr = (void*)a1;
                uint64 ret;

                ret = (uint64)MemoryAlloc::mem_free(addr);

                __asm__ volatile("mv a0, %0"::"r"(ret));

                break;
            }
            case THREAD_CREATE_RISCV: {
                auto* thr = (thread_t*) a1;
                auto* stack_space = (uint64*) MemoryAlloc::mem_alloc((DEFAULT_STACK_SIZE *sizeof(uint64))/MEM_BLOCK_SIZE);
                auto start_routine = (CCB::Body) a2;
                void *args = (void*) a3;
                *thr = CCB::createWithStack(start_routine, args, stack_space);
                __asm__ volatile("mv a0, %0":: "r"(thr));
                __asm__ volatile("sd a0, 10*8(fp)");
                break;
            }
            case THREAD_EXIT_RISCV: {
                uint64 ret = 0;

                if (!CCB::running->isFinished()) {
                    CCB::running->setFinished(true);
                    CCB::dispatch();
                } else {
                    ret = -1;
                }
                __asm__ volatile("mv %0, a1": "=r"(ret));

                break;
            }
            case THREAD_DISPATCH_RISCV: {
                CCB::dispatch();
                break;
            }
            case THREAD_JOIN_RISCV: {
                auto pcb = (thread_t) a1;

                CCB::join(pcb);
                break;
            }
            case SEM_OPEN_RISCV:{
                auto val = (unsigned) a2;
                auto* handle = (sem_t*) a1;

                sem* Sem = new sem(handle, val);

                __asm__ volatile("mv a0, %0"::"r"(&Sem));

                break;
            }
            case SEM_CLOSE_RISCV: {
                auto s = (sem_t) a1;
                int ret = s->close();

                delete s;

                __asm__ volatile("mv a0, %0"::"r"(ret));
                break;
            }
            case SEM_WAIT_RISCV: {
                auto s = (sem_t) a1;
                int ret = s->wait();

                __asm__ volatile("mv a0, %0"::"r"(ret));
                break;
            }
            case SEM_SIGNAL_RISCV: {
                auto s = (sem_t) a1;
                int ret = s->signal();
                __asm__ volatile("mv a0, %0"::"r"(ret));
                break;
            }
            case GETC_RISCV: {
                char c = __getc();
                __asm__ volatile("mv a0, %0"::"r"(c));
                __asm__ volatile("sd a0, 10*8(fp)");
                break;
            }
            case PUTC_RISCV: {
                char c = (char)a1;
                __putc(c);
                break;
            }
            case USER_MODE_RISCV: {
                sstatus &= ~SSTATUS_SPP; //0 korisnicki, 1 sistemski
                break;
            }

            default : {
                printString("Nije unet dobar operacioni kod");
                break;
            }
        }
        w_sepc(sepc + 4);
        w_sstatus(sstatus);

}

inline void Riscv::handleTimer() {
    uint64 sepc = r_sepc();
    uint64 sstatus = r_sstatus();
    mc_sip(SIP_SSIP);
    w_sstatus(sstatus);
    w_sepc(sepc);
}

inline void Riscv::handleConsole() {
    console_handler();
}

inline void Riscv::handleOther(uint64 scause) {
    if(scause == LOAD_ACCESS_FAULT){

        printString("Load access fault");
        while(1);
    }
    else if(scause == STORE_ACCESS_FAULT){
        printString("STORE ACCESS FAULT");
        while(1);
    }
    else if(scause == ILLEGAL_OPERATION) {
        printString("ILLEGAL OPERATION");
        while(1);
    } else {

        uint64 stval = r_stval();
        uint64 stvec = r_stvec();
        uint64 sepc = r_sepc();
        printString("SCAUSE \n");
        printInt((int)scause);
        printString("STVAL: \n");
        printInt((int)stval);
        printString("STVEC: \n");
        printInt((int)stvec);
        printString("SEPC: \n");
        printInt((int)sepc);
    }
}

void Riscv::handleSupervisorTrap() {

    uint64 volatile scause = r_scause();
    //env
    if(scause == 8 || scause == 9) {
        handleSyscall();
    }
    else if(scause == CONSOLE_RISCV){
        handleConsole();
    }
    else if(scause == TIMER){
        handleTimer();
    }
    else {
        handleOther(scause);
    }

}