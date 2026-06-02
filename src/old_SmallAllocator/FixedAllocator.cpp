#include "FixedAllocator.h"
#include "../Helper.h"

FixedAllocator::FixedAllocator() : blockSize_(0), numBlocks_(0), chunks_(0), allocChunk_(nullptr), deallocChunk_(nullptr), emptyChunk_(nullptr) {}

bool FixedAllocator::Init(std::size_t blockSize, unsigned char numBlocks) {
	
	DEB("FixedAllocator::Init()");

	if (blockSize <= 0 || numBlocks <= 0) { return ERR("FixedAllocator::Init(): Invalid parameters"); }

	blockSize_ = blockSize;
	numBlocks_ = numBlocks;
	
	//chunks_.clear();
	//chunks_.reserve(32); //Cuscinetto per aumentare le prestazioni (meno syscall)
}

bool FixedAllocator::MakeNewChunk() {

	DEB("FixedAllocator::MakeNewChunk()");

	chunks_.reserve(chunks_.size() * 2);	//Euristica, TODO: valutare se è meglio cambiare (es +5, *5/3, ...)
	
	//Crea un nuovo chunk
	Chunk newChunk;
	if (!newChunk.Init(blockSize_, numBlocks_)) { return ERR("FixedAllocator::MakeNewChunk(): Failed to initialize new chunk"); }
	
	//Alloca il nuovo chunk
	chunks_.push_back(newChunk);
	
	//Aggiorna i puntatori
	allocChunk_ = &chunks_.back();
	deallocChunk_ = &chunks_.back();

	return true;
}


void* FixedAllocator::Allocate() {
	
	DEB("FixedAllocator::Allocate()");

	if (allocChunk_ == nullptr || allocChunk_->blocksAvailable_ == 0) {	//Chunk attuale pieno -> dobbiamo usarne un altro
		//Se c'è un chunk vuoto, lo usiamo senza scorrerli tutti
		if (emptyChunk_ != nullptr) { allocChunk_ = emptyChunk_; emptyChunk_ = nullptr; }

		else {
			for (ChunkIter i = chunks_.begin(); ; i++) {	//Cerchiamo, nella lista di Chunk, uno disponibile
				if (i == chunks_.end()) {						//Se arriviamo in fondo alla lista, ne creiamo uno nuovo
					if (!MakeNewChunk()) { ERR("FixedAllocator::Allocate(): Failed to create new chunk"); return nullptr; }
					break;
				}

				//TODO, fallendo il controllo emptyChunk != nullptr, potremo mai entrare in questo if?
				if (i->blocksAvailable_ > 0) {					//Se ne troviamo uno disponibile, lo usiamo
					allocChunk_ = &*i;
					break;
				}
			}
		}
	}
	else if (allocChunk_ == emptyChunk_) { emptyChunk_ = nullptr; }	//Se il chunk attuale è già quello vuoto, segno empty = null

	if (allocChunk_ == nullptr) { ERR("FixedAllocator::Allocate(): No chunk available"); return nullptr; }
	if (allocChunk_->blocksAvailable_ == 0) { ERR("FixedAllocator::Allocate(): Current chunk is full"); return nullptr; }

	return allocChunk_->Allocate(blockSize_);	//Allochiamo blockSize nel Chunk (chiamiamo Chunk::Allocate())
}

bool FixedAllocator::Deallocate(void* p) { 
	
	DEB("FixedAllocator::Deallocate()");

	if (chunks_.empty()) { return ERR("FixedAllocator::Deallocate(): Chunks is empty"); }

	Chunk* foundChunk = nullptr;

	foundChunk = VicinityFind(p);

	if (foundChunk == nullptr) { return ERR("FixedAllocator::Deallocate(): Pointer not found in any chunk"); }

	deallocChunk_ = foundChunk;

	return RealDeallocate(p); 
}



Chunk* FixedAllocator::VicinityFind(void* p) {

	if (chunks_.empty()) { ERR("FixedAllocator::VicinityFind(): Chunks is empty"); return nullptr; }

	if (deallocChunk_ == nullptr) { ERR("FixedAllocator::VicinityFind(): deallocChunk_ is nullptr"); return nullptr; }

	const std::size_t length = numBlocks_ * blockSize_;

	Chunk* l = deallocChunk_;		//Indice che parte dall'ultimo deallocato e scorre "verso sinistra"
	Chunk* r = deallocChunk_ + 1;	//Indice che parte dal Chunk successivo all'ultimo deallocato e scorre "verso destra"
	Chunk* lGuard = &chunks_.front();		//Guard per evitare che si "sfori" a sinistra
	Chunk* rGuard = &chunks_.back() + 1;	//Guard per evitare che si "sfori" a destra

	if (r == rGuard) { r = nullptr; }		//Caso speciale, se deallocChunk è l'ultimo dell'array (quindi r + 1 è out of bound) 

	for (;;) {
		if (l) {	//Se l non è ancora stato azzerato (non è ancora arrivato al limite sinistro
			if (p >= l->pData_ && p < l->pData_ + length) { return l; }	//Se p è tra l e l + length -> abbiamo trovato il Chunk
			if (l == lGuard) { l = nullptr; } else { --l; }	//Controlla se si ha raggiunto il confine, altrimenti decremente
		}
		if (r) {	//Speculare di l
			if (p >= r->pData_ && p < r->pData_ + length) { return r; }
			if (++r == rGuard) { r = nullptr; }	//Incrementa (prima, per la logica del limite destro) ed eventualmente azzera
		}
	}

	return nullptr;
}

bool FixedAllocator::RealDeallocate(void* p) {
	deallocChunk_->Deallocate(p, blockSize_);	//Dealloca il blocco p

	if (deallocChunk_->blocksAvailable_ == numBlocks_) {	//Se, svuotando, il chunk diventa vuoto, si valuta se rimuoverlo

		if (emptyChunk_ != nullptr) {

			//Abbiamo due chunk vuoti, quello appena svuotato (deallocChunk_) e quello già vuoto (emptyChunk_)
			//Nota: per logica, emptyChunk_ era vuoto prima di questa funzione, quindi non può essere lo stesso di deallocChunk_

			//-> Conclusione: siamo nel caso in cui abbiamo due chunk vuoti e ne dobbiamo rimuovere uno
			
			Chunk* last = &chunks_.back();	//Otteniamo l'ultimo chunk della lista chunks

			if (deallocChunk_ == last) {	
				//Se il chunk appena svuotato (deallocChunk) è l'ultimo della lista (quindi vogliamo rimuoverlo)
				//Facciamo puntare emptyChunk_ a deallocChunk_, in modo che non punti ad un elemento che sta per essere rimosso
				//Per costruzione, non può accadere che emptyChunk_ == deallocChunk_ (vedi sopra)
				//Per euristica, deallocChunk_ vogliamo che punti ad un chunk vuoto
				deallocChunk_ = emptyChunk_;
			}

			else if (last != emptyChunk_) {
				//Per euristica, vogliamo che l'ultimo blocco sia quello libero
				//Se emptyChunk_ non è l'ultimo della lista, lo spostiamo in fondo (swap con l'ultimo)
				//Re:mind: scambia i contenuti, non gli indirizzi, *last punterà ancora all'ultimo elemento di chunks (il "nuovo vuoto")
				std::swap(*emptyChunk_, *last);
			}

			if (allocChunk_ == last) { 
				//Come sopra, per evitare che allocChunk_ punti ad un elemento che sta per essere rimosso
				allocChunk_ = emptyChunk_; 
			}	

			last->Release();	//Dealloca l'ultimo chunk (che sappiamo essere vuoto e non puntato da nessuno)

			chunks_.pop_back();	//Rimuove l'ultimo chunk dalla lista

		}
		
		emptyChunk_ = deallocChunk_;	//Segniamo il chunk appena svuotato come vuoto

		return true;
	}
}

FixedAllocator::~FixedAllocator() { for (ChunkIter i = chunks_.begin(); i != chunks_.end(); ++i) { i->Release(); } }

