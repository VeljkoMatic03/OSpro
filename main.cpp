//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"

int main()
{
    /*TCB *threads[5];

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

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    char* arr = (char*) mem_alloc(26*sizeof (char));
    char c = 65;
    for(int i = 0; i < 26; i++){
        arr[i] = c + i;
    }
    for(int i = 0; i < 26; i++) __putc(arr[i]);

    int status = mem_free(arr);
    printInteger(status);

    return 0;
}
