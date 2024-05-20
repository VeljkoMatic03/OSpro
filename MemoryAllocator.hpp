//
// Created by os on 5/20/24.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

struct BlockHeader {
    struct BlockHeader* next;
    size_t sizeBlocks;
};

class MemoryAllocator {
public:
    static void* malloc(size_t size);
    static int free(void* addr);
    static BlockHeader* freeMemHead;
private:
    static void joinBlocks (struct BlockHeader*);
    static bool initialized;

};


#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
