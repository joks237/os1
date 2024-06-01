#include "../inc/sem.hpp"

sem::sem(sem_t* handle, unsigned v) {
    *handle = this;
    this->val = (int)v;
    blockedList.head = blockedList.tail = nullptr;
    mutexClosed = false;
}

int sem::wait() {
    if(mutexClosed) return -1;
    if(--this->val < 0) {
        this->blockedList.addLast(CCB::running);
        CCB::running->setBlocked(true);
        CCB::dispatch();
    }
    return 0;
}

int sem::signal() {
    if(mutexClosed) return -1;
    val++;
    if(val<=0) {
        CCB* unblockedThread = blockedList.removeFirst();
        unblockedThread->setBlocked(false);
        Scheduler::put(unblockedThread);
    }
    return 0;
}


sem::~sem() {
    close();
}

int sem::close() {
    if(mutexClosed) return -1;
    int i = 0;
    while(i < blockedList.getSize()) {
        CCB* thread = blockedList.removeFirst();
        thread->setBlocked(false);
        Scheduler::put(thread);
        i++;
    }
    mutexClosed = true;
    return 0;
}
