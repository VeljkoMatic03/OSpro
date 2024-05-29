//
// Created by marko on 20.4.22..
//
/*
#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../lib/mem.h"

void usermain(void*) {
    for(int i = 0; i < 100; i++) {
        //printInteger(i);
        __putc('a');
        __putc('\n');
    }
}

int main()
{
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    //Riscv::mc_sstatus(Riscv::SSTATUS_SIE);

    *TCB *threads[5];

    threads[0] = TCB::createThread(nullptr);
    TCB::running = threads[0];

    threads[1] = TCB::createThread(workerBodyA);
    printString("ThreadA created\n");
    threads[2] = TCB::createThread(workerBodyB);
    printString("ThreadB created\n");
    threads[3] = TCB::createThread(workerBodyC);
    printString("ThreadC created\n");
    threads[4] = TCB::createThread(workerBodyD);
    printString("ThreadD created\n");

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    while (!(threads[1]->isFinished() &&
             threads[2]->isFinished() &&
             threads[3]->isFinished() &&
             threads[4]->isFinished()))
    {
        TCB::yield();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }
    printString("Finished\n");

    return 0;*/



    /*char* arr = (char*) mem_alloc(26*sizeof (char));
    char c = 65;
    for(int i = 0; i < 26; i++){
        arr[i] = c + i;
    }
    for(int i = 0; i < 26; i++) __putc(arr[i]);

    int status = mem_free(arr);
    printInteger(status);*/

    /*
    TCB* main;
    thread_create(&main, nullptr, nullptr);
    //TCB::createThread(&main, nullptr, nullptr, nullptr);
    TCB::running = main;
    TCB* nit;
    //uint64* stack = (uint64*) MemoryAllocator::malloc(DEFAULT_STACK_SIZE / TCB::STACK_SIZE + 1);
    //auto stack = (uint64*) (new uint64[DEFAULT_STACK_SIZE]);

    //TCB::createThread(&nit, usermain, nullptr, stack);

    thread_create(&nit, usermain, nullptr);

    TCB::yield();






    return 0;
}
*/

#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"

#include "../h/syscall_c.h"

int main()
{
    //TCB *threads[5];

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    //Allocator::init();

    TCB *threads[5];

    thread_create(&threads[0],nullptr, nullptr);
    TCB::running = threads[0];
    thread_create(&threads[1],workerBodyA, nullptr);
    thread_create(&threads[2],workerBodyB, nullptr);
    thread_create(&threads[3],workerBodyC, nullptr);
    thread_create(&threads[4],workerBodyD, nullptr);

    printString("ThreadA created\n");
    printString("ThreadB created\n");
    printString("ThreadC created\n");
    printString("ThreadD created\n");

    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    while (!(threads[1]->isFinished() &&
             threads[2]->isFinished() &&
             threads[3]->isFinished() &&
             threads[4]->isFinished()))
    {
        //thread_dispatch();
        TCB::dispatch();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }

    printString("Finished\n");

    return 0;
}