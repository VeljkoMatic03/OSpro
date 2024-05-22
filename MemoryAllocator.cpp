#include "../h/MemoryAllocator.hpp"

BlockHeader* MemoryAllocator::freeMemHead = nullptr;
bool MemoryAllocator::initialized = false;

int MemoryAllocator::free(void *addr) {
    if((uint64*)addr < (uint64*)HEAP_START_ADDR || (uint64*)addr > (uint64*)HEAP_END_ADDR) return -1;
    if((uint64*)addr < (uint64*)freeMemHead) {
        BlockHeader* old = freeMemHead;
        freeMemHead = (BlockHeader*) ((uint64*)addr - sizeof(BlockHeader));
        freeMemHead->next = old;
        joinBlocks(freeMemHead);
        return 0;
    }
    if(!freeMemHead) {
        BlockHeader* newNode = (BlockHeader*) ((uint64*)addr - sizeof(BlockHeader));
        newNode->next = nullptr;
        freeMemHead = newNode;
        return 0;
    }
    BlockHeader *prev = freeMemHead, *curr = freeMemHead->next;
    for(; !(prev < addr && addr < curr) && curr; prev = curr, curr = curr->next);
    if(!curr){
        BlockHeader* newNode = (BlockHeader*) ((uint64*)addr - sizeof(BlockHeader));
        newNode->next = nullptr;
        prev->next = newNode;
        joinBlocks(prev);
        return 0;
    }
    BlockHeader* newNode = (BlockHeader*) ((uint64*)addr - sizeof (BlockHeader));
    prev->next = newNode;
    newNode->next = curr;
    joinBlocks(newNode);
    joinBlocks(prev);
    return 0;
}

void* MemoryAllocator::malloc(size_t size) {
    if(!freeMemHead && !initialized){
        initialized = true;
        freeMemHead = (BlockHeader*) ((uint64*) HEAP_START_ADDR);
        freeMemHead->next = nullptr;
        freeMemHead->sizeBlocks = (uint64) ((uint8*) HEAP_END_ADDR - (uint8*) HEAP_START_ADDR -
                                                             sizeof(BlockHeader)) / MEM_BLOCK_SIZE;
    }
    BlockHeader *curr = freeMemHead, *prev= nullptr;
    for(; curr; prev = curr, curr = curr->next)
        if(curr->sizeBlocks > size) break;
    if(!curr) return 0;
    if(curr->sizeBlocks == size + 1) {
        if(prev) prev->next = curr->next;
        else freeMemHead = curr->next;
        return (void*)((uint8*) curr + sizeof(BlockHeader));
    }
    BlockHeader* newNode = (BlockHeader*) ((uint8*) curr + (size + 1) * MEM_BLOCK_SIZE);
    if(prev) prev->next = newNode;
    else freeMemHead = newNode;
    newNode->next = curr->next;
    newNode->sizeBlocks = curr->sizeBlocks - size - 1;
    curr->sizeBlocks = size + 1;
    return (void*) ((uint8*) curr + sizeof(BlockHeader));
}

void MemoryAllocator::joinBlocks(BlockHeader* prev) {
    if(!prev->next) return;
    if((uint64 *)prev + prev->sizeBlocks * MEM_BLOCK_SIZE == (uint64*) prev->next) {
        BlockHeader* old = prev->next;
        prev->next = old->next;
        prev->sizeBlocks += old->sizeBlocks;
    }
    return;
}