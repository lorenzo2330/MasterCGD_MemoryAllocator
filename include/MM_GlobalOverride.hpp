#pragma once

//File per gestire l'override di new e delete a livello globale, in modo da usare quelli custom del progetto
//NOTA: bisogna includerlo solo in un .cpp, altrimenti da errore di multiple definition

/*
* Nota: è buona pratica, se ne si ridefinisce una, farlo con tutte, quindi:
* -new (throwing (se fallisce lancia un'eccezione) e non-throwing (se fallisce restituisce nullptr))
* -new[] (throwing e non-throwing)
* -delete (sized (chiede la size in input) e non-sized)
* -delete[] (sized e non-sized)
*/

//TODO: in teoria questo file non serve, perchè non so se useremo mai quello globale, quindi forse posso toglierlo

#ifdef USE_MM_GLOBAL

#include "MM_Core.hpp"
#include <cstdlib>    
#include <new>        

//new throwing variants (se fallisce lancia un'eccezione)
void* operator new(std::size_t size) {
    void* ptr = MM::MM_MALLOC(size);
    if (!ptr) throw std::bad_alloc{};
    return ptr;
}

//new[] throwing variants (se fallisce lancia un'eccezione)
void* operator new[](std::size_t size) {
    void* ptr = MM::MM_MALLOC(size);
    if (!ptr) throw std::bad_alloc{};
    return ptr;
}

//new non-throwing variants (se fallisce restituisce nullptr)
void* operator new(std::size_t size, const std::nothrow_t&) noexcept { return MM::MM_MALLOC(size); }

//new non-throwing variants (se fallisce restituisce nullptr)
void* operator new[](std::size_t size, const std::nothrow_t&) noexcept { return MM::MM_MALLOC(size); }

//delete non-sized variants
void operator delete(void* ptr) noexcept { MM::MM_FREE(ptr, 0); }

//delete[] non-sized variants 
void operator delete[](void* ptr) noexcept { MM::MM_FREE(ptr, 0); }

//delete sized variants
void operator delete(void* ptr, std::size_t size) noexcept { MM::MM_FREE(ptr, size); }

//delete[] sized variants
void operator delete[](void* ptr, std::size_t size) noexcept { MM::MM_FREE(ptr, size); }

#endif