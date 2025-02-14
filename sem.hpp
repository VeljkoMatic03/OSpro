//
// Created by os on 5/29/24.
//

#ifndef PROJECT_BASE_SEM_H
#define PROJECT_BASE_SEM_H

#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/list.hpp"

class TCB;

class sem {
private:

    List<TCB> blockedQueue;
    int value;
    bool closed;

public:

    sem(int init = 1) : value(init), closed(false) {}

    int wait();

    int signal();

    int close();

    int timedwait(time_t timeout);

    int trywait();

    static int createSem(sem** handle, unsigned init);

    static void* operator new(size_t size) {
        return MemoryAllocator::malloc(size / MEM_BLOCK_SIZE + 1);
    }
    static void* operator new[](size_t size) {
        return MemoryAllocator::malloc(size / MEM_BLOCK_SIZE + 1);
    }

    static void operator delete(void *ptr) {
        MemoryAllocator::free(ptr);
    }
    static void operator delete[](void *ptr) {
        MemoryAllocator::free(ptr);
    }


};


#endif //PROJECT_BASE_SEM_H
