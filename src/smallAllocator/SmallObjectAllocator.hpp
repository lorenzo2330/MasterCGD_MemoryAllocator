#pragma once

#include <vector>
#include "FixedAllocator.hpp"

class SmallObjectAllocator {
public:
	SmallObjectAllocator(
		std::size_t chunkSize,		//in questo caso si decide che ogni chunk potrà avere lo stesso numero di elementi
		std::size_t maxObjectSize	//dimensione massima di ogni elemento del chunk (quindi del chunk stesso)
	);

	void* Allocate(std::size_t numBytes);		//Numero di byte da allocare (lo inoltra al FixedAllocator)
	void Deallocate(void* p, std::size_t size);	//Size permette di evitare di capire in quale FixedAlloc si trova *p

private:
	FixedAllocator* pLastAlloc_;
	FixedAllocator* pLastDealloc_;
	//std::vector<FixedAllocator> pool_;

	//Usa la tecnica del last alloc/dealloc + vettore pool, andrebbe bene anche avere un array con una size per ogni [i]
};


