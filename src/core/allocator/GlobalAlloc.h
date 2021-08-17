#pragma once
#include "IAllocator.h"

namespace Core::Detail
{
	/**
	 * Get the address of the pointer pointing to a global alloc
	 * \return Pointer to the pointer storing the global allocator
	 */
	auto GetGlobalAllocAddr() ->Alloc::IAllocator*&;
}

namespace Core
{
	/**
	* Get the global allocator
	* \return Global allocator
	* \note The allocator is not guaranteed to live pass the end of main(), try to avoid deallocation depending on static or global destruction
	*/
	CORE_API auto GetGlobalAlloc() ->Alloc::IAllocator&;
	/**
	* Set the global allocator
	* \param[in] alloc New global allocator
	* \note The allocator being assigned needs to live longer than all allocations made by it
	*/
	CORE_API auto SetGlobalAlloc(Alloc::IAllocator& alloc) -> void;
}

#define g_GlobalAlloc (::Core::GetGlobalAlloc())