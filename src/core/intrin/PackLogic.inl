#pragma once
#if __RESHARPER__
#include "Pack.h"
#endif

namespace Core::Intrin
{
	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::And(const Pack& other) const noexcept -> Pack
	{
		Pack pack{ Detail::Uninit{} };
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
		Pack pack{ Detail::Uninit{} };

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
		Pack pack{ Detail::Uninit{} };
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
		Pack pack{ Detail::Uninit{} };
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
		Pack pack{ Detail::Uninit{} };
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
}
