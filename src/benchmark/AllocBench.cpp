#include "Config.h"

#if BENCH_ALLOCS
#include "core/Core.h"

#define BENCH_ALLOCS_SINGLE 0
#define BENCH_ALLOCS_MULTI 1

#if BENCH_ALLOCS_SINGLE

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

auto LinearAllocatorBench(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	Core::Alloc::LinearAllocator alloc{ &mallocator, 1024, 8 };
	for (auto _ : state)
	{
		auto mem = alloc.Allocate<u8>(state.range(0));
		alloc.Reset();
	}
}
BENCHMARK(LinearAllocatorBench)
	->DenseRange(256, 1024, 256)
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
	Core::Alloc::PoolAllocator<1024, 1> alloc{ &mallocator };
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
	Core::Alloc::BitmapAllocator<32, 32> alloc{ &mallocator };
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
	Core::Alloc::BitmapAllocator<256, 4> alloc{ &mallocator };
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
	Core::Alloc::BuddyAllocator<1024, 2> alloc{ &mallocator };
	for (auto _ : state)
	{
		auto mem = alloc.Allocate<u8>(state.range(0));
		alloc.Deallocate(StdMove(mem));
	}
}
BENCHMARK(BuddyAllocatorBench)
	->DenseRange(256, 1024, 256)
	->Complexity();

auto FreeListAllocatorBench(benchmark::State& state) -> void
{
	Core::Alloc::Mallocator mallocator;
	Core::Alloc::FreeListAllocator<1024> alloc{ &mallocator };
	for (auto _ : state)
	{
		auto mem = alloc.Allocate<u8>(state.range(0));
		alloc.Deallocate(StdMove(mem));
	}
}
BENCHMARK(FreeListAllocatorBench)
->DenseRange(256, 1024, 256)
->Complexity();
#endif

#if BENCH_ALLOCS_MULTI

struct MultiAllocDummy
{
	MultiAllocDummy()
		: ref(nullptr)
	{
	}

	Core::MemRef<u8> ref;
};

auto MallocBenchMulti(benchmark::State& state) -> void
{
	MultiAllocDummy refs[128];

	void* allocs[128];
	usize count = usize(state.range(0));
	for (auto _ : state)
	{
		for (usize i = 0; i < count; ++i)
		{
			allocs[i] = malloc(32);
		}
		for (usize i = 0; i < count; ++i)
		{
			free(allocs[i]);
		}
	}
}
BENCHMARK(MallocBenchMulti)
	->DenseRange(32, 128, 32)
	->Complexity();

auto MallocatorBenchMulti(benchmark::State& state) -> void
{
	MultiAllocDummy refs[128];

	Core::Alloc::Mallocator alloc;
	usize count = usize(state.range(0));
	for (auto _ : state)
	{
		for (usize i = 0; i < count; ++i)
		{
			refs[i].ref = alloc.Allocate<u8>(32);
		}
		for (usize i = 0; i < count; ++i)
		{
			alloc.Deallocate(StdMove(refs[i].ref));
		}
	}
}
BENCHMARK(MallocBenchMulti)
	->DenseRange(32, 128, 32)
	->Complexity();

auto LinearAllocatorBenchMulti(benchmark::State& state) -> void
{
	MultiAllocDummy refs[128];

	Core::Alloc::Mallocator mallocator;
	Core::Alloc::LinearAllocator alloc{ &mallocator, usize(state.range(0)) * 32, 8 };
	usize count = usize(state.range(0));
	for (auto _ : state)
	{
		for (usize i = 0; i < count; ++i)
		{
			refs[i].ref = alloc.Allocate<u8>(32);
		}
		alloc.Reset();
	}
}
BENCHMARK(LinearAllocatorBenchMulti)
	->DenseRange(32, 128, 32)
	->Complexity();

auto StackAllocatorBenchMulti(benchmark::State& state) -> void
{
	MultiAllocDummy refs[128];

	Core::Alloc::Mallocator mallocator;
	Core::Alloc::StackAllocator alloc{ &mallocator, usize(state.range(0)) * 32, 8 };
	usize count = usize(state.range(0));
	usize end = count - 1;
	for (auto _ : state)
	{
		for (usize i = 0; i < count; ++i)
		{
			refs[i].ref = alloc.Allocate<u8>(32);
		}
		for (usize i = 0; i < count; ++i)
		{
			alloc.Deallocate(StdMove(refs[end - i].ref));
		}
	}
}
BENCHMARK(StackAllocatorBenchMulti)
	->DenseRange(32, 128, 32)
	->Complexity();

auto PoolAllocatorBenchMulti(benchmark::State& state) -> void
{
	MultiAllocDummy refs[128];

	Core::Alloc::Mallocator mallocator;
	usize count = usize(state.range(0));
	Core::Alloc::PoolAllocator<32, 128> alloc{ &mallocator };
	for (auto _ : state)
	{
		for (usize i = 0; i < count; ++i)
		{
			refs[i].ref = alloc.Allocate<u8>(32);
		}
		for (usize i = 0; i < count; ++i)
		{
			alloc.Deallocate(StdMove(refs[i].ref));
		}
	}
}
BENCHMARK(PoolAllocatorBenchMulti)
	->DenseRange(32, 128, 32)
	->Complexity();

auto BitmapAllocatorBenchSmallBlockMulti(benchmark::State& state) -> void
{
	MultiAllocDummy refs[128];

	Core::Alloc::Mallocator mallocator;
	usize count = usize(state.range(0));
	Core::Alloc::BitmapAllocator<8, 128 * 4> alloc{ &mallocator };
	for (auto _ : state)
	{
		for (usize i = 0; i < count; ++i)
		{
			refs[i].ref = alloc.Allocate<u8>(32);
		}
		for (usize i = 0; i < count; ++i)
		{
			alloc.Deallocate(StdMove(refs[i].ref));
		}
	}
}
BENCHMARK(BitmapAllocatorBenchSmallBlockMulti)
	->DenseRange(32, 128, 32)
	->Complexity();

auto BitmapAllocatorBenchLargeBlockMulti(benchmark::State& state) -> void
{
	MultiAllocDummy refs[128];

	Core::Alloc::Mallocator mallocator;
	usize count = usize(state.range(0));
	Core::Alloc::BitmapAllocator<32, 128> alloc{ &mallocator };
	for (auto _ : state)
	{
		for (usize i = 0; i < count; ++i)
		{
			refs[i].ref = alloc.Allocate<u8>(32);
		}
		for (usize i = 0; i < count; ++i)
		{
			alloc.Deallocate(StdMove(refs[i].ref));
		}
	}
}
BENCHMARK(BitmapAllocatorBenchLargeBlockMulti)
	->DenseRange(32, 128, 32)
	->Complexity();

auto BuddyAllocatorBenchMulti(benchmark::State& state) -> void
{
	MultiAllocDummy refs[128];

	Core::Alloc::Mallocator mallocator;
	usize count = usize(state.range(0));
	count = count == 96 ? 128 : count;
	u8 numDivs = Core::Log2(count);
	Core::Alloc::BuddyAllocator<128 * 32, Core::Log2(128u)> alloc{ &mallocator };
	for (auto _ : state)
	{
		for (usize i = 0; i < count; ++i)
		{
			refs[i].ref = alloc.Allocate<u8>(32);
		}
		for (usize i = 0; i < count; ++i)
		{
			alloc.Deallocate(StdMove(refs[i].ref));
		}
	}
}
BENCHMARK(BuddyAllocatorBenchMulti)
	->DenseRange(32, 128, 32)
	->Complexity();

auto FreeListAllocatorBenchMulti(benchmark::State& state) -> void
{
	MultiAllocDummy refs[128];

	Core::Alloc::Mallocator mallocator;
	usize count = usize(state.range(0));
	Core::Alloc::FreeListAllocator<128 * 64> alloc{ &mallocator };
	for (auto _ : state)
	{
		for (usize i = 0; i < count; ++i)
		{
			refs[i].ref = alloc.Allocate<u8>(32);
		}
		for (usize i = 0; i < count; ++i)
		{
			alloc.Deallocate(StdMove(refs[i].ref));
		}
	}
}
BENCHMARK(FreeListAllocatorBenchMulti)
->DenseRange(32, 128, 32)
->Complexity();

#endif

#endif