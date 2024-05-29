//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"

TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

int TCB::createThread(TCB** handle, Body body, void* arg, uint64* stackptr)
{
    *handle = new TCB(body, arg, TIME_SLICE, stackptr);
    if(*handle) return 0;
    return -1;
}

void TCB::yield()
{
    //__asm__ volatile ("li a0, 0x12");
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished()) { Scheduler::put(old); }
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    //Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    TCB::dispatch();
}
