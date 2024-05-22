//
// Created by os on 5/22/24.
//

#ifndef PROJECT_BASE_SYSCALL_C_H
#define PROJECT_BASE_SYSCALL_C_H

#include "../lib/hw.h"


void* mem_alloc(size_t size);

int mem_free(void*);


#endif //PROJECT_BASE_SYSCALL_C_H
