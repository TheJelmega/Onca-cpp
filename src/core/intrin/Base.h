#pragma once
#include "core/MinInclude.h"

// TODO: AVX512
// TODO: ARM NEON/HELIUM

#if !defined(DISABLE_SSE)
#	define DISABLE_SSE 0
#endif
#if !defined(DISABLE_SSE2)
#	define DISABLE_SSE2 0
#endif
#if !defined(DISABLE_SSE3)
#	define DISABLE_SSE3 0
#endif
#if !defined(DISABLE_SSSE3)
#	define DISABLE_SSSE3 0
#endif
#if !defined(DISABLE_SSE4_1)
#	define DISABLE_SSE4_1 0
#endif
#if !defined(DISABLE_SSE4_2)
#	define DISABLE_SSE4_2 0
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

#if !DISABLE_SSE && (defined(__SSE__) || defined(_M_X64))
#	define HAS_SSE 1
#else
#	define HAS_SSE 0
#endif

#if HAS_SSE && !DISABLE_SSE2 && (defined(__SSE2__) || defined(_M_X64))
#	define HAS_SSE2 1
#else
#	define HAS_SSE2 0
#endif

#if HAS_SSE2 && !DISABLE_SSE3 && (defined(__SSE3__) || defined(__AVX__))
#	define HAS_SSE3 1
#else
#	define HAS_SSE3 0
#endif

#if HAS_SSE3 && !DISABLE_SSSE3 && (defined(__SSSE3__) || defined(__AVX__))
#	define HAS_SSSE3 1
#else
#	define HAS_SSSE3 0
#endif

#if HAS_SSSE3 && !DISABLE_SSE4_1 && (defined(__SSE4_1__) || defined(__AVX__))
#	define HAS_SSE4_1 1
#else
#	define HAS_SSE4_1 0
#endif

#if HAS_SSE4_1 && !DISABLE_SSE4_2 && (defined(__SSE4_2__) || defined(__AVX__))
#	define HAS_SSE4_2 1
#else
#	define HAS_SSE4_2 0
#endif

#if HAS_SSE4_2 && !DISABLE_AVX && defined(__AVX__)
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

#ifdef __RESHARPER__
#undef HAS_SSE
#define HAS_SSE 1
#undef HAS_SSE2
#define HAS_SSE2 1
#undef HAS_SSE3
#define HAS_SSE3 1
#undef HAS_SSSE3
#define HAS_SSSE3 1
#undef HAS_SSE4_1
#define HAS_SSE4_1 1
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

#if HAS_SSE
#	include <mmintrin.h>
#	include <xmmintrin.h>
#endif

#if HAS_SSE2
#	include <emmintrin.h>
#endif

#if HAS_SSE3
#	include <pmmintrin.h>
#endif

#if HAS_SSSE3
#	include <tmmintrin.h>
#endif

#if HAS_SSE4_1
#	include <smmintrin.h>
#endif

#if HAS_SSE4_2
#	include <nmmintrin.h>
#endif

#if HAS_AVX
#	include <immintrin.h> // AVX, AVX2, AVX512, FMA
#endif

// MSVC sometimes doesn't include certain intrinsics, like _mm_setr_epix64,
// <intrin.h> allows us to use them
#if COMPILER_MSVC
#	include <intrin.h>
#endif
