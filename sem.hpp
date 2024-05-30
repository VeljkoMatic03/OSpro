//
// Created by os on 5/29/24.
//

#ifndef PROJECT_BASE_SEM_H
#define PROJECT_BASE_SEM_H

#include "../h/list.hpp"
#include "../h/tcb.hpp"
#include "../h/scheduler.hpp"

class sem {
private:

    List<TCB> blockedQueue;
    unsigned value;
    bool closed;

public:

    sem(unsigned init) : value(init), closed(false) {}

    static int wait(sem* s);

    static int signal(sem* s);

    static int close(sem* s);

    static int timedwait(sem* s, time_t timeout);

    static int trywait(sem* s);

    static int createSem(sem** handle, unsigned init);

};


#endif //PROJECT_BASE_SEM_H
