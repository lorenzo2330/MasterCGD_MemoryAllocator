#pragma once
#include "../src/smallAllocator/Chunk.hpp"
#include <cassert>
#include <iostream>

/*	Lista test:
*	-tC_init: Verifico che il chunk venga inizializzato correttamente
*	-tC_allocate: Verifico che, facendo un'allocazione, i risultati siano quelli attesi
*	-tC_deallocate: Verifico che, facendo una deallocazione, i risultati siano quelli attesi
*	-tC_allocate_deallocate_max_fifo: Verifico che, facendo [blocks] allocazioni, si saturi il Chunk (e poi dealloco)
*	-tC_allocate_deallocate_lifo: Verifico che, facendo n allocazioni e deallocando in modo inverso, i risultati siano quelli attesi
*	-tC_allocate_deallocate_odds: Verifico che, facendo n (pari) allocazioni e deallocando solo i chunk di indice dispari, i risultati siano quelli attesi
*	-tC_reset: Verifico che il chunk si resetti correttamente (quindi che "l'ordine torni corretto")
*/

void tC_init(Chunk &c, size_t size, unsigned char blocks) {

	TEST("tC_init - Inizio");

	c.Init(size, blocks);

    //Test se i valori corrispondono a quelli dichiarati
    assert(c.blocksAvailable_ == blocks);
    assert(c.firstAvailableBlock_ == 0);

    //Test se il primo byte di ogni blocco contiene l'indice del successivo
    unsigned char* p = c.pData_;
	for (int i = 0; i < blocks; i++) { assert(*p == i + 1); p += size; }
	
	//Test per verificare che ogni blocco abbia la size corretta (quindi che si siano allocati size * blocks byte)
	assert((p - c.pData_) / blocks == size);

	TEST("tC_init - Fine");
}

void* tC_allocate(Chunk& c, size_t size) {
	
	TEST("tC_allocate - Inizio");

	void* ret;

	if (c.blocksAvailable_ == 0) {
		ret = c.Allocate(size);
		assert(ret == nullptr);
	}
	else {
		unsigned char prevBlocksAvailable = c.blocksAvailable_;
		ret = c.Allocate(size);
		assert(ret != nullptr);
		assert(c.blocksAvailable_ + 1 == prevBlocksAvailable);
	}

	TEST("tC_allocate - Fine");

	return ret;
	
}

void tC_deallocate(Chunk& c, size_t size, unsigned char blocks, void* p) {
	
	TEST("tC_deallocate - Inizio");

	if (c.blocksAvailable_ == blocks) { ERR("Error: cannot invoke Deallocate() on an empty Chunk"); return; }

	unsigned char prevBlocksAvailable = c.blocksAvailable_;

	c.Deallocate(p, size);

	assert(c.blocksAvailable_ - 1 == prevBlocksAvailable);

	TEST("tC_deallocate - Fine");

}

void tC_allocate_deallocate_max_fifo(Chunk& c, size_t size, unsigned char blocks) {

	TEST("tC_allocate_deallocate_max_fifo - Inizio");

	//Verifico che si parta con un chunk vuoto
	assert(c.blocksAvailable_ == blocks);

	void** tmp = new void* [blocks];

	for (unsigned char i = 0; i < blocks; i++) {
		tmp[i] = c.Allocate(size);
		assert(tmp[i] != nullptr);
	}

	//Verifico di aver saturato il chunk
	assert(c.blocksAvailable_ == 0);

	//Verifico che, tentando di fare una nuova allocazione, restituisca nullptr
	assert(nullptr == c.Allocate(size));

	for (unsigned char i = 0; i < blocks; i++) { c.Deallocate(tmp[i], size); }

	//Verifico che si sia tornati ad avere un chunk vuoto
	assert(c.blocksAvailable_ == blocks);

	delete[] tmp;

	TEST("tC_allocate_deallocate_max_fifo - Fine");
}

void tC_allocate_deallocate_max_lifo(Chunk& c, size_t size, unsigned char blocks) {

	TEST("tC_allocate_deallocate_max_lifo - Inizio");

	//Verifico che si parta con un chunk vuoto
	assert(c.blocksAvailable_ == blocks);

	void** tmp = new void* [blocks];

	for (unsigned char i = 0; i < blocks; i++) {
		tmp[i] = c.Allocate(size);
		assert(tmp[i] != nullptr);
	}

	//Verifico di aver saturato il chunk
	assert(c.blocksAvailable_ == 0);

	//Verifico che, tentando di fare una nuova allocazione, restituisca nullptr
	assert(nullptr == c.Allocate(size));

	TEST("tC_allocate_deallocate_max_lifo - Pre deallocate");

	for (unsigned char i = blocks; i > 0; i--) { c.Deallocate(tmp[i-1], size); }

	TEST("tC_allocate_deallocate_max_lifo - Post deallocate");

	//Verifico che si sia tornati ad avere un chunk vuoto
	assert(c.blocksAvailable_ == blocks);

	delete[] tmp;

	TEST("tC_allocate_deallocate_max_lifo - Fine");
}

void tC_allocate_deallocate_max_odd(Chunk& c, size_t size, unsigned char blocks) {

	TEST("tC_allocate_deallocate_max_odd - Inizio");

	//Verifico che si parta con un chunk vuoto
	assert(c.blocksAvailable_ == blocks);

	void** tmp = new void* [blocks];

	for (unsigned char i = 0; i < blocks; i++) {
		tmp[i] = c.Allocate(size);
		assert(tmp[i] != nullptr);
	}

	c.Print(size, blocks);

	//Verifico di aver saturato il chunk
	assert(c.blocksAvailable_ == 0);

	//Verifico che, tentando di fare una nuova allocazione, restituisca nullptr
	assert(nullptr == c.Allocate(size));

	for (unsigned char i = 0; i < blocks; i++) { if (i % 2 != 0) { c.Deallocate(tmp[i], size); } }

	//Verifico che si siano blocks / 2 (Re:mind: in c++ 5/2 = 2 (tronca))
	assert(c.blocksAvailable_ == blocks / 2);

	c.Print(size, blocks);

	for (unsigned char i = 0; i < blocks; i++) { if (i % 2 != 0) { c.Allocate(size); } }

	//Verifico che si sia tornati ad avere tutto il chunk pieno
	assert(c.blocksAvailable_ == 0);

	c.Print(size, blocks);

	delete[] tmp;

	TEST("tC_allocate_deallocate_max_odd - Fine");
}

void tC_reset(Chunk& c, size_t size, unsigned char blocks, bool print) {
	
	TEST("tC_reset - Inizio");

	if (print) c.Print(size, blocks);

	c.Reset(size, blocks);

	//Verifico che il numero di blocchi sia rimasto lo stesso
	assert(c.blocksAvailable_ == blocks);

	if (print) c.Print(size, blocks);

	TEST("tC_reset - Fine");
}

