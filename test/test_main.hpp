#pragma once

#include "../src/smallAllocator/Chunk.hpp"
#include "../src/smallAllocator/FixedAllocator.hpp"
#include "../include/MM_Utils.hpp"

#include "testChunk.hpp"
#include "testFixedAllocator.hpp"


void test_chunk(bool print) {
	size_t size = 1;			//Ogni blocco avrà 1 byte
	//size_t size = 2;			//Ogni blocco avrà 2 byte
	unsigned char blocks = 4;	//Ogni chunk avrà 4 blocchi
	Chunk c;

	TEST("Test Chunk - Inizio");
	tC_init(c, size, blocks);
	tC_deallocate(c, size, blocks, tC_allocate(c, size));
	tC_allocate_deallocate_max_fifo(c, size, blocks);
	tC_reset(c, size, blocks, false);
	tC_allocate_deallocate_max_lifo(c, size, blocks);
	tC_reset(c, size, blocks, false);
	tC_allocate_deallocate_max_odd(c, size, blocks);
	tC_reset(c, size, blocks, false);
	TEST("Test Chunk - Fine");
}

void test_fixedAllocator(bool print) {
	size_t size = 4;
	unsigned char blocks = 3;
	FixedAllocator fa;
	
	test_fixedAllocator_init(fa, size, blocks);
	TEST("FixedAllocator::Init()-Fine");

	TEST("FixedAllocator::Allocate&Deallocate()-Inizio");
	test_fixedAllocator_allocate_and_deallocate(fa, size, blocks);
	TEST("FixedAllocator::Allocate&Deallocate()-Fine");

	TEST("FixedAllocator::Release()-Inizio");
	test_fixedAllocator_release(fa, size, blocks);
	TEST("FixedAllocator::Release()-Fine");


	TEST("FixedAllocator::AllocateCreate()-Inizio");
	test_fixedAllocator_allocate_create(fa, size, blocks);
	TEST("FixedAllocator::AllocateCreate()-Fine");
	
}

