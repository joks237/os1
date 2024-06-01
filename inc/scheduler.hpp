#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "list.hpp"

class CCB;

class Scheduler
{
private:
    static List<CCB> coroutineReadyQueue;

public:
    static CCB *get();

    static void put(CCB *pcb);

};

#endif