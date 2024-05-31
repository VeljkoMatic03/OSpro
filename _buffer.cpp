//
// Created by os on 5/31/24.
//

#include "../h/_buffer.hpp"
#include "../h/syscall_c.hpp"
#include "../h/MemoryAllocator.hpp"

_buffer::_buffer(int _cap) : cap(_cap + 1), head(0), tail(0) {
    buffer = (char *) MemoryAllocator::malloc(sizeof(char) * cap / MEM_BLOCK_SIZE + 1);
    sem_open(&itemAvailable, 0);
    sem_open(&spaceAvailable, _cap);
}

_buffer::~_buffer() {
    MemoryAllocator::free(buffer);
    sem_close(itemAvailable);
    sem_close(spaceAvailable);
}

void _buffer::put(char val) {
    sem_wait(spaceAvailable);

    buffer[tail] = val;
    tail = (tail + 1) % cap;

    sem_signal(itemAvailable);

}

char _buffer::get() {
    sem_wait(itemAvailable);


    char ret = buffer[head];
    head = (head + 1) % cap;

    sem_signal(spaceAvailable);

    return ret;
}

int _buffer::getCnt() {
    int ret;


    if (tail >= head) {
        ret = tail - head;
    } else {
        ret = cap - head + tail;
    }


    return ret;
}
