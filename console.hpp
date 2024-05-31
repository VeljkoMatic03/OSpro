//
// Created by os on 5/31/24.
//

#ifndef PROJECT_BASE_CONSOLE_HPP
#define PROJECT_BASE_CONSOLE_HPP

#include "../h/_buffer.hpp"

class sem;

class console {
public:
    static _buffer *input;
    static _buffer *output;
    static void init();
    static void putc(char c);
    static char getc();
};


#endif //PROJECT_BASE_CONSOLE_HPP
