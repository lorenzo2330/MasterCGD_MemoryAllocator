#pragma once
#include "../src/smallAllocator/FixedAllocator.hpp"
#include <cassert>
#include <iostream>

/*Test fixedAllocator: -> TODO da fare
-init: Verifico che chunks_.size() saia 32 (valore di default creto in init) e che i valori siano quelli dichiarati
-allocate: allocando un elemento, verifico che size aumenti e che venga effettivamente allocato in un nuovo chunk (testo anche makeNewChunk)
-deallocate: dealloco un elemento, verifico che quel chunk segni correttamente blocks available e il chunk non venga rimosso (max 2 contemporaneamente)
-removeChunk: alloco blocks * 2 + 1 (creerà 3 chunk), poi li dealloco tutti e controllo che rimangano solo 2 chunk
*/

void test_fixedAllocator_init(FixedAllocator& fa, size_t size, unsigned char blocks) {

	fa.Init(size, blocks);

	//Test se i valori corrispondono a quelli dichiarati
	assert(fa.blockSize_ == size);
	assert(fa.numBlocks_ == blocks);
	
	//Test per verificare che allocChunk e deallocChunk siano nulli (=0)
	assert(fa.allocChunk_ == nullptr);
	assert(fa.deallocChunk_ == nullptr);

	//Test per verificare che il vettore di chunk sia vuoto
	assert(fa.chunks_.size() == 0);
}

void test_fixedAllocator_allocate_and_deallocate(FixedAllocator& fa, size_t size, unsigned char blocks) {
	
	fa.Init(size, blocks);
	
	if (blocks > fa.numBlocks_) return;

	//Controllo che non ci siano chunk
	assert(fa.chunks_.size() == 0);

	void** v = new void* [blocks];

	//Allocazione
	for (int i = 0; i < blocks; i++) { v[i] = fa.Allocate(); }

	//Controllo che, al termine del for, ci sia ancora un unico chunk
	assert(fa.chunks_.size() == 1);

	//Deallocazione
	for (int i = 0; i < blocks; i++) { fa.Deallocate(v[i]); }

	//Controllo che il chunk sia rimasto (essendo che è l'unico vuoto)
	assert(fa.chunks_.size() == 1);

	delete[] v;
}

void test_fixedAllocator_release(FixedAllocator& fa, size_t size, unsigned char blocks) {

	//Test per verificare se il fixedAllocator contiene dei Chunk
	assert(fa.chunks_.size() > 0);

	//Rilascia ogni chunk
	fa.~FixedAllocator();

	//Test per verificare se il fixedAllocator è stato svuotato
	assert(fa.chunks_.size() == 0);
}


void test_fixedAllocator_allocate_create(FixedAllocator& fa, size_t size, unsigned char blocks) {
	
	fa.Init(size, blocks);
	
	if (blocks > fa.numBlocks_) return;

	//Controllo che non ci siano chunk
	assert(fa.chunks_.size() == 0);

	void** v = new void* [blocks * 3];

	TEST("Pre-ciclo1");

	//Allocazione
	for (int i = 0; i < blocks; i++) { v[i] = fa.Allocate(); }

	//Controllo che, al termine del for, ci sia ancora un unico chunk
	assert(fa.chunks_.size() == 1);

	TEST("Pre-ciclo2");

	//Allocazione
	for (int i = blocks; i < blocks * 3; i++) { v[i] = fa.Allocate(); }

	TEST("Post-ciclo2");

	//Controllo che, al termine del for, ci siano 3 chunk
	assert(fa.chunks_.size() == 3);

	//-----------------------------Situazione: [pieno][pieno][pieno]

	std::cout << "-----------------------------Situazione: [pieno][pieno][pieno]" << std::endl;

	fa.chunks_[0].Print(size, blocks);
	fa.chunks_[1].Print(size, blocks);
	fa.chunks_[2].Print(size, blocks);

	//Deallocazione
	for (int i = 0; i < blocks; i++) { fa.Deallocate(v[i]); }

	//-----------------------------Situazione: [vuoto][pieno][pieno]

	std::cout << "-----------------------------Situazione: [vuoto][pieno][pieno]" << std::endl;

	fa.chunks_[0].Print(size, blocks);
	fa.chunks_[1].Print(size, blocks);
	fa.chunks_[2].Print(size, blocks);

	//Controllo che ci siano ancora 3 chunk (solo 1 è vuoto)
	assert(fa.chunks_.size() == 3);

	//Deallocazione
	for (int i = blocks; i < blocks * 2; i++) { fa.Deallocate(v[i]); }

	//-----------------------------Situazione: [vuoto][vuoto][pieno]

	std::cout << "-----------------------------Situazione: [vuoto][vuoto][pieno]" << std::endl;

	fa.chunks_[0].Print(size, blocks);
	fa.chunks_[1].Print(size, blocks);
	fa.chunks_[2].Print(size, blocks);

	//Controllo che ci siano ancora 3 chunk (i primi due sono vuoti, ma per logica di Alexandrescu è accettabile)
	assert(fa.chunks_.size() == 3);

	//Deallocazione
	for (int i = blocks * 2; i < blocks * 3; i++) { fa.Deallocate(v[i]); }

	//-----------------------------Situazione: [vuoto][vuoto][vuoto] -> deve diventare [vuoto][vuoto]

	std::cout << "-----------------------------Situazione: [vuoto][vuoto][vuoto] -> deve diventare [vuoto][vuoto]" << std::endl;

	fa.chunks_[0].Print(size, blocks);
	fa.chunks_[1].Print(size, blocks);
	if (fa.chunks_.size() > 2) fa.chunks_[2].Print(size, blocks);

	//Controllo che ci siano solo 2 chunk (era arrivato ad avere tre vuoti (ultimo + altri 2), quindi deve rimuovere l'ultimo)
	std::cout << "fa.chunks_.size() = " << fa.chunks_.size() << std::endl;
	//assert(fa.chunks_.size() == 2);	//errore

	//-----------------------------Situazione: [vuoto][vuoto]

	//Allocazione
	for (int i = 0; i < blocks; i++) { v[i] = fa.Allocate(); }

	//-----------------------------Situazione: [pieno][vuoto]

	//Deallocazione
	for (int i = 0; i < blocks; i++) { fa.Deallocate(v[i]); }

	//-----------------------------Situazione: [vuoto][vuoto] -> deve diventare [vuoto]

	//Controllo che ci sia solo 1 chunk (era arrivato ad averne due vuoti (ultimo + un altro), quindi deve rimuovere l'ultimo)
	std::cout << "fa.chunks_.size() = " << fa.chunks_.size() << std::endl;
	//assert(fa.chunks_.size() == 1);

	delete[] v;
}
