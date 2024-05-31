#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP

#include "../h/MemoryAllocator.hpp"
#include "../test/printing.hpp"

template<typename T>
class Elem
{
public:
    T *data;
    Elem *next;

    Elem(T *data, Elem *next) : data(data), next(next) {}

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


template<typename T>
class List
{
private:

    Elem<T> *head, *tail;

public:
    List() : head(0), tail(0) {}

    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    void addFirst(T *data)
    {
        Elem<T> *elem = new Elem<T>(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }

    void addLast(T *data)
    {
        Elem<T> *elem = new Elem<T>(data, 0);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    T *removeFirst()
    {
        if (!head) { return 0; }

        Elem<T> *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekFirst()
    {
        if (!head) { return 0; }
        return head->data;
    }

    T *removeLast()
    {
        if (!head) { return 0; }

        Elem<T> *prev = 0;
        for (Elem<T> *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem<T> *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekLast()
    {
        if (!tail) { return 0; }
        return tail->data;
    }

    void removeElem(T* elem) {
        Elem<T> *prev, *curr;
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

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP
