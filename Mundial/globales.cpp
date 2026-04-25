#include "Globales.h"
#include <cstdlib>

size_t memoriaGlobal = 0;
long iteracionesGlobales = 0;

void* operator new(size_t size) {
    memoriaGlobal += size;
    return malloc(size);
}

void* operator new[](size_t size) {
    memoriaGlobal += size;
    return malloc(size);
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}