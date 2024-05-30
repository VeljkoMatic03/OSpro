//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../test/printing.hpp"
#include "../h/riscv.hpp"

#include "../test/ConsumerProducer_C_API_test.hpp"
#include "../test/System_Mode_test.hpp"


#include "../h/syscall_c.hpp"

static sem* s;
static int i = 0;
static char[] arr = mem_alloc(sizeof (char) * 3);


void put(void*) {

}

void take(void*) {

}


int main()
{

    /*Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Allocator::init();

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

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    while (!(threads[1]->isFinished() &&
    threads[2]->isFinished() &&
    threads[3]->isFinished() &&
    threads[4]->isFinished()))
    {
    thread_dispatch();
    }

    for (auto &thread: threads)
    {
    delete thread;
    }

    printString("Finished\n");

    return 0;*/

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    TCB* nit;
    thread_create(&nit, nullptr, nullptr);
    TCB::running = nit;
    /*TCB* usermain;
    thread_create(&usermain, umain, nullptr);*/
    TCB *nit1, *nit2;
    thread_create(&nit1, umain, nullptr);
    thread_create(&nit2, umain, nullptr);

    sem_open(&s, 1);


    while(!nit1->isFinished() || !nit2->isFinished()) thread_dispatch();

    sem_close(s);
    printString("\n");
    printInteger(i);


    return 0;
}