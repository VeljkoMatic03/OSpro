//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../h/console.hpp"

extern void userMain();

void wrapper(void* v) {
    userMain();
}

int main()
{
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    TCB* mainNit;
    thread_create(&mainNit, nullptr, nullptr);
    TCB::running = mainNit;

    TCB *userNit;
    thread_create(&userNit, wrapper, nullptr);

    console::init();

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    while(!userNit->isFinished()) {
        thread_dispatch();
    }
    __asm__ volatile ("li t3, 0x100000");
    __asm__ volatile ("li t4, 0x5555");
    __asm__ volatile ("sw t4, 0(t3)");
    return 0;
}