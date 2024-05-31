//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../test/printing.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../lib/console.h"


extern void userMain();


static sem_t sem;
void workersA(void * arg){
    //sem_open(&sem,1);
    sem_wait(sem);
    for(int i=0;i<10000;i++)
    {
        for(int j=0;j<10000;j++){
            int a = i+j;
            a=a;
        }
        printString("A");
        if(i==100){
            time_sleep(10);
            //time_sleep(20);
        }
    }
    printString("\n");

    sem_signal(sem);
}
void workersB(void * arg){
    //sem_open(&sem,1);
    sem_timedwait(sem,4);
    //sem_wait(sem);
    for(int i=0;i<100;i++)
    {
        for(int j=0;j<10000;j++){

        }
        printString("B");
    }
    printString("\n");

    sem_signal(sem);
}
void workersC(void* arg){
    //sem_open(&sem,1);
    sem_wait(sem);
    for(int i=0;i<100;i++)
    {
        for(int j=0;j<10000;j++){

        }
        printString("C");
    }
    printString("\n");
    sem_signal(sem);
}



void wrapper(void* v) {
    userMain();
}


int main()
{

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    TCB* nit;
    thread_create(&nit, nullptr, nullptr);
    TCB::running = nit;

    sem_open(&sem, 1);

    TCB *nit1, *nit2, *nit3;
    thread_create(&nit1, workersA, nullptr);
    thread_create(&nit2, workersB, nullptr);
    thread_create(&nit3, workersC, nullptr);
    //TCB *unit;
    //thread_create(&unit, wrapper, nullptr);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    while(!nit1->isFinished() || !nit2->isFinished() || !nit3->isFinished()) thread_dispatch();
    //printString("Zapocet userMain()\n");

    /*while(!unit->isFinished()) {
        thread_dispatch();
    }*/

    printString("Gotovo\n");

    return 0;
}