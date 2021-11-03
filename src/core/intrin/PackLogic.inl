#pragma once
#if __RESHARPER__
#include "Pack.h"
#endif

namespace Core::Intrin
{
	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::And(const Pack& other) const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_and_pd(data.sse_m128d, other.data.sse_m128d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_and_ps(data.sse_m128, other.data.sse_m128);
					return pack;
#endif
				}
				else
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_and_si128(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_and_pd(data.sse_m256d, other.data.sse_m256d);
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128d[0] = _mm_and_pd(data.sse_m128d[0], other.data.sse_m128d[0]);
					pack.data.sse_m128d[1] = _mm_and_pd(data.sse_m128d[1], other.data.sse_m128d[1]);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_and_ps(data.sse_m256, other.data.sse_m256);
					return pack;
#elif HAS_SSE
					pack.data.sse_m128[0] = _mm_and_ps(data.sse_m128[0], other.data.sse_m128[0]);
					pack.data.sse_m128[1] = _mm_and_ps(data.sse_m128[1], other.data.sse_m128[1]);
					return pack;
#endif
				}
				else
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_and_si256(data.sse_m256i, other.data.sse_m256i);
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128i[0] = _mm_and_si128(data.sse_m128i[0], other.data.sse_m128i[0]);
					pack.data.sse_m128i[1] = _mm_and_si128(data.sse_m128i[1], other.data.sse_m128i[1]);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
		{
			if constexpr (sizeof(T) == 8)
				pack.data.u64_[i] = data.u64_[i] & other.data.u64_[i];
			else if constexpr (sizeof(T) == 4)
				pack.data.u32_[i] = data.u32_[i] & other.data.u32_[i];
			else if constexpr (sizeof(T) == 2)
				pack.data.u16_[i] = data.u16_[i] & other.data.u16_[i];
			else
				pack.data.u8_[i]  = data.u8_[i]  & other.data.u8_[i];
		}
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::AndNot(const Pack& other) const noexcept -> Pack
	{
		Pack pack{ UnInit };

		// NOTE: SSE and AVX andnot are in reverse order, i.e.: ~a & b, so args are inverted
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_andnot_pd(other.data.sse_m128d, data.sse_m128d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_andnot_ps(other.data.sse_m128, data.sse_m128);
					return pack;
#endif
				}
				else
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_andnot_si128(other.data.sse_m128i, data.sse_m128i);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_andnot_pd(other.data.sse_m256d, data.sse_m256d);
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128d[0] = _mm_andnot_pd(other.data.sse_m128d[0], data.sse_m128d[0]);
					pack.data.sse_m128d[1] = _mm_andnot_pd(other.data.sse_m128d[1], data.sse_m128d[1]);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_andnot_ps(other.data.sse_m256, data.sse_m256);
					return pack;
#elif HAS_SSE
					pack.data.sse_m128[0] = _mm_andnot_ps(other.data.sse_m128[0], data.sse_m128[0]);
					pack.data.sse_m128[1] = _mm_andnot_ps(other.data.sse_m128[1], data.sse_m128[1]);
					return pack;
#endif
				}
				else
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_andnot_si256(other.data.sse_m256i, data.sse_m256i);
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128i[0] = _mm_andnot_si128(other.data.sse_m128i[0], data.sse_m128i[0]);
					pack.data.sse_m128i[1] = _mm_andnot_si128(other.data.sse_m128i[1], data.sse_m128i[1]);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
		{
			if constexpr (sizeof(T) == 8)
				pack.data.u64_[i] = data.u64_[i] & ~other.data.u64_[i];
			else if constexpr (sizeof(T) == 4)
				pack.data.u32_[i] = data.u32_[i] & ~other.data.u32_[i];
			else if constexpr (sizeof(T) == 2)
				pack.data.u16_[i] = data.u16_[i] & ~other.data.u16_[i];
			else
				pack.data.u8_[i]  = data.u8_[i]  & ~other.data.u8_[i];
		}
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Xor(const Pack& other) const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_xor_pd(data.sse_m128d, other.data.sse_m128d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_xor_ps(data.sse_m128, other.data.sse_m128);
					return pack;
#endif
				}
				else
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_xor_si128(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_xor_pd(data.sse_m256d, other.data.sse_m256d);
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128d[0] = _mm_xor_pd(data.sse_m128d[0], other.data.sse_m128d[0]);
					pack.data.sse_m128d[1] = _mm_xor_pd(data.sse_m128d[1], other.data.sse_m128d[1]);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_xor_ps(data.sse_m256, other.data.sse_m256);
					return pack;
#elif HAS_SSE
					pack.data.sse_m128[0] = _mm_xor_ps(data.sse_m128[0], other.data.sse_m128[0]);
					pack.data.sse_m128[1] = _mm_xor_ps(data.sse_m128[1], other.data.sse_m128[1]);
					return pack;
#endif
				}
				else
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_xor_si256(data.sse_m256i, other.data.sse_m256i);
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128i[0] = _mm_xor_si128(data.sse_m128i[0], other.data.sse_m128i[0]);
					pack.data.sse_m128i[1] = _mm_xor_si128(data.sse_m128i[1], other.data.sse_m128i[1]);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
		{
			if constexpr (sizeof(T) == 8)
				pack.data.u64_[i] = data.u64_[i] ^ other.data.u64_[i];
			else if constexpr (sizeof(T) == 4)
				pack.data.u32_[i] = data.u32_[i] ^ other.data.u32_[i];
			else if constexpr (sizeof(T) == 2)
				pack.data.u16_[i] = data.u16_[i] ^ other.data.u16_[i];
			else
				pack.data.u8_[i]  = data.u8_[i]  ^ other.data.u8_[i];
		}
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Or(const Pack& other) const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_or_pd(data.sse_m128d, other.data.sse_m128d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_or_ps(data.sse_m128, other.data.sse_m128);
					return pack;
#endif
				}
				else
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_or_si128(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_or_pd(data.sse_m256d, other.data.sse_m256d);
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128d[0] = _mm_or_pd(data.sse_m128d[0], other.data.sse_m128d[0]);
					pack.data.sse_m128d[1] = _mm_or_pd(data.sse_m128d[1], other.data.sse_m128d[1]);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_or_ps(data.sse_m256, other.data.sse_m256);
					return pack;
#elif HAS_SSE
					pack.data.sse_m128[0] = _mm_or_ps(data.sse_m128[0], other.data.sse_m128[0]);
					pack.data.sse_m128[1] = _mm_or_ps(data.sse_m128[1], other.data.sse_m128[1]);
					return pack;
#endif
				}
				else
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_or_si256(data.sse_m256i, other.data.sse_m256i);
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128i[0] = _mm_or_si128(data.sse_m128i[0], other.data.sse_m128i[0]);
					pack.data.sse_m128i[1] = _mm_or_si128(data.sse_m128i[1], other.data.sse_m128i[1]);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
		{
			if constexpr (sizeof(T) == 8)
				pack.data.u64_[i] = data.u64_[i] | other.data.u64_[i];
			else if constexpr (sizeof(T) == 4)
				pack.data.u32_[i] = data.u32_[i] | other.data.u32_[i];
			else if constexpr (sizeof(T) == 2)
				pack.data.u16_[i] = data.u16_[i] | other.data.u16_[i];
			else
				pack.data.u8_[i]  = data.u8_[i]  | other.data.u8_[i];
		}
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Not() const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL 
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					__m128d one = _mm_cmpeq_pd(data.sse_m128d, data.sse_m128d);
					pack.data.sse_m128d = _mm_xor_pd(data.sse_m128d, one);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					__m128 one = _mm_cmpeq_ps(data.sse_m128, data.sse_m128);
					pack.data.sse_m128 = _mm_xor_ps(data.sse_m128, one);
					return pack;
#endif
				}
				else
				{
#if HAS_SSE2
					__m128i one = _mm_cmpeq_epi32(data.sse_m128i, data.sse_m128i);
					pack.data.sse_m128i = _mm_xor_si128(data.sse_m128i, one);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					__m256d one = _mm256_cmp_pd(data.sse_m256d, data.sse_m256d, _CMP_EQ_OQ);
					pack.data.sse_m256d = _mm256_xor_pd(data.sse_m256d, one);
					return pack;
#elif HAS_SSE2
					__m128d one = _mm_cmpeq_pd(data.sse_m128d[0], data.sse_m128d[0]);
					pack.data.sse_m128d[0] = _mm_xor_pd(data.sse_m128d[0], one);
					pack.data.sse_m128d[1] = _mm_xor_pd(data.sse_m128d[1], one);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					__m256 one = _mm256_cmp_ps(data.sse_m256, data.sse_m256, _CMP_EQ_OQ);
					pack.data.sse_m256 = _mm256_xor_ps(data.sse_m256, one);
					return pack;
#elif HAS_SSE
					__m128 one = _mm_cmpeq_ps(data.sse_m128[0], data.sse_m128[0]);
					pack.data.sse_m128[0] = _mm_xor_ps(data.sse_m128[0], one);
					pack.data.sse_m128[1] = _mm_xor_ps(data.sse_m128[1], one);
					return pack;
#endif
				}
				else
				{
#if HAS_AVX2
					__m256i one = _mm256_cmpeq_epi64(data.sse_m256i, data.sse_m256i);
					pack.data.sse_m256i = _mm256_xor_si256(data.sse_m256i, one);
					return pack;
#elif HAS_SSE2
					__m128i one = _mm_cmpeq_epi32(data.sse_m128i[0], data.sse_m128i[0]);
					pack.data.sse_m128i[0] = _mm_xor_si128(data.sse_m128i[0], one);
					pack.data.sse_m128i[1] = _mm_xor_si128(data.sse_m128i[1], one);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
		{
			if constexpr (sizeof(T) == 8)
				pack.data.u64_[i] = ~data.u64_[i];
			else if constexpr (sizeof(T) == 4)
				pack.data.u32_[i] = ~data.u32_[i];
			else if constexpr (sizeof(T) == 2)
				pack.data.u16_[i] = ~data.u16_[i];
			else
				pack.data.u8_[i]  = ~data.u8_[i];
		}
		return pack;
	}

	template <SimdBaseType T, usize Width>
	template<IntegralOfSameSize<T> U>
	constexpr auto Pack<T, Width>::ShiftL(const Pack<U, Width>& count) const noexcept -> Pack
	{
		Pack pack{ UnInit };

		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX2
					pack.data.sse_m128i = _mm_sllv_epi64(data.sse_m128i, count.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_AVX2
					pack.data.sse_m128i = _mm_sllv_epi32(data.sse_m128i, count.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_AVX512BW && HAS_AVX512VL
					pack.data.sse_m128i = _mm_sllv_epi16(data.sse_m128i, count.data.sse_m128i);
					return pack;
#elif HAS_AVX2
					__m128i oddMask = _mm_set1_epi32(0x0000'FFFF);
					__m128i evenMask = _mm_set1_epi32(0xFFFF'0000);

					__m128i oddShift = _mm_and_si128(count.data.sse_m128i, oddMask);
					__m128i odd = _mm_sllv_epi32(data.sse_m128i, oddShift);
					odd = _mm_and_si128(odd, oddMask);

					__m128i even = _mm_and_si128(data.sse_m128i, evenMask);
					__m128i evenShift = _mm_srli_epi32(count.data.sse_m128i, 16);
					even = _mm_sllv_epi32(even, evenShift);

					pack.data.sse_m128i = _mm_or_si128(even, odd);
					return pack;
#endif
				}
				else
				{
#if HAS_AVX512BW && HAS_AVX512VL
					__m128i oddMask = _mm_set1_epi16(i16(0x00FF));
					__m128i evenMask = _mm_set1_epi16(i16(0xFF00));

					__m128i oddShift = _mm_and_si128(count.data.sse_m128i, oddMask);
					__m128i odd = _mm_sllv_epi16(data.sse_m128i, oddShift);
					odd = _mm_and_si128(odd, oddMask);

					__m128i even = _mm_and_si128(data.sse_m128i, evenMask);
					__m128i evenShift = _mm_srli_epi116(count.data.sse_m128i, 8);
					even = _mm_sllv_epi16(even, evenShift);

					pack.data.sse_m128i = _mm_or_si128(even, odd);
					return pack;
#elif HAS_AVX2
					__m128i aMask = _mm_set1_epi32(0x0000'00FF);
					__m128i bMask = _mm_set1_epi32(0x0000'FF00);
					__m128i cMask = _mm_set1_epi32(0x00FF'0000);
					__m128i dMask = _mm_set1_epi32(0xFF00'0000);

					__m128i aShift = _mm_and_si128(count.data.sse_m128i, aMask);
					__m128i a = _mm_sllv_epi32(data.sse_m128i, aShift);
					a = _mm_and_si128(a, aMask);

					__m128i bShift = _mm_and_si128(count.data.sse_m128i, bMask);
					bShift = _mm_srli_epi32(bShift, 8);
					__m128i b = _mm_and_si128(data.sse_m128i, bMask);
					b = _mm_sllv_epi32(b, bShift);
					b = _mm_and_si128(b, bMask);

					__m128i cShift = _mm_and_si128(count.data.sse_m128i, cMask);
					cShift = _mm_srli_epi32(cShift, 16);
					__m128i c = _mm_and_si128(data.sse_m128i, cMask);
					c = _mm_sllv_epi32(c, cShift);
					c = _mm_and_si128(c, cMask);

					__m128i d = _mm_and_si128(data.sse_m128i, dMask);
					__m128i dShift = _mm_srli_epi32(count.data.sse_m128i, 24);
					d = _mm_sllv_epi32(d, dShift);

					pack.data.sse_m128i = _mm_or_si128(_mm_or_si128(a, b), _mm_or_si128(c, d));
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_sllv_epi64(data.sse_m256i, count.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_sllv_epi32(data.sse_m256i, count.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_AVX512BW && HAS_AVX512VL
					pack.data.sse_m256i = _mm256_sllv_epi16(data.sse_m256i, count.data.sse_m256i);
					return pack;
#elif HAS_AVX2
					__m256i oddMask = _mm256_set1_epi32(0x0000'FFFF);
					__m256i evenMask = _mm256_set1_epi32(0xFFFF'0000);

					__m256i oddShift = _mm256_and_si256(count.data.sse_m256i, oddMask);
					__m256i odd = _mm256_sllv_epi32(data.sse_m256i, oddShift);
					odd = _mm256_and_si256(odd, oddMask);

					__m256i even = _mm256_and_si256(data.sse_m256i, evenMask);
					__m256i evenShift = _mm256_srli_epi32(count.data.sse_m256i, 16);
					even = _mm256_sllv_epi32(even, evenShift);

					pack.data.sse_m256i = _mm256_or_si256(even, odd);
					return pack;
#endif
				}
				else
				{
#if HAS_AVX512BW && HAS_AVX512VL
					__m256i oddMask = _mm256_set1_epi16(i16(0x00FF));
					__m256i evenMask = _mm256_set1_epi16(i16(0xFF00));

					__m256i oddShift = _mm256_and_si256(count.data.sse_m256i, oddMask);
					__m256i odd = _mm256_sllv_epi16(data.sse_m256i, oddShift);
					odd = _mm256_and_si256(odd, oddMask);

					__m256i even = _mm256_and_si256(data.sse_m256i, evenMask);
					__m256i evenShift = _mm256_srli_epi116(count.data.sse_m256i, 8);
					even = _mm256_sllv_epi16(even, evenShift);

					pack.data.sse_m256i = _mm256_or_si256(even, odd);
					return pack;
#elif HAS_AVX2
					__m256i aMask = _mm256_set1_epi32(0x0000'00FF);
					__m256i bMask = _mm256_set1_epi32(0x0000'FF00);
					__m256i cMask = _mm256_set1_epi32(0x00FF'0000);
					__m256i dMask = _mm256_set1_epi32(0xFF00'0000);

					__m256i aShift = _mm256_and_si256(count.data.sse_m256i, aMask);
					__m256i a = _mm256_sllv_epi32(data.sse_m256i, aShift);
					a = _mm256_and_si256(a, aMask);

					__m256i bShift = _mm256_and_si256(count.data.sse_m256i, bMask);
					bShift = _mm256_srli_epi32(bShift, 8);
					__m256i b = _mm256_and_si256(data.sse_m256i, bMask);
					b = _mm256_sllv_epi32(b, bShift);
					b = _mm256_and_si256(b, bMask);

					__m256i cShift = _mm256_and_si256(count.data.sse_m256i, cMask);
					cShift = _mm256_srli_epi32(cShift, 16);
					__m256i c = _mm256_and_si256(data.sse_m256i, cMask);
					c = _mm256_sllv_epi32(c, cShift);
					c = _mm256_and_si256(c, cMask);

					__m256i d = _mm256_and_si256(data.sse_m256i, dMask);
					__m256i dShift = _mm256_srli_epi32(count.data.sse_m256i, 24);
					d = _mm256_sllv_epi32(d, dShift);

					pack.data.sse_m256i = _mm256_or_si256(_mm256_or_si256(a, b), _mm256_or_si256(c, d));
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.bits[i] = data.bits[i] << count.data.bits[i];
		return pack;
	}

	template <SimdBaseType T, usize Width>
	template <UnsignedIntegral U>
	constexpr auto Pack<T, Width>::ShiftL(U count) const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_SSE2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m128i = _mm_sll_epi64(data.sse_m128i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_SSE2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m128i = _mm_sll_epi32(data.sse_m128i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_SSE2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m128i = _mm_sll_epi16(data.sse_m128i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_SSE2
					__m128i shift = _mm_set1_epi64x(count);
					__m128i oddMask = _mm_set1_epi16(i16(0x00FF));
					__m128i evenMask = _mm_set1_epi16(i16(0xFF00));
					
					__m128i odd = _mm_sll_epi16(data.sse_m128i, shift);
					odd = _mm_and_si128(odd, oddMask);

					__m128i even = _mm_and_si128(data.sse_m128i, evenMask);
					even = _mm_sll_epi16(even, shift);

					pack.data.sse_m128i = _mm_or_si128(even, odd);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m256i = _mm256_sll_epi64(data.sse_m256i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_AVX2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m256i = _mm256_sll_epi32(data.sse_m256i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_AVX2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m256i = _mm256_sll_epi16(data.sse_m256i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_AVX2
					__m128i shift = _mm_set1_epi64x(count);
					__m256i oddMask = _mm256_set1_epi16(i16(0x00FF));
					__m256i evenMask = _mm256_set1_epi16(i16(0xFF00));

					__m256i odd = _mm256_sll_epi16(data.sse_m256i, shift);
					odd = _mm256_and_si256(odd, oddMask);

					__m256i even = _mm256_and_si256(data.sse_m256i, evenMask);
					even = _mm256_sll_epi16(even, shift);

					pack.data.sse_m256i = _mm256_or_si256(even, odd);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.bits[i] = data.bits[i] << count;
		return pack;
	}

	template <SimdBaseType T, usize Width>
	template <usize Count>
	constexpr auto Pack<T, Width>::ShiftL() const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_slli_epi64(data.sse_m128i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_slli_epi32(data.sse_m128i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_slli_epi16(data.sse_m128i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_SSE2
					__m128i oddMask = _mm_set1_epi16(i16(0x00FF));
					__m128i evenMask = _mm_set1_epi16(i16(0xFF00));

					__m128i odd = _mm_slli_epi16(data.sse_m128i, Count);
					odd = _mm_and_si128(odd, oddMask);

					__m128i even = _mm_and_si128(data.sse_m128i, evenMask);
					even = _mm_slli_epi16(even, Count);

					pack.data.sse_m128i = _mm_or_si128(even, odd);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_slli_epi64(data.sse_m256i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_slli_epi32(data.sse_m256i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_slli_epi16(data.sse_m256i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_AVX2
					__m256i oddMask = _mm256_set1_epi16(i16(0x00FF));
					__m256i evenMask = _mm256_set1_epi16(i16(0xFF00));

					__m256i odd = _mm256_slli_epi16(data.sse_m256i, Count);
					odd = _mm256_and_si256(odd, oddMask);

					__m256i even = _mm256_and_si256(data.sse_m256i, evenMask);
					even = _mm256_slli_epi16(even, Count);

					pack.data.sse_m256i = _mm256_or_si256(even, odd);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.bits[i] = data.bits[i] << Count;
		return pack;
	}

	template <SimdBaseType T, usize Width>
	template<IntegralOfSameSize<T> U>
	constexpr auto Pack<T, Width>::ShiftRA(const Pack<U, Width>& count) const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX512F && HAS_AVX512VL
					pack.data.sse_m128i = _mm_srav_epi64(data.sse_m128i, count.data.sse_m128i);
					return pack;
#elif HAS_AVX2
					__m128i shifted = _mm_srlv_epi64(data.sse_m128i, count.data.sse_m128i);

					__m128i upperBit = _mm_set1_epi64x(0x7000'0000);
					upperBit = _mm_srlv_epi64(upperBit, count.data.sse_m128i);

					__m128i invShift = _mm_set1_epi64x(64);
					invShift = _mm_sub_epi64(invShift, count.data.sse_m128i);
					__m128i extension = _mm_set1_epi64x(~0ull);
					extension = _mm_sllv_epi64(extension, invShift);

					__m128i needExt = _mm_and_si128(shifted, upperBit);
					__m128i extMask = _mm_cmpeq_epi64(needExt, upperBit);
					extension = _mm_and_si128(extension, extMask);
					pack.data.sse_m128i = _mm_or_si128(shifted, extension);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_AVX2
					pack.data.sse_m128i = _mm_srav_epi32(data.sse_m128i, count.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_AVX512BW && HAS_AVX512VL
					pack.data.sse_m128i = _mm_srav_epi16(data.sse_m128i, count.data.sse_m128i);
					return pack;
#elif HAS_AVX2
					__m128i lowerMask = _mm_set1_epi32(0x0000'FFFF);
					__m128i upperMask = _mm_set1_epi32(0xFFFF'0000);

					__m128i upperShift = _mm_srli_epi32(count.data.sse_m128i, 16);
					__m128i upper = _mm_srav_epi32(data.sse_m128i, upperShift);
					upper = _mm_and_si128(upper, upperMask);

					__m128i lowerShift = _mm_and_si128(count.data.sse_m128i, lowerMask);
					__m128i lower = _mm_slli_epi32(data.sse_m128i, 16);
					lower = _mm_srav_epi32(lower, lowerShift);
					lower = _mm_srli_epi32(lower, 16);

					pack.data.sse_m128i = _mm_or_si128(upper, lower);
					return pack;
#endif
				}
				else
				{
#if HAS_AVX512BW && HAS_AVX512VL
					__m128i lowerMask = _mm_set1_epi16(0x00FF);
					__m128i upperMask = _mm_set1_epi16(0xFF00);

					__m128i upperShift = _mm_srli_epi32(count.data.sse_m128i, 8);
					__m128i upper = _mm_srav_epi16(data.sse_m128i, upperShift);
					upper = _mm_and_si128(upper, upperMask);

					__m128i lowerShift = _mm_and_si128(count.data.sse_m128i, lowerMask);
					__m128i lower = _mm_slli_epi16(data.sse_m128i, 8);
					lower = _mm_srav_epi16(lower, lowerShift);
					lower = _mm_srli_epi16(lower, 8);

					pack.data.sse_m128i = _mm_or_si128(upper, lower);
					return pack;
#elif HAS_AVX2
					__m128i upperMask = _mm_set1_epi32(0xFF00'0000);
					__m128i lowerMask = _mm_set1_epi32(0x0000'00FF);

					__m128i a = _mm_slli_epi32(data.sse_m128i, 24);
					__m128i aShift = _mm_and_si128(count.data.sse_m128i, lowerMask);
					a = _mm_srav_epi32(a, aShift);
					a = _mm_srli_epi32(a, 24);

					__m128i b = _mm_slli_epi32(data.sse_m128i, 16);
					__m128i bShift = _mm_srli_epi32(count.data.sse_m128i, 8);
					bShift = _mm_and_si128(bShift, lowerMask);
					b = _mm_srav_epi32(b, bShift);
					b = _mm_and_si128(b, upperMask);
					b = _mm_srli_epi32(b, 16);

					__m128i c = _mm_slli_epi32(data.sse_m128i, 8);
					__m128i cShift = _mm_srli_epi32(count.data.sse_m128i, 16);
					cShift = _mm_and_si128(cShift, lowerMask);
					c = _mm_srav_epi32(c, cShift);
					c = _mm_and_si128(c, upperMask);
					c = _mm_srli_epi32(c, 8);

					__m128i dShift = _mm_srli_epi32(count.data.sse_m128i, 24);
					__m128i d = _mm_srav_epi32(data.sse_m128i, dShift);
					d = _mm_and_si128(d, upperMask);

					pack.data.sse_m128i = _mm_or_si128(_mm_or_si128(a, b), _mm_or_si128(c, d));
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX512F && HAS_AVX512VL
					pack.data.sse_m256i = _mm256_srav_epi64(data.sse_m256i, count.data.sse_m256i);
					return pack;
#elif HAS_AVX2
					__m256i shifted = _mm256_srlv_epi64(data.sse_m256i, count.data.sse_m256i);

					__m256i upperBit = _mm256_set1_epi64x(0x7000'0000);
					upperBit = _mm256_srlv_epi64(upperBit, count.data.sse_m256i);

					__m256i invShift = _mm256_set1_epi64x(64);
					invShift = _mm256_sub_epi64(invShift, count.data.sse_m256i);
					__m256i extension = _mm256_set1_epi64x(~0ull);
					extension = _mm256_sllv_epi64(extension, invShift);

					__m256i needExt = _mm256_and_si256(shifted, upperBit);
					__m256i extMask = _mm256_cmpeq_epi64(needExt, upperBit);
					extension = _mm256_and_si256(extension, extMask);
					pack.data.sse_m256i = _mm256_or_si256(shifted, extension);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_srav_epi32(data.sse_m256i, count.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_AVX512BW && HAS_AVX512VL
					pack.data.sse_m256i = _mm256_srav_epi16(data.sse_m256i, count.data.sse_m256i);
					return pack;
#elif HAS_AVX2
					__m256i lowerMask = _mm256_set1_epi32(0x0000'FFFF);
					__m256i upperMask = _mm256_set1_epi32(0xFFFF'0000);

					__m256i upperShift = _mm256_srli_epi32(count.data.sse_m256i, 16);
					__m256i upper = _mm256_srav_epi32(data.sse_m256i, upperShift);
					upper = _mm256_and_si256(upper, upperMask);

					__m256i lowerShift = _mm256_and_si256(count.data.sse_m256i, lowerMask);
					__m256i lower = _mm256_slli_epi32(data.sse_m256i, 16);
					lower = _mm256_srav_epi32(lower, lowerShift);
					lower = _mm256_srli_epi32(lower, 16);

					pack.data.sse_m256i = _mm256_or_si256(upper, lower);
					return pack;
#endif
				}
				else
				{
#if HAS_AVX512BW && HAS_AVX512VL
					__m256i lowerMask = _mm256_set1_epi16(0x00FF);
					__m256i upperMask = _mm256_set1_epi16(0xFF00);

					__m256i upperShift = _mm256_srli_epi32(count.data.sse_m256i, 8);
					__m256i upper = _mm256_srav_epi16(data.sse_m256i, upperShift);
					upper = _mm256_and_si256(upper, upperMask);

					__m256i lowerShift = _mm256_and_si256(count.data.sse_m256i, lowerMask);
					__m256i lower = _mm256_slli_epi16(data.sse_m256i, 8);
					lower = _mm256_srav_epi16(lower, lowerShift);
					lower = _mm256_srli_epi16(lower, 8);

					pack.data.sse_m256i = _mm256_or_si256(upper, lower);
					return pack;
#elif HAS_AVX2
					__m256i upperMask = _mm256_set1_epi32(0xFF00'0000);
					__m256i lowerMask = _mm256_set1_epi32(0x0000'00FF);

					__m256i a = _mm256_slli_epi32(data.sse_m256i, 24);
					__m256i aShift = _mm256_and_si256(count.data.sse_m256i, lowerMask);
					a = _mm256_srav_epi32(a, aShift);
					a = _mm256_srli_epi32(a, 24);

					__m256i b = _mm256_slli_epi32(data.sse_m256i, 16);
					__m256i bShift = _mm256_srli_epi32(count.data.sse_m256i, 8);
					bShift = _mm256_and_si256(bShift, lowerMask);
					b = _mm256_srav_epi32(b, bShift);
					b = _mm256_and_si256(b, upperMask);
					b = _mm256_srli_epi32(b, 16);

					__m256i c = _mm256_slli_epi32(data.sse_m256i, 8);
					__m256i cShift = _mm256_srli_epi32(count.data.sse_m256i, 16);
					cShift = _mm256_and_si256(cShift, lowerMask);
					c = _mm256_srav_epi32(c, cShift);
					c = _mm256_and_si256(c, upperMask);
					c = _mm256_srli_epi32(c, 8);

					__m256i dShift = _mm256_srli_epi32(count.data.sse_m256i, 24);
					__m256i d = _mm256_srav_epi32(data.sse_m256i, dShift);
					d = _mm256_and_si256(d, upperMask);

					pack.data.sse_m256i = _mm256_or_si256(_mm256_or_si256(a, b), _mm256_or_si256(c, d));
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.sbits[i] = data.sbits[i] >> count.data.bits[i];
		return pack;
	}

	template <SimdBaseType T, usize Width>
	template <UnsignedIntegral U>
	constexpr auto Pack<T, Width>::ShiftRA(U count) const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX512F && HAS_AVX512VL
					pack.data.sse_m128i = _mm_srav_epi64(data.sse_m128i, count.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					__m128i shift = _mm_set1_epi64x(count);
					__m128i shifted = _mm_srl_epi64(data.sse_m128i, shift);

					__m128i upperBit = _mm_set1_epi64x(0x7000'0000);
					upperBit = _mm_srl_epi64(upperBit, shift);

					__m128i invShift = _mm_set1_epi64x(64 - count);
					__m128i extension = _mm_set1_epi64x(~0ull);
					extension = _mm_sll_epi64(extension, invShift);

					__m128i needExt = _mm_and_si128(shifted, upperBit);
					__m128i extMask = _mm_cmpeq_epi64(needExt, upperBit);
					extension = _mm_and_si128(extension, extMask);
					pack.data.sse_m128i = _mm_or_si128(shifted, extension);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_SSE2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m128i = _mm_sra_epi32(data.sse_m128i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_SSE2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m128i = _mm_sra_epi16(data.sse_m128i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_SSE2
					__m128i shift = _mm_set1_epi64x(count);
					__m128i evenMask = _mm_set1_epi16(i16(0xFF00));

					__m128i odd = _mm_slli_epi16(data.sse_m128i, 8);
					odd = _mm_sra_epi16(odd, shift);
					odd = _mm_srli_epi16(odd, 8);
					
					__m128i even = _mm_sra_epi16(data.sse_m128i, shift);
					even = _mm_and_si128(even, evenMask);
					
					pack.data.sse_m128i = _mm_or_si128(even, odd);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX512F && HAS_AVX512VL
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m256i = _mm256_sra_epi64(data.sse_m256i, shift);
					return pack;
#elif HAS_AVX2
					__m128i shift = _mm_set1_epi64x(count);
					__m256i shifted = _mm256_srl_epi64(data.sse_m256i, shift);

					__m256i upperBit = _mm256_set1_epi64x(0x7000'0000);
					upperBit = _mm256_srl_epi64(upperBit, shift);

					__m128i invShift = _mm_set1_epi64x(64 - count);
					__m256i extension = _mm256_set1_epi64x(~0ull);
					extension = _mm256_sll_epi64(extension, invShift);

					__m256i needExt = _mm256_and_si256(shifted, upperBit);
					__m256i extMask = _mm256_cmpeq_epi64(needExt, upperBit);
					extension = _mm256_and_si256(extension, extMask);
					pack.data.sse_m256i = _mm256_or_si256(shifted, extension);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_AVX2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m256i = _mm256_sra_epi32(data.sse_m256i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_AVX2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m256i = _mm256_sra_epi16(data.sse_m256i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_AVX2
					__m128i shift = _mm_set1_epi64x(count);
					__m256i evenMask = _mm256_set1_epi16(i16(0xFF00));
					
					__m256i odd = _mm256_slli_epi16(data.sse_m256i, 8);
					odd = _mm256_sra_epi16(odd, shift);
					odd = _mm256_srli_epi16(odd, 8);
					
					__m256i even = _mm256_sra_epi16(data.sse_m256i, shift);
					even = _mm256_and_si256(even, evenMask);
					
					pack.data.sse_m256i = _mm256_or_si256(even, odd);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.sbits[i] = data.sbits[i] >> count;
		return pack;
	}

	template <SimdBaseType T, usize Width>
	template <usize Count>
	constexpr auto Pack<T, Width>::ShiftRA() const noexcept -> Pack
	{
		Pack pack{ UnInit };
		//IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX512F && HAS_AVX512VL
					pack.data.sse_m128i = _mm_srav_epi64(data.sse_m128i, count.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					__m128i shifted = _mm_srli_epi64(data.sse_m128i, Count);

					__m128i upperBit = _mm_set1_epi64x(0x7000'0000);
					upperBit = _mm_srli_epi64(upperBit, Count);
					
					__m128i extension = _mm_set1_epi64x(~0ull);
					extension = _mm_slli_epi64(extension, 64 - Count);

					__m128i needExt = _mm_and_si128(shifted, upperBit);
					__m128i extMask = _mm_cmpeq_epi64(needExt, upperBit);
					extension = _mm_and_si128(extension, extMask);
					pack.data.sse_m128i = _mm_or_si128(shifted, extension);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_srai_epi32(data.sse_m128i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_srai_epi16(data.sse_m128i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_SSE2
					__m128i evenMask = _mm_set1_epi16(i16(0xFF00));

					__m128i odd = _mm_slli_epi16(data.sse_m128i, 8);
					odd = _mm_srai_epi16(odd, Count);
					odd = _mm_srli_epi16(odd, 8);

					__m128i even = _mm_srai_epi16(data.sse_m128i, Count);
					even = _mm_and_si128(even, evenMask);

					pack.data.sse_m128i = _mm_or_si128(even, odd);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX512F && HAS_AVX512VL
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m256i = _mm256_srai_epi64(data.sse_m256i, shift);
					return pack;
#elif HAS_AVX2
					__m256i shifted = _mm256_srli_epi64(data.sse_m256i, Count);

					__m256i upperBit = _mm256_set1_epi64x(0x7000'0000);
					upperBit = _mm256_srli_epi64(upperBit, Count);
					
					__m256i extension = _mm256_set1_epi64x(~0ull);
					extension = _mm256_slli_epi64(extension, 64 - Count);

					__m256i needExt = _mm256_and_si256(shifted, upperBit);
					__m256i extMask = _mm256_cmpeq_epi64(needExt, upperBit);
					extension = _mm256_and_si256(extension, extMask);
					pack.data.sse_m256i = _mm256_or_si256(shifted, extension);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_srai_epi32(data.sse_m256i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_srai_epi16(data.sse_m256i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_AVX2
					__m256i evenMask = _mm256_set1_epi16(i16(0xFF00));

					__m256i odd = _mm256_slli_epi16(data.sse_m256i, 8);
					odd = _mm256_srai_epi16(odd, Count);
					odd = _mm256_srli_epi16(odd, 8);

					__m256i even = _mm256_srai_epi16(data.sse_m256i, Count);
					even = _mm256_and_si256(even, evenMask);

					pack.data.sse_m256i = _mm256_or_si256(even, odd);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.sbits[i] = data.sbits[i] >> Count;
		return pack;
	}

	template <SimdBaseType T, usize Width>
	template<IntegralOfSameSize<T> U>
	constexpr auto Pack<T, Width>::ShiftRL(const Pack<U, Width>& count) const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX2
					pack.data.sse_m128i = _mm_srlv_epi64(data.sse_m128i, count.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_AVX2
					pack.data.sse_m128i = _mm_srlv_epi32(data.sse_m128i, count.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_AVX512BW && HAS_AVX512VL
					pack.data.sse_m128i = _mm_srlv_epi16(data.sse_m128i, count.data.sse_m128i);
					return pack;
#elif HAS_AVX2
					__m128i oddMask = _mm_set1_epi32(0x0000'FFFF);
					__m128i evenMask = _mm_set1_epi32(0xFFFF'0000);

					__m128i oddShift = _mm_and_si128(count.data.sse_m128i, oddMask);
					__m128i odd = _mm_and_si128(data.sse_m128i, oddMask);
					odd = _mm_srlv_epi32(odd, oddShift);
					odd = _mm_and_si128(odd, oddMask);
					
					__m128i evenShift = _mm_srli_epi32(count.data.sse_m128i, 16);
					__m128i even = _mm_srlv_epi32(data.sse_m128i, evenShift);
					even = _mm_and_si128(even, evenMask);

					pack.data.sse_m128i = _mm_or_si128(even, odd);
					return pack;
#endif
				}
				else
				{
#if HAS_AVX512BW && HAS_AVX512VL
					__m128i oddMask = _mm_set1_epi16(0x00FF);
					__m128i evenMask = _mm_set1_epi16(0xFF00);

					__m128i oddShift = _mm_and_si128(count.data.sse_m128i, oddMask);
					__m128i odd = _mm_and_si128(data.sse_m128i, oddMask);
					odd = _mm_srlv_epi16(odd, oddShift);

					__m128i evenShift = _mm_srli_epi16(count.data.sse_m128i, 8);
					__m128i even = _mm_srlv_epi16(data.sse_m128i, evenShift);
					even = _mm_and_si128(even, evenMask);

					pack.data.sse_m128i = _mm_or_si128(even, odd);
					return pack;
#elif HAS_AVX2
					__m128i aMask = _mm_set1_epi32(0x0000'00FF);
					__m128i bMask = _mm_set1_epi32(0x0000'FF00);
					__m128i cMask = _mm_set1_epi32(0x00FF'0000);
					__m128i dMask = _mm_set1_epi32(0xFF00'0000);

					__m128i aShift = _mm_and_si128(count.data.sse_m128i, aMask);
					__m128i a = _mm_and_si128(data.sse_m128i, aMask);
					a = _mm_srlv_epi32(a, aShift);

					__m128i bShift = _mm_and_si128(count.data.sse_m128i, bMask);
					bShift = _mm_srli_epi32(bShift, 8);
					__m128i b = _mm_and_si128(data.sse_m128i, bMask);
					b = _mm_srlv_epi32(b, bShift);
					b = _mm_and_si128(b, bMask);

					__m128i cShift = _mm_and_si128(count.data.sse_m128i, cMask);
					cShift = _mm_srli_epi32(cShift, 16);
					__m128i c = _mm_and_si128(data.sse_m128i, cMask);
					c = _mm_srlv_epi32(c, cShift);
					c = _mm_and_si128(c, cMask);
					
					__m128i dShift = _mm_srli_epi32(count.data.sse_m128i, 24);
					__m128i d = _mm_srlv_epi32(data.sse_m128i, dShift);
					d = _mm_and_si128(d, dMask);

					pack.data.sse_m128i = _mm_or_si128(_mm_or_si128(a, b), _mm_or_si128(c, d));
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_srlv_epi64(data.sse_m256i, count.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_srlv_epi32(data.sse_m256i, count.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_AVX512BW && HAS_AVX512VL
					pack.data.sse_m256i = _mm256_srlv_epi16(data.sse_m256i, count.data.sse_m256i);
					return pack;
#elif HAS_AVX2
					__m256i oddMask = _mm256_set1_epi32(0x0000'FFFF);
					__m256i evenMask = _mm256_set1_epi32(0xFFFF'0000);

					__m256i oddShift = _mm256_and_si256(count.data.sse_m256i, oddMask);
					__m256i odd = _mm256_and_si256(data.sse_m256i, oddMask);
					odd = _mm256_srlv_epi32(odd, oddShift);
					odd = _mm256_and_si256(odd, oddMask);

					__m256i evenShift = _mm256_srli_epi32(count.data.sse_m256i, 16);
					__m256i even = _mm256_srlv_epi32(data.sse_m256i, evenShift);
					even = _mm256_and_si256(even, evenMask);

					pack.data.sse_m256i = _mm256_or_si256(even, odd);
					return pack;
#endif
				}
				else
				{
#if HAS_AVX512BW && HAS_AVX512VL
					__m256i oddMask = _mm256_set1_epi16(0x00FF);
					__m256i evenMask = _mm256_set1_epi16(0xFF00);

					__m256i oddShift = _mm256_and_si256(count.data.sse_m256i, oddMask);
					__m256i odd = _mm256_and_si256(data.sse_m256i, oddMask);
					odd = _mm256_srlv_epi16(odd, oddShift);

					__m256i evenShift = _mm256_srli_epi16(count.data.sse_m256i, 8);
					__m256i even = _mm256_srlv_epi16(data.sse_m256i, evenShift);
					even = _mm256_and_si256(even, evenMask);

					pack.data.sse_m256i = _mm256_or_si256(even, odd);
					return pack;
#elif HAS_AVX2
					__m256i aMask = _mm256_set1_epi32(0x0000'00FF);
					__m256i bMask = _mm256_set1_epi32(0x0000'FF00);
					__m256i cMask = _mm256_set1_epi32(0x00FF'0000);
					__m256i dMask = _mm256_set1_epi32(0xFF00'0000);

					__m256i aShift = _mm256_and_si256(count.data.sse_m256i, aMask);
					__m256i a = _mm256_and_si256(data.sse_m256i, aMask);
					a = _mm256_srlv_epi32(a, aShift);

					__m256i bShift = _mm256_and_si256(count.data.sse_m256i, bMask);
					bShift = _mm256_srli_epi32(bShift, 8);
					__m256i b = _mm256_and_si256(data.sse_m256i, bMask);
					b = _mm256_srlv_epi32(b, bShift);
					b = _mm256_and_si256(b, bMask);

					__m256i cShift = _mm256_and_si256(count.data.sse_m256i, cMask);
					cShift = _mm256_srli_epi32(cShift, 16);
					__m256i c = _mm256_and_si256(data.sse_m256i, cMask);
					c = _mm256_srlv_epi32(c, cShift);
					c = _mm256_and_si256(c, cMask);

					__m256i dShift = _mm256_srli_epi32(count.data.sse_m256i, 24);
					__m256i d = _mm256_srlv_epi32(data.sse_m256i, dShift);
					d = _mm256_and_si256(d, dMask);

					pack.data.sse_m256i = _mm256_or_si256(_mm256_or_si256(a, b), _mm256_or_si256(c, d));
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.bits[i] = data.bits[i] >> count.data.bits[i];
		return pack;
	}

	template <SimdBaseType T, usize Width>
	template <UnsignedIntegral U>
	constexpr auto Pack<T, Width>::ShiftRL(U count) const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_SSE2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m128i = _mm_srl_epi64(data.sse_m128i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_SSE2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m128i = _mm_srl_epi32(data.sse_m128i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_SSE2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m128i = _mm_srl_epi16(data.sse_m128i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_SSE2
					__m128i shift = _mm_set1_epi64x(count);
					__m128i oddMask = _mm_set1_epi16(i16(0x00FF));
					__m128i evenMask = _mm_set1_epi16(i16(0xFF00));

					__m128i odd = _mm_and_si128(data.sse_m128i, oddMask);
					odd = _mm_srl_epi16(odd, shift);
					
					__m128i even = _mm_srl_epi16(data.sse_m128i, shift);
					even = _mm_and_si128(even, evenMask);

					pack.data.sse_m128i = _mm_or_si128(even, odd);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m256i = _mm256_srl_epi64(data.sse_m256i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_AVX2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m256i = _mm256_srl_epi32(data.sse_m256i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_AVX2
					__m128i shift = _mm_set1_epi64x(count);
					pack.data.sse_m256i = _mm256_srl_epi16(data.sse_m256i, shift);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_AVX2
					__m128i shift = _mm_set1_epi64x(count);
					__m256i oddMask = _mm256_set1_epi16(i16(0x00FF));
					__m256i evenMask = _mm256_set1_epi16(i16(0xFF00));

					__m256i odd = _mm256_and_si256(data.sse_m256i, oddMask);
					odd = _mm256_srl_epi16(odd, shift);

					__m256i even = _mm256_srl_epi16(data.sse_m256i, shift);
					even = _mm256_and_si256(even, evenMask);

					pack.data.sse_m256i = _mm256_or_si256(even, odd);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.bits[i] = data.bits[i] >> count;
		return pack;
	}

	template <SimdBaseType T, usize Width>
	template <usize Count>
	constexpr auto Pack<T, Width>::ShiftRL() const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_srli_epi64(data.sse_m128i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_srli_epi32(data.sse_m128i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_srli_epi16(data.sse_m128i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_SSE2
					__m128i oddMask = _mm_set1_epi16(i16(0x00FF));
					__m128i evenMask = _mm_set1_epi16(i16(0xFF00));

					__m128i odd = _mm_and_si128(data.sse_m128i, oddMask);
					odd = _mm_srli_epi16(odd, Count);

					__m128i even = _mm_srli_epi16(data.sse_m128i, Count);
					even = _mm_and_si128(even, evenMask);

					pack.data.sse_m128i = _mm_or_si128(even, odd);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_srli_epi64(data.sse_m256i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_srli_epi32(data.sse_m256i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_srli_epi16(data.sse_m256i, Count);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_AVX2
					__m256i oddMask = _mm256_set1_epi16(i16(0x00FF));
					__m256i evenMask = _mm256_set1_epi16(i16(0xFF00));

					__m256i odd = _mm256_and_si256(data.sse_m256i, oddMask);
					odd = _mm256_srli_epi16(odd, Count);

					__m256i even = _mm256_srli_epi16(data.sse_m256i, Count);
					even = _mm256_and_si256(even, evenMask);

					pack.data.sse_m256i = _mm256_or_si256(even, odd);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.bits[i] = data.bits[i] >> Count;
		return pack;
	}
}
