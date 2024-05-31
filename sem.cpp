//
// Created by os on 5/29/24.
//

#include "../h/sem.hpp"
#include "../test/printing.hpp"


int sem::wait() {
    if(closed) return -1;
    value -= 1;
    if((int)(value) < 0) {
        blockedQueue.addLast(TCB::running);
        //TCB::running->block();
        TCB::running->isBlocked = true;
        TCB::dispatch();
        if(closed) return -1;
    }
    return 0;
}

int sem::signal() {
    if(closed) return -1;
    if((int) ++(value) <= 0) {
        TCB* t = blockedQueue.peekFirst();
        blockedQueue.removeFirst();
        if(t) t->unblock();
    }
    return 0;
}

int sem::close() {
    if(closed) return -1;
    closed = true;
    while(blockedQueue.peekFirst()) {
        TCB* t = blockedQueue.removeFirst();
        t->unblock();
    }
    return 0;
}

int sem::timedwait(time_t timeout) {
    if(closed) return -1;
    value--;
    if((int) value < 0) {
        blockedQueue.addLast(TCB::running);
        TCB::running->isBlocked = true;
        TCB::toSleep(timeout);
    }
    int retval = 0;
    if(closed) return -1;
    if(TCB::running->isBlocked) {
        printString("\nTIMEOUT\n");
        TCB::running->isBlocked = false;
        blockedQueue.removeElem(TCB::running);
        retval = -2;
    }
    if(TCB::running->isSleeping) {
        printString("\nUNBLOCKED\n");
        TCB::running->isSleeping = false;
        TCB::pullOutAsleepThread(TCB::running);
    }
    return retval;
}

int sem::trywait() {
    if(closed) return -1;
    return (value <= 0 ? 0 : 1);
}

int sem::createSem(sem** handle, unsigned int init) {
    *handle = new sem(init);
    return (*handle ? 0 : -1);
}