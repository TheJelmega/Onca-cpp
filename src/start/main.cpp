#include <iostream>
#include <vector>

#include "core/Core.h"

auto main(int argc, char* argv[]) -> int
{
	Core::Alloc::Mallocator mallocator;

	Core::Alloc::PoolAllocator alloc{ &mallocator, 8, 1024, 1 };

	//Core::MemRef<u8> mem = expandableArena.Allocate<u8>();
	
	std::cout << "Hello world";
}
