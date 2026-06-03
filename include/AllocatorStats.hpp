#pragma once

//Tiene traccia delle statistiche dell'allocatore (n allocazioni / deallocazioni, memoria usata, ...)

#include "MM_Config.hpp"
#include <cstddef>
#include <cstdint>
#include <ostream>

namespace MM {

    struct AllocatorStats {

		///Numero totale di allocazioni
        uint64_t nAlloc = 0;
        ///Numero totale di deallocazioni
        uint64_t nDealloc = 0;
		///Numero totale di allocazioni fallite (<= nAlloc)
        uint64_t nFailedAlloc = 0;

        ///Quantità di byte attualmente allocati (quindi utilizzati dall'utente)
        std::size_t nByteAlloc = 0;
        ///Picco massimo di allocazioni contemporanee
        std::size_t maxByteAlloc = 0;
		///Quantità totale di byte allocati (quindi utilizzati dall'utente) durante tutta l'esecuzione
        std::size_t totalBytes = 0;

		///Helper da chiamare quando si effettua un'allocazione (size = numero di byte allocati)
        ///Se MM_ENABLE_STATS == 0 -> no-op
        void RecordAlloc(std::size_t size) {
#if MM_ENABLE_STATS
            ++nAlloc;
            nByteAlloc += size;
            totalBytes += size;
            if (nByteAlloc > maxByteAlloc) { maxByteAlloc = nByteAlloc; }
#else
            (void)size;
#endif
        }

		///Helper da chiamare quando si effettua una deallocazione (size = numero di byte deallocati)
        ///Se MM_ENABLE_STATS == 0 -> no-op
        void RecordFree(std::size_t size) {
#if MM_ENABLE_STATS
            ++nDealloc;
			if (nByteAlloc >= size) { nByteAlloc -= size; } else { nByteAlloc = 0; }
#else
            (void)size;
#endif
        }
        
		///Helper da chiamare quando un'allocazione fallisce
        ///Se MM_ENABLE_STATS == 0 -> no-op
        void RecordFailure() {
#if MM_ENABLE_STATS
            ++nFailedAlloc;
#endif
        }

        uint64_t LiveBlocks() const { return (nAlloc > nDealloc) ? (nAlloc - nDealloc) : 0; }

        ///Utility per il MemoryManager per aggregare le statistiche dei vari allocatori
        ///Somma tra loro i vari parametri
        AllocatorStats& operator+=(const AllocatorStats& rhs) {
			nAlloc += rhs.nAlloc;               //Somma il numero di allocazioni fatte tra i due allocatori
			nDealloc += rhs.nDealloc;           //Somma il numero di deallocazioni fatte tra i due allocatori
			nFailedAlloc += rhs.nFailedAlloc;   //Somma il numero di allocazioni fallite tra i due allocatori
			nByteAlloc += rhs.nByteAlloc;       //Somma la quantità di byte attualmente allocati tra i due allocatori
            maxByteAlloc = (rhs.maxByteAlloc > maxByteAlloc) ? rhs.maxByteAlloc : maxByteAlloc; //Picco massimo (max(piccoAlloc1, piccoAlloc2))
			totalBytes += rhs.totalBytes;       //Somma la quantità totale di byte allocati tra i due allocatori
            return *this;
        }

		///Utility per il MemoryManager per stampare le statistiche dei vari allocatori (su file / console / ...)
        void Print(std::ostream& out, const char* label = "Allocator") const;
    };

}