//
// Created by os on 5/30/24.
//

#ifndef PROJECT_BASE__NEW_HPP
#define PROJECT_BASE__NEW_HPP

using size_t = decltype(sizeof(0));

void *operator new(size_t n);

void *operator new[](size_t n);

void operator delete(void *p) noexcept;

void operator delete[](void *p) noexcept;


#endif //PROJECT_BASE__NEW_HPP
