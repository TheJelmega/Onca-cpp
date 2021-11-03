#pragma once

#if __RESHARPER__
#include "Pack.h"
#endif

namespace Core::Intrin
{
#if HAS_AVX
	constexpr u8 AvxCmpImm8Mapping[] =
	{
		_CMP_EQ_OQ,
		_CMP_GE_OQ,
		_CMP_GT_OQ,
		_CMP_LE_OQ,
		_CMP_LT_OQ,
		_CMP_NEQ_OQ,
		_CMP_ORD_Q,
		_CMP_UNORD_Q,
	};
#endif

	template <SimdBaseType T, usize Width>
	template <ComparisonOp Op>
	constexpr auto Pack<T, Width>::Compare(const Pack& other) const noexcept -> Pack
	{
		STATIC_ASSERT(Op <= ComparisonOp::Unord, "Invalid ComparisonOp");
		STATIC_ASSERT(!Integral<T> || Op <= ComparisonOp::NEq, "Invalid integer ComparisonOp, cannot be Ord or Unord");

		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m128d = _mm_cmp_pd(data.sse_m128d, other.data.sse_m128d, AvxCmpImm8Mapping[u8(Op)]);
					return pack;
#endif

					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_SSE2
						pack.data.sse_m128d = _mm_cmpeq_pd(data.sse_m128d, other.data.sse_m128d);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_SSE2
						pack.data.sse_m128d = _mm_cmpge_pd(data.sse_m128d, other.data.sse_m128d);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_SSE2
						pack.data.sse_m128d = _mm_cmpgt_pd(data.sse_m128d, other.data.sse_m128d);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_SSE2
						pack.data.sse_m128d = _mm_cmple_pd(data.sse_m128d, other.data.sse_m128d);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_SSE2
						pack.data.sse_m128d = _mm_cmplt_pd(data.sse_m128d, other.data.sse_m128d);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_SSE2
						pack.data.sse_m128d = _mm_cmpneq_pd(data.sse_m128d, other.data.sse_m128d);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ord)
					{
#if HAS_SSE2
						pack.data.sse_m128d = _mm_cmpord_pd(data.sse_m128d, other.data.sse_m128d);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Unord)
					{
#if HAS_SSE2
						pack.data.sse_m128d = _mm_cmpunord_pd(data.sse_m128d, other.data.sse_m128d);
						return pack;
#endif
					}
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m128 = _mm_cmp_ps(data.sse_m128, other.data.sse_m128, AvxCmpImm8Mapping[u8(Op)]);
					return pack;
#endif

					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_SSE
						pack.data.sse_m128 = _mm_cmpeq_ps(data.sse_m128, other.data.sse_m128);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_SSE
						pack.data.sse_m128 = _mm_cmpge_ps(data.sse_m128, other.data.sse_m128);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_SSE
						pack.data.sse_m128 = _mm_cmpgt_ps(data.sse_m128, other.data.sse_m128);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_SSE
						pack.data.sse_m128 = _mm_cmple_ps(data.sse_m128, other.data.sse_m128);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_SSE
						pack.data.sse_m128 = _mm_cmplt_ps(data.sse_m128, other.data.sse_m128);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_SSE
						pack.data.sse_m128 = _mm_cmpneq_ps(data.sse_m128, other.data.sse_m128);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ord)
					{
#if HAS_SSE
						pack.data.sse_m128 = _mm_cmpord_ps(data.sse_m128, other.data.sse_m128);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Unord)
					{
#if HAS_SSE
						pack.data.sse_m128 = _mm_cmpunord_ps(data.sse_m128, other.data.sse_m128);
						return pack;
#endif
					}
				}
				else if constexpr (IsI64<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_SSE4_1
						pack.data.sse_m128i = _mm_cmpeq_epi64(data.sse_m128i, other.data.sse_m128i);
						return pack;
#elif HAS_SSE2
						alignas(sizeof(Pack)) i64 vals[2];
						alignas(sizeof(Pack)) i64 otherVals[2];
						_mm_store_si128(reinterpret_cast<__m128i*>(vals), data.sse_m128i);
						_mm_store_si128(reinterpret_cast<__m128i*>(otherVals), other.data.sse_m128i);

						for (usize i = 0; i < Width; ++i)
							vals[i] = vals[i] == otherVals[i] ? ~i64(0) : 0;

						pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(vals));
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_SSE4_2
						__m128i eq = _mm_cmpeq_epi64(data.sse_m128i, other.data.sse_m128i);
						__m128i gt = _mm_cmpgt_epi64(data.sse_m128i, other.data.sse_m128i);
						pack.data.sse_m128i = _mm_or_si128(eq, gt);
						return pack;
#elif HAS_SSE2
						alignas(sizeof(Pack)) i64 vals[2];
						alignas(sizeof(Pack)) i64 otherVals[2];
						_mm_store_si128(reinterpret_cast<__m128i*>(vals), data.sse_m128i);
						_mm_store_si128(reinterpret_cast<__m128i*>(otherVals), other.data.sse_m128i);

						for (usize i = 0; i < Width; ++i)
							vals[i] = vals[i] >= otherVals[i] ? ~u64(0) : 0;

						pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(vals));
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_SSE4_2
						pack.data.sse_m128i = _mm_cmpgt_epi64(data.sse_m128i, other.data.sse_m128i);
						return pack;
#elif HAS_SSE2
						alignas(sizeof(Pack)) i64 vals[2];
						alignas(sizeof(Pack)) i64 otherVals[2];
						_mm_store_si128(reinterpret_cast<__m128i*>(vals), data.sse_m128i);
						_mm_store_si128(reinterpret_cast<__m128i*>(otherVals), other.data.sse_m128i);

						for (usize i = 0; i < Width; ++i)
							vals[i] = vals[i] > otherVals[i] ? ~i64(0) : 0;

						pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(vals));
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_SSE4_2
						__m128i lt = _mm_cmpgt_epi64(other.data.sse_m128i, data.sse_m128i);
						__m128i eq = _mm_cmpeq_epi64(data.sse_m128i, other.data.sse_m128i);
						pack.data.sse_m128i = _mm_or_si128(lt, eq);
						return pack;
#elif HAS_SSE2
						alignas(sizeof(Pack)) i64 vals[2];
						alignas(sizeof(Pack)) i64 otherVals[2];
						_mm_store_si128(reinterpret_cast<__m128i*>(vals), data.sse_m128i);
						_mm_store_si128(reinterpret_cast<__m128i*>(otherVals), other.data.sse_m128i);

						for (usize i = 0; i < Width; ++i)
							vals[i] = vals[i] <= otherVals[i] ? ~i64(0) : 0;

						pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(vals));
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_SSE4_2
						pack.data.sse_m128i = _mm_cmpgt_epi64(other.data.sse_m128i, data.sse_m128i);
						return pack;
#elif HAS_SSE2
						alignas(sizeof(Pack)) i64 vals[2];
						alignas(sizeof(Pack)) i64 otherVals[2];
						_mm_store_si128(reinterpret_cast<__m128i*>(vals), data.sse_m128i);
						_mm_store_si128(reinterpret_cast<__m128i*>(otherVals), other.data.sse_m128i);

						for (usize i = 0; i < Width; ++i)
							vals[i] = vals[i] < otherVals[i] ? ~i64(0) : 0;

						pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(vals));
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_SSE4_1
						__m128i eq = _mm_cmpeq_epi64(data.sse_m128i, other.data.sse_m128i);
						__m128i ones = _mm_cmpeq_epi64(data.sse_m128i, data.sse_m128i);
						pack.data.sse_m128i = _mm_xor_si128(eq, ones);
						return pack;
#elif HAS_SSE2
						alignas(sizeof(Pack)) i64 vals[2];
						alignas(sizeof(Pack)) i64 otherVals[2];
						_mm_store_si128(reinterpret_cast<__m128i*>(vals), data.sse_m128i);
						_mm_store_si128(reinterpret_cast<__m128i*>(otherVals), other.data.sse_m128i);

						for (usize i = 0; i < Width; ++i)
							vals[i] = vals[i] != otherVals[i] ? ~i64(0) : 0;

						pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(vals));
						return pack;
#endif
					}
				}
				else if constexpr (IsU64<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_SSE4_1
						pack.data.sse_m128i = _mm_cmpeq_epi64(data.sse_m128i, other.data.sse_m128i);
						return pack;
#elif HAS_SSE2
						alignas(sizeof(Pack)) u64 vals[2];
						alignas(sizeof(Pack)) u64 otherVals[2];
						_mm_store_si128(reinterpret_cast<__m128i*>(vals), data.sse_m128i);
						_mm_store_si128(reinterpret_cast<__m128i*>(otherVals), other.data.sse_m128i);

						for (usize i = 0; i < Width; ++i)
							vals[i] = vals[i] == otherVals[i] ? ~i64(0) : 0;

						pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(vals));
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_SSE4_2
						__m128i offset = _mm_set1_epi64x(0x8000'0000'0000'0000);
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						__m128i eq = _mm_cmpeq_epi64(compA, compB);
						__m128i gt = _mm_cmpgt_epi64(compA, compB);
						pack.data.sse_m128i = _mm_or_si128(eq, gt);
						return pack;
#elif HAS_SSE2
						alignas(sizeof(Pack)) u64 vals[2];
						alignas(sizeof(Pack)) u64 otherVals[2];
						_mm_store_si128(reinterpret_cast<__m128i*>(vals), data.sse_m128i);
						_mm_store_si128(reinterpret_cast<__m128i*>(otherVals), other.data.sse_m128i);

						for (usize i = 0; i < Width; ++i)
							vals[i] = vals[i] >= otherVals[i] ? ~i64(0) : 0;

						pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(vals));
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_SSE4_2
						__m128i offset = _mm_set1_epi64x(0x8000'0000'0000'0000);
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						pack.data.sse_m128i = _mm_cmpgt_epi64(compA, compB);
						return pack;
#elif HAS_SSE2
						alignas(sizeof(Pack)) u64 vals[2];
						alignas(sizeof(Pack)) u64 otherVals[2];
						_mm_store_si128(reinterpret_cast<__m128i*>(vals), data.sse_m128i);
						_mm_store_si128(reinterpret_cast<__m128i*>(otherVals), other.data.sse_m128i);

						for (usize i = 0; i < Width; ++i)
							vals[i] = vals[i] > otherVals[i] ? ~i64(0) : 0;

						pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(vals));
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_SSE4_2
						__m128i offset = _mm_set1_epi64x(0x8000'0000'0000'0000);
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						__m128i lt = _mm_cmpgt_epi64(compB, compA);
						__m128i eq = _mm_cmpeq_epi64(compB, compA);
						pack.data.sse_m128i = _mm_or_si128(lt, eq);
						return pack;
#elif HAS_SSE2
						alignas(sizeof(Pack)) u64 vals[2];
						alignas(sizeof(Pack)) u64 otherVals[2];
						_mm_store_si128(reinterpret_cast<__m128i*>(vals), data.sse_m128i);
						_mm_store_si128(reinterpret_cast<__m128i*>(otherVals), other.data.sse_m128i);

						for (usize i = 0; i < Width; ++i)
							vals[i] = vals[i] <= otherVals[i] ? ~i64(0) : 0;

						pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(vals));
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_SSE4_2
						__m128i offset = _mm_set1_epi64x(0x8000'0000'0000'0000);
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						pack.data.sse_m128i = _mm_cmpgt_epi64(compB, compA);
						return pack;
#elif HAS_SSE2
						alignas(sizeof(Pack)) u64 vals[2];
						alignas(sizeof(Pack)) u64 otherVals[2];
						_mm_store_si128(reinterpret_cast<__m128i*>(vals), data.sse_m128i);
						_mm_store_si128(reinterpret_cast<__m128i*>(otherVals), other.data.sse_m128i);

						for (usize i = 0; i < Width; ++i)
							vals[i] = vals[i] < otherVals[i] ? ~i64(0) : 0;

						pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(vals));
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_SSE4_1
						__m128i eq = _mm_cmpeq_epi64(data.sse_m128i, other.data.sse_m128i);
						__m128i ones = _mm_cmpeq_epi64(data.sse_m128i, data.sse_m128i);
						pack.data.sse_m128i = _mm_xor_si128(eq, ones);
						return pack;
#elif HAS_SSE2
						alignas(sizeof(Pack)) u64 vals[2];
						alignas(sizeof(Pack)) u64 otherVals[2];
						_mm_store_si128(reinterpret_cast<__m128i*>(vals), data.sse_m128i);
						_mm_store_si128(reinterpret_cast<__m128i*>(otherVals), other.data.sse_m128i);

						for (usize i = 0; i < Width; ++i)
							vals[i] = vals[i] != otherVals[i] ? ~i64(0) : 0;

						pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(vals));
						return pack;
#endif
					}
				}
				else if constexpr (IsI32<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_SSE2
						pack.data.sse_m128i = _mm_cmpeq_epi32(data.sse_m128i, other.data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_SSE2
						__m128i eq = _mm_cmpeq_epi32(data.sse_m128i, other.data.sse_m128i);
						__m128i gt = _mm_cmpgt_epi32(data.sse_m128i, other.data.sse_m128i);
						pack.data.sse_m128i = _mm_or_si128(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_SSE2
						pack.data.sse_m128i = _mm_cmpgt_epi32(data.sse_m128i, other.data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_SSE2
						__m128i lt = _mm_cmpgt_epi32(other.data.sse_m128i, data.sse_m128i);
						__m128i eq = _mm_cmpeq_epi32(data.sse_m128i, other.data.sse_m128i);
						pack.data.sse_m128i = _mm_or_si128(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_SSE2
						pack.data.sse_m128i = _mm_cmpgt_epi32(other.data.sse_m128i, data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_SSE2
						__m128i eq = _mm_cmpeq_epi32(data.sse_m128i, other.data.sse_m128i);
						__m128i ones = _mm_cmpeq_epi32(data.sse_m128i, data.sse_m128i);
						pack.data.sse_m128i = _mm_xor_si128(eq, ones);
						return pack;
#endif
					}
				}
				else if constexpr (IsU16<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_SSE2
						pack.data.sse_m128i = _mm_cmpeq_epi16(data.sse_m128i, other.data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi16(i16(0x8000));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						__m128i eq = _mm_cmpeq_epi16(compA, compB);
						__m128i gt = _mm_cmpgt_epi16(compA, compB);
						pack.data.sse_m128i = _mm_or_si128(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi16(i16(0x8000));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						pack.data.sse_m128i = _mm_cmpgt_epi16(compA, compB);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi16(i16(0x8000));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						__m128i lt = _mm_cmplt_epi16(compA, compB);
						__m128i eq = _mm_cmpeq_epi16(compA, compB);
						pack.data.sse_m128i = _mm_or_si128(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi16(i16(0x8000));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						pack.data.sse_m128i = _mm_cmplt_epi16(compA, compB);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_SSE2
						__m128i eq = _mm_cmpeq_epi16(data.sse_m128i, other.data.sse_m128i);
						__m128i ones = _mm_cmpeq_epi16(data.sse_m128i, data.sse_m128i);
						pack.data.sse_m128i = _mm_xor_si128(eq, ones);
						return pack;
#endif
					}
				}
				else if constexpr (IsI16<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_SSE2
						pack.data.sse_m128i = _mm_cmpeq_epi16(data.sse_m128i, other.data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_SSE2
						__m128i eq = _mm_cmpeq_epi16(data.sse_m128i, other.data.sse_m128i);
						__m128i gt = _mm_cmpgt_epi16(data.sse_m128i, other.data.sse_m128i);
						pack.data.sse_m128i = _mm_or_si128(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_SSE2
						pack.data.sse_m128i = _mm_cmpgt_epi16(data.sse_m128i, other.data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_SSE2
						__m128i lt = _mm_cmpgt_epi16(other.data.sse_m128i, data.sse_m128i);
						__m128i eq = _mm_cmpeq_epi16(data.sse_m128i, other.data.sse_m128i);
						pack.data.sse_m128i = _mm_or_si128(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_SSE2
						pack.data.sse_m128i = _mm_cmpgt_epi16(other.data.sse_m128i, data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_SSE2
						__m128i eq = _mm_cmpeq_epi16(data.sse_m128i, other.data.sse_m128i);
						__m128i ones = _mm_cmpeq_epi16(data.sse_m128i, data.sse_m128i);
						pack.data.sse_m128i = _mm_xor_si128(eq, ones);
						return pack;
#endif
					}
				}
				else if constexpr (IsU16<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_SSE2
						pack.data.sse_m128i = _mm_cmpeq_epi16(data.sse_m128i, other.data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi16(i16(0x8000));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						__m128i eq = _mm_cmpeq_epi16(compA, compB);
						__m128i gt = _mm_cmpgt_epi16(compA, compB);
						pack.data.sse_m128i = _mm_or_si128(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi16(i16(0x8000));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						pack.data.sse_m128i = _mm_cmpgt_epi16(compA, compB);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi16(i16(0x8000));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						__m128i lt = _mm_cmplt_epi16(compA, compB);
						__m128i eq = _mm_cmpeq_epi16(compA, compB);
						pack.data.sse_m128i = _mm_or_si128(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi16(i16(0x8000));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						pack.data.sse_m128i = _mm_cmpeq_epi16(compA, compB);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_SSE2
						__m128i eq = _mm_cmpeq_epi16(data.sse_m128i, other.data.sse_m128i);
						__m128i ones = _mm_cmpeq_epi16(data.sse_m128i, data.sse_m128i);
						pack.data.sse_m128i = _mm_xor_si128(eq, ones);
						return pack;
#endif
					}
				}
				else if constexpr (IsU8<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_SSE2
						pack.data.sse_m128i = _mm_cmpeq_epi8(data.sse_m128i, other.data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi8(i8(0x80));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						__m128i eq = _mm_cmpeq_epi8(compA, compB);
						__m128i gt = _mm_cmpgt_epi8(compA, compB);
						pack.data.sse_m128i = _mm_or_si128(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi8(i8(0x80));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						pack.data.sse_m128i = _mm_cmpgt_epi8(compA, compB);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi8(i8(0x80));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						__m128i lt = _mm_cmplt_epi8(compA, compB);
						__m128i eq = _mm_cmpeq_epi8(compA, compB);
						pack.data.sse_m128i = _mm_or_si128(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi8(i8(0x80));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						pack.data.sse_m128i = _mm_cmplt_epi8(compA, compB);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_SSE2
						__m128i eq = _mm_cmpeq_epi8(data.sse_m128i, other.data.sse_m128i);
						__m128i ones = _mm_cmpeq_epi8(data.sse_m128i, data.sse_m128i);
						pack.data.sse_m128i = _mm_xor_si128(eq, ones);
						return pack;
#endif
					}
				}
				else if constexpr (IsI8<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_SSE2
						pack.data.sse_m128i = _mm_cmpeq_epi8(data.sse_m128i, other.data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_SSE2
						__m128i eq = _mm_cmpeq_epi8(data.sse_m128i, other.data.sse_m128i);
						__m128i gt = _mm_cmpgt_epi8(data.sse_m128i, other.data.sse_m128i);
						pack.data.sse_m128i = _mm_or_si128(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_SSE2
						pack.data.sse_m128i = _mm_cmpgt_epi8(data.sse_m128i, other.data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_SSE2
						__m128i lt = _mm_cmpgt_epi8(other.data.sse_m128i, data.sse_m128i);
						__m128i eq = _mm_cmpeq_epi8(data.sse_m128i, other.data.sse_m128i);
						pack.data.sse_m128i = _mm_or_si128(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_SSE2
						pack.data.sse_m128i = _mm_cmpgt_epi8(other.data.sse_m128i, data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_SSE2
						__m128i eq = _mm_cmpeq_epi8(data.sse_m128i, other.data.sse_m128i);
						__m128i ones = _mm_cmpeq_epi8(data.sse_m128i, data.sse_m128i);
						pack.data.sse_m128i = _mm_xor_si128(eq, ones);
						return pack;
#endif
					}
				}
				else if constexpr (IsU8<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_SSE2
						pack.data.sse_m128i = _mm_cmpeq_epi8(data.sse_m128i, other.data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi8(i8(0x80));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						__m128i eq = _mm_cmpeq_epi8(compA, compB);
						__m128i gt = _mm_cmpgt_epi8(compA, compB);
						pack.data.sse_m128i = _mm_or_si128(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi8(i8(0x80));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						pack.data.sse_m128i = _mm_cmpgt_epi8(compA, compB);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi8(i8(0x80));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						__m128i lt = _mm_cmplt_epi8(compA, compB);
						__m128i eq = _mm_cmpeq_epi8(compA, compB);
						pack.data.sse_m128i = _mm_or_si128(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_SSE2
						__m128i offset = _mm_set1_epi8(i8(0x80));
						__m128i compA = _mm_xor_si128(data.sse_m128i, offset);
						__m128i compB = _mm_xor_si128(other.data.sse_m128i, offset);
						pack.data.sse_m128i = _mm_cmplt_epi8(compA, compB);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_SSE2
						__m128i eq = _mm_cmpeq_epi8(data.sse_m128i, other.data.sse_m128i);
						__m128i ones = _mm_cmpeq_epi8(data.sse_m128i, data.sse_m128i);
						pack.data.sse_m128i = _mm_xor_si128(eq, ones);
						return pack;
#endif
					}
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
#if !HAS_AVX2 && HAS_SSE
				if constexpr (!HAS_AVX || Integral<T>)
				{
					pack.data.m128[0] = Pack<T, Width / 2>{ data.m128[0] }.Compare<Op>(Pack<T, Width / 2>{ other.data.m128[0] }).data;
					pack.data.m128[1] = Pack<T, Width / 2>{ data.m128[1] }.Compare<Op>(Pack<T, Width / 2>{ other.data.m128[1] }).data;
					return pack;
				}
#endif

				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_cmp_pd(data.sse_m256d, other.data.sse_m256d, AvxCmpImm8Mapping[u8(Op)]);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_cmp_ps(data.sse_m256, other.data.sse_m256, AvxCmpImm8Mapping[u8(Op)]);
					return pack;
#endif
				}
				else if constexpr (IsI64<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpeq_epi64(data.sse_m256i, other.data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_AVX2
						__m256i eq = _mm256_cmpeq_epi64(data.sse_m256i, other.data.sse_m256i);
						__m256i gt = _mm256_cmpgt_epi64(data.sse_m256i, other.data.sse_m256i);
						pack.data.sse_m256i = _mm256_or_si256(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpgt_epi64(data.sse_m256i, other.data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_AVX2
						__m256i lt = _mm256_cmpgt_epi64(other.data.sse_m256i, data.sse_m256i);
						__m256i eq = _mm256_cmpeq_epi64(data.sse_m256i, other.data.sse_m256i);
						pack.data.sse_m256i = _mm256_or_si256(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpgt_epi64(other.data.sse_m256i, data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_AVX2
						__m256i eq = _mm256_cmpeq_epi64(data.sse_m256i, other.data.sse_m256i);
						__m256i ones = _mm256_cmpeq_epi64(data.sse_m256i, data.sse_m256i);
						pack.data.sse_m256i = _mm256_xor_si256(eq, ones);
						return pack;
#endif
					}
				}
				else if constexpr (IsU64<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpeq_epi64(data.sse_m256i, other.data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi64x(0x8000'0000'0000'0000);
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						__m256i eq = _mm256_cmpeq_epi64(compA, compB);
						__m256i gt = _mm256_cmpgt_epi64(compA, compB);
						pack.data.sse_m256i = _mm256_or_si256(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi64x(0x8000'0000'0000'0000);
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						pack.data.sse_m256i = _mm256_cmpgt_epi64(compA, compB);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi64x(0x8000'0000'0000'0000);
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						__m256i lt = _mm256_cmpgt_epi64(compB, compA);
						__m256i eq = _mm256_cmpeq_epi64(compB, compA);
						pack.data.sse_m256i = _mm256_or_si256(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi64x(0x8000'0000'0000'0000);
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						pack.data.sse_m256i = _mm256_cmpgt_epi64(compB, compA);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_AVX2
						__m256i eq = _mm256_cmpeq_epi64(data.sse_m256i, other.data.sse_m256i);
						__m256i ones = _mm256_cmpeq_epi64(data.sse_m256i, data.sse_m256i);
						pack.data.sse_m256i = _mm256_xor_si256(eq, ones);
						return pack;
#endif
					}
				}
				else if constexpr (IsI32<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpeq_epi32(data.sse_m256i, other.data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_AVX2
						__m256i eq = _mm256_cmpeq_epi32(data.sse_m256i, other.data.sse_m256i);
						__m256i gt = _mm256_cmpgt_epi32(data.sse_m256i, other.data.sse_m256i);
						pack.data.sse_m256i = _mm256_or_si256(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpgt_epi32(data.sse_m256i, other.data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_AVX2
						__m256i lt = _mm256_cmpgt_epi32(other.data.sse_m256i, data.sse_m256i);
						__m256i eq = _mm256_cmpeq_epi32(data.sse_m256i, other.data.sse_m256i);
						pack.data.sse_m256i = _mm256_or_si256(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpgt_epi32(other.data.sse_m256i, data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_AVX2
						__m256i eq = _mm256_cmpeq_epi32(data.sse_m256i, other.data.sse_m256i);
						__m256i ones = _mm256_cmpeq_epi32(data.sse_m256i, data.sse_m256i);
						pack.data.sse_m256i = _mm256_xor_si256(eq, ones);
						return pack;
#endif
					}
				}
				else if constexpr (IsU16<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpeq_epi16(data.sse_m256i, other.data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi16(i16(0x8000));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						__m256i eq = _mm256_cmpeq_epi16(compA, compB);
						__m256i gt = _mm256_cmpgt_epi16(compA, compB);
						pack.data.sse_m256i = _mm256_or_si256(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi16(i16(0x8000));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						pack.data.sse_m256i = _mm256_cmpgt_epi16(compA, compB);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi16(i16(0x8000));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						__m256i lt = _mm256_cmpgt_epi16(compB, compA);
						__m256i eq = _mm256_cmpeq_epi16(compB, compA);
						pack.data.sse_m256i = _mm256_or_si256(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi16(i16(0x8000));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						pack.data.sse_m256i = _mm256_cmpgt_epi16(compB, compA);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_AVX2
						__m256i eq = _mm256_cmpeq_epi16(data.sse_m256i, other.data.sse_m256i);
						__m256i ones = _mm256_cmpeq_epi16(data.sse_m256i, data.sse_m256i);
						pack.data.sse_m256i = _mm256_xor_si256(eq, ones);
						return pack;
#endif
					}
				}
				else if constexpr (IsI16<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpeq_epi16(data.sse_m256i, other.data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_AVX2
						__m256i eq = _mm256_cmpeq_epi16(data.sse_m256i, other.data.sse_m256i);
						__m256i gt = _mm256_cmpgt_epi16(data.sse_m256i, other.data.sse_m256i);
						pack.data.sse_m256i = _mm256_or_si256(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpgt_epi16(data.sse_m256i, other.data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_AVX2
						__m256i lt = _mm256_cmpgt_epi16(other.data.sse_m256i, data.sse_m256i);
						__m256i eq = _mm256_cmpeq_epi16(data.sse_m256i, other.data.sse_m256i);
						pack.data.sse_m256i = _mm256_or_si256(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpgt_epi16(other.data.sse_m256i, data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_AVX2
						__m256i eq = _mm256_cmpeq_epi16(data.sse_m256i, other.data.sse_m256i);
						__m256i ones = _mm256_cmpeq_epi16(data.sse_m256i, data.sse_m256i);
						pack.data.sse_m256i = _mm256_xor_si256(eq, ones);
						return pack;
#endif
					}
				}
				else if constexpr (IsU16<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpeq_epi16(data.sse_m256i, other.data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi8(i8(0x8000));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						__m256i eq = _mm256_cmpeq_epi16(compA, compB);
						__m256i gt = _mm256_cmpgt_epi16(compA, compB);
						pack.data.sse_m256i = _mm256_or_si256(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi8(i8(0x8000));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						pack.data.sse_m256i = _mm256_cmpgt_epi16(compA, compB);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi8(i8(0x8000));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						__m256i lt = _mm256_cmpgt_epi16(compB, compA);
						__m256i eq = _mm256_cmpeq_epi16(compB, compA);
						pack.data.sse_m256i = _mm256_or_si256(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi8(i8(0x8000));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						pack.data.sse_m256i = _mm256_cmpgt_epi16(compB, compA);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_AVX2
						__m256i eq = _mm256_cmpeq_epi16(data.sse_m256i, other.data.sse_m256i);
						__m256i ones = _mm256_cmpeq_epi16(data.sse_m256i, data.sse_m256i);
						pack.data.sse_m256i = _mm256_xor_si256(eq, ones);
						return pack;
#endif
					}
				}
				else if constexpr (IsU8<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpeq_epi8(data.sse_m256i, other.data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi8(i8(0x80));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						__m256i eq = _mm256_cmpeq_epi8(compA, compB);
						__m256i gt = _mm256_cmpgt_epi8(compA, compB);
						pack.data.sse_m256i = _mm256_or_si256(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi8(i8(0x80));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						pack.data.sse_m256i = _mm256_cmpgt_epi8(compA, compB);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi8(i8(0x80));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						__m256i lt = _mm256_cmpgt_epi8(compB, compA);
						__m256i eq = _mm256_cmpeq_epi8(compB, compA);
						pack.data.sse_m256i = _mm256_or_si256(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi8(i8(0x80));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						pack.data.sse_m256i = _mm256_cmpgt_epi8(compB, compA);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_AVX2
						__m256i eq = _mm256_cmpeq_epi8(data.sse_m256i, other.data.sse_m256i);
						__m256i ones = _mm256_cmpeq_epi8(data.sse_m256i, data.sse_m256i);
						pack.data.sse_m256i = _mm256_xor_si256(eq, ones);
						return pack;
#endif
					}
				}
				else if constexpr (IsI8<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpeq_epi8(data.sse_m256i, other.data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_AVX2
						__m256i eq = _mm256_cmpeq_epi8(data.sse_m256i, other.data.sse_m256i);
						__m256i gt = _mm256_cmpgt_epi8(data.sse_m256i, other.data.sse_m256i);
						pack.data.sse_m256i = _mm256_or_si256(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpgt_epi8(data.sse_m256i, other.data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_AVX2
						__m256i lt = _mm256_cmpgt_epi8(other.data.sse_m256i, data.sse_m256i);
						__m256i eq = _mm256_cmpeq_epi8(data.sse_m256i, other.data.sse_m256i);
						pack.data.sse_m256i = _mm256_or_si256(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpgt_epi8(other.data.sse_m256i, data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_AVX2
						__m256i eq = _mm256_cmpeq_epi8(data.sse_m256i, other.data.sse_m256i);
						__m256i ones = _mm256_cmpeq_epi8(data.sse_m256i, data.sse_m256i);
						pack.data.sse_m256i = _mm256_xor_si256(eq, ones);
						return pack;
#endif
					}
				}
				else if constexpr (IsU8<T>)
				{
					if constexpr (Op == ComparisonOp::Eq)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_cmpeq_epi8(data.sse_m256i, other.data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Ge)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi8(i8(0x80));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						__m256i eq = _mm256_cmpeq_epi8(compA, compB);
						__m256i gt = _mm256_cmpgt_epi8(compA, compB);
						pack.data.sse_m256i = _mm256_or_si256(eq, gt);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Gt)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi8(i8(0x80));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						pack.data.sse_m256i = _mm256_cmpgt_epi8(compA, compB);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Le)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi8(i8(0x80));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						__m256i lt = _mm256_cmpgt_epi8(compB, compA);
						__m256i eq = _mm256_cmpeq_epi8(compB, compA);
						pack.data.sse_m256i = _mm256_or_si256(lt, eq);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::Lt)
					{
#if HAS_AVX2
						__m256i offset = _mm256_set1_epi8(i8(0x80));
						__m256i compA = _mm256_xor_si256(data.sse_m256i, offset);
						__m256i compB = _mm256_xor_si256(other.data.sse_m256i, offset);
						pack.data.sse_m256i = _mm256_cmpeq_epi8(compA, compB);
						return pack;
#endif
					}
					else if constexpr (Op == ComparisonOp::NEq)
					{
#if HAS_AVX2
						__m256i eq = _mm256_cmpeq_epi8(data.sse_m256i, other.data.sse_m256i);
						__m256i ones = _mm256_cmpeq_epi8(data.sse_m256i, data.sse_m256i);
						pack.data.sse_m256i = _mm256_xor_si256(eq, ones);
						return pack;
#endif
					}
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
		{
			using Unsigned = typename Detail::PackData<T, DataSize>::Unsigned;
			Unsigned mask = ~Unsigned(0);

			if constexpr (Op == ComparisonOp::Eq)
				pack.data.bits[i] = data.raw[i] == other.data.raw[i] ? mask : 0;
			else if constexpr (Op == ComparisonOp::Ge)
				pack.data.bits[i] = data.raw[i] >= other.data.raw[i] ? mask : 0;
			else if constexpr (Op == ComparisonOp::Gt)
				pack.data.bits[i] = data.raw[i] >  other.data.raw[i] ? mask : 0;
			else if constexpr (Op == ComparisonOp::Le)
				pack.data.bits[i] = data.raw[i] <= other.data.raw[i] ? mask : 0;
			else if constexpr (Op == ComparisonOp::Lt)
				pack.data.bits[i] = data.raw[i] <  other.data.raw[i] ? mask : 0;
			else if constexpr (Op == ComparisonOp::NEq)
				pack.data.bits[i] = data.raw[i] != other.data.raw[i] ? mask : 0;
			else if constexpr (FloatingPoint<T>)
			{
				if constexpr (Op == ComparisonOp::Ord)
					pack.data.bits[i] = !Math::IsNaN(data.raw[i]) && !Math::IsNaN(other.data.raw[i]) ? mask : 0;
				else if constexpr (Op == ComparisonOp::Unord)
					pack.data.bits[i] = Math::IsNaN(data.raw[i]) || Math::IsNaN(other.data.raw[i]) ? mask : 0;
			}
		}
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Min(const Pack& other) const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_min_pd(data.sse_m128d, other.data.sse_m128d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_min_ps(data.sse_m128, other.data.sse_m128);
					return pack;
#endif
				}
				else if constexpr (IsU64<T>)
				{
#if HAS_AVX512F && HAS_AVX512VL
					pack.data.sse_m128i = _mm_min_epu64(data.sse_m128i, other.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Lt>(other);
#if HAS_SSE4_1
					pack.data.sse_m128i = _mm_blendv_epi8(other.data.sse_m128i, data.sse_m128i, mask.data.sse_m128i);
#elif HAS_SSE2
					__m128i val0 = _mm_and_si128(mask.data.sse_m128i, data.sse_m128i);
					__m128i val1 = _mm_andnot_si128(mask.data.sse_m128i, other.data.sse_m128i);
					pack.data.sse_m128i = _mm_or_si128(val0, val1);
#endif
					return pack;
#endif
				}
				else if constexpr (IsU32<T>)
				{
#if HAS_SSE4_1
					pack.data.sse_m128i = _mm_min_epu32(data.sse_m128i, other.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Lt>(other);
					__m128i val0 = _mm_and_si128(mask.data.sse_m128i, data.sse_m128i);
					__m128i val1 = _mm_andnot_si128(mask.data.sse_m128i, other.data.sse_m128i);
					pack.data.sse_m128i = _mm_or_si128(val0, val1);
					return pack;
#endif
				}
				else if constexpr (IsU16<T>)
				{
#if HAS_SSE4_1
					pack.data.sse_m128i = _mm_min_epu16(data.sse_m128i, other.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Lt>(other);
					__m128i val0 = _mm_and_si128(mask.data.sse_m128i, data.sse_m128i);
					__m128i val1 = _mm_andnot_si128(mask.data.sse_m128i, other.data.sse_m128i);
					pack.data.sse_m128i = _mm_or_si128(val0, val1);
					return pack;
#endif
				}
				else if constexpr (IsU8<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_min_epu8(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI64<T>)
				{
#if HAS_AVX512F && HAS_AVX512VL
					pack.data.sse_m128i = _mm_min_epi64(data.sse_m128i, other.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Lt>(other);
#if HAS_SSE4_1
					pack.data.sse_m128i = _mm_blendv_epi8(other.data.sse_m128i, data.sse_m128i, mask.data.sse_m128i);
#elif HAS_SSE2
					__m128i val0 = _mm_and_si128(mask.data.sse_m128i, data.sse_m128i);
					__m128i val1 = _mm_andnot_si128(mask.data.sse_m128i, other.data.sse_m128i);
					pack.data.sse_m128i = _mm_or_si128(val0, val1);
#endif
					return pack;
#endif
				}
				else if constexpr (IsI32<T>)
				{
#if HAS_SSE4_1
					pack.data.sse_m128i = _mm_min_epi32(data.sse_m128i, other.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Lt>(other);
					__m128i val0 = _mm_and_si128(mask.data.sse_m128i, data.sse_m128i);
					__m128i val1 = _mm_andnot_si128(mask.data.sse_m128i, other.data.sse_m128i);
					pack.data.sse_m128i = _mm_or_si128(val0, val1);
					return pack;
#endif
				}
				else if constexpr (IsI16<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_min_epi16(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI8<T>)
				{
#if HAS_SSE4_1
					pack.data.sse_m128i = _mm_min_epi8(data.sse_m128i, other.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Lt>(other);
					__m128i val0 = _mm_and_si128(mask.data.sse_m128i, data.sse_m128i);
					__m128i val1 = _mm_andnot_si128(mask.data.sse_m128i, other.data.sse_m128i);
					pack.data.sse_m128i = _mm_or_si128(val0, val1);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
#if !HAS_AVX2 && HAS_SSE
				if constexpr (!HAS_AVX || Integral<T>)
				{
					pack.data.sse_m128i[0] = Pack<T, Width / 2>{ pack.data.m128[0] }.Min(Pack<T, Width / 2>{ other.data.m128[0] }).data;
					pack.data.sse_m128i[1] = Pack<T, Width / 2>{ pack.data.m128[1] }.Min(Pack<T, Width / 2>{ other.data.m128[1] }).data;
					return pack;
				}
#endif

				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_min_pd(data.sse_m256d, other.data.sse_m256d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_min_ps(data.sse_m256, other.data.sse_m256);
					return pack;
#endif
				}
				else if constexpr (IsU64<T>)
				{
#if HAS_AVX512F && HAS_AVX512VL
					pack.data.sse_m256i = _mm256_min_epu64(data.sse_m256i, other.data.sse_m256i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Lt>(other);
					pack.data.sse_m256i = _mm256_blendv_epi8(other.data.sse_m256i, data.sse_m256i, mask.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU32<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_min_epi32(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_min_epi16(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_min_epi8(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI64<T>)
				{
#if HAS_AVX512F && HAS_AVX512VL
					pack.data.sse_m256i = _mm256_min_epi64(data.sse_m256i, other.data.sse_m256i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Lt>(other);
					pack.data.sse_m256i = _mm256_blendv_epi8(other.data.sse_m256i, data.sse_m256i, mask.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI32<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_min_epu32(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_min_epu16(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_min_epu8(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.raw[i] = Math::Min(data.raw[i], other.data.raw[i]);
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Max(const Pack& other) const noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_max_pd(data.sse_m128d, other.data.sse_m128d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_max_ps(data.sse_m128, other.data.sse_m128);
					return pack;
#endif
				}
				else if constexpr (IsU64<T>)
				{
#if HAS_AVX512F && HAS_AVX512VL
					pack.data.sse_m128i = _mm_max_epu64(data.sse_m128i, other.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Gt>(other);
#if HAS_SSE4_1
					pack.data.sse_m128i = _mm_blendv_epi8(other.data.sse_m128i, data.sse_m128i, mask.data.sse_m128i);
#elif HAS_SSE2
					__m128i val0 = _mm_and_si128(mask.data.sse_m128i, data.sse_m128i);
					__m128i val1 = _mm_andnot_si128(mask.data.sse_m128i, other.data.sse_m128i);
					pack.data.sse_m128i = _mm_or_si128(val0, val1);
#endif
					return pack;
#endif
				}
				else if constexpr (IsU32<T>)
				{
#if HAS_SSE4_1
					pack.data.sse_m128i = _mm_max_epu32(data.sse_m128i, other.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Gt>(other);
					__m128i val0 = _mm_and_si128(mask.data.sse_m128i, data.sse_m128i);
					__m128i val1 = _mm_andnot_si128(mask.data.sse_m128i, other.data.sse_m128i);
					pack.data.sse_m128i = _mm_or_si128(val0, val1);
					return pack;
#endif
				}
				else if constexpr (IsU16<T>)
				{
#if HAS_SSE4_1
					pack.data.sse_m128i = _mm_max_epu16(data.sse_m128i, other.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Gt>(other);
					__m128i val0 = _mm_and_si128(mask.data.sse_m128i, data.sse_m128i);
					__m128i val1 = _mm_andnot_si128(mask.data.sse_m128i, other.data.sse_m128i);
					pack.data.sse_m128i = _mm_or_si128(val0, val1);
					return pack;
#endif
				}
				else if constexpr (IsU8<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_max_epu8(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI64<T>)
				{
#if HAS_AVX512F && HAS_AVX512VL
					pack.data.sse_m128i = _mm_max_epi64(data.sse_m128i, other.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Gt>(other);
#if HAS_SSE4_1
					pack.data.sse_m128i = _mm_blendv_epi8(other.data.sse_m128i, data.sse_m128i, mask.data.sse_m128i);
#elif HAS_SSE2
					__m128i val0 = _mm_and_si128(mask.data.sse_m128i, data.sse_m128i);
					__m128i val1 = _mm_andnot_si128(mask.data.sse_m128i, other.data.sse_m128i);
					pack.data.sse_m128i = _mm_or_si128(val0, val1);
#endif
					return pack;
#endif
				}
				else if constexpr (IsI32<T>)
				{
#if HAS_SSE4_1
					pack.data.sse_m128i = _mm_max_epi32(data.sse_m128i, other.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Gt>(other);
					__m128i val0 = _mm_and_si128(mask.data.sse_m128i, data.sse_m128i);
					__m128i val1 = _mm_andnot_si128(mask.data.sse_m128i, other.data.sse_m128i);
					pack.data.sse_m128i = _mm_or_si128(val0, val1);
					return pack;
#endif
				}
				else if constexpr (IsI16<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_max_epi16(data.sse_m128i, other.data.sse_m128i);
					return pack;
#endif
				}
				else if constexpr (IsI8<T>)
				{
#if HAS_SSE4_1
					pack.data.sse_m128i = _mm_max_epi8(data.sse_m128i, other.data.sse_m128i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Gt>(other);
					__m128i val0 = _mm_and_si128(mask.data.sse_m128i, data.sse_m128i);
					__m128i val1 = _mm_andnot_si128(mask.data.sse_m128i, other.data.sse_m128i);
					pack.data.sse_m128i = _mm_or_si128(val0, val1);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
#if !HAS_AVX2 && HAS_SSE
			if constexpr (!HAS_AVX || Integral<T>)
			{
				pack.data.sse_m128i[0] = Pack<T, Width / 2>{ pack.data.m128[0] }.Max(Pack<T, Width / 2>{ other.data.m128[0] }).data;
				pack.data.sse_m128i[1] = Pack<T, Width / 2>{ pack.data.m128[1] }.Max(Pack<T, Width / 2>{ other.data.m128[1] }).data;
				return pack;
			}
#endif
				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_max_pd(data.sse_m256d, other.data.sse_m256d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_max_ps(data.sse_m256, other.data.sse_m256);
					return pack;
#endif
				}
				else if constexpr (IsU64<T>)
				{
#if HAS_AVX512F && HAS_AVX512VL
					pack.data.sse_m256i = _mm256_max_epu64(data.sse_m256i, other.data.sse_m256i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Gt>(other);
					pack.data.sse_m256i = _mm256_blendv_epi8(other.data.sse_m256i, data.sse_m256i, mask.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU32<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_max_epi32(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_max_epi16(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsU8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_max_epi8(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI64<T>)
				{
#if HAS_AVX512F && HAS_AVX512VL
					pack.data.sse_m256i = _mm256_max_epi64(data.sse_m256i, other.data.sse_m256i);
					return pack;
#elif HAS_SSE2
					Pack mask = Compare<ComparisonOp::Gt>(other);
					pack.data.sse_m256i = _mm256_blendv_epi8(other.data.sse_m256i, data.sse_m256i, mask.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI32<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_max_epu32(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_max_epu16(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
				else if constexpr (IsI8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_max_epu8(data.sse_m256i, other.data.sse_m256i);
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.raw[i] = Math::Max(data.raw[i], other.data.raw[i]);
		return pack;
	}
}
