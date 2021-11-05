#pragma once
#if __RESHARPER__
#include "Pack.h"
#endif

namespace Core::Intrin
{
	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Neg() const noexcept -> Pack
	{
		Pack pack{ UnInit };

		if (UnsignedIntegral<T>)
			return *this;

		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_sub_pd(_mm_setzero_pd(), data.sse_m128d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_sub_ps(_mm_setzero_ps(), data.sse_m128);
					return pack;
#endif
				}
				else if constexpr (IsI64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_sub_epi64(_mm_setzero_si128(), data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI32<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_sub_epi32(_mm_setzero_si128(), data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI16<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_sub_epi16(_mm_setzero_si128(), data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI8<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_sub_epi8(_mm_setzero_si128(), data.sse_m128i);
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = HalfPack(0).Neg().data;
					pack.data.m128[1] = HalfPack(1).Neg().data;
					return pack;
				}

				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_sub_pd(_mm256_setzero_pd(), data.sse_m256d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_sub_ps(_mm256_setzero_ps(), data.sse_m256);
					return pack;
#endif
				}
				else if constexpr (IsI64<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_sub_epi64(_mm256_setzero_si256(), data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI32<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_sub_epi32(_mm256_setzero_si256(), data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI16<T>)
				{
#if HAS_AVX
					pack.data.sse_m256i = _mm256_sub_epi16(_mm256_setzero_si256(), data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_sub_epi8(_mm256_setzero_si256(), data.sse_m256i);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.raw[i] = -data.raw[i];
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Add(const Pack& other) const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_add_pd(data.sse_m128d, other.data.sse_m128d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_add_ps(data.sse_m128, other.data.sse_m128);
					return pack;
#endif
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_add_epi64(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_add_epi32(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_add_epi16(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_add_epi8(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = HalfPack(0).Add(other.HalfPack(0)).data;
					pack.data.m128[1] = HalfPack(1).Add(other.HalfPack(1)).data;
				}

				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_add_pd(data.sse_m256d, other.data.sse_m256d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_add_ps(data.sse_m256, other.data.sse_m256);
					return pack;
#endif
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_add_epi64(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_add_epi32(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_add_epi16(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_add_epi8(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.raw[i] = data.raw[i] + other.data.raw[i];
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Sub(const Pack& other) const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_sub_pd(data.sse_m128d, other.data.sse_m128d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_sub_ps(data.sse_m128, other.data.sse_m128);
					return pack;
#endif
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_sub_epi64(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_sub_epi32(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_sub_epi16(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_sub_epi8(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = HalfPack(0).Sub(other.HalfPack(0)).data;
					pack.data.m128[1] = HalfPack(1).Sub(other.HalfPack(1)).data;
				}

				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_sub_pd(data.sse_m256d, other.data.sse_m256d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_sub_ps(data.sse_m256, other.data.sse_m256);
					return pack;
#endif
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_sub_epi64(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_sub_epi32(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_sub_epi16(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_sub_epi8(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.raw[i] = data.raw[i] - other.data.raw[i];
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Mul(const Pack& other) const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_mul_pd(data.sse_m128d, other.data.sse_m128d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_mul_ps(data.sse_m128, other.data.sse_m128);
					return pack;
#endif
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_SSE4_1
					__m128i bswap = _mm_shuffle_epi32(other.data.sse_m128i, 0xB1);      // (b0H,b0L,b1H,b1L) <- 0xB1 = 10 11 00 01 => (1,0,3,2)
					__m128i prodlh = _mm_mullo_epi32(data.sse_m128i, bswap);             // (a0L*b0H,a0H*a0L,a1L*b1H,a1H*a1L)
					__m128i zero = _mm_setzero_si128();
					__m128i prodlh2 = _mm_hadd_epi32(prodlh, zero);                       // (a0L*b0H+a0H*aOL,a1L*b1H+a1H*b1L,0,0)
					__m128i prodlh3 = _mm_shuffle_epi32(prodlh2, 0x73);                   // (0,a0L*b0H+a0H*b0L,0,a1L*b1H+a1H*b1L) <- 0x73 = 01 11 00 11 (3,0,1,0)
					__m128i prodll = _mm_mul_epu32(data.sse_m128i, other.data.sse_m128i); // (a0L*b0L,a1L*b1L)
					pack.data.sse_m128i = _mm_add_epi64(prodlh3, prodll);           // (a0L*b0L+((a0L*b0H+a0H*b0L)<<32),a1L*b1L+((a1L*b1H+a1H*b1L)<<32))
					return pack;
#elif HAS_SSE2
					alignas(DataSize) T a[2];
					alignas(DataSize) T b[2];
					_mm_store_si128(reinterpret_cast<__m128i*>(a), data.sse_m128i);
					_mm_store_si128(reinterpret_cast<__m128i*>(b), other.data.sse_m128i);

					a[0] *= b[0];
					a[1] *= b[1];
					pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(a));
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_SSE4_1
					pack.data.sse_m128i = _mm_mullo_epi32(data.sse_m128i, other.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					__m128i a13 = _mm_shuffle_epi32(data.sse_m128i, 0xF5);                // (-,a3,-,a1) <- 0xF5 = 11 11 01 01 (3,3,1,1)
					__m128i b13 = _mm_shuffle_epi32(other.data.sse_m128i, 0xF5);          // (-,b3,-,b1) <- 0xF5 = 11 11 01 01 (3,3,1,1)
					__m128i prod02 = _mm_mul_epu32(data.sse_m128i, other.data.sse_m128i); // (-,a2*b2,-,a0*b0)
					__m128i prod13 = _mm_mul_epu32(a13, b13);                             // (-,a3*b3,-,a1*b1)
					__m128i prod01 = _mm_unpacklo_epi32(prod02, prod13);                  // (-,-,a1*b1,a0*b0)
					__m128i prod23 = _mm_unpackhi_epi32(prod02, prod13);                  // (-,-,a3*b3,a2*b2)
					pack.data.sse_m128i = _mm_unpacklo_epi64(prod01, prod23);             // (a3*b3,a2*b2,a1*b1,a0*b0)
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_mullo_epi16(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}

				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_SSE2
					__m128i aodd = _mm_srli_epi16(data.sse_m128i, 8);
					__m128i bodd = _mm_srli_epi16(other.data.sse_m128i, 8);
					__m128i mulEven = _mm_mullo_epi16(data.sse_m128i, other.data.sse_m128i);
					__m128i mulOdd = _mm_mullo_epi16(aodd, bodd);
					mulOdd = _mm_slli_epi16(mulOdd, 8);

					// blend results
					__m128i mask = _mm_set1_epi32(0x00FF00FF);
#if HAS_SSE4_1
					pack.data.sse_m128i = _mm_blendv_epi8(mulOdd, mulEven, mask);
#else
					pack.data.sse_m128i = _mm_or_si128(_mm_and_si128(mask, mulEven), _mm_andnot_si128(mask, mulOdd));
#endif
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = HalfPack(0).Mul(other.HalfPack(0)).data;
					pack.data.m128[1] = HalfPack(1).Mul(other.HalfPack(1)).data;
				}

				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_mul_pd(data.sse_m256d, other.data.sse_m256d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_mul_ps(data.sse_m256, other.data.sse_m256);
					return pack;
#endif
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_AVX2
					__m256i bswap = _mm256_shuffle_epi32(other.data.sse_m256i, 0xB1);      // (b0H,b0L,b1H,b1L,b2H,b2L,b3H,b3L) <- 0xB1 = 10 11 00 01 => (1,0,3,2)
					__m256i prodlh = _mm256_mullo_epi32(data.sse_m256i, bswap);             // (a0L*b0H,a0H*a0L,a1L*b1H,a1H*a1L,a2L*b2H,a2H*b2L,a3L*b3H,a3H*b3L)
					__m256i zero = _mm256_setzero_si256();
					__m256i prodlh2 = _mm256_hadd_epi32(prodlh, zero);                       // (a0L*b0H+a0H*aOL,a1L*b1H+a1H*b1L,0,0,a2L*b2H+a2H*b2L,a3L*b3H+a3H*b3L)
					__m256i prodlh3 = _mm256_shuffle_epi32(prodlh2, 0x73);                   // (0,a0L*b0H+a0H*b0L,0,a1L*b1H+a1H*b1L,0,a2L*b2H+a2H*b2L,0,a3L*b3H+a3H*b3L) <- 0x73 = 01 11 00 11 (3,0,1,0)
					__m256i prodll = _mm256_mul_epu32(data.sse_m256i, other.data.sse_m256i); // (a0L*b0L,a1L*b1L,a2L*b2L,a3L*b3L)
					pack.data.sse_m256i = _mm256_add_epi64(prodlh3, prodll);           // (a0L*b0L+((a0L*b0H+a0H*b0L)<<32),a1L*b1L+((a1L*b1H+a1H*b1L)<<32),
					                                                                   //  a2L*b2L+((a2L*b2H+a2H*b2L)<<32),a3L*b3L+((a3L*b3H+a3H*b3L)<<32))
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_mullo_epi32(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_mullo_epi16(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_AVX2
					__m256i aodd = _mm256_srli_epi16(data.sse_m256i, 8);
					__m256i bodd = _mm256_srli_epi16(other.data.sse_m256i, 8);
					__m256i mulEven = _mm256_mullo_epi16(data.sse_m256i, other.data.sse_m256i);
					__m256i mulOdd = _mm256_mullo_epi16(aodd, bodd);
					mulOdd = _mm256_slli_epi16(mulOdd, 8);

					__m256i mask = _mm256_set1_epi32(0x00FF00FF);
					pack.data.sse_m256i = _mm256_blendv_epi8(mulOdd, mulEven, mask);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.raw[i] = data.raw[i] * other.data.raw[i];
		return pack;
	}

	// TODO: Create faster integer division with SIMD if possible
	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Div(const Pack& other) const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_div_pd(data.sse_m128d, other.data.sse_m128d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_div_ps(data.sse_m128, other.data.sse_m128);
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256() && FloatingPoint<T>)
				{
					pack.data.m128[0] = HalfPack(0).Div(other.HalfPack(0)).data;
					pack.data.m128[1] = HalfPack(1).Div(other.HalfPack(1)).data;
				}

				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_div_pd(data.sse_m256d, other.data.sse_m256d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_div_ps(data.sse_m256, other.data.sse_m256);
					return pack;
#endif
				}
			}

			if constexpr (IsNative())
			{
				alignas(DataSize) T a[Width];
				alignas(DataSize) T b[Width];
				AlignedStore(a);
				other.AlignedStore(b);

				for (usize i = 0; i < Width; ++i)
					a[i] /= b[i];
				return Pack::AlignedLoad(a);
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.raw[i] = data.raw[i] / other.data.raw[i];
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::AddSaturated(const Pack& other) const noexcept -> Pack
	{
		Pack pack{ UnInit };

		if constexpr (FloatingPoint<T>)
			return Add(other);

		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsU16<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_adds_epu16(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsU8<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_adds_epu8(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI16<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_adds_epi16(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI8<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_adds_epi8(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256() &&
							  (IsU16<T> || IsI16<T> || IsU8<T> || IsI8<T>))
				{
					pack.data.m128[0] = HalfPack(0).AddSaturated(other.HalfPack(0)).data;
					pack.data.m128[1] = HalfPack(1).AddSaturated(other.HalfPack(1)).data;
					return pack;
				}

				if constexpr (IsU16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_adds_epu16(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_adds_epu8(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_adds_epi16(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_adds_epi8(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
			}

			if constexpr (IsNative())
			{
				Pack sum = Add(other);
				Pack mask = Compare<ComparisonOp::Gt>(sum);
				Pack sat = Pack::Set(Math::MaxVal<T>);
				pack = sum.Blend(sat, mask);
				return pack;
			}
		}

		for (usize i = 0; i < Width; ++i)
		{
			if (Math::MaxVal<T> -data.raw[i] < other.data.raw[i])
				pack.data.raw[i] = Math::MaxVal<T>;
			else
				pack.data.raw[i] = data.raw[i] + other.data.raw[i];
		}
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::SubSaturated(const Pack& other) const noexcept -> Pack
	{
		Pack pack{ UnInit };

		if constexpr (FloatingPoint<T>)
			return Sub(other);

		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsU16<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_subs_epu16(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsU8<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_subs_epu8(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI16<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_subs_epi16(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI8<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_subs_epi8(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256() &&
							  (IsU16<T> || IsI16<T> || IsU8<T> || IsI8<T>))
				{
					pack.data.m128[0] = HalfPack(0).SubSaturated(other.HalfPack(0)).data;
					pack.data.m128[1] = HalfPack(1).SubSaturated(other.HalfPack(1)).data;
					return pack;
				}

				if constexpr (IsU16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_subs_epu16(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_subs_epu8(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_subs_epi16(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_subs_epi8(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
			}

			if constexpr (IsNative())
			{
				Pack sum = Sub(other);
				Pack mask = Compare<ComparisonOp::Lt>(sum);
				Pack sat = Pack::Set(Math::MinVal<T>);
				pack = sum.Blend(sat, mask);
				return pack;
			}
		}

		for (usize i = 0; i < Width; ++i)
		{
			if (Core::Math::LowestVal<T> + other.data.raw[i] > data.raw[i])
				pack.data.raw[i] = Math::LowestVal<T>;
			else
				pack.data.raw[i] = data.raw[i] - other.data.raw[i];
		}
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Rcp() const noexcept -> Pack
	{
		Pack pack{ UnInit };

		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_AVX512F && HAS_AVX512VL
					// TODO: Check if additional newton-raphson iteration is useful here
					// has a precision of 2^-14
					pack.data.sse_m128d = _mm_rcp14_pd(data.sse_m128i);
#elif HAS_SSE2
					__m128d ones = _mm_set1_pd(1.0);
					pack.data.sse_m128d = _mm_div_pd(ones, data.sse_m128d);
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX512F && HAS_AVX512VL
					// TODO: Check if additional newton-raphson iteration is useful here
					// has a precision of 2^-14 
					pack.data.sse_m128 = _mm_rcp14_ps(data.sse_m128);
#elif HAS_SSE
					// only has a precision of 1.5*2^-12
					__m128 rcp = _mm_rcp_ps(data.sse_m128);

					// Newton-Raphson iteration
					__m128 a = _mm_mul_ps(rcp, rcp);
					__m128 b = _mm_mul_ps(data.sse_m128, a);
					__m128 c = _mm_add_ps(rcp, rcp);
					pack.data.sse_m128 = _mm_sub_ps(c, b);
					return pack;
#endif
				}
				else if constexpr (UnsignedIntegral<T>)
				{
					// Unsigned rcp: 1 -> 1, other -> 0
#if HAS_SSE2
					Pack ones = Pack::Set(1);
					Pack onesMask = Compare<ComparisonOp::Eq>(ones);
					return ones & onesMask;
#endif
				}
				else if constexpr (SignedIntegral<T>)
				{
					// Signed rcp: 1 -> 1, -1 -> -1, other -> 0
#if HAS_SSE2
					Pack ones = Pack::Set(1);
					Pack onesMask = Compare<ComparisonOp::Eq>(ones);
					Pack negOnes = Pack::Set(-1);
					Pack negOnesMask = Compare<ComparisonOp::Eq>(negOnes);
					return (ones & onesMask) | (negOnes & negOnesMask);
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = HalfPack(0).Rcp().data;
					pack.data.m128[1] = HalfPack(1).Rcp().data;
					return pack;
				}

				if constexpr (IsF64<T>)
				{
#if HAS_AVX512F && HAS_AVX512VL
					// TODO: Check if additional newton-raphson iteration is useful here
					// has a precision of 2^-14
					pack.data.sse_m256d = _mm256_rcp14_pd(data.sse_m256i);
#elif HAS_AVX
					__m256d ones = _mm256_set1_pd(1.0);
					pack.data.sse_m256d = _mm256_div_pd(ones, data.sse_m256d);
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX512F && HAS_AVX512VL
					// TODO: Check if additional newton-raphson iteration is useful here
					// has a precision of 2^-14
					pack.data.sse_m256 = _mm256_rcp14_ps(data.sse_m256);
#elif HAS_AVX
					// only has a precision of 1.5*2^-12
					__m256 rcp = _mm256_rcp_ps(data.sse_m256);

					// Newton-Raphson iteration
					__m256 a = _mm256_mul_ps(rcp, rcp);
					__m256 b = _mm256_mul_ps(data.sse_m256, a);
					__m256 c = _mm256_add_ps(rcp, rcp);
					pack.data.sse_m256 = _mm256_sub_ps(c, b);
					return pack;
#endif
				}
				else if constexpr (UnsignedIntegral<T>)
				{
					// Unsigned rcp: 1 -> 1, other -> 0
#if HAS_AVX2
					Pack ones = Pack::Set(1);
					Pack onesMask = Compare<ComparisonOp::Eq>(ones);
					return ones & onesMask;
#endif
				}
				else if constexpr (SignedIntegral<T>)
				{
#if HAS_AVX2
					Pack ones = Pack::Set(1);
					Pack onesMask = Compare<ComparisonOp::Eq>(ones);
					Pack negOnes = Pack::Set(-1);
					Pack negOnesMask = Compare<ComparisonOp::Eq>(negOnes);
					return (ones & onesMask) | (negOnes & negOnesMask);
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.raw[i] = Math::Rcp(data.raw[i]);
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Sqrt() const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_sqrt_pd(data.sse_m128d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_sqrt_ps(data.sse_m128);
					return pack;
#endif
				}
				// TODO: SIMD integer version
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = HalfPack(0).Sqrt().data;
					pack.data.m128[1] = HalfPack(1).Sqrt().data;
					return pack;
				}

				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_sqrt_pd(data.sse_m256d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_sqrt_ps(data.sse_m256);
					return pack;
#endif
				}
				// TODO: SIMD integer version
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.raw[i] = Math::Sqrt(data.raw[i]);
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::RSqrt() const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF32<T>)
				{
#if HAS_SSE
					__m128 rsqrt = _mm_rsqrt_ps(data.sse_m128);

					__m128 three = _mm_set1_ps(3.0f);
					__m128 half = _mm_set1_ps(0.5f);
					__m128 a = _mm_mul_ps(data.sse_m128, rsqrt);
					__m128 b = _mm_mul_ps(a, rsqrt);
					__m128 c = _mm_mul_ps(half, rsqrt);
					__m128 d = _mm_sub_ps(three, b);
					pack.data.sse_m128 = _mm_mul_ps(c, d);
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsF32<T>)
				{
#if HAS_AVX
					__m256 rsqrt = _mm256_rsqrt_ps(data.sse_m256);

					__m256 three = _mm256_set1_ps(3.0f);
					__m256 half = _mm256_set1_ps(0.5f);
					__m256 a = _mm256_mul_ps(data.sse_m256, rsqrt);
					__m256 b = _mm256_mul_ps(a, rsqrt);
					__m256 c = _mm256_mul_ps(half, rsqrt);
					__m256 d = _mm256_sub_ps(three, b);
					pack.data.sse_m256 = _mm256_mul_ps(c, d);
					return pack;
#elif HAS_SSE
					pack.data.m128[0] = HalfPack(0).RSqrt().data;
					pack.data.m128[1] = HalfPack(1).RSqrt().data;
					return pack;
#endif
				}
			}
		}

		return Sqrt().Rcp();
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Abs() const noexcept -> Pack
	{
		if (UnsignedIntegral<T>)
			return Pack{ data };

		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					__m128d mask = _mm_set1_pd(Math::IntBitsToFloat(0x7FFF'FFFF'FFFF'FFFF));
					pack.data.sse_m128d = _mm_and_pd(data.sse_m128d, mask);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					__m128 mask = _mm_set1_ps(Math::IntBitsToFloat(0x7FFF'FFFF));
					pack.data.sse_m128 = _mm_and_ps(data.sse_m128, mask);
					return pack;
#endif
				}
				else if constexpr (IsI64<T>)
				{
#if HAS_AVX512F && HAS_AVX512VL
					pack.data.sse_m128i = _mm_abs_epi64(data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI32<T>)
				{
#if HAS_SSSE3
					pack.data.sse_m128i = _mm_abs_epi32(data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI16<T>)
				{
#if HAS_SSSE3
					pack.data.sse_m128i = _mm_abs_epi16(data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI8<T>)
				{
#if HAS_SSSE3
					pack.data.sse_m128i = _mm_abs_epi8(data.sse_m128i);
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = HalfPack(0).Abs().data;
					pack.data.m128[1] = HalfPack(1).Abs().data;
					return pack;
				}

				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					__m256i mask = _mm256_set1_epi64x(0x7FFF'FFFF'FFFF'FFFF);
					pack.data.sse_m256i = _mm256_and_si256(data.sse_m256i, mask);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					__m256i mask = _mm256_set1_epi32(0x7FFF'FFFF);
					pack.data.sse_m256i = _mm256_and_si256(data.sse_m256i, mask);
					return pack;
#endif
				}
				else if constexpr (IsI64<T>)
				{
#if HAS_AVX512F && HAS_AVX512VL
					pack.data.sse_m256i = _mm256_abs_epi64(data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI32<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_abs_epi32(data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_abs_epi16(data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_abs_epi8(data.sse_m256i);
					return pack;
#endif
				}
			}

			if constexpr (IsNative())
			{
				constexpr usize Shift = sizeof(T) * 8 - 1;
				Pack sign = ShiftRA<Shift>();
				Pack inv = Xor(sign);
				return inv.Sub(sign);
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.raw[i] = Math::Abs(data.raw[i]);
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Sign() const noexcept -> Pack
	{
		Pack pack{ UnInit };

		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (FloatingPoint<T> || IsI64<T>)
				{
					Pack ones = Pack::Set(1);
					Pack negOnes = Pack::Set(-1);
					Pack zero = Pack::Zero();

					Pack oneMask = Compare<ComparisonOp::Gt>(zero);
					Pack negOneMask = Compare<ComparisonOp::Lt>(zero);

					ones = ones.And(oneMask);
					negOnes = negOnes.And(negOneMask);

					return ones.Or(negOnes);
				}
				else if constexpr (UnsignedIntegral<T>)
				{
					Pack ones = Pack::Set(1);
					Pack zero = Pack::Zero();

					Pack oneMask = Compare<ComparisonOp::Gt>(zero);
					return ones.And(oneMask);
				}
				else if constexpr (IsI32<T>)
				{
#if HAS_SSSE3
					__m128i ones = _mm_set1_epi32(1);
					pack.data.sse_m128i = _mm_sign_epi32(ones, data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI16<T>)
				{
#if HAS_SSSE3
					__m128i ones = _mm_set1_epi16(1);
					pack.data.sse_m128i = _mm_sign_epi16(ones, data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI8<T>)
				{
#if HAS_SSSE3
					__m128i ones = _mm_set1_epi8(1);
					pack.data.sse_m128i = _mm_sign_epi8(ones, data.sse_m128i);
					return pack;
#endif
				}
				else
				{
					
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = HalfPack(0).Sign().data;
					pack.data.m128[1] = HalfPack(1).Sign().data;
					return pack;
				}

				if constexpr (FloatingPoint<T> || IsI64<T>)
				{
					Pack ones = Pack::Set(1);
					Pack negOnes = Pack::Set(-1);
					Pack zero = Pack::Zero();

					Pack oneMask = Compare<ComparisonOp::Gt>(zero);
					Pack negOneMask = Compare<ComparisonOp::Lt>(zero);

					ones = ones.And(oneMask);
					negOnes = negOnes.And(negOneMask);

					return ones.Or(negOnes);
				}
				else if constexpr (UnsignedIntegral<T>)
				{
					Pack ones = Pack::Set(1);
					Pack zero = Pack::Zero();

					Pack oneMask = Compare<ComparisonOp::Gt>(zero);
					return ones.And(oneMask);
				}
				else if constexpr (IsI32<T>)
				{
#if HAS_AVX2
					__m256i ones = _mm256_set1_epi32(1);
					pack.data.sse_m256i = _mm256_sign_epi32(ones, data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI16<T>)
				{
#if HAS_AVX2
					__m256i ones = _mm256_set1_epi16(1);
					pack.data.sse_m256i = _mm256_sign_epi16(ones, data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI8<T>)
				{
#if HAS_AVX2
					__m256i ones = _mm256_set1_epi8(1);
					pack.data.sse_m256i = _mm256_sign_epi8(ones, data.sse_m256i);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.raw[i] = T(Math::Sign(data.raw[i]));
		return pack;
	}
}
