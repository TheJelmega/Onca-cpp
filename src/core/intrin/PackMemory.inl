#pragma once

#if __RESHARPER__
#include "Pack.h"
#endif

namespace Core::Intrin
{
	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Zero() noexcept -> Pack
	{
		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_setzero_pd();
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_setzero_ps();
					return pack;
#endif

				}
				else
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_setzero_si128();
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_setzero_ps();
					return pack;
#elif HAS_SSE
					pack.data.sse_m128[0] = _mm_setzero_ps();
					pack.data.sse_m128[1] = _mm_setzero_ps();
					return pack;
#endif
				}
				else if constexpr (IsF64<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256d = _mm256_setzero_pd();
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128d[0] = _mm_setzero_pd();
					pack.data.sse_m128d[1] = _mm_setzero_pd();
					return pack;
#endif
				}
				else
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_setzero_si256();
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128i[0] = _mm_setzero_si128();
					pack.data.sse_m128i[1] = _mm_setzero_si128();
					return pack;
#endif
				}
			}
		}

		MemClearData(pack);
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Set(T val) noexcept -> Pack
	{
		Pack pack{ UnInit };
		//IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_set1_pd(val);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_set1_ps(val);
					return pack;
#endif

				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_set1_epi64x(i64(val));
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_set1_epi32(i32(val));
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_set1_epi16(i16(val));
					return pack;
#endif
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_set1_epi8(i8(val));
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_set1_ps(val);
					return pack;
#elif HAS_SSE
					pack.data.sse_m128[0] = _mm_set1_ps(val);
					pack.data.sse_m128[1] = _mm_set1_ps(val);
					return pack;
#endif
				}
				else if constexpr (IsF64<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256d = _mm256_set1_pd(val);
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128d[0] = _mm_set1_pd(val);
					pack.data.sse_m128d[1] = _mm_set1_pd(val);
					return pack;
#endif
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_set1_epi64x(i64(val));
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128i[0] = _mm_set1_epi64x(i64(val));
					pack.data.sse_m128i[1] = _mm_set1_epi64x(i64(val));
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_set1_epi32(i32(val));
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128i[0] = _mm_set1_epi32(i32(val));
					pack.data.sse_m128i[1] = _mm_set1_epi32(i32(val));
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_set1_epi16(i16(val));
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128i[0] = _mm_set1_epi16(i16(val));
					pack.data.sse_m128i[1] = _mm_set1_epi16(i16(val));
					return pack;
#endif
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_set1_epi8(i8(val));
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128i[0] = _mm_set1_epi8(i8(val));
					pack.data.sse_m128i[1] = _mm_set1_epi8(i8(val));
					return pack;
#endif
				}
			}
		}

		for (usize i = 0; i < Width; ++i)
			pack.data.raw[i] = val;
		return pack;
	}

	template<typename S, SimdBaseType T, typename TupType, usize... Inds>
	auto X86Set128Helper(TupType&& args, IndexSequence<Inds...>) -> S
	{
		if constexpr (IsF32<T>)
		{
#if HAS_SSE
			return _mm_setr_ps(std::get<Inds>(args)...);
#endif
		}
#if HAS_SSE2
		else if constexpr (IsF64<T>)
		{
			return  _mm_setr_pd(std::get<Inds>(args)...);
		}
		else if constexpr (IsU64<T> || IsI64<T>)
		{
			return _mm_setr_epi64x(std::get<Inds>(args)...);
		}
		else if constexpr (IsU32<T> || IsI32<T>)
		{
			return _mm_setr_epi32(std::get<Inds>(args)...);
		}
		else if constexpr (IsU16<T> || IsI16<T>)
		{
			return _mm_setr_epi16(std::get<Inds>(args)...);
		}
		else if constexpr (IsU8<T> || IsI8<T>)
		{
			return _mm_setr_epi8(std::get<Inds>(args)...);
		}
#endif

		ASSERT(false, "Invalid Set128HalfHelper() call");
		return S{};
	}

	template<SimdBaseType T, usize Width>
	auto SetDefaultHelper(Pack<T, Width>& pack, InitializerList<T> il) -> void
	{
		MemCpy(&pack, il.begin(), il.size() * sizeof(T));
	}

	template <SimdBaseType T, usize Width>
	template <SameAs<T>... Args>
	constexpr auto Pack<T, Width>::Set(Args... vals) noexcept -> Pack
	{
		STATIC_ASSERT(sizeof...(Args) == Width, "Expected 'Width' values");

		Pack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_setr_pd(vals...);
					return pack;
#endif
				}
				else if constexpr  (IsF64<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_setr_ps(vals...);
					return pack;
#endif
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_setr_epi64x(i64(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_setr_epi32(i32(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_setr_epi16(i16(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_setr_epi8(i8(vals)...);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (IsF32<T>)
				{
					if constexpr (IsF64<T>)
					{
#if HAS_AVX
						pack.data.sse_m256d = _mm256_setr_pd(vals...);
						return pack;
#elif HAS_SSE2
						pack.data.sse_m128d[0] = X86Set128Helper<__m128d, T>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<0        , Width / 2>{});
						pack.data.sse_m128d[1] = X86Set128Helper<__m128d, T>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<Width / 2, Width>{});
						return pack;
#endif
					}
					else if constexpr (IsF64<T>)
					{
#if HAS_AVX
						pack.data.sse_m256 = _mm256_setr_ps(vals...);
						return pack;
#elif HAS_SSE
						pack.data.sse_m128[0] = X86Set128Helper<__m128, T>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<0        , Width / 2>{});
						pack.data.sse_m128[1] = X86Set128Helper<__m128, T>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<Width / 2, Width>{});
						return pack;
#endif
					}
					else if constexpr (IsU64<T> || IsI64<T>)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_setr_epi64x(i64(vals)...);
						return pack;
#elif HAS_SSE2
						pack.data.sse_m128i[0] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<0        , Width / 2>{});
						pack.data.sse_m128i[1] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<Width / 2, Width>{});
						return pack;
#endif
					}
					else if constexpr (IsU32<T> || IsI32<T>)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_setr_epi32(i32(vals)...);
						return pack;
#elif HAS_SSE2
						pack.data.sse_m128i[0] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<0        , Width / 2>{});
						pack.data.sse_m128i[1] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<Width / 2, Width>{});
						return pack;
#endif
					}
					else if constexpr (IsU16<T> || IsI16<T>)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_setr_epi16(i16(vals)...);
						return pack;
#elif HAS_SSE2
						pack.data.sse_m128i[0] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<0        , Width / 2>{});
						pack.data.sse_m128i[1] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<Width / 2, Width>{});
						return pack;
#endif
					}
					else if constexpr (IsU8<T> || IsI8<T>)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_setr_epi8(i8(vals)...);
						return pack;
#elif HAS_SSE2
						pack.data.sse_m128i[0] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<0        , Width / 2>{});
						pack.data.sse_m128i[1] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeIndexSequenceRange<Width / 2, Width>{});
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

	template<typename S, SimdBaseType T, typename TupType, usize... Inds>
	auto X86SetR128Helper(TupType&& args, IndexSequence<Inds...>) -> S
	{
		if constexpr (IsF32<T>)
		{
#if HAS_SSE
			return _mm_set_ps(std::get<Inds>(args)...);
#endif
		}
#if HAS_SSE2
		else if constexpr (IsF64<T>)
		{
			return _mm_set_pd(std::get<Inds>(args)...);
		}
		else if constexpr (IsU64<T> || IsI64<T>)
		{
			return _mm_set_epi64x(std::get<Inds>(args)...);
		}
		else if constexpr (IsU32<T> || IsI32<T>)
		{
			return _mm_set_epi32(std::get<Inds>(args)...);
		}
		else if constexpr (IsU16<T> || IsI16<T>)
		{
			return _mm_set_epi16(std::get<Inds>(args)...);
		}
		else if constexpr (IsU8<T> || IsI8<T>)
		{
			return _mm_set_epi8(std::get<Inds>(args)...);
		}
#endif

		ASSERT(false, "Invalid SetR128HalfHelper() call");
		return {};
	}

	template<SimdBaseType T, usize Width, typename TupType, usize... Inds>
	constexpr auto SetRDefaultHelper(Pack<T, Width>& pack, TupType&& args, IndexSequence<Inds...>) -> void
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
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_set_pd(vals...);
					return pack;
#endif
				}
				else if constexpr (IsF64<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_set_ps(vals...);
					return pack;
#endif
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_set_epi64x(i64(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_set_epi32(i32(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_set_epi16(i16(vals)...);
					return pack;
#endif
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_set_epi8(i8(vals)...);
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (IsF32<T>)
				{
					if constexpr (IsF64<T>)
					{
#if HAS_AVX
						pack.data.sse_m256d = _mm256_set_pd(vals...);
						return pack;
#elif HAS_SSE2
						pack.data.sse_m128d[1] = X86Set128Helper<__m128d, T>(Tuple<Args...>{vals...}, MakeReverseIndexSequenceRange<0        , Width / 2>{});
						pack.data.sse_m128d[0] = X86Set128Helper<__m128d, T>(Tuple<Args...>{vals...}, MakeReverseIndexSequenceRange<Width / 2, Width>{});
						return pack;
#endif
					}
					else if constexpr (IsF64<T>)
					{
#if HAS_AVX
						pack.data.sse_m256 = _mm256_set_ps(vals...);
						return pack;
#elif HAS_SSE
						pack.data.sse_m128[1] = X86Set128Helper<__m128, T>(Tuple<Args...>{vals...}, MakeReverseIndexSequenceRange<0        , Width / 2>{});
						pack.data.sse_m128[0] = X86Set128Helper<__m128, T>(Tuple<Args...>{vals...}, MakeReverseIndexSequenceRange<Width / 2, Width>{});
						return pack;
#endif
					}
					else if constexpr (IsU64<T> || IsI64<T>)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_set_epi64x(i64(vals)...);
						return pack;
#elif HAS_SSE2
						pack.data.sse_m128i[1] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeReverseIndexSequenceRange<0        , Width / 2>{});
						pack.data.sse_m128i[0] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeReverseIndexSequenceRange<Width / 2, Width>{});
						return pack;
#endif
					}
					else if constexpr (IsU32<T> || IsI32<T>)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_set_epi32(i32(vals)...);
						return pack;
#elif HAS_SSE2
						pack.data.sse_m128i[1] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeReverseIndexSequenceRange<0        , Width / 2>{});
						pack.data.sse_m128i[0] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeReverseIndexSequenceRange<Width / 2, Width>{});
						return pack;
#endif
					}
					else if constexpr (IsU16<T> || IsI16<T>)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_set_epi16(i16(vals)...);
						return pack;
#elif HAS_SSE2
						pack.data.sse_m128i[1] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeReverseIndexSequenceRange<0        , Width / 2>{});
						pack.data.sse_m128i[0] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeReverseIndexSequenceRange<Width / 2, Width>{});
						return pack;
#endif
					}
					else if constexpr (IsU8<T> || IsI8<T>)
					{
#if HAS_AVX2
						pack.data.sse_m256i = _mm256_set_epi8(i8(vals)...);
						return pack;
#elif HAS_SSE2
						pack.data.sse_m128i[1] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeReverseIndexSequenceRange<0        , Width / 2>{});
						pack.data.sse_m128i[0] = X86Set128Helper<__m128i, T>(Tuple<Args...>{vals...}, MakeReverseIndexSequenceRange<Width / 2, Width>{});
						return pack;
#endif
					}
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
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_loadu_pd(addr);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_loadu_ps(addr);
					return pack;
#endif
				}
				else
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_loadu_si128(reinterpret_cast<const __m128i*>(addr));
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_loadu_pd(addr);
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128d[0] = _mm_loadu_pd(addr);
					pack.data.sse_m128d[1] = _mm_loadu_pd(addr + sizeof(__m128) / sizeof(T));
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_loadu_ps(addr);
					return pack;
#elif HAS_SSE
					pack.data.sse_m128[0] = _mm_loadu_ps(addr);
					pack.data.sse_m128[1] = _mm_loadu_ps(addr + sizeof(__m128) / sizeof(T));
					return pack;
#endif
				}
				else
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(addr));
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128i[0] = _mm_loadu_si128(reinterpret_cast<const __m128i*>(addr));
					pack.data.sse_m128i[1] = _mm_loadu_si128(reinterpret_cast<const __m128i*>(addr) + 1);
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
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					pack.data.sse_m128d = _mm_load_pd(addr);
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					pack.data.sse_m128 = _mm_load_ps(addr);
					return pack;
#endif
				}
				else
				{
#if HAS_SSE2
					pack.data.sse_m128i = _mm_load_si128(reinterpret_cast<const __m128i*>(addr));
					return pack;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					pack.data.sse_m256d = _mm256_load_pd(addr);
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128d[0] = _mm_load_pd(addr);
					pack.data.sse_m128d[1] = _mm_load_pd(addr + sizeof(__m128d) / sizeof(T));
					return pack;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					pack.data.sse_m256 = _mm256_load_ps(addr);
					return pack;
#elif HAS_SSE
					pack.data.sse_m128[0] = _mm_load_ps(addr);
					pack.data.sse_m128[1] = _mm_load_ps(addr + sizeof(__m128) / sizeof(T));
					return pack;
#endif
				}
				else
				{
#if HAS_AVX2
					pack.data.sse_m256i = _mm256_load_si256(reinterpret_cast<const __m256i*>(addr));
					return pack;
#elif HAS_SSE2
					pack.data.sse_m128i[0] = _mm_load_si128(reinterpret_cast<const __m128i*>(addr));
					pack.data.sse_m128i[1] = _mm_load_si128(reinterpret_cast<const __m128i*>(addr) + 1);
					return pack;
#endif
				}
			}
		}

		MemCpy(&pack, addr, DataSize);
		return pack;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::Store(T* addr) const noexcept -> void
	{
		IF_NOT_CONSTEVAL
		{
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					_mm_storeu_pd(addr, data.sse_m128d);
					return;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					_mm_storeu_ps(addr, data.sse_m128);
					return;
#endif
				}
				else
				{
#if HAS_SSE2
					_mm_storeu_si128(reinterpret_cast<__m128i*>(addr), data.sse_m128i);
					return;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					_mm256_storeu_pd(addr, data.sse_m256d);
					return;
#elif HAS_SSE2
					_mm_storeu_pd(addr                              , data.sse_m128d[0]);
					_mm_storeu_pd(addr + sizeof(__m128d) / sizeof(T), data.sse_m128d[1]);
					return;
#endif

				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					_mm256_storeu_ps(addr, data.sse_m256);
					return;
#elif HAS_SSE
					_mm_storeu_ps(addr                              , data.sse_m128[0]);
					_mm_storeu_ps(addr + sizeof(__m128d) / sizeof(T), data.sse_m128[1]);
					return;
#endif

				}
				else
				{
#if HAS_AVX2
					_mm256_storeu_si256(reinterpret_cast<__m256i*>(addr), data.sse_m256i);
					return;
#elif HAS_SSE2
					_mm_storeu_si128(reinterpret_cast<__m128i*>(addr)    , data.sse_m128i[0]);
					_mm_storeu_si128(reinterpret_cast<__m128i*>(addr) + 1, data.sse_m128i[1]);
					return;
#endif

				}
			}
		}
		MemCpy(addr, &data, DataSize);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::AlignedStore(T* addr) const noexcept -> void
	{
		IF_NOT_CONSTEVAL
		{
			ASSERT((usize(addr) & (DataSize - 1)) == 0, "'addr' needs to have the same alignment as Pack");
			if constexpr (Detail::IsSIMD128<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_SSE2
					_mm_store_pd(addr, data.sse_m128d);
					return;
#endif
				}
				else if constexpr (IsF32<T>)
				{
#if HAS_SSE
					_mm_store_ps(addr, data.sse_m128);
					return;
#endif
				}
				else
				{
#if HAS_SSE2
					_mm_store_si128(reinterpret_cast<__m128i*>(addr), data.sse_m128i);
					return;
#endif
				}
			}
			else if constexpr (Detail::IsSIMD256<DataSize>)
			{
				if constexpr (IsF64<T>)
				{
#if HAS_AVX
					_mm256_store_pd(addr, data.sse_m256d);
					return;
#elif HAS_SSE2
					_mm_store_pd(addr                              , data.sse_m128d[0]);
					_mm_store_pd(addr + sizeof(__m128d) / sizeof(T), data.sse_m128d[1]);
					return;
#endif

				}
				else if constexpr (IsF32<T>)
				{
#if HAS_AVX
					_mm256_store_ps(addr, data.sse_m256);
					return;
#elif HAS_SSE
					_mm_store_ps(addr                              , data.sse_m128[0]);
					_mm_store_ps(addr + sizeof(__m128d) / sizeof(T), data.sse_m128[1]);
					return;
#endif

				}
				else
				{
#if HAS_AVX2
					_mm256_store_si256(reinterpret_cast<__m256i*>(addr), data.sse_m256i);
					return;
#elif HAS_SSE2
					_mm_store_si128(reinterpret_cast<__m128i*>(addr)    , data.sse_m128i[0]);
					_mm_store_si128(reinterpret_cast<__m128i*>(addr) + 1, data.sse_m128i[1]);
					return;
#endif

				}
			}
		}
		MemCpy(addr, &data, DataSize);
	}
}
