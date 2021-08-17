#include <iostream>
#include <vector>

#include "core/Core.h"

auto main(int argc, char* argv[]) -> int
{
	Core::Alloc::Mallocator mallocator;

	Core::Alloc::ExpandableArena<Core::Alloc::StackAllocator, usize, u16> expandableArena{ &mallocator, 64, 8 };

	//Core::MemRef<u8> mem = expandableArena.Allocate<u8>();
	
	std::cout << "Hello world";
}
