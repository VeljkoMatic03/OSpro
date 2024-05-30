//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../test/printing.hpp"

TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

int TCB::createThread(TCB** handle, Body body, void* arg, uint64* stackptr)
{
    *handle = new TCB(body, arg, DEFAULT_TIME_SLICE, stackptr);
    if(*handle) return 0;
    return -1;
}

void TCB::yield()
{
    __asm__ volatile ("li a0, 0x13");
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && !old->isBlocked) { Scheduler::put(old); }
    running = Scheduler::get();

    /*if(!running->body) {
        Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
    }
    else Riscv::ms_sstatus(Riscv::SSTATUS_SPP);*/

    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    if(running->body) Riscv::ms_sstatus(Riscv::SSTATUS_SPP);
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    TCB::yield();
}

int TCB::exit() {
    running->setFinished(true);
    TCB::dispatch();
    return 0;
}

void TCB::block() {
    running->isBlocked = true;
    TCB::yield();
}

void TCB::unblock() {
    this->isBlocked = false;
    Scheduler::put(this);
}