#pragma once

#include "../SmallAllocator/Chunk.h"
#include "../SmallAllocator/FixedAllocator.h"
#include "../Helper.h"

#include "testChunk.h"
#include "testFixedAllocator.h"

/*

void test_chunk(bool print) {
	size_t size = 4;
	unsigned char blocks = 3;
	Chunk c;

	TEST("Chunk::Init()-Inizio");
	test_chunk_init(c, size, blocks);
	TEST("Chunk::Init()-Fine");
	
	if(print) c.Print(size, blocks);

	TEST("Chunk::Allocate()-Inizio");
	void* p1 = test_chunk_allocate(c, size, blocks);
	TEST("Chunk::Allocate()-Fine");

	if (print) c.Print(size, blocks);
	
	TEST("Chunk::Deallocate()-Inizio");
	test_chunk_deallocate_reuse(c, size, blocks, p1);
	TEST("Chunk::Deallocate()-Fine");

	if (print) c.Print(size, blocks);
}
*/

/*
void test_fixedAllocator(bool print) {
	size_t size = 4;
	unsigned char blocks = 3;
	FixedAllocator* fa = new FixedAllocator;

	TEST("FixedAllocator::Init()-Inizio");
	test_fixedAllocator_init(*fa, size, blocks);
	TEST("FixedAllocator::Init()-Fine");

	TEST("FixedAllocator::Allocate&Deallocate()-Inizio");
	test_fixedAllocator_allocate_and_deallocate(*fa, size, blocks);
	TEST("FixedAllocator::Allocate&Deallocate()-Fine");

	TEST("FixedAllocator::AllocateCreate()-Inizio");
	test_fixedAllocator_allocate_create(*fa, size, blocks);
	TEST("FixedAllocator::AllocateCreate()-Fine");
	
}
*/
