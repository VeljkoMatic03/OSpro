#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/print.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 scause = r_scause();
    uint64 volatile a0 = Riscv::r_a0();
    uint64 volatile a1 = Riscv::r_a1();
    uint64 volatile a2 = Riscv::r_a2();
    uint64 volatile a3 = Riscv::r_a3();
    uint64 volatile a4 = Riscv::r_a4();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        a2 = a2;
        a3 = a3;
        a4 = a4;
        int retval;

        switch (a0) {
            case 0x01:
                //mem_alloc(size_t)
                void* pointer;
                /*printString("\nSize:");
                printInteger(size);*/
                pointer = MemoryAllocator::malloc((size_t) a1);
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(pointer));
                break;

            case 0x02:
                //mem_free(void*)
                retval = MemoryAllocator::free((void*) a1);
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(retval));
                break;

            case 0x11:
                //thread_create
                break;
        }



        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        // unexpected trap cause
    }
}