#ifndef GLOBALES_H
#define GLOBALES_H
#include <cstddef>

extern size_t memoriaGlobal;
extern long iteracionesGlobales;

void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;

#endif