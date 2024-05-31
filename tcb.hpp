#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "../h/MemoryAllocator.hpp"

// Thread Control Block
class TCB
{
public:
    ~TCB() {
        delete[] stack;
    }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    static int createThread(TCB** handle, Body body, void* arg, uint64* stackptr);

    static void yield();

    static TCB *running;

    static uint64 constexpr STACK_SIZE = 1024;

    static void dispatch();

    static int exit();

    static void block();

    void unblock();

    static int toSleep(time_t timeout);

    static void wakeUp();

    static void updateAsleep();

    static void pullOutAsleepThread(TCB* t);

    static void* operator new(size_t size) {
        return MemoryAllocator::malloc(size / MEM_BLOCK_SIZE + 1);
    }
    static void* operator new[](size_t size) {
        return MemoryAllocator::malloc(size / MEM_BLOCK_SIZE + 1);
    }

    static void operator delete(void *ptr) {
        if(!ptr) return;
        MemoryAllocator::free(ptr);
    }
    static void operator delete[](void *ptr) {
        if(!ptr) return;
        MemoryAllocator::free(ptr);
    }


private:
    TCB(Body body, void* arg, uint64 timeSlice, uint64* stackptr) :
            body(body),
            arg(arg),
            stack(stackptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            finished(false),
            isBlocked(false),
            thisTimeLeftAsleep(0),
            nextAsleep(nullptr),
            isSleeping(false)
    {
        if (body != nullptr) { Scheduler::put(this); }
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* arg;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    bool isBlocked;
    uint64 thisTimeLeftAsleep;
    TCB* nextAsleep;
    bool isSleeping;

    friend class Riscv;
    friend class sem;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static TCB *head, *tail;

    static uint64 timeSliceCounter;
    static uint64 firstTimeLeftAsleep;

    static uint64 constexpr TIME_SLICE = 2;
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
