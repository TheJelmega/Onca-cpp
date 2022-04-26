#include "GlobalAlloc.h"

namespace Core::Detail
{
	auto GetGlobalAllocAddr() -> Core::Alloc::IAllocator*&
	{
		static Alloc::IAllocator* pAlloc;
		return pAlloc;
	}
}

namespace Core
{
	CORE_API auto GetGlobalAlloc() -> Core::Alloc::IAllocator&
	{
		return *Detail::GetGlobalAllocAddr();
	}

	CORE_API void SetGlobalAlloc(Alloc::IAllocator& alloc)
	{
		Alloc::IAllocator** ppAlloc = &Detail::GetGlobalAllocAddr();
		*ppAlloc = &alloc;
	}
}