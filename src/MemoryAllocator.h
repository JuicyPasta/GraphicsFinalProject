//
// Created by Darrow, Jackson on 11/13/17.
//

#ifndef INC_3DBILLIARDS_MEMORYALLOCATOR_H
#define INC_3DBILLIARDS_MEMORYALLOCATOR_H


#include <foundation/PxAllocatorCallback.h>
#include <foundation/PxAssert.h>
#include <common/PxPhysXCommonConfig.h>

#include <stdlib.h>
using namespace physx;

#if PX_WINDOWS
// on win32 we only have 8-byte alignment guaranteed, but the CRT provides special aligned allocation fns
PX_FORCE_INLINE void* platformAlignedAlloc(size_t size)
{
	return _aligned_malloc(size, 16);
}

PX_FORCE_INLINE void platformAlignedFree(void* ptr)
{
	_aligned_free(ptr);
}
#elif PX_LINUX_FAMILY || PX_SWITCH
PX_FORCE_INLINE void* platformAlignedAlloc(size_t size)
{
	return ::memalign(16, size);
}

PX_FORCE_INLINE void platformAlignedFree(void* ptr)
{
	::free(ptr);
}
#else
// on all other platforms we get 16-byte alignment by default
PX_FORCE_INLINE void* platformAlignedAlloc(size_t size)
{
	return ::malloc(size);
}

PX_FORCE_INLINE void platformAlignedFree(void* ptr)
{
	::free(ptr);
}
#endif

/**
\brief default implementation of the allocator interface required by the SDK
*/
class MemoryAllocator : public PxAllocatorCallback
{
public:
	void* allocate(size_t size, const char*, const char*, int)
	{
		void* ptr = malloc(size);
		PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15)==0);
		return ptr;
	}

	void deallocate(void* ptr)
	{
		free(ptr);
	}
};


#endif //INC_3DBILLIARDS_MEMORYALLOCATOR_H
