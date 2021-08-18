#pragma once
#include "Config.h"
#include <cassert>

#if ENABLE_ASSERT
// TODO: better assert impl
#	define ASSERT(cond, msg) assert((cond) && (msg))
#else
#	define ASSERT(cond, msg)
#endif

#if ENABLE_FREQ_ASSERT
// TODO: better assert impl
#	define FREQ_ASSERT(cond, msg) assert((cond) && (msg))
#else
#	define FREQ_ASSERT(cond, msg)
#endif

#if ENABLE_SLOW_ASSERT
// TODO: better assert impl
#	define SLOW_ASSERT(cond, msg) assert((cond) && (msg))
#else
#	define SLOW_ASSERT(cond, msg)
#endif


#define STATIC_ASSERT(cond, msg) static_assert((cond), msg)