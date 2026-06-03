#pragma once

#include "../src/smallAllocator/Chunk.hpp"
#include "../src/smallAllocator/FixedAllocator.hpp"
#include "../include/MM_Utils.hpp"

#include "testChunk.hpp"
#include "testFixedAllocator.hpp"


void test_chunk(bool print) {
	size_t size = 4;
	unsigned char blocks = 3;
	Chunk c;


	ENDL(2);
	TEST("Chunk::Init()-Inizio");
	test_chunk_init(c, size, blocks);
	TEST("Chunk::Init()-Fine");
	ENDL(2);
	
	if(print) c.Print(size, blocks);

	ENDL(2);
	TEST("Chunk::Allocate()-Inizio");
	void* p1 = test_chunk_allocate(c, size, blocks);
	TEST("Chunk::Allocate()-Fine");
	ENDL(2);

	if (print) c.Print(size, blocks);
	
	ENDL(2);
	TEST("Chunk::DeallocateReuse()-Inizio");
	test_chunk_deallocate_reuse(c, size, blocks, p1);
	TEST("Chunk::DeallocateReuse()-Fine");
	ENDL(2);

	if (print) c.Print(size, blocks);
}

void test_fixedAllocator(bool print) {
	size_t size = 4;
	unsigned char blocks = 3;
	FixedAllocator fa;

	TEST("FixedAllocator::Init()-Inizio");
	test_fixedAllocator_init(fa, size, blocks);
	TEST("FixedAllocator::Init()-Fine");

	TEST("FixedAllocator::Allocate&Deallocate()-Inizio");
	test_fixedAllocator_allocate_and_deallocate(fa, size, blocks);
	TEST("FixedAllocator::Allocate&Deallocate()-Fine");

	TEST("FixedAllocator::AllocateCreate()-Inizio");
	test_fixedAllocator_allocate_create(fa, size, blocks);
	TEST("FixedAllocator::AllocateCreate()-Fine");
	
}

