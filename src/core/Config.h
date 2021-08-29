#pragma once

#if defined(_MSC_VER)
#	define COMPILER_MSVC 1
#	define COMPILER_GCC 0
#	define COMPILER_CLANG 0
#elif defined(__GNUC__)
#	define COMPILER_MSVC 0
#	define COMPILER_GCC 1
#	define COMPILER_CLANG 0
#elif defined(__clang__)
#	define COMPILER_MSVC 0
#	define COMPILER_GCC 0
#	define COMPILER_CLANG 1
#else
#	error Unsupported compiler
#endif

#if RELEASE_

#define PROFILE_ 0

#define ENABLE_ASSERT 0
#define ENABLE_FREQ_ASSERT 0
#define ENABLE_SLOW_ASSERT 0
#define ENABLE_ALLOC_STATS 0

#elif PROFILE_

#define RELEASE_ 0

#define ENABLE_ASSERT 1
#define ENABLE_FREQ_ASSERT 0
#define ENABLE_SLOW_ASSERT 0
#define ENABLE_ALLOC_STATS 1

#else

#define RELEASE_ 0
#define PROFILE_ 0

#define ENABLE_ASSERT 1
#define ENABLE_FREQ_ASSERT 1
#define ENABLE_SLOW_ASSERT 1
#define ENABLE_ALLOC_STATS 1

#endif

#ifdef PLATFORM_WINDOWS
#	define PLATFORM_LINUX 0
#elif PLATFORM_LINUX
#	define PLATFORM_WINDOWS 0
#else
#	error Unsupported platform
#endif

// TODO: Based on architecture
#define IS_LITTLE_ENDIAN 1

/**
 * \def INL
 * Defines inline specifier
 */
#define INL inline

/**
 * \def NO_DISCARD(msg)
 * Defines nodiscard attribute
 */
#define NO_DISCARD(msg) [[nodiscard(msg)]]

/**
 * \def FALLTHROUGH
 * Defines fallthrough
 */
#define FALLTHROUGH [[fallthrough]]
/**
 * \def DEPRECATED(msg)
 * Defines deprecation attribute
 */
#define DEPRECATED(msg) [[deprecated(msg)]]
/**
 * \def NO_UNIQUE_ADDRESS
 * Defines no unique address attribute
 */
#define NO_UNIQUE_ADDRESS [[no_unique_address]]

/**
 * \def LIKELY
 * Defines likely attribute
 */
#define LIKELY [[likely]]
/** 
 * \def UNLIKELY
 * Defines unlikely attribute
 */
#define UNLIKELY [[unlikely]]

#define UNUSED(val) ((void)(val))
#define EMPTY_FOR_BODY UNUSED(0)

#define KB(x) ((x) << 10)
#define MB(x) ((x) << 20)

/**
 * \def EXPORT_API
 * Defines dynamic library linkage for export
 */

 /**
  * \def IMPORT_API
  * Defines dynamic library linkage for import
  */
#if PLATFORM_WINDOWS
#	if COMPILER_MSVC
#		define EXPORT_API __declspec(dllexport)
#	else
#		define EXPORT_API __attribute__((dllexport))
#	endif
#	if COMPILER_MSVC
#		define IMPORT_API __declspec(dllimport)
#	else
#		define IMPORT_API __attribute__((dllimport))
#	endif
#else
#	define EXPORT_API
#	define IMPORT_API
#endif

/**
 * \def FORCE_INL
 * Defines an attribute to try and force a function to be inlined (inlining is not guaranteed, but is strongly suggested)
 */
/**
 * \def DEBUG_BREAK
 * Defines a break in a debugger
 */

#if COMPILER_MSVC
#	define FORCE_INL __forceinline
#	define DEBUG_BREAK __debugbreak()
#else
#	define FORCE_INL __attribute__((always_inline))
#	define DEBUG_BREAK __asm__ __volatile__("int3")
#endif


// Project specific
#ifndef EXPORT_CORE
#	define EXPORT_CORE 0
#endif

/**
 * \def CORE_API
 * Dynamic library linkage for core
 */
#if EXPORT_CORE
#	define CORE_API EXPORT_API
#else
#	define CORE_API IMPORT_API
#endif

#define FORCE_ARRAY_RUNTIME_ASSERT 0