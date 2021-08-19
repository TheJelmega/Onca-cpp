#include "Config.h"

#if BENCH_ALLOCS
#include "core/Core.h"

auto MallocBench(benchmark::State& state) -> void
{
	for (auto _ : state)
	{
		void* mem = malloc(state.range(0));
		free(mem);
	}
}
BENCHMARK(MallocBench)
	->DenseRange(256, 1024, 256)
	->DenseRange(256 << 10, 1024 << 10, 256 << 10)
	->Complexity();

auto MallocatorBench(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	for (auto _ : state)
	{
		auto mem = mallocator.Allocate<u8>(state.range(0));
		mallocator.Deallocate(StdMove(mem));
	}
}
BENCHMARK(MallocatorBench)
	->DenseRange(256, 1024, 256)
	->DenseRange(256 << 10, 1024 << 10, 256 << 10)
	->Complexity();

auto StackAllocatorBench(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	Core::Alloc::StackAllocator alloc{ &mallocator, 1024, 8 };
	for (auto _ : state)
	{
		auto mem = alloc.Allocate<u8>(state.range(0));
		alloc.Deallocate(StdMove(mem));
	}
}
BENCHMARK(StackAllocatorBench)
	->DenseRange(256, 1024, 256)
	->Complexity();

auto PoolAllocatorBench(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	Core::Alloc::PoolAllocator alloc{ &mallocator, 8, 1024, 1 };
	for (auto _ : state)
	{
		auto mem = alloc.Allocate<u8>(state.range(0));
		alloc.Deallocate(StdMove(mem));
	}
}
BENCHMARK(PoolAllocatorBench)
	->DenseRange(256, 1024, 256)
	->Complexity();

auto SmallBlockBitmapAllocatorBench(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	Core::Alloc::BitmapAllocator alloc{ &mallocator, 32, 32 };
	for (auto _ : state)
	{
		auto mem = alloc.Allocate<u8>(state.range(0));
		alloc.Deallocate(StdMove(mem));
	}
}
BENCHMARK(SmallBlockBitmapAllocatorBench)
	->DenseRange(256, 1024, 256)
	->Complexity();

auto LargeBlockBitmapAllocatorBench(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	Core::Alloc::BitmapAllocator alloc{ &mallocator, 256, 4 };
	for (auto _ : state)
	{
		auto mem = alloc.Allocate<u8>(state.range(0));
		alloc.Deallocate(StdMove(mem));
	}
}
BENCHMARK(LargeBlockBitmapAllocatorBench)
	->DenseRange(256, 1024, 256)
	->Complexity();

auto BuddyAllocatorBench(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	Core::Alloc::BuddyAllocator alloc{ &mallocator, 1024, 2 };
	for (auto _ : state)
	{
		auto mem = alloc.Allocate<u8>(state.range(0));
		alloc.Deallocate(StdMove(mem));
	}
}
BENCHMARK(BuddyAllocatorBench)
	->DenseRange(256, 1024, 256)
	->Complexity();
#endif