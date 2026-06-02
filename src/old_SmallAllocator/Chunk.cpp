#include "Chunk.h"
#include <cassert>
#include <iostream>
#include "../Helper.h"

void Chunk::Print(std::size_t blockSize, unsigned char blocks) {

	std::cout << "pData = " << (void*)this->pData_ << std::endl;
	std::cout << "firstAvailableBlock_ = " << (size_t)this->firstAvailableBlock_ << std::endl;
	std::cout << "blocksAvailable_ = " << (size_t)this->blocksAvailable_ << std::endl;

	unsigned char* p = this->pData_;
	for (unsigned char i = 0; i != blocks; p += blockSize) {
		std::cout << "| i = " << (size_t)i << " | index = " << (void*)p << " | ";
		std::cout << "value = [";
		for (std::size_t byte = 0; byte < blockSize; byte++) {	//Scorre ogni byte del blocco					
			std::cout << (size_t) * (p + byte) << " ";
		}
		std::cout << "] |" << std::endl;
		++i;
	}

	std::cout << "--------------------------------------------------------------------------------" << std::endl;
}

bool Chunk::Init(std::size_t blockSize, unsigned char blocks) {
	
	DEB("Chunk::Init()");

	if (blockSize < 0) { return ERR("Chunk::Init(): blockSize negative");}

	if (blocks < 0) { return ERR("Chunk::Init(): n blocks negative"); }

	if ((blockSize * blocks) / blockSize != blocks) { return ERR("Chunk::Init(): overflow"); } //TODO cos'è?

	pData_ = new unsigned char[blockSize * blocks];				//Allocazione di dimBlocchi * numBlocchi -> area di memoria del chunk

	return Reset(blockSize, blocks);							//Inizializza il chunk
}

bool Chunk::Reset(std::size_t blockSize, unsigned char blocks) {
	
	DEB("Chunk::Reset()");

	if (blockSize < 0) { return ERR("Chunk::Reset(): blockSize negative"); }

	if (blocks < 0) { return ERR("Chunk::Reset(): n blocks negative"); }

	if ((blockSize * blocks) / blockSize != blocks) { return ERR("Chunk::Reset(): overflow"); } //TODO cos'è?

	firstAvailableBlock_ = 0;									//Inizialmente il primo blocco disponibile è il primo blocco del chunk
	blocksAvailable_ = blocks;									//Inizialmente tutti i blocchi sono disponibili

	unsigned char* p = pData_;									//Valore temporaneo, usato per scorrere e inizializzare il chunk

	for (unsigned char i = 0; i != blocks; p += blockSize) {	//Ad ogni itearazione di avanza di blockSize
		*p = ++i;												//Ogni blocco contiene l'indice del successivo (essendo tutti liberi)
	}

	return true;
}

void* Chunk::Allocate(std::size_t blockSize) {

	DEB("Chunk::Allocate()");

	if (!blocksAvailable_) { return nullptr; }								//Se non ci sono blocchi disponibili, restituisce nullptr

	unsigned char* pResult = pData_ + (firstAvailableBlock_ * blockSize);	//Calcola l'indirizzo del primo blocco disponibile

	firstAvailableBlock_ = *pResult;	//Aggiorna firstAvailableBlock_ (il contenuto di pResult è l'indice del prossimo blocco libero)

	--blocksAvailable_;														//Diminuisce il numero di blocchi disponibili

	return pResult;
}

void Chunk::Deallocate(void* p, std::size_t blockSize) {

	DEB("Chunk::Deallocate()");

	if (p == nullptr || p >= pData_) { ERR("Chunk::Deallocate(): Invalid parameters"); return; };

	unsigned char* release = static_cast<unsigned char*>(p);

	unsigned char index = static_cast<unsigned char>((release - pData_) / blockSize);	//Calcolo l'indice del blocco da liberare

	*release = firstAvailableBlock_;			//Assegno come valore di questo blocco l'ex firstAvailable
	
	firstAvailableBlock_ = index;				//Imposto come nuovo firstAvailable

	++blocksAvailable_;							//Aumento il numero di blocchi disponibili
}

bool Chunk::Release() { 
	DEB("Chunk::Release()");
	if (pData_ == nullptr) { return ERR("Chunk::Release(): pData_ null"); };
	delete[] pData_; 
	return true;
}
