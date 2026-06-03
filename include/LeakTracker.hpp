#pragma once

//Tiene traccia delle allocazioni in corso
//Ogni Allocate() registra un blocco, ogni Deallocate() lo rimuove
//Al termine del programma, se contiene ancora delle voci, significa che ci sono dei memory leak

#include "MM_Config.hpp"
#include <cstddef>
#include <ostream>

#if MM_ENABLE_LEAKS
#include <unordered_map>
#include <mutex>
#endif

namespace MM {

	///Struttura per rappresentare un'allocazione in corso
    struct LeakRecord {
        std::size_t  size = 0;
        const char* file = nullptr;
        int line = 0;

        LeakRecord() = default;
        LeakRecord(std::size_t s, const char* f, int l) : size(s), file(f), line(l) {}
    };

    ///Singleton per tenere traccia delle allocazioni
    class LeakTracker {
    public:
        ///Tecnica per garantire che ci sia solo un'istanza in tutto il programma (Meyer's singleton)
        ///-LeakTracker::Instance().Record(ptr, size, file, line);
        ///-LeakTracker::Instance().Forget(ptr);
        ///-LeakTracker::Instance().ReportLeaks(std::cerr);
        static LeakTracker& Instance();

        ///Registra una nuova allocazione
        ///Se ptr == null o MM_ENABLE_LEAKS == 0 -> no-op
        void Record(void* ptr, std::size_t  size, const char* file = nullptr, int line = 0);

        ///Rimuove un'allocazione precedentemente salvata
		///Se ptr == nullptr / ptr non presente nella tabella (es double free) / MM_ENABLE_LEAKS == 0 -> no-op
        void Forget(void* ptr);

		///Scrive tutte le allocazioni ancora registrate su out, restituendo il conteggio totale
        std::size_t ReportLeaks(std::ostream& out) const;

        ///Restituisce il numero totale di allocazioni attualmente registrate
        std::size_t LiveCount() const;

        ///Restituisce il numero totale di byte attualmente registrati
        std::size_t LiveBytes() const;

    private:
		LeakTracker() = default;                                //Costruttore privato per evitare istanze multiple (singleton)
		~LeakTracker() = default;                               //Distruttore di default (buona norma dichiararlo)
        LeakTracker(const LeakTracker&) = delete;               //Per evitare copie (singleton)
		LeakTracker& operator=(const LeakTracker&) = delete;    //Per evitare assegnazioni (singleton)

#if MM_ENABLE_LEAKS                                             //Se MM_ENABLE_LEAKS = 0 -> non servono -> classe vuota
        mutable std::mutex mutex_;                              //Semaforo per accesso concorrente alla tabella
        std::unordered_map<void*, LeakRecord> table_;           //Tabella (indirizzo-record) contenente le allocazioni attive
        std::size_t liveBytes_ = 0;                             //Numero di byte attualmente registrati
#endif
    };

}