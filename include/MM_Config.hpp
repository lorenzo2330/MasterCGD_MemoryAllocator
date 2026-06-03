#pragma once

//File di config per settare i vari valori

#include <cstddef>

namespace MM {

	static constexpr std::size_t SMALL_THRESHOLD = 128;     //Se size <= SMALL_TRESHOLD -> SmallObjectAllocator
                                                            //Se SMALL_TRESHOLD < size < LARGE_THRESHOLD -> DefaultAllocator
	static constexpr std::size_t LARGE_THRESHOLD = 128;     //Se size >= LARGE_THRESHOLD -> GeneralPurposeAllocator (TODO big?)

    //SmallObjectAllocator parameters
    static constexpr std::size_t SMALL_OBJECT_MAX_SIZE = 128;   //Dim massima di un FixedAllocator (quindi di un Chunk) (=SMALL_THRESHOLD)   //TODO sostituire con SMALL_THRESHOLD
    static constexpr std::size_t SMALL_OBJECT_ALIGN = 8;        //Allineamento dei Chunk (se si richiede 13 -> si danno 16 (prossimo multiplo di 8)
    static constexpr std::size_t CHUNK_SIZE = 4096;             //Numero massimo di elementi per ogni chunk

    //GeneralPurposeAllocator parameters
    static constexpr std::size_t GPA_INITIAL_POOL_SIZE = 1024 * 1024 * 256; //256 MB di pool iniziale
    static constexpr std::size_t GPA_ALIGNMENT = 16;            //Allineamento garantito per ogni allocazione

    //Parametri per fill pattern
    static constexpr unsigned char FILL_ALLOC_PATTERN = 0xCD;   //Valore per pulire la memoria appena allocata (CD per convenzione Microsoft)
    static constexpr unsigned char FILL_FREE_PATTERN = 0xDD;    //Valore per pulire la memoria appena deallocata (DD per convenzione Microsoft)

	//Parametri magic number
    static constexpr unsigned int MAGIC_ALLOCATED = 0xA110CA1D; //Valore nell'header per indicare che la memoria è occupata (quindi liberabile e non nuovamente occupabile)
	static constexpr unsigned int MAGIC_FREED = 0xDEADBEEF;     //Valore nell'header per indicare che la memoria è libera (quindi allocabile e non nuovamente liberabile)

#ifndef MM_ENABLE_STATS
#ifdef NDEBUG
#define MM_ENABLE_STATS 0
#else
#define MM_ENABLE_STATS 1
#endif
#endif

#ifndef MM_ENABLE_LEAKS
#ifdef NDEBUG
#define MM_ENABLE_LEAKS 0
#else
#define MM_ENABLE_LEAKS 1
#endif
#endif

//Utilizza i magic number nell'header
#ifndef MM_ENABLE_CORRUPTION_DETECTION
#define MM_ENABLE_CORRUPTION_DETECTION 1
#endif

//Utilizza i fill pattern
#ifndef MM_ENABLE_FILL_PATTERN
#define MM_ENABLE_FILL_PATTERN 1
#endif

#ifndef MM_THREAD_SAFE
#define MM_THREAD_SAFE 1
#endif

}