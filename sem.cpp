//
// Created by os on 5/29/24.
//

#include "../h/sem.hpp"


int sem::wait(sem* s) {
    if(s->closed) return -1;
    if((int) --(s->value) < 0) {
        s->blockedQueue.addLast(TCB::running);
        TCB::running->block();
        if(s->closed) return -1;
    }
    return 0;
}

int sem::signal(sem* s) {
    if(s->closed) return -1;
    if((int) ++(s->value) <= 0) {
        TCB* t = s->blockedQueue.removeFirst();
        if(t) t->unblock();
    }
    return 0;
}

int sem::close(sem* s) {
    if(s->closed) return -1;
    s->closed = true;
    while(s->blockedQueue.peekFirst()) {
        TCB* t = s->blockedQueue.removeFirst();
        t->unblock();
    }
    return 0;
}

int sem::timedwait(sem* s, time_t timeout) {
    return 0;
}

int sem::trywait(sem* s) {
    if(s->closed) return -1;
    return (s->value > 0 ? 1 : 0);
}

int sem::createSem(sem** handle, unsigned int init) {
    *handle = new sem(init);
    return (*handle ? 0 : -1);
}