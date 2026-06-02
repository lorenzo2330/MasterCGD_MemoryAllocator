#include <cstddef>

struct AllocatorStats {
	std::size_t totalAllocated;
	std::size_t totalDeallocated;
	std::size_t currentUsage;
	std::size_t peakUsage;
	std::size_t memoryLeakCount;
	//Altro?
};