//
// Created by os on 5/22/24.
//

#include "../h/syscall_c.h"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/mem.h"
#include "../h/MemoryAllocator.hpp"

void* mem_alloc(size_t size) {
    size = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    __asm__ volatile ("mv a1, %0" : : "r"(size));
    __asm__ volatile ("li a0, 0x01");
    __asm__ volatile ("ecall");
    void* volatile ptr;
    __asm__ volatile ("mv %0, a0" : "=r"(ptr));
    return (void*)ptr;
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

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    uint64* volatile stack = (uint64*) mem_alloc(DEFAULT_STACK_SIZE);
    //auto stack = (uint64*) (new uint64[DEFAULT_STACK_SIZE]);
    uint64 volatile argument = (uint64) arg;
    uint64 volatile func = (uint64) start_routine;
    uint64 volatile handler = (uint64) handle;
    __asm__ volatile ("mv a4, %0" : : "r"((uint64)stack));
    __asm__ volatile ("mv a3, %0" : : "r"(argument));
    __asm__ volatile ("mv a2, %0" : : "r"(func));
    __asm__ volatile ("mv a1, %0" : : "r"(handler));
    __asm__ volatile ("li a0, 0x11");
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}
