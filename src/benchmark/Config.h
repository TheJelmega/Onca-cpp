#pragma once

#ifdef __INTELLISENSE__
#pragma diag_suppress 26812 // Disable use enum class instead of enum
#endif

#ifdef _MSC_VER
#define _ITERATOR_DEBUG_LEVEL 0
#endif

#include "core/Config.h"

#if COMPILER_MSVC
#	pragma warning(push)
#	pragma warning(disable:26812 26495)
#endif

#include <benchmark/benchmark.h>

#if COMPILER_MSVC
#	pragma warning(pop)
#endif

#define BENCH_ALLOCS 1
#define BENCH_DYNARRAY 0