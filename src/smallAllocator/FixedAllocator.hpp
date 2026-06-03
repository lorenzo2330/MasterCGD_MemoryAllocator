#pragma once
#include <vector>
#include "Chunk.hpp"

typedef std::vector<Chunk> Chunks;				//Typedef per evitare si scrivere ogni volta std::vector<Chunk>
typedef Chunks::iterator ChunkIter;				//Typedef per evitare si scrivere ogni volta std::vector<Chunk>
typedef Chunks::const_iterator ChunkConstIter;	//Typedef per evitare si scrivere ogni volta std::vector<Chunk>

class FixedAllocator {

private:
	std::size_t blockSize_;			//Dimensione di ogni blocco di ogni chunk
	unsigned char numBlocks_;		//Dimensione di ogni chunk

	Chunks chunks_;					//Vettore contenente tutti i chunks_
	Chunk* allocChunk_;				//Puntatore all'ultimo Chunk in cui si ha effettuato l'allocazione
	Chunk* deallocChunk_;			//Puntatore all'ultimo Chunk in cui si ha effettuato la deallocazione
	Chunk* emptyChunk_;				//Puntatore all'unico chunk vuoto (se non esiste -> nullptr)	//TODO trade off buono?

	bool MakeNewChunk();			//Crea un nuovo chunk e lo aggiunge a chunks_
	Chunk* VicinityFind(void* p);	//Funzione di Alexandrescu, trova il Chunk che contiene *p in modo efficiente
	bool RealDeallocate(void* p);	//Funzione di Alexandrescu, effettua la vera deallocazione, invocata in Deallocate

public:
	
	FixedAllocator();
	
	bool Init(std::size_t blockSize, unsigned char numBlocks);
	
	void* Allocate();
	
	bool Deallocate(void* p);

	~FixedAllocator();				//Per chiamare la release su ogni chunk

	inline std::size_t BlockSize() const { return blockSize_; }

	//--------------------TEST--------------------
	
	friend void test_fixedAllocator_init(FixedAllocator&, size_t, unsigned char);
	friend void test_fixedAllocator_allocate_and_deallocate(FixedAllocator&, size_t, unsigned char);
	friend void test_fixedAllocator_allocate_create(FixedAllocator&, size_t, unsigned char);
	//friend void test_fixedAllocator_stress(FixedAllocator&, unsigned char);
	//friend void test_fixedAllocator_deallocate_reverse(FixedAllocator&, unsigned char);
	
};


