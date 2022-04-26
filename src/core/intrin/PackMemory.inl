#pragma once

#if __RESHARPER__
#include "Pack.h"
#endif

namespace Onca::Intrin
{
	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Zero() noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128d = _mm_setzero_pd();
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128 = _mm_setzero_ps();
					return pack;
#endif
				}
				else
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_setzero_si128();
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = Pack<T, Width / 2>::Zero().data;
					pack.data.m128[1] = Pack<T, Width / 2>::Zero().data;
					return pack;
				}

				if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_setzero_ps();
					return pack;
#endif
				}
				else if constexpr (IsF64<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256d = _mm256_setzero_pd();
					return pack;
#endif
				}
				else
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_setzero_si256();
					return pack;
#endif
				}
			}
		}

		MemClearData(pack);
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Ones() noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128d = _mm_cmpeq_pd(pack.data.sse_m128d, pack.data.sse_m128d);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128 = _mm_cmpeq_ps(pack.data.sse_m128, pack.data.sse_m128);
					return pack;
#endif

				}
				else
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_cmpeq_epi32(pack.data.sse_m128i, pack.data.sse_m128i);
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = Pack<T, Width / 2>::Ones().data;
					pack.data.m128[1] = Pack<T, Width / 2>::Ones().data;
					return pack;
				}
				
				if constexpr (IsF64<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256d = _mm256_cmp_pd(pack.data.sse_m256d, pack.data.sse_m256d, _CMP_EQ_OQ);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_cmp_ps(pack.data.sse_m256, pack.data.sse_m256, _CMP_EQ_OQ);
					return pack;
#endif
				}
				else
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_cmpeq_epi64(pack.data.sse_m256i, pack.data.sse_m256i);
					return pack;
#endif
				}
			}
		}

		MemSet(&pack.data, 0xFF, DataSize);
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Set(T val) noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128d = _mm_set1_pd(val);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128 = _mm_set1_ps(val);
					return pack;
#endif

				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_set1_epi64x(i64(val));
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_set1_epi32(i32(val));
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_set1_epi16(i16(val));
					return pack;
#endif
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_set1_epi8(i8(val));
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = Pack<T, Width / 2>::Set(val).data;
					pack.data.m128[0] = Pack<T, Width / 2>::Set(val).data;
				}

				if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_set1_ps(val);
					return pack;
#endif
				}
				else if constexpr (IsF64<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256d = _mm256_set1_pd(val);
					return pack;
#endif
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_set1_epi64x(i64(val));
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_set1_epi32(i32(val));
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_set1_epi16(i16(val));
					return pack;
#endif
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_set1_epi8(i8(val));
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.raw[i] = val;
		return pack;
	}

	template<SimdBaseType T, usize Width, typename TupType, usize... Inds>
	auto X86Set128Helper(TupType&& args, IndexSequence<Inds...>) -> Detail::PackData<T, Width>
	{
#if HAS_SSE_SUPPORT
		if constexpr (IsF32<T>)
		{
			return { .sse_m128 = _mm_setr_ps(std::get<Inds>(args)...) };
		}
		else if constexpr (IsF64<T>)
		{
			return { .sse_m128d = _mm_setr_pd(std::get<Inds>(args)...) };
		}
		else if constexpr (IsU64<T> || IsI64<T>)
		{
			return { .sse_m128i = _mm_setr_epi64x(std::get<Inds>(args)...) };
		}
		else if constexpr (IsU32<T> || IsI32<T>)
		{
			return { .sse_m128i = _mm_setr_epi32(std::get<Inds>(args)...) };
		}
		else if constexpr (IsU16<T> || IsI16<T>)
		{
			return { .sse_m128i = _mm_setr_epi16(std::get<Inds>(args)...) };
		}
		else if constexpr (IsU8<T> || IsI8<T>)
		{
			return { .sse_m128i = _mm_setr_epi8(std::get<Inds>(args)...) };
		}
#endif

		ASSERT(false, "Invalid Set128HalfHelper() call");
		return {};
	}

	template <SimdBaseType T, usize Width>
	template <SameAs<T>... Args>
	constexpr auto Pack<T, Width>::Set(Args... vals) noexcept -> Pack
	{
		STATIC_ASSERT(sizeof...(Args) == Width, "Expected 'Width' values");

		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128d = _mm_setr_pd(vals...);
					return pack;
#endif
				}
				else if constexpr  (IsF64<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128 = _mm_setr_ps(vals...);
					return pack;
#endif
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_setr_epi64x(i64(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_setr_epi32(i32(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_setr_epi16(i16(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_setr_epi8(i8(vals)...);
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = X86Set128Helper<T, Width / 2>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<0, Width / 2>{});
					pack.data.m128[1] = X86Set128Helper<T, Width / 2>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<Width / 2, Width>{});
					return pack;
				}

				if constexpr (IsF32<T>)
				{
					if constexpr (IsF64<T>)
					{
#if HAS_AVX
						pack.data.sse_m256d = _mm256_setr_pd(vals...);
						return pack;
#endif
					}
					else if constexpr (IsF64<T>)
					{
#if HAS_AVX
						pack.data.sse_m256 = _mm256_setr_ps(vals...);
						return pack;
#endif
					}
					else if constexpr (IsU64<T> || IsI64<T>)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_setr_epi64x(i64(vals)...);
						return pack;
#endif
					}
					else if constexpr (IsU32<T> || IsI32<T>)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_setr_epi32(i32(vals)...);
						return pack;
#endif
					}
					else if constexpr (IsU16<T> || IsI16<T>)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_setr_epi16(i16(vals)...);
						return pack;
#endif
					}
					else if constexpr (IsU8<T> || IsI8<T>)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_setr_epi8(i8(vals)...);
						return pack;
#endif
					}
				}
			}
		}

		InitializerList<T> il = { vals... };
		MemCpy(&pack, il.begin(), il.size() * sizeof(T));
		return pack;
	}

	template<SimdBaseType T, usize Width, typename TupType, usize... Inds>
	auto X86SetR128Helper(TupType&& args, IndexSequence<Inds...>) -> Detail::PackData<T, Width>
	{
#if HAS_SSE_SUPPORT
		if constexpr (IsF32<T>)
		{
			return { .sse_m128 = _mm_set_ps(std::get<Inds>(args)...) };
		}
		else if constexpr (IsF64<T>)
		{
			return { .sse_m128d = _mm_set_pd(std::get<Inds>(args)...) };
		}
		else if constexpr (IsU64<T> || IsI64<T>)
		{
			return { .sse_m128i = _mm_set_epi64x(std::get<Inds>(args)...) };
		}
		else if constexpr (IsU32<T> || IsI32<T>)
		{
			return { .sse_m128i = _mm_set_epi32(std::get<Inds>(args)...) };
		}
		else if constexpr (IsU16<T> || IsI16<T>)
		{
			return { .sse_m128i = _mm_set_epi16(std::get<Inds>(args)...) };
		}
		else if constexpr (IsU8<T> || IsI8<T>)
		{
			return { .sse_m128i = _mm_set_epi8(std::get<Inds>(args)...) };
		}
#endif

		ASSERT(false, "Invalid SetR128HalfHelper() call");
		return {};
	}

	template<SimdBaseType T, usize Width, typename TupType, usize... Inds>
	constexpr void SetRDefaultHelper(Pack<T, Width>& pack, TupType&& args, IndexSequence<Inds...>)
	{
		InitializerList<T> il = { std::get<Inds>(args)... };
		MemCpy(&pack, il.begin(), il.size() * sizeof(T));
	}

	template <SimdBaseType T, usize Width>
	template <SameAs<T> ... Args>
	constexpr auto Pack<T, Width>::SetR(Args... vals) noexcept -> Pack
	{
		STATIC_ASSERT(sizeof...(Args) == Width, "Expected 'Width' values");

		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128d = _mm_set_pd(vals...);
					return pack;
#endif
				}
				else if constexpr (IsF64<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128 = _mm_set_ps(vals...);
					return pack;
#endif
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_set_epi64x(i64(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_set_epi32(i32(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_set_epi16(i16(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_set_epi8(i8(vals)...);
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = X86SetR128Helper<T, Width / 2>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<Width / 2, Width>{});
					pack.data.m128[1] = X86SetR128Helper<T, Width / 2>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<0, Width / 2>{});
					return pack;
				}

				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_set_pd(vals...);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_set_ps(vals...);
					return pack;
#endif
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_set_epi64x(i64(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_set_epi32(i32(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_set_epi16(i16(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_set_epi8(i8(vals)...);
					return pack;
#endif
				}
			}
		}

		SetRDefaultHelper(pack, Tuple<Args...>{ vals... }, MakeReverseIndexSequence<Width>{});
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Load(const T* addr) noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128d = _mm_loadu_pd(addr);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128 = _mm_loadu_ps(addr);
					return pack;
#endif
				}
				else
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_loadu_si128(reinterpret_cast<const __m128i*>(addr));
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = Pack<T, Width / 2>::Load(addr).data;
					pack.data.m128[1] = Pack<T, Width / 2>::Load(addr + Width / 2).data;
					return pack;
				}

				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_loadu_pd(addr);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_loadu_ps(addr);
					return pack;
#endif
				}
				else
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(addr));
					return pack;
#endif
				}
			}
		}
		
		MemCpy(&pack, addr, DataSize);
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::AlignedLoad(const T* addr) noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			ASSERT((usize(addr) & (DataSize - 1)) == 0, "'addr' needs to have the same alignment as Pack");
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128d = _mm_load_pd(addr);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128 = _mm_load_ps(addr);
					return pack;
#endif
				}
				else
				{
#if HAS_SSE_SUPPORT
					pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(addr));
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					pack.data.m128[0] = Pack<T, Width / 2>::AlignedLoad(addr).data;
					pack.data.m128[0] = Pack<T, Width / 2>::AlignedLoad(addr + Width / 2).data;
				}

				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_load_pd(addr);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_load_ps(addr);
					return pack;
#endif
				}
				else
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_load_si256(reinterpret_cast<const __m256i*>(addr));
					return pack;
#endif
				}
			}
		}

		MemCpy(&pack, addr, DataSize);
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr void Pack<T, Width>::Store(T* addr) const noexcept
	{
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE_SUPPORT
					_mm_storeu_pd(addr, data.sse_m128d);
					return;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE_SUPPORT
					_mm_storeu_ps(addr, data.sse_m128);
					return;
#endif
				}
				else
				{
#if HAS_SSE_SUPPORT
					_mm_storeu_si128(reinterpret_cast<__m128i*>(addr), data.sse_m128i);
					return;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					HalfPack(0).Store(addr);
					HalfPack(1).Store(addr + Width / 2);
					return;
				}

				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					_mm256_storeu_pd(addr, data.sse_m256d);
					return;
#endif

				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					_mm256_storeu_ps(addr, data.sse_m256);
					return;
#endif

				}
				else
				{
#if HAS_AVX2
					_mm256_storeu_si256(reinterpret_cast<__m256i*>(addr), data.sse_m256i);
					return;
#endif

				}
			}
		}
		MemCpy(addr, &data, DataSize);
	}

	template <SimdBaseType T, usize Width>
	constexpr void Pack<T, Width>::AlignedStore(T* addr) const noexcept
	{
		IF_NOT_CONSTEVAL
		{
			ASSERT((usize(addr) & (DataSize - 1)) == 0, "'addr' needs to have the same alignment as Pack");
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE_SUPPORT
					_mm_store_pd(addr, data.sse_m128d);
					return;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE_SUPPORT
					_mm_store_ps(addr, data.sse_m128);
					return;
#endif
				}
				else
				{
#if HAS_SSE_SUPPORT
					_mm_store_si128(reinterpret_cast<__m128i*>(addr), data.sse_m128i);
					return;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative() && !IsNative256())
				{
					HalfPack(0).AlignedStore(addr);
					HalfPack(1).AlignedStore(addr + Width / 2);
					return;
				}

				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					_mm256_store_pd(addr, data.sse_m256d);
					return;
#endif

				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					_mm256_store_ps(addr, data.sse_m256);
					return;
#endif

				}
				else
				{
#if HAS_AVX2
					_mm256_store_si256(reinterpret_cast<__m256i*>(addr), data.sse_m256i);
					return;
#endif

				}
			}
		}
		MemCpy(addr, &data, DataSize);
	}

	template <SimdBaseType T, usize Width>
	template <usize Index>
	constexpr auto Pack<T, Width>::Insert(T val) const noexcept -> Pack
	{
		STATIC_ASSERT(Index < Width, "Index out of range");

		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_SSE_SUPPORT
					i64 ival = *reinterpret_cast<i64*>(&val);
					pack.data.sse_m128i = _mm_insert_epi64(data.sse_m128i, ival, Index);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_SSE_SUPPORT
					i32 ival = *reinterpret_cast<i32*>(&val);
					pack.data.sse_m128i = _mm_insert_epi32(data.sse_m128i, ival, Index);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_SSE_SUPPORT
					i16 ival = *reinterpret_cast<i16*>(&val);
					pack.data.sse_m128i = _mm_insert_epi16(data.sse_m128i, ival, Index);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_SSE_SUPPORT
					i8 ival = *reinterpret_cast<i8*>(&val);
					pack.data.sse_m128i = _mm_insert_epi8(data.sse_m128i, ival, Index);
					return pack;
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (IsNative256() && sizeof(T) == 8)
				{
#if HAS_AVX
					i64 ival = *reinterpret_cast<i64*>(&val);
					pack.data.sse_m256i = _mm256_insert_epi64(data.sse_m256i, ival, Index);
					return pack;
#endif
				}
				else if constexpr (IsNative256() && sizeof(T) == 4)
				{
#if HAS_AVX
					i32 ival = *reinterpret_cast<i32*>(&val);
					pack.data.sse_m256i = _mm256_insert_epi32(data.sse_m256i, ival, Index);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_AVX
					i16 ival = *reinterpret_cast<i16*>(&val);
					pack.data.sse_m256i = _mm256_insert_epi16(data.sse_m256i, ival, Index);
					return pack;
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_AVX
					i8 ival = *reinterpret_cast<i8*>(&val);
					pack.data.sse_m256i = _mm256_insert_epi8(data.sse_m256i, ival, Index);
					return pack;
#endif
				}
			}

			if constexpr (IsNative())
			{
				alignas(DataSize) T vals[Width];
				AlignedStore(vals);
				vals[Index] = val;
				return Pack::AlignedLoad(vals);
			}
		}

		pack.data.raw[Index] = val;
		return pack;
	}

	template <SimdBaseType T, usize Width>
	template <usize Index>
	constexpr auto Pack<T, Width>::Extract() const noexcept -> T
	{
		STATIC_ASSERT(Index < Width, "Index out of range");

		{
			if constexpr (Is128Bit())
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_SSE_SUPPORT
					i64 val = _mm_extract_epi64(data.sse_m128i, Index);
					return *reinterpret_cast<T*>(&val);
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_SSE_SUPPORT
					i64 val;
					if constexpr (IsF32<T>)
						val = _mm_extract_ps(data.sse_m128, Index);
					else
						val = _mm_extract_epi32(data.sse_m128i, Index);
					return *reinterpret_cast<T*>(&val);
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_SSE_SUPPORT
					return T(_mm_extract_epi16(data.sse_m128i, Index));
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_SSE_SUPPORT
					return T(_mm_extract_epi8(data.sse_m128i, Index));
#endif
				}
			}
			else if constexpr (Is256Bit())
			{
				if constexpr (sizeof(T) == 8)
				{
#if HAS_AVX
					i64 val = _mm256_extract_epi64(data.sse_m256i, Index);
					return *reinterpret_cast<T*>(&val);
#endif
				}
				else if constexpr (sizeof(T) == 4)
				{
#if HAS_AVX
					i32 val = _mm256_extract_epi32(data.sse_m256i, Index);
					return *reinterpret_cast<T*>(&val);
#endif
				}
				else if constexpr (sizeof(T) == 2)
				{
#if HAS_AVX2
					return T(_mm256_extract_epi16(data.sse_m256i, Index));
#elif HAS_AVX
					i32 vals = _mm256_extract_epi32(data.sse_m256i, Index >> 1);
					return T(reinterpret_cast<i16*>(vals)[Index & 1]);
#endif
				}
				else if constexpr (sizeof(T) == 1)
				{
#if HAS_AVX2
					return T(_mm256_extract_epi8(data.sse_m256i, Index));
#elif HAS_AVX
					i32 vals = _mm256_extract_epi32(data.sse_m256i, Index >> 2);
					return T(reinterpret_cast<i8*>(vals)[Index & 3]);
#endif
				}
			}

			if constexpr (IsNative())
			{
				alignas(DataSize) T vals[Width];
				AlignedStore(vals);
				return vals[Index];
			}
		}

		return data.raw[Index];
	}
}
