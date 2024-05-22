//
// Created by os on 5/22/24.
//

#include "../h/syscall_c.h"
#include "../h/print.hpp"
#include "../h/riscv.hpp"

void* mem_alloc(size_t size) {
    size_t newsize = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    __asm__ volatile ("li a0, 0x01");
    __asm__ volatile ("mv a1, %0" : : "r"(newsize));
    __asm__ volatile ("ecall");
    void* ptr;
    __asm__ volatile ("mv %0, a0" : "=r"(ptr));
    return ptr;
}

int mem_free(void* ptr) {
    uint64 memptr = (uint64) ptr;
    __asm__ volatile ("mv a1, %0" : : "r"(memptr));
    __asm__ volatile ("li a0, 0x02");
    __asm__ volatile ("ecall");
    int retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}