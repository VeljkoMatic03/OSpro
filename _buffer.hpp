//
// Created by os on 5/31/24.
//

#ifndef PROJECT_BASE__BUFFER_HPP
#define PROJECT_BASE__BUFFER_HPP

class sem;
typedef sem* sem_t;

class _buffer {
private:
    int cap;
    char *buffer;
    int head, tail;

    sem_t spaceAvailable;
    sem_t itemAvailable;

public:
    _buffer(int _cap);
    ~_buffer();

    void put(char val);
    char get();

    int getCnt();



};


#endif //PROJECT_BASE__BUFFER_HPP
