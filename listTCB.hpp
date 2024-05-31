//
// Created by os on 5/31/24.
//

#ifndef PROJECT_BASE_LISTTCB_HPP
#define PROJECT_BASE_LISTTCB_HPP

#include "../h/tcb.hpp"

class TCB;

class ElemTCB
{
public:
    TCB *data;
    ElemTCB *next;

    ElemTCB(TCB *data, ElemTCB *next) : data(data), next(next) {}

    /*static void* operator new(size_t size) {
        return MemoryAllocator::malloc(size / MEM_BLOCK_SIZE + 1);
    }
    static void* operator new[](size_t size) {
        return MemoryAllocator::malloc(size / MEM_BLOCK_SIZE + 1);
    }

    static void operator delete(void *ptr) {
        MemoryAllocator::free(ptr);
    }
    static void operator delete[](void *ptr) {
        MemoryAllocator::free(ptr);
    }*/
};


class ListTCB
{
private:

    ElemTCB *head, *tail;

public:
    ListTCB() : head(0), tail(0) {}

    ListTCB(const ListTCB &) = delete;

    ListTCB &operator=(const ListTCB &) = delete;

    void addFirst(TCB *data)
    {
        ElemTCB *elem = new ElemTCB(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }

    void addLast(TCB *data)
    {
        ElemTCB *elem = new ElemTCB(data, 0);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    TCB *removeFirst()
    {
        if (!head) { return 0; }

        ElemTCB *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        TCB *ret = elem->data;
        delete elem;
        return ret;
    }

    TCB *peekFirst()
    {
        if (!head) { return 0; }
        return head->data;
    }

    TCB *removeLast()
    {
        if (!head) { return 0; }

        ElemTCB *prev = 0;
        for (ElemTCB *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        ElemTCB *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        TCB *ret = elem->data;
        delete elem;
        return ret;
    }

    TCB *peekLast()
    {
        if (!tail) { return 0; }
        return tail->data;
    }

    void removeElem(TCB* elem) {
        ElemTCB *prev, *curr;
        prev = nullptr;
        curr = head;
        while(curr) {
            if(curr->data == elem) {
                if(prev) prev->next = curr->next;
                else head = curr->next;
                delete curr;
                return;
            }
        }
    }


    static void* operator new(size_t size) {
        return MemoryAllocator::malloc(size / MEM_BLOCK_SIZE + 1);
    }
    static void* operator new[](size_t size) {
        return MemoryAllocator::malloc(size / MEM_BLOCK_SIZE + 1);
    }

    static void operator delete(void *ptr) {
        MemoryAllocator::free(ptr);
    }
    static void operator delete[](void *ptr) {
        MemoryAllocator::free(ptr);
    }

};


#endif //PROJECT_BASE_LISTTCB_HPP
