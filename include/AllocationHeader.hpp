#pragma once

//Header per ogni blocco allocato   //TODO: da usare solo in debug o solo per GP, per lo small è un overhead inutile
//Struttura:                                v---puntatore restituito al chiamante
//    [ AllocationHeader ][ optional guard ][ user data ][ optional guard ]

#include "MM_Config.hpp"
#include <cstddef>
#include <cstdint>

namespace MM {

	//Tag per identificare quale allocator possiede un blocco
    enum class AllocatorTag : uint8_t { Unknown = 0, Small = 1, General = 2, System = 3 };
    
	//Contiene informazioni su ogni blocco allocato
    struct AllocationHeader {
        std::size_t  size;          //Dimensione richiesta dall'utente
        AllocatorTag tag;           //Allocatore utilizzato (0 = unknown, 1 = small, 2 = GP, 3 = system)
        uint8_t      _pad[3];       //Padding per mantenere una struct size multipla di 4
#if MM_ENABLE_CORRUPTION_DETECTION
        uint32_t     magic;         //Magic number (vedi OneNote > AdvC++ > Homework3)
#endif
#if MM_ENABLE_LEAKS
        const char* file;           //File per i leak report
        int line;                   //Riga del file per il leak report
#endif
    };

	///Dimensione dell'header, allineata a GPA_ALIGNMENT
    inline constexpr std::size_t HeaderSize() { return (sizeof(AllocationHeader) + MM::GPA_ALIGNMENT - 1) & ~(MM::GPA_ALIGNMENT - 1); }

    ///Dato il puntatore al blocco, restituisce l'indirizzo del primo byte utilizzabile dall'utente (inizio blocco + dim header)
    inline void* UserPtrFromRaw(void* raw) { return static_cast<char*>(raw) + HeaderSize(); }

    ///Dato il puntatore al primo blocco utilizzabile dall'utente, restituisce l'indirizzo del blocco (utente - dim header)
    inline void* RawFromUserPtr(void* user) { return static_cast<char*>(user) - HeaderSize(); }

    ///Dato il puntatore al primo blocco utilizzabile dall'utente, restituisce un riferimento all'header corrispondente
    inline AllocationHeader* HeaderFromUserPtr(void* user) { return reinterpret_cast<AllocationHeader*>(RawFromUserPtr(user)); }

	///Inizializza l'header di un blocco con i dati forniti
    inline void InitHeader(void* rawBlock, std::size_t  size, AllocatorTag tag, const char* file = nullptr, int line = 0)
    {
        AllocationHeader* hdr = reinterpret_cast<AllocationHeader*>(rawBlock);
        hdr->size = size;
        hdr->tag = tag;
        hdr->_pad[0] = hdr->_pad[1] = hdr->_pad[2] = 0;

#if MM_ENABLE_CORRUPTION_DETECTION
        hdr->magic = MAGIC_ALLOCATED;
#endif
#if MM_ENABLE_LEAKS
        hdr->file = file;
        hdr->line = line;
#else
        (void)file; (void)line;
#endif
    }

} // namespace MM