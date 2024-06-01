
#include "../inc/scheduler.hpp"

List<CCB> Scheduler::coroutineReadyQueue;

CCB *Scheduler::get()
{
    return coroutineReadyQueue.removeFirst();
}

void Scheduler::put(CCB *ccb)
{
    coroutineReadyQueue.addLast(ccb);
}