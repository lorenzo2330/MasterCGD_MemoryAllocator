#pragma once

#include "SmallObjectAllocator.hpp"

class SmallObject {
public:
	static void* operator new(std::size_t size);	//prof
	static void operator delete(void* p);			//prof
	virtual ~SmallObject() {}						//prof

private:
};

