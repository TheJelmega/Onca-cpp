#pragma once
#include "Config.h"
#include <cassert>

// TODO: make asserts work, or disable them, during consteval 

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

#if ENABLE_MATH_ASSERT
// TODO: better assert impl
#	define MATH_ASSERT(cond, msg) IF_NOT_CONSTEVAL assert((cond) && (msg))
#else
#	define MATH_ASSERT(cond, msg)
#endif

#define STATIC_ASSERT(cond, msg) static_assert((cond), msg)