#pragma once
#include "core/MinInclude.h"

// TODO: AVX512
// TODO: ARM NEON/HELIUM

#if !defined(DISABLE_SSE_SUPPORT)
#	define DISABLE_SSE_SUPPORT 0
#endif
#if !defined(DISABLE_AVX)
#	define DISABLE_AVX 0
#endif
#if !defined(DISABLE_AVX2)
#	define DISABLE_AVX2 0
#endif
#if !defined(DISABLE_AVX512)
#	define DISABLE_AVX512 0
#endif

#if !DISABLE_SSE_SUPPORT && (defined(__SSE4_2__) || defined(__AVX__))
#	define HAS_SSE_SUPPORT 1
#else
#	define HAS_SSE_SUPPORT 0
#endif

#if HAS_SSE_SUPPORT && !DISABLE_AVX && defined(__AVX__)
#	define HAS_AVX 1
#else
#	define HAS_AVX 0
#endif

#if HAS_AVX && !DISABLE_AVX2 && defined(__AVX2__)
#	define HAS_AVX2 1
#else
#	define HAS_AVX2 0
#endif

// Enable all for resharper

// TODO: check if this is always the case
#define HAS_POPCNT HAS_SSE_SUPPORT

#ifdef __RESHARPER__
#undef HAS_SSSE3
#define HAS_SSSE3 1
#undef HAS_SSE_SUPPORT
#define HAS_SSE_SUPPORT 1
#undef HAS_POPCNT
#define HAS_POPCNT 1
#undef HAS_AVX
#define HAS_AVX 1
#undef HAS_AVX2
#define HAS_AVX2 1
#endif

// Currently we have no working implementations for AVX512
#define HAS_AVX512F 0
#define HAS_AVX512CD 0
#define HAS_AVX512ER 0
#define HAS_AVX512PF 0
#define HAS_AVX512VL 0
#define HAS_AVX512DQ 0
#define HAS_AVX512BW 0
#define HAS_AVX512IFMA 0
#define HAS_AVX512VBMI 0
#define HAS_AVX5124VNNIW 0
#define HAS_AVX5124FMAPS 0
#define HAS_AVX512VPOPCNTDQ 0
#define HAS_AVX512VNNI 0
#define HAS_AVX512VBMI2 0
#define HAS_AVX512BITALG 0
#define HAS_AVX512VP2INTERSECT 0
#define HAS_AVX512GFNI 0
#define HAS_AVX512VPCLMULQDQ 0
#define HAS_AVX512VAES 0

#if HAS_SSE_SUPPORT
#	include <immintrin.h> // all SIMD intrinsics, should work on all x86/64 compilers
#endif

// MSVC sometimes doesn't include certain intrinsics, like _mm_setr_epix64,
// <intrin.h> allows us to use them
#if COMPILER_MSVC
#	include <intrin.h>
#endif
