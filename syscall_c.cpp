//
// Created by os on 5/22/24.
//

#include "../h/syscall_c.hpp"
#include "../test/printing.hpp"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/mem.h"
#include "../h/MemoryAllocator.hpp"

void* mem_alloc(size_t size) {
    void* volatile ptr;
    size = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    __asm__ volatile ("mv a1, %0" : : "r"(size));
    __asm__ volatile ("li a0, 0x01");
    __asm__ volatile ("ecall");
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
    uint64* stack = (uint64*) MemoryAllocator::malloc(DEFAULT_STACK_SIZE * sizeof (uint64) /  MEM_BLOCK_SIZE + 1);
//    printInteger((uint64) stack);
//    printString(" THREAD STACK\n");
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

int thread_exit() {
    __asm__ volatile ("li a0, 0x12");
    __asm__ volatile ("ecall");

    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

void thread_dispatch() {
    __asm__ volatile ("li a0, 0x13");
    __asm__ volatile ("ecall");
}

int sem_open(sem_t* handle, unsigned init) {
    uint64 volatile handler = (uint64) handle;
    __asm__ volatile ("mv a2, %0" : : "r"(init));
    __asm__ volatile ("mv a1, %0" : : "r"(handler));
    __asm__ volatile ("li a0, 0x21");
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

int sem_close(sem_t handle) {
    uint64 volatile handler = (uint64) handle;
    __asm__ volatile ("mv a1, %0" : : "r"(handler));
    __asm__ volatile ("li a0, 0x22");
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

int sem_wait(sem_t id) {
    uint64 volatile handler = (uint64) id;
    __asm__ volatile ("mv a1, %0" : : "r"(handler));
    __asm__ volatile ("li a0, 0x23");
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

int sem_signal(sem_t id) {
    uint64 volatile handler = (uint64) id;
    __asm__ volatile ("mv a1, %0" : : "r"(handler));
    __asm__ volatile ("li a0, 0x24");
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

int sem_timedwait(sem_t id, time_t timeout) {
    uint64 volatile handler = (uint64) id;
    __asm__ volatile ("mv a2, %0" : : "r"(timeout));
    __asm__ volatile ("mv a1, %0" : : "r"(handler));
    __asm__ volatile ("li a0, 0x25");
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

int sem_trywait(sem_t id) {
    uint64 volatile handler = (uint64) id;
    __asm__ volatile ("mv a1, %0" : : "r"(handler));
    __asm__ volatile ("li a0, 0x26");
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

int time_sleep(time_t timeout) {
    __asm__ volatile ("mv a1, %0" : : "r"(timeout));
    __asm__ volatile ("li a0, 0x31");
    __asm__ volatile ("ecall");
    int retval;
    __asm__ volatile ("mv a0, %0" : "=r"(retval));
    return retval;
}