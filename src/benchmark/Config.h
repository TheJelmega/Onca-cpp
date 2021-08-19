#pragma once

#ifdef __INTELLISENSE__
#pragma diag_suppress 26812 // Disable use enum class instead of enum
#endif

#include <benchmark/benchmark.h>

#define BENCH_ALLOCS 1
#define BENCH_DYNARRAY 0