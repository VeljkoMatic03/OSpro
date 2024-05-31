//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../test/printing.hpp"

TCB *TCB::running = nullptr;

TCB* TCB::head = nullptr;
TCB* TCB::tail = nullptr;

uint64 TCB::timeSliceCounter = 0;
uint64 TCB::firstTimeLeftAsleep = 0;

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
    if (!old->isFinished() && !old->isBlocked && !old->isSleeping) { Scheduler::put(old); }
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
    TCB::dispatch();
}

void TCB::unblock() {
    this->isBlocked = false;
    Scheduler::put(this);
}

int TCB::toSleep(time_t timeout) {
    if(!TCB::running) return -1;
    TCB::running->thisTimeLeftAsleep = timeout;
    TCB::running->isSleeping = true;
    if(!head) {
        head = tail = TCB::running;
        TCB::running->nextAsleep = nullptr;
        TCB::firstTimeLeftAsleep = TCB::running->thisTimeLeftAsleep;
        TCB::dispatch();
        return 0;
    }
    TCB *curr = TCB::head;
    TCB *prev = nullptr;
    while(curr) {
        uint64 prevTime = (prev ? prev->thisTimeLeftAsleep : 0);
        if(TCB::running->thisTimeLeftAsleep - prevTime < curr->thisTimeLeftAsleep) {
            TCB::running->thisTimeLeftAsleep -= prevTime;
            TCB::running->nextAsleep = curr;
            if(prev) prev->nextAsleep = TCB::running;
            else {
                head = TCB::running;
                TCB::firstTimeLeftAsleep = TCB::running->thisTimeLeftAsleep;
            }
            prevTime = TCB::running->thisTimeLeftAsleep;
            while(curr) {
                //printString("i ");
                curr->thisTimeLeftAsleep -= prevTime;
                prevTime = curr->thisTimeLeftAsleep;
                curr = curr->nextAsleep;
            }
            TCB::dispatch();
            return 0;
        }
        else {
            TCB::running->thisTimeLeftAsleep -= prevTime;
            prev = curr;
            curr = curr->nextAsleep;
        }
    }
    TCB::running->thisTimeLeftAsleep -= TCB::tail->thisTimeLeftAsleep;
    tail->nextAsleep = TCB::running;
    TCB::running->nextAsleep = nullptr;
    tail = TCB::running;
    TCB::dispatch();
    return 0;
}

void TCB::wakeUp() {
    TCB *curr = TCB::head;
    //int i = 0;
    while(curr && curr->thisTimeLeftAsleep <= 0) {
//        printInteger(i++);
//        printString("  ");
        curr->isSleeping = false;
        Scheduler::put(curr);
        curr = curr->nextAsleep;
        TCB::head = curr;
        TCB::firstTimeLeftAsleep = (TCB::head ? TCB::head->thisTimeLeftAsleep : 0);
    }
}

void TCB::updateAsleep() {
    if(!head) return;
    TCB::head->thisTimeLeftAsleep--;
    TCB::firstTimeLeftAsleep--;
    if(TCB::firstTimeLeftAsleep == 0) wakeUp();
}

void TCB::pullOutAsleepThread(TCB* t) {
    TCB *prev, *curr;
    prev = nullptr;
    curr = TCB::head;
    while(curr) {
        if(curr == t) {
            TCB *nextcurr = curr->nextAsleep;
            if(nextcurr) nextcurr->thisTimeLeftAsleep += curr->thisTimeLeftAsleep;
            if(prev) prev->nextAsleep = nextcurr;
            else head = nextcurr;
            if(curr == tail) tail = prev;
            break;
        }
        else {
            prev = curr;
            curr = curr->nextAsleep;
        }
    }
}