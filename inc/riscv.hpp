#ifndef RISCV_HPP
#define RISCV_HPP
#include "../inc/memory.hpp"
#include "../inc/ccb.hpp"
#include "../inc/sem.hpp"
#include "../lib/console.h"

#include "../inc/printing.hpp"

#include "../lib/hw.h"

constexpr uint64 MEM_ALLOC_RISCV = 0x01;
constexpr uint64 MEM_FREE_RISCV = 0x02;
constexpr uint64 THREAD_CREATE_RISCV = 0x11;
constexpr uint64 THREAD_EXIT_RISCV = 0x12;
constexpr uint64 USER_MODE_RISCV = 0x50;
constexpr uint64 THREAD_DISPATCH_RISCV = 0x13;
constexpr uint64 GETC_RISCV = 0x41;
constexpr uint64 PUTC_RISCV = 0x42;
constexpr uint64 THREAD_JOIN_RISCV = 0x14;
constexpr uint64 SEM_OPEN_RISCV = 0x21;
constexpr uint64 SEM_CLOSE_RISCV = 0x22;
constexpr uint64 SEM_WAIT_RISCV = 0x23;
constexpr uint64 SEM_SIGNAL_RISCV = 0x24;

constexpr uint64 TIMER = 0x01UL << 63 | 0x1;
constexpr uint64 CONSOLE_RISCV = 0x01UL << 63 | 0x9;
constexpr uint64 LOAD_ACCESS_FAULT = 7;
constexpr uint64 STORE_ACCESS_FAULT = 5;
constexpr uint64 ILLEGAL_OPERATION = 2;


class Riscv {
public:


    static void popSppSpie(); //popuje sstatus.spp i sstatus.spie bitove (mora da bude non inline funkcija)
    static uint64 r_scause(); //read registar scause
    static void w_scause(uint64 scause); //upisuje u registar scause
    static uint64 r_sepc(); //read registar sepc
    static void w_sepc(uint64 sepc); //upisuje u registar sepc
    static uint64 r_stvec(); //read registar stvec
    static void w_stvec(uint64 stvec); //upisuje u registar stvec
    static uint64 r_stval(); //read registar stval
    static void w_stval(uint64 stval); //upisuje u registar stval

    enum BitMaskSip {
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9),
    };

    static void ms_sip(uint64 mask); //mask set register sip
    static void mc_sip(uint64 mask); //mask clear register sip

    static uint64 r_sip(); //cita registar sip

    static void w_sip(uint64 sip); //upisuje u registar sip

    enum BitMaskSstatus {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8),
    };

    static void ms_sstatus(uint64 mask); //mask set register sstatus
    static void mc_sstatus(uint64 mask); //mask clear register sstatus

    static uint64 r_sstatus(); //cita registar sstatus

    static void w_sstatus(uint64 sstatus); //upisuje u registar sstatus

    static void supervisorTrap(); //zahtev za prekidom,exception, kada se ispunjeni uslovi za njegov handler

private:
    static void handleSupervisorTrap();
    inline static void handleSyscall();
    inline static void handleOther(uint64 tscause);
    inline static void handleTimer();
    inline static void handleConsole();
};

inline uint64 Riscv::r_scause() {
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void Riscv::w_scause(uint64 scause) {
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 Riscv::r_sepc() {
    uint64 volatile sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}

inline void Riscv::w_sepc(uint64 sepc) {
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 Riscv::r_stvec() {
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}

inline void Riscv::w_stvec(uint64 stvec) {
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 Riscv::r_stval() {
    uint64 volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}

inline void Riscv::w_stval(uint64 stval) {
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline void Riscv::ms_sip(uint64 mask) {
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sip(uint64 mask) {
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sip() {
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline void Riscv::w_sip(uint64 sip) {
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void Riscv::ms_sstatus(uint64 mask) {
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sstatus(uint64 mask) {
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sstatus() {
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void Riscv::w_sstatus(uint64 sstatus) {
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

#endif