#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/sem.hpp"
#include "../h/console.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 volatile scause = r_scause();
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

        int volatile retval;
        char volatile c;

        switch (a0) {
            case 0x01:
                //mem_alloc(size_t)
                void* volatile pointer;
                pointer = MemoryAllocator::malloc((size_t) a1);
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(pointer));
                break;

            case 0x02:
                //mem_free(void*)
                retval = MemoryAllocator::free((void*) a1);
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(retval));
                break;

            case 0x11:
                //create_thread()
                retval = TCB::createThread((TCB**) a1, (void(*)(void*)) a2, (void*) a3, (uint64*) a4);
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(retval));
                break;

            case 0x12:
                //thread_exit()
                TCB::exit();
                __asm__ volatile ("li t1, 0x0");
                __asm__ volatile ("sw t1, 80(s0)");
                break;

            case 0x013:
                //thread_dispatch()
                TCB::dispatch();
                break;

            case 0x21:
                //sem_open()
                retval = sem::createSem((sem**) a1, (unsigned) a2);
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(retval));
                break;

            case 0x22:
                //sem_close()
                retval = ((sem*)a1)->close();
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(retval));
                break;

            case 0x23:
                //sem_wait()
                retval = ((sem*)a1)->wait();
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(retval));
                break;

            case 0x24:
                //sem_signal()
                retval = ((sem*)a1)->signal();
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(retval));
                break;

            case 0x25:
                //sem_timedwait()
                retval = ((sem*)a1)->timedwait(a2);
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(retval));
                break;

            case 0x26:
                //sem_trywait()
                retval = ((sem*)a1)->trywait();
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(retval));
                break;

            case 0x31:
                //time_sleep()
                retval = TCB::toSleep(a1);
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(retval));
                break;

            case 0x41:
                //getc()
                /*while((*((char*) CONSOLE_STATUS) & CONSOLE_RX_STATUS_BIT) && console::input->getCnt() != 100) {
                    volatile char c = *(char *) CONSOLE_RX_DATA;
                    console::input->put(c);
                }*/
                c = console::getc();
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(c));
                break;

            case 0x42:
                //putc()
                console::putc((char) a1);
                while ((*((char *) CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT) && console::output->getCnt() != 0) {
                    char volatile c = console::output->get();
                    *(volatile char *) CONSOLE_TX_DATA = c;
                }
                break;

            default:
                break;
        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        TCB::updateAsleep();
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
        uint64 sepc = r_sepc();
        uint64 sstatuc = r_sstatus();

        uint64 ir_num = plic_claim();
        plic_complete((int)ir_num);
        Riscv::mc_sip(Riscv::SIP_SSIP);

        if(ir_num == CONSOLE_IRQ) {
            while((*((char*) CONSOLE_STATUS) & CONSOLE_RX_STATUS_BIT) && console::input->getCnt() != 100) {
                volatile char c = *(char *) CONSOLE_RX_DATA;
                console::input->put(c);
            }
            while ((*((char *) CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT) && console::output->getCnt() != 0) {
                char volatile c = console::output->get();
                *(volatile char *) CONSOLE_TX_DATA = c;
            }
        }


        w_sepc(sepc);
        w_sstatus(sstatuc);
    }
    else
    {
        // unexpected trap cause
    }
}