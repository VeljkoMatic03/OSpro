//
// Created by os on 5/31/24.
//

#include "../h/console.hpp"

_buffer* console::input = nullptr;
_buffer* console::output = nullptr;

void console::init() {
    input = new _buffer(100);
    output = new _buffer(100);
}

void console::putc(char c) {
    output->put(c);
}

char console::getc() {
    return input->get();
}