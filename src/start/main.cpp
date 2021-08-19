#include <iostream>
#include <vector>

#include "core/Core.h"

auto main(int argc, char* argv[]) -> int
{
	Core::Alloc::Mallocator mallocator;
	Core::Alloc::BuddyAllocator alloc{ &mallocator, 256, 4 };
	/*for (usize i = 256; i < 1024; i += 256)
	{
		auto mem = alloc.Allocate<u8>(i);
		alloc.Deallocate(StdMove(mem));
	}*/

	auto mem0 = alloc.Allocate<u8>(256);
	alloc.Deallocate(StdMove(mem0));
}
