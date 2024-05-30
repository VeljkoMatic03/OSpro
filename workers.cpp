//
// Created by marko on 20.4.22..
//

#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../test/printing.hpp"
#include "../h/syscall_c.hpp"
#include "../lib/console.h"

void workerBodyA(void*)
{
    for (uint64 i = 0; i < 10; i++)
    {
        printString("A: i=");
        printInteger(i);
        printString("\n");
        /*__putc('a');
        __putc('\n');*/
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                // busy wait
            }
            //TCB::dispatch();
            thread_dispatch();
        }
    }
}

void workerBodyB(void*)
{
    for (uint64 i = 0; i < 16; i++)
    {
        printString("B: i=");
        printInteger(i);
        printString("\n");
        /*__putc('b');
        __putc(i + 65);
        __putc('\n');*/
        if(i == 10) {
            __asm__ volatile ("csrr t6, sepc");
        }
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                // busy wait
            }
            //TCB::dispatch();
            thread_dispatch();
        }
    }
}

static uint64 fibonacci(uint64 n)
{
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) { //TCB::dispatch();
        thread_dispatch();
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyC(void*)
{
    uint8 i = 0;
    for (; i < 3; i++)
    {
        printString("C: i=");
        printInteger(i);
        printString("\n");
        /*__putc('c');
        __putc('\n');*/
    }

    printString("C: yield\n");
    __asm__ ("li t1, 7");
    //TCB::dispatch();
    thread_dispatch();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    printString("C: t1=");
    printInteger(t1);
    printString("\n");

    uint64 result = fibonacci(12);
    printString("C: fibonaci=");
    printInteger(result);
    printString("\n");

    for (; i < 6; i++)
    {
        printString("C: i=");
        printInteger(i);
        printString("\n");
        //__putc('c');
        //__putc('\n');
    }
    //TCB::dispatch();
    thread_dispatch();
}

void workerBodyD(void*)
{
    uint8 i = 10;
    for (; i < 13; i++)
    {
        printString("D: i=");
        printInteger(i);
        printString("\n");
        //__putc('d');
        //__putc('\n');
    }

    printString("D: yield\n");
    __asm__ ("li t1, 5");
    //TCB::dispatch();
    thread_dispatch();

    uint64 result = fibonacci(16);
    printString("D: fibonaci=");
    printInteger(result);
    printString("\n");

    for (; i < 16; i++)
    {
        printString("D: i=");
        printInteger(i);
        printString("\n");
        //__putc('d');
        //__putc('\n');
    }
    //TCB::dispatch();
    thread_dispatch();
}