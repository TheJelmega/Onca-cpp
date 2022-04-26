#include "GlobalAlloc.h"

namespace Onca::Detail
{
	auto GetGlobalAllocAddr() -> Onca::Alloc::IAllocator*&
	{
		static Alloc::IAllocator* pAlloc;
		return pAlloc;
	}
}

namespace Onca
{
	CORE_API auto GetGlobalAlloc() -> Onca::Alloc::IAllocator&
	{
		return *Detail::GetGlobalAllocAddr();
	}

	CORE_API void SetGlobalAlloc(Alloc::IAllocator& alloc)
	{
		Alloc::IAllocator** ppAlloc = &Detail::GetGlobalAllocAddr();
		*ppAlloc = &alloc;
	}
}