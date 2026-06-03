#pragma once

//Interfaccia per astrarre le operazioni di allocazione e deallocazione

#include <cstddef>

namespace MM {

    class IAllocator {
    public:

        ///Nome dell'allocatore, usato in log / debug per capire di quale allocatore si sta parlando
        virtual const char* Name() const = 0;

        ///Chiamato una sola volta (all'avvio dell'applicazione)
        ///Fa un'allocazione iniziale (chunk iniziali (small) / pool (GP)), senza azzerare o inizializzare i dati
        virtual void Initialize() {}

        ///Alloca <size> byte, restituisce nullptr se fallisce
        virtual void* Allocate(std::size_t size) = 0;

        ///Delloca un blocco precedentemente allocato
        ///Se ptr == nullptr => no-op (standard)
        virtual void  Deallocate(void* ptr, std::size_t size) = 0;


        ///Controlla se l'allocatore è in grado di fornire un blocco di <size> byte
        virtual bool CanAllocate(std::size_t size) const = 0;

        ///Chiamato solo al termine dell'esecuzione dell'applicazione
		///Impone il rilascio di tutta la memoria (pool, chunk, ...) gestita dall'allocatore
        virtual void Shutdown() {}
        
        virtual ~IAllocator() = default;
    };
}