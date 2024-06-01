#include "../inc/syscall_cpp.hpp"

#include "../inc/printing.hpp"

Thread::Thread(void (*body)(void *), void *arg) {
    thread_create(&myHandle, body, arg);
}

Thread::~Thread() {
}

int Thread::start() {
    if(!this->created) {
        thread_create(&myHandle, &threadRoutine, this);
        this->created = true;

    }
    return 0;
}

void Thread::join() {
    thread_join((thread_t)this->myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread() {
    start();

}

void Thread::threadRoutine(void* t){
    Thread *thread;
    thread = (Thread*)t;
    thread->run();
}


char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

Semaphore::Semaphore(unsigned int val) {
    sem_open(&myHandle, val);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
        int ret = sem_wait(myHandle);
        return ret;
}

int Semaphore::signal() {
        int ret = sem_signal(myHandle);
        return ret;
}