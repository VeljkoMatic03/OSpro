//
// Created by os on 5/22/24.
//

#ifndef PROJECT_BASE_SYSCALL_C_H
#define PROJECT_BASE_SYSCALL_C_H

#include "../lib/hw.h"


void* mem_alloc(size_t size);

int mem_free(void*);

class TCB;
typedef TCB* thread_t;

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);

int thread_exit();

void thread_dispatch();


#endif //PROJECT_BASE_SYSCALL_C_H
