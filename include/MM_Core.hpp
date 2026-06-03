#pragma once

//Unico file da includere per accedere a tutte le funzionalità del MM (pattern "umbrella header")

#include "MM_Config.hpp"
#include "IAllocator.hpp"
#include "AllocationHeader.hpp"
#include "AllocatorStats.hpp"
#include "LeakTracker.hpp"

// --- Global new/delete override (Step 5) ---
// #include "MM_GlobalOverride.hpp"   // uncomment when Step 5 is ready

namespace MM {

	class MemoryManager;    //Forward declaration

    ///Definizioni delle funzioni di allocazione e deallocazione
    
    void* MM_MALLOC(std::size_t size, const char* file = nullptr, int line = 0);

    void  MM_FREE(void* ptr, std::size_t size); 

    template<typename T>
    T* MM_NEW(const char* file = nullptr, int line = 0) { 
        return static_cast<T*>(MM_MALLOC(sizeof(T), file, line)); 
    }

    template<typename T>
    T* MM_NEW_A(std::size_t count, const char* file = nullptr, int line = 0) { 
        return static_cast<T*>(MM_MALLOC(sizeof(T) * count, file, line));
    }

    template<typename T>
    void  MM_DELETE(T* ptr) {
        if (ptr) { ptr->~T(); MM_FREE(ptr, sizeof(T)); }
    }

    template<typename T>
    void  MM_DELETE_A(T* ptr, std::size_t count) {
        if (ptr) { 
            for (std::size_t i = 0; i < count; ++i) { ptr[i].~T(); }
            MM_FREE(ptr, sizeof(T) * count);
        }
    }

}

//Macro per evitare di scrivere ogni volta __FILE__ e __LINE__
#define MM_MALLOC_LOC(size) MM::MM_MALLOC((size),  __FILE__, __LINE__)
#define MM_FREE_LOC(ptr, size) MM::MM_FREE((ptr), (size))
#define MM_NEW_LOC(T) MM::MM_NEW<T>(__FILE__, __LINE__)
#define MM_NEW_A_LOC(T, count) MM::MM_NEW_A<T>((count), __FILE__, __LINE__)
#define MM_DELETE_LOC(ptr) MM::MM_DELETE(ptr)
#define MM_DELETE_A_LOC(ptr, n) MM::MM_DELETE_A((ptr), (n))