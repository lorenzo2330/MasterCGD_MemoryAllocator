#pragma once
#include "../SmallAllocator/Chunk.h"
#include <cassert>

/*
void test_chunk_init(Chunk &c, size_t size, unsigned char blocks) {

	c.Init(size, blocks);

    //Test se i valori corrispondono a quelli dichiarati
    assert(c.blocksAvailable_ == blocks);
    assert(c.firstAvailableBlock_ == 0);

    //Test se il primo byte di ogni blocco contiene l'indice del successivo
    unsigned char* p = c.pData_;
	for (int i = 0; i < blocks; i++) { assert(*p == (i + 1) % blocks); p += size; }
}

void* test_chunk_allocate(Chunk &c, size_t size, unsigned char blocks) {
    
    void* p1 = c.Allocate(size);
    void* p2 = c.Allocate(size);
    void* p3 = c.Allocate(size);

    //Test per controllare che si saturi il chunk
    assert(c.blocksAvailable_ == 0);
    assert(c.Allocate(size) == nullptr);

    return p1;

}

void test_chunk_deallocate_reuse(Chunk &c, size_t size, unsigned char blocks, void* p1) {

    c.Deallocate(p1, size);

    void* p4 = c.Allocate(size);

    //Test per verificare che si utilizzi nuovamente lo spazio precedentemente occupato da p1 e poi rilasciato
    assert(p1 == p4);
}



void test_allocazioni_deallocazioni_multiple(Chunk &c, size_t size, unsigned char blocks) {
	int* p1 = (int*)c.Allocate(size);
	*p1 = 91;
	std::cout << "Print post allocazione1" << std::endl;
	c.Print(size, blocks);

	int* p2 = (int*)c.Allocate(size);
	*p2 = 92;
	std::cout << "Print post allocazione2" << std::endl;
	c.Print(size, blocks);

	c.Deallocate(p1, size);
	std::cout << "Print post deallocazione1" << std::endl;
	c.Print(size, blocks);

	int* p3 = (int*)c.Allocate(size);
	*p3 = 93;
	std::cout << "Print post allocazione3" << std::endl;
	c.Print(size, blocks);

	int* p4 = (int*)c.Allocate(size);
	*p4 = 94;
	std::cout << "Print post allocazione4" << std::endl;
	c.Print(size, blocks);

	c.Deallocate(p2, size);
	std::cout << "Print post deallocazione2" << std::endl;
	c.Print(size, blocks);

	int* p5 = (int*)c.Allocate(size);
	*p5 = 95;
	std::cout << "Print post allocazione5" << std::endl;
	c.Print(size, blocks);

	int* p6 = (int*)c.Allocate(size);
	*p6 = 96;
	std::cout << "Print post allocazione6" << std::endl;
	c.Print(size, blocks);

	int* p7 = (int*)c.Allocate(size);
	*p7 = 97;
	std::cout << "Print post allocazione7" << std::endl;
	c.Print(size, blocks);

	int* p8 = (int*)c.Allocate(size);
	*p8 = 98;
	std::cout << "Print post allocazione8" << std::endl;
	c.Print(size, blocks);

	int* p9 = (int*)c.Allocate(size);
	*p9 = 99;
	std::cout << "Print post allocazione9" << std::endl;
	c.Print(size, blocks);

	c.Deallocate(p4, size);
	std::cout << "Print post deallocazione4" << std::endl;
	c.Print(size, blocks);
}


void test_chunk_old() {

	size_t size = 4;
	unsigned char blocks = 8;

	Chunk c;

	c.Init(size, blocks);

	std::cout << "Print post init" << std::endl;
	c.Print(size, blocks);

	test_allocazioni_deallocazioni_multiple(c, size, blocks);
}

*/