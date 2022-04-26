#pragma once

#if __RESHARPER__
#include "Pack.h"
#endif

#if COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4309) // 'argument': truncation of constant value <- for _mm*_set_epi8 calls
#endif

namespace Core::Intrin
{
	template<SimdBaseType From, SimdBaseType T, usize Width>
	void SignExtend(Pack<T, Width>& pack) noexcept
	{
		if constexpr (Pack<T, Width>::Is128Bit())
		{
			if constexpr (sizeof(T) == 2)
			{
#if HAS_SSE_SUPPORT
				__m128i signExtension = _mm_set1_epi16(i16(0xFF00));
				__m128i needsExtensionMask = _mm_set1_epi16(i16(0x0080));
				__m128i needsExtensionBit = _mm_and_si128(pack.data.sse_m128i, needsExtensionMask); // a bit is set in each value that needs a sign extension
				__m128i signMask = _mm_cmpeq_epi16(needsExtensionBit, needsExtensionMask);
				__m128i maskedSign = _mm_and_si128(signMask, signExtension);
				pack.data.sse_m128i = _mm_or_si128(maskedSign, pack.data.sse_m128i);
#endif
			}
			else if constexpr (sizeof(T) == 4)
			{
#if HAS_SSE_SUPPORT
				constexpr i32 signExt = sizeof(From) == 1 ? T(0xFFFF'FF00) : T(0xFFFF'0000);
				constexpr i32 mask = sizeof(From) == 1 ? T(0x0000'0080) : T(0x0000'8000);

				__m128i signExtension = _mm_set1_epi32(signExt);
				__m128i needsExtensionMask = _mm_set1_epi32(mask);
				__m128i needsExtensionBit = _mm_and_si128(pack.data.sse_m128i, needsExtensionMask); // a bit is set in each value that needs a sign extension
				__m128i signMask = _mm_cmpeq_epi32(needsExtensionBit, needsExtensionMask);
				__m128i maskedSign = _mm_and_si128(signMask, signExtension);
				pack.data.sse_m128i = _mm_or_si128(maskedSign, pack.data.sse_m128i);
#endif
			}
			else if constexpr (sizeof(T) == 8)
			{
#if HAS_SSE_SUPPORT
				constexpr i64 signExt = sizeof(From) == 1 ? T(0xFFFF'FFFF'FFFF'FF00) : sizeof(From) == 2 ? T(0xFFFF'FFFF'FFFF'0000) : T(0xFFFF'FFFF'0000'0000);
				constexpr i64 mask = sizeof(From) == 1 ? T(0x0000'0000'0000'0080) : sizeof(From) == 2 ? T(0x0000'0000'0000'8000) : T(0x0000'0000'8000'0000);

				__m128i signExtension = _mm_set1_epi64x(0xFFFF'FFFF'FFFF'FF00);
				__m128i needsExtensionMask = _mm_set1_epi64x(0x0000'0000'0000'0080);
				__m128i needsExtensionCmp = _mm_set1_epi32(0x0000'0080);
				__m128i needsExtensionBit = _mm_and_si128(pack.data.sse_m128i, needsExtensionMask); // a bit is set in each value that needs a sign extension
				__m128i halfSignMask = _mm_cmpeq_epi32(needsExtensionBit, needsExtensionCmp);
				__m128i signMask = _mm_unpacklo_epi32(halfSignMask, halfSignMask);
				__m128i maskedSign = _mm_and_si128(signMask, signExtension);
				pack.data.sse_m128i = _mm_or_si128(maskedSign, pack.data.sse_m128i);
#endif
			}
		}
	}

	template<SimdBaseType U, usize NewWidth, SimdBaseType T, usize Width>
	constexpr auto DefPackCvt(const Pack<T, Width>& srcPack) -> Pack<U, NewWidth>
	{
		constexpr usize MinWidth = Math::Min(NewWidth, Width);
		constexpr usize DataSize = Pack<T, Width>::DataSize;

		Pack<U, NewWidth> pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			alignas(DataSize) T src[Width];
			srcPack.AlignedStore(reinterpret_cast<T*>(src));

			alignas(DataSize) U dst[NewWidth] = {};
			for (usize i = 0; i < MinWidth; ++i)
				dst[i] = U(src[i]);
			return Pack<U, NewWidth>::AlignedLoad(reinterpret_cast<U*>(dst));
		}

		for (usize i = 0; i < MinWidth; ++i)
			pack.data.raw[i] = U(srcPack.data.raw[i]);
		return pack;
	}

    // TODO: Create fast SIMD version for unsupported cases
	template <SimdBaseType T, usize Width>
	template <typename U>
	constexpr auto Pack<T, Width>::Convert() const noexcept -> Pack<U, DataSize / sizeof(U)>
	{
		constexpr usize NewWidth = DataSize / sizeof(U);
		using ResPack = Pack<U, NewWidth>;

		if constexpr (SameAs<T, U>)
			return *this;
		if constexpr (Integral<T> && Integral<U> && sizeof(T) == sizeof(U))
			return ResPack{ data };

		ResPack pack{ UnInit };
		IF_NOT_CONSTEVAL
		{
			if constexpr (Is128Bit())
			{
				if constexpr (IsF64<T>)
				{
					if constexpr (IsF32<U>)
					{
#if HAS_SSE_SUPPORT
						pack.data.sse_m128 = _mm_cvtpd_ps(data.sse_m128d);
						return pack;
#endif
					}
					else if constexpr (IsU64<U>)
					{
#if HAS_AVX512DQ && HAS_AVX512VL
						pack.data.sse_m128i = _mm_cvtpd_epu64(data.sse_m128d);
						return pack;
#elif HAS_SSE_SUPPORT
						// TODO
#endif
					}
					else if constexpr (IsU32<U>)
					{
#if HAS_AVX512F && HAS_AVX512VL
						pack.data.sse_m128i = _mm_cvtpd_epu32(data.sse_m128d);
						return pack;
#elif HAS_SSE_SUPPORT
						// TODO
#endif
					}
					else if constexpr (IsI64<U>)
					{
#if HAS_AVX512DQ && HAS_AVX512VL
						pack.data.sse_m128i = _mm_cvtpd_epi64(data.sse_m128d);
						return pack;
#elif HAS_SSE_SUPPORT
						// TODO
#endif
					}
					else if constexpr (IsI32<U>)
					{
#if HAS_SSE_SUPPORT
						pack.data.sse_m128i = _mm_cvtpd_epi32(data.sse_m128d);
						return pack;
#endif
					}
					else if constexpr (IsU16<U> || IsI16<U> || IsU8<U> || IsI8<U>)
					{
#if HAS_SSE_SUPPORT
						Pack<i32, 4> tmp;
						tmp.data.sse_m128i = _mm_cvtpd_epi32(data.sse_m128d);
						return tmp.Convert<U>();
#endif
					}
				}
				else if constexpr (IsF32<T>)
				{
					if constexpr (IsF64<U>)
					{
#if HAS_SSE_SUPPORT
						pack.data.sse_m128d = _mm_cvtps_pd(data.sse_m128);
						return pack;
#endif
					}
					else if constexpr (IsU64<U>)
					{
#if HAS_AVX512DQ && HAS_AVX512VL
						pack.data.sse_m128i = _mm_cvtps_epu64(data.sse_m128);
						return pack;
#elif HAS_SSE_SUPPORT
						// TODO
#endif
					}
					else if constexpr (IsU32<U>)
					{
#if HAS_AVX512F && HAS_AVX512VL
						pack.data.sse_m128i = _mm_cvtps_epu32(data.sse_m128);
						return pack;
#elif HAS_SSE_SUPPORT
						// TODO
#endif
					}
					else if constexpr (IsI64<U>)
					{
#if HAS_AVX512DQ && HAS_AVX512VL
						pack.data.sse_m128i = _mm_cvtps_epi64(data.sse_m128);
						return pack;
#elif HAS_SSE_SUPPORT
						// TODO
#endif
					}
					else if constexpr (IsI32<U>)
					{
#if HAS_SSE_SUPPORT
						pack.data.sse_m128i = _mm_cvtps_epi32(data.sse_m128);
						return pack;
#endif
					}
					else if constexpr (IsU16<U> || IsI16<U> || IsU8<T> || IsI8<U>)
					{
#if HAS_SSE_SUPPORT
						Pack<i32, 4> tmp;
						tmp.data.sse_m128i = _mm_cvtps_epi32(data.sse_m128);
						return tmp.Convert<U>();
#endif
					}
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
					if constexpr (IsF64<U>)
					{
#if HAS_AVX512DQ && HAS_AVX512VL
						if (IsU64<U>)
							pack.data.sse_m128d = _mm_cvtepu64_pd(data.sse_m128i);
						else
							pack.data.sse_m128d = _mm_cvtepi64_pd(data.sse_m128i);
						return pack;
#elif HAS_SSE_SUPPORT
						// https://stackoverflow.com/questions/41144668/
						if (IsU64<T>)
						{
							__m128d k84 = _mm_set1_pd(19342813113834066795298816.0); // 2^84
							__m128d k52 = _mm_set1_pd(0x0010'0000'0000'0000); // 2^52
							__m128d k84_52 = _mm_set1_pd(19342813118337666422669312.0); // 2^84 + 2^52

							__m128i hi = _mm_srli_epi64(data.sse_m128i, 32);
							hi = _mm_or_si128(hi, _mm_castpd_si128(k84));
							__m128i lo = _mm_blend_epi16(data.sse_m128i, _mm_castpd_si128(k52), 0xCC);
							__m128d dbl = _mm_sub_pd(_mm_castsi128_pd(hi), k84_52);
							pack.data.sse_m128d = _mm_add_pd(dbl, _mm_castsi128_pd(lo));
						}
						else
						{
							__m128i zero = _mm_setzero_si128();
							__m128d k3_67 = _mm_set1_pd(442721857769029238784.0); // 3*2^67
							__m128d k52 = _mm_set1_pd(0x0010'0000'0000'0000); // 2^52
							__m128d k3_67_52 = _mm_set1_pd(442726361368656609280.0); // 3*2^67 + 2^52

							__m128i hi = _mm_srai_epi32(data.sse_m128i, 16);
							hi = _mm_blend_epi16(hi, zero, 0x33);
							hi = _mm_add_epi64(hi, _mm_castpd_si128(k3_67));
							__m128i lo = _mm_blend_epi16(data.sse_m128i, _mm_castpd_si128(k52), 0x88);
							__m128d dbl = _mm_sub_pd(_mm_castsi128_pd(hi), k3_67_52);
							pack.data.sse_m128d = _mm_add_pd(dbl, _mm_castsi128_pd(lo));
						}
						return pack;
#endif
					}
					else if constexpr (IsF32<U>)
					{
#if HAS_AVX512DQ && HAS_AVX512VL
						if (IsU64<U>)
							pack.data.sse_m128 = _mm_cvtepu64_ps(data.sse_m128i);
						else 
							pack.data.sse_m128 = _mm_cvtepi64_ps(data.sse_m128i);
						return pack;
#elif HAS_SSE_SUPPORT
						return Convert<f64>().Convert<f32>();
#endif
					}
					else if constexpr (IsU32<U> || IsI32<U>)
					{
#if HAS_AVX512F && HAS_AVX512VL
						pack.data.sse_m128d = _mm_cvtepi64_epi32(data.sse_m128i);
						return pack;
#elif HAS_SSE_SUPPORT
						__m128i shuffleMask = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0B, 0x0A, 0x09, 0x08, 0x03, 0x02, 0x01, 0x00);
						pack.data.sse_m128i = _mm_shuffle_epi8(data.sse_m128i, shuffleMask); // (lo,hi,lo,hi) -> (-,-,lo,lo)
						return pack;
#endif
					}
					else if constexpr (IsU16<U> || IsI16<U>)
					{
#if HAS_AVX512F && HAS_AVX512VL
						pack.data.sse_m128d = _mm_cvtepi64_epi16(data.sse_m128i);
						return pack;
#elif HAS_SSE_SUPPORT
						__m128i shuffleMask = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x09, 0x08, 0x01, 0x00);
						pack.data.sse_m128i = _mm_shuffle_epi8(data.sse_m128i, shuffleMask); // (0a,0b,0c,0d,1a,1b,1c,1d) -> (---,0a,1a)
						return pack;
#endif
					}
					else if constexpr (IsU8<U> || IsI8<U>)
					{
#if HAS_AVX512F && HAS_AVX512VL
						pack.data.sse_m128d = _mm_cvtepi64_epi8(data.sse_m128i);
						return pack;
#elif HAS_SSE_SUPPORT
						__m128i shuffleMask = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x08, 0x00);
						pack.data.sse_m128i = _mm_shuffle_epi8(data.sse_m128i, shuffleMask); // (0a,0b,0c,0d,0e,0f,0g,0h,1a,1b,1c,1d,1e,1f,1g,1h) -> (---,0a,1a)
						return pack;
#endif
					}
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
					if constexpr (IsF64<U>)
					{
#if HAS_SSE_SUPPORT
						if constexpr (IsI32<T>)
						{
							pack.data.sse_m128d = _mm_cvtepi32_pd(data.sse_m128i);
							return pack;
						}
						else
						{
#if HAS_AVX512F
							pack.data.sse_m128d = _mm_cvtepu32_pd(data.sse_m128i);
							return pack;
#else
							return Convert<i64>().Convert<f64>();
#endif
						}
#endif
					}
					else if constexpr (IsF32<U>)
					{
#if HAS_SSE_SUPPORT
						if constexpr (IsI32<T>)
						{
							pack.data.sse_m128 = _mm_cvtepi32_ps(data.sse_m128i);
							return pack;
						}
						else
						{
#if HAS_AVX512F
							pack.data.sse_m128 = _mm_cvtepu32_ps(data.sse_m128i);
							return pack;
#else
							// https://stackoverflow.com/questions/34066228/how-to-perform-uint32-float-conversion-with-sse
							__m128i maskLo = _mm_set1_epi32(0xFFFF);
							__m128 k65536f = _mm_set1_ps(65536.0f);

							__m128i lo = _mm_and_si128(data.sse_m128i, maskLo); // extract the 16 lowest significant bits of this
							__m128i hi = _mm_srli_epi32(data.sse_m128i, 16); // 16 most significant bits of this
							__m128 loFlt = _mm_cvtepi32_ps(lo); // no rounding
							__m128 hiFlt = _mm_cvtepi32_ps(hi); // no rounding
							hiFlt = _mm_mul_ps(k65536f, hiFlt); // no rounding
							pack.data.sse_m128 = _mm_add_ps(hiFlt, loFlt); // rounding may occur here, mul and and may fuse to fm for haswell and newer
							// _mm_add_ps is guarenteed to give results with an erro of at most o.5 ULP (Units in Last Place)
							return pack;
#endif
						}
#endif
					}
					else if constexpr (IsU64<U> || IsI64<U>)
					{
#if HAS_SSE_SUPPORT
						if constexpr (IsU32<T>)
							pack.data.sse_m128i = _mm_cvtepu32_epi64(data.sse_m128i);
						else 
							pack.data.sse_m128i = _mm_cvtepi32_epi64(data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (IsU16<U> || IsI16<U>)
					{
#if HAS_AVX512F && HAS_AVX512VL
						pack.data.sse_m128i = _mm_cvtepi32_epi16(data.sse_m128i);
						return pack;
#elif HAS_SSE_SUPPORT
						__m128i mask = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0D, 0x0C, 0x09, 0x08, 0x05, 0x04, 0x01, 0x00);
						pack.data.sse_m128i = _mm_shuffle_epi8(data.sse_m128i, mask);
						return pack;
#endif
					}
					else if constexpr (IsU8<U> || IsI8<U>)
					{
#if HAS_AVX512F && HAS_AVX512VL
						pack.data.sse_m128i = _mm_cvtepi32_epi8(data.sse_m128i);
						return pack;
#elif HAS_SSE_SUPPORT
						__m128i mask = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0C, 0x08, 0x04, 0x00);
						pack.data.sse_m128i = _mm_shuffle_epi8(data.sse_m128i, mask);
						return pack;
#endif
					}
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
					if constexpr (IsF64<U> || IsF32<U>)
					{
						return Convert<i32>().Convert<U>();
					} 
					else if constexpr (IsU64<U> || IsI64<U>)
					{
#if HAS_SSE_SUPPORT
						if constexpr (IsU16<T>)
							pack.data.sse_m128i = _mm_cvtepu16_epi64(data.sse_m128i);
						else 
							pack.data.sse_m128i = _mm_cvtepi16_epi64(data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (IsU32<U> || IsI32<U>)
					{
#if HAS_SSE_SUPPORT
						if constexpr (IsU16<T>)
							pack.data.sse_m128i = _mm_cvtepu16_epi32(data.sse_m128i);
						else
							pack.data.sse_m128i = _mm_cvtepi16_epi32(data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (IsU8<U> || IsI8<U>)
					{
#if HAS_AVX512BW && HAS_AVX512VL
						pack.data.sse_m128i = _mm_cvtepi16_epi8(data.sse_m128i);
						return pack;
#elif HAS_SSE_SUPPORT
						__m128i mask = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0E, 0x0C, 0x0A, 0x08, 0x06, 0x04, 0x02, 0x00);
						pack.data.sse_m128i = _mm_blendv_epi8(data.sse_m128i, data.sse_m128i, mask);
#endif
					}
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
					if constexpr (IsF64<U> || IsF32<U>)
					{
						return Convert<i32>().Convert<U>();
					}
					else if constexpr (IsU64<U> || IsI64<U>)
					{
#if HAS_SSE_SUPPORT
						if (IsU8<T>)
							pack.data.sse_m128i = _mm_cvtepu8_epi64(data.sse_m128i);
						else
							pack.data.sse_m128i = _mm_cvtepi8_epi64(data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (IsU32<U> || IsI32<U>)
					{
#if HAS_SSE_SUPPORT
						if (IsU8<T>)
							pack.data.sse_m128i = _mm_cvtepu8_epi32(data.sse_m128i);
						else
							pack.data.sse_m128i = _mm_cvtepi8_epi32(data.sse_m128i);
						return pack;
#endif
					}
					else if constexpr (IsU16<U> || IsI16<U>)
					{
#if HAS_SSE_SUPPORT
						if (IsU8<T>)
							pack.data.sse_m128i = _mm_cvtepu8_epi16(data.sse_m128i);
						else
							pack.data.sse_m128i = _mm_cvtepi8_epi16(data.sse_m128i);
						return pack;
#endif
					}
				}
			}
			else if constexpr (Is128Bit())
			{
#if HAS_SSE_SUPPORT
				if constexpr (IsNative() && !IsNative256())
				{
					if constexpr (Width == NewWidth)
					{
						pack.data.m128[0] = HalfPack(0).template Convert<U>().data;
						pack.data.m128[1] = HalfPack(1).template Convert<U>().data;
					}
					else if constexpr (Width > NewWidth)
					{
						Detail::PackData<T, Width / 2> hi = data.m128[0];
						constexpr i32 shift = NewWidth / 2 * sizeof(T);
						hi.sse_m128i = _mm_bsrli_si128(hi.sse_m128i, shift);

						pack.data.m128[0] = HalfPack(0).template Convert<U>().data;
						pack.data.m128[1] = Pack<T, Width / 2>{ hi }.template Convert<U>().data;
					}
					else // Width < NewWidth
					{
						__m128i lo = HalfPack(0).template Convert<U>().data.sse_m128i;
						__m128i hi = HalfPack(1).template Convert<U>().data.sse_m128i;
						
						constexpr i32 shift = Width / 2 * sizeof(U);
						hi = _mm_bslli_si128(hi, shift);
						lo = _mm_or_si128(hi, lo);

						pack.data.sse_m128i[0] = lo;
						pack.data.sse_m128i[1] = _mm_setzero_si128();
					}
					return pack;
				}
#endif

				// NOTE(jelte): AVX: certain functions return a smaller register than needed, to fix this, the value needs to be cast,
				// if we were to assign it to the first sub 'register', the other will be filled with junk,
				// I expect that this is caused by either compiler optimizations or the compiler not understanding the code as expected

				if constexpr  (IsF64<T>)
				{
					if constexpr (IsF32<U>)
					{
#if HAS_AVX
						pack.data.sse_m256 = _mm256_castps128_ps256(_mm256_cvtpd_ps(data.sse_m256d));
						return pack;
#endif
					}
					else if constexpr (IsU64<U>)
					{
#if HAS_AVX512DQ && HAS_AVX512VL
						pack.data.sse_m256i = _mm256_cvtpd_epi64(data.sse_m256d);
						return pack;
#else
						// TODO
#endif
					}
					else if constexpr (IsU32<U>)
					{
#if HAS_AVX512F && HAS_AVX512VL
						pack.data.sse_m256i = _mm256_castsi128_si256(_mm256_cvtpd_epu32(data.sse_m256d));
						return pack;
#else
						// TODO
#endif
					} 
					else if constexpr (IsU16<U> || IsU8<U>)
					{
						return Convert<u32>().Convert<U>();
					}
					else if constexpr (IsI64<U>)
					{
#if HAS_AVX512DQ && HAS_AVX512VL
						pack.data.sse_m256i = _mm256_cvtpd_epi64(data.sse_m256d);
						return pack;
#else
						// TODO
#endif
					}
					else if constexpr (IsI32<U>)
					{
#if HAS_AVX
						pack.data.sse_m256i = _mm256_castsi128_si256(_mm256_cvtpd_epi32(data.sse_m256d));
						return pack;
#endif
					}
					else if constexpr (IsI16<U> || IsI8<U>)
					{
						return Convert<i32>().Convert<U>();
					}
				}
				else if constexpr (IsF32<T>)
				{
					if constexpr (IsF64<U>)
					{
#if HAS_AVX
						pack.data.sse_m256d = _mm256_cvtps_pd(data.sse_m128[0]);
						return pack;
#endif
					}
					else if constexpr (IsU64<U>)
					{

#if HAS_AVX512DQ && HAS_AVX512VL
						pack.data.sse_m256i = _mm256_cvtps_epu64(data.sse_m128[0]);
						return pack;
#else
						// TODO
#endif
					}
					else if constexpr (IsU32<U>)
					{
#if HAS_AVX512F && HAS_AVX512VL
						pack.data.sse_m256i = _mm256_cvtps_epu32(data.sse_m256[0]);
						return pack;
#else
						// TODO
#endif
					}
					else if constexpr (IsU16<U> || IsU8<U>)
					{
						return Convert<u32>().Convert<U>();
					}
					else if constexpr (IsI64<U>)
					{
#if HAS_AVX512DQ && HAS_AVX512VL
						pack.data.sse_m256i = _mm256_cvtps_epi64(data.sse_m256);
						return pack;
#else
						// TODO
#endif
					}
					else if constexpr (IsI32<U>)
					{
#if HAS_AVX
						pack.data.sse_m256i = _mm256_cvtps_epi32(data.sse_m256);
#endif
					}
					else if constexpr (IsI16<U> || IsI8<U>)
					{
						return Convert<i32>().Convert<U>();
					}
				}
				else if constexpr (IsU64<T> || IsI64<T>)
				{
					if constexpr (IsF64<U>)
					{
#if HAS_AVX512DQ && HAS_AVX512VL
						if constexpr (IsU64<T>)
							pack.data.sse_m256d = _mm_cvtepu64_pd(data.sse_m256i);
						else
							pack.data.sse_m256d = _mm_cvtepi64_pd(data.sse_m256i);
						return pack;
#elif HAS_AVX2
						// https://stackoverflow.com/questions/41144668/
						if constexpr (IsU64<T>)
						{
							__m256d k84 = _mm256_set1_pd(19342813113834066795298816.0); // 2^84
							__m256d k52 = _mm256_set1_pd(0x0010'0000'0000'0000); // 2^52
							__m256d k84_52 = _mm256_set1_pd(19342813118337666422669312.0); // 2^84 + 2^52

							__m256i hi = _mm256_srli_epi64(data.sse_m256i, 32);
							hi = _mm256_or_si256(hi, _mm256_castpd_si256(k84));
							__m256i lo = _mm256_blend_epi32(data.sse_m256i, _mm256_castpd_si256(k52), 0xAA);
							__m256d dbl = _mm256_sub_pd(_mm256_castsi256_pd(hi), k84_52);
							pack.data.sse_m256d = _mm256_add_pd(dbl, _mm256_castsi256_pd(lo));
						}
						else
						{
							__m256i zero = _mm256_setzero_si256();
							__m256d k3_67 = _mm256_set1_pd(442721857769029238784.0); // 3*2^67
							__m256d k52 = _mm256_set1_pd(0x0010'0000'0000'0000); // 2^52
							__m256d k3_67_52 = _mm256_set1_pd(442726361368656609280.0); // 3*2^67 + 2^52

							__m256i hi = _mm256_srai_epi32(data.sse_m256i, 16);
							hi = _mm256_blend_epi32(hi, zero, 0x55);
							hi = _mm256_add_epi64(hi, _mm256_castpd_si256(k3_67));
							__m256i lo = _mm256_blend_epi16(data.sse_m256i, _mm256_castpd_si256(k52), 0x88);
							__m256d dbl = _mm256_sub_pd(_mm256_castsi256_pd(hi), k3_67_52);
							pack.data.sse_m256d = _mm256_add_pd(dbl, _mm256_castsi256_pd(lo));
						}
						return pack;
#endif
					}
					else if constexpr (IsF32<U>)
					{
#if HAS_AVX512DQ && HAS_AVX512VL
						if constexpr (IsU64<T>)
							pack.data.sse_m256d = _mm256_castps128_ps256(_mm_cvtepu64_ps(data.sse_m256i));
						else
							pack.data.sse_m256d = _mm256_castps128_ps256(_mm_cvtepi64_ps(data.sse_m256i));
						return pack;
#endif
					}
					else if constexpr (IsU32<U> || IsI32<T>)
					{
#if HAS_AVX512F && HAS_AVX512VL
						pack.data.sse_m256d = _mm256_castps128_ps256(_mm_cvtepi64_epi32(data.sse_m256i));
						return pack;
#elif HAS_AVX2
						__m256i shuffleMask = _mm256_set_epi8(
							0x0B, 0x0A, 0x09, 0x08, 0x03, 0x02, 0x01, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
							0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0B, 0x0A, 0x09, 0x08, 0x03, 0x02, 0x01, 0x00
															  );
						__m256i shuffled = _mm256_shuffle_epi8(data.sse_m256i, shuffleMask);
						__m256i hi = _mm256_permute2x128_si256(shuffled, shuffled, 0x81);
						__m256i mixed = _mm256_or_si256(hi, shuffled);
						__m256i mask = _mm256_set_epi64x(0, 0, ~0ull, ~0ull);
						pack.data.sse_m256i = _mm256_and_si256(mixed, mask);
						return pack;
#endif
					}
					else if constexpr (IsU16<U> || IsI16<U>)
					{
#if HAS_AVX512F && HAS_AVX512VL
						pack.data.sse_m256d = _mm256_castps128_ps256(_mm_cvtepi64_epi16(data.sse_m256i));
						return pack;
#elif HAS_AVX2
						__m256i shuffleMask = _mm256_set_epi8(
							0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x09, 0x08, 0x01, 0x00, 0x80, 0x80, 0x80, 0x80,
							0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x09, 0x08, 0x01, 0x00
						);
						__m256i shuffled = _mm256_shuffle_epi8(data.sse_m256i, shuffleMask);
						__m256i hi = _mm256_permute2x128_si256(shuffled, shuffled, 0x81);
						__m256i mixed = _mm256_or_si256(hi, shuffled);
						__m256i mask = _mm256_set_epi64x(0, 0, 0, ~0ull);
						pack.data.sse_m256i = _mm256_and_si256(mixed, mask);
						return pack;
#endif
					}
					else if constexpr (IsU8<U> || IsI8<U>)
					{
#if HAS_AVX512F && HAS_AVX512VL
						pack.data.sse_m256d = _mm256_castps128_ps256(_mm_cvtepi64_epi8(data.sse_m256i));
						return pack;
#elif HAS_AVX2
						__m256i shuffleMask = _mm256_set_epi8(
							0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x08, 0x00, 0x80, 0x80,
							0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x08, 0x00
						);
						__m256i shuffled = _mm256_shuffle_epi8(data.sse_m256i, shuffleMask);
						__m256i hi = _mm256_permute2x128_si256(shuffled, shuffled, 0x81);
						__m256i mixed = _mm256_or_si256(hi, shuffled);
						__m256i mask = _mm256_set_epi32(0, 0, 0, 0, 0, 0, 0, ~0u);
						pack.data.sse_m256i = _mm256_and_si256(mixed, mask);
						return pack;
#endif
					}
				}
				else if constexpr (IsU32<T> || IsI32<T>)
				{
					if constexpr (IsF64<U>)
					{
						if (IsU32<T>)
						{
#if HAS_AVX512F && HAS_AVX512VL
							pack.data.sse_m256d = _mm256_cvtepu32_pd(_mm256_castsi256_si128(data.sse_m256i));
#else
							pack = Convert<u64>().Convert<f64>();
#endif
						}
						else
						{
#if HAS_AVX2
							pack.data.sse_m256d = _mm256_cvtepi32_pd(_mm256_castsi256_si128(data.sse_m256i));
#endif
						}
						return pack;

					}
					else if constexpr (IsF32<U>)
					{
#if HAS_AVX2
						pack.data.sse_m256 = _mm256_cvtepi32_ps(data.sse_m256i);
						return pack;
#endif
					}
					else if constexpr (IsU64<U> || IsI64<U>)
					{
#if HAS_AVX2
						if constexpr (IsU64<T>)
							pack.data.sse_m256i = _mm256_cvtepu32_epi64(_mm256_castsi256_si128(data.sse_m256i));
						else
							pack.data.sse_m256i = _mm256_cvtepi32_epi64(_mm256_castsi256_si128(data.sse_m256i));
						return pack;
#endif
					}
					else if constexpr (IsU16<U> || IsI16<U>)
					{
#if HAS_AVX512F && HAS_AVX512VL
						pack.data.sse_m256i = _mm256_castsi128_si256(_mm256_cvtepi32_epi16(data.sse_m256i));
#elif HAS_AVX2
						__m256i shuffleMask = _mm256_set_epi8(
							0x0D, 0x0C, 0x09, 0x08, 0x05, 0x04, 0x01, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
							0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0D, 0x0C, 0x09, 0x08, 0x05, 0x04, 0x01, 0x00
						);
						__m256i shuffled = _mm256_shuffle_epi8(data.sse_m256i, shuffleMask);
						__m256i hi = _mm256_permute2x128_si256(shuffled, shuffled, 0x81);
						__m256i mixed = _mm256_or_si256(hi, shuffled);
						__m256i mask = _mm256_set_epi64x(0, 0, ~0ull, ~0ull);
						pack.data.sse_m256i = _mm256_and_si256(mixed, mask);
						return pack;
#endif
					}
					else if constexpr (IsU8<U> || IsI8<U>)
					{
#if HAS_AVX512F && HAS_AVX512VL
						pack.data.sse_m256i = _mm256_castsi128_si256(_mm256_cvtepi32_epi8(data.sse_m256i));
#elif HAS_AVX2
						__m256i shuffleMask = _mm256_set_epi8(
							0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0C, 0x08, 0x04, 0x00, 0x80, 0x80, 0x80, 0x80,
							0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0C, 0x08, 0x04, 0x00
						);
						__m256i shuffled = _mm256_shuffle_epi8(data.sse_m256i, shuffleMask);
						__m256i hi = _mm256_permute2x128_si256(shuffled, shuffled, 0x81);
						__m256i mixed = _mm256_or_si256(hi, shuffled);
						__m256i mask = _mm256_set_epi64x(0, 0, 0, ~0ull);
						pack.data.sse_m256i = _mm256_and_si256(mixed, mask);
						return pack;
#endif
					}
				}
				else if constexpr (IsU16<T> || IsI16<T>)
				{
					if constexpr (IsF64<U>)
					{
						if constexpr (IsU16<T>)
							return Convert<u64>().Convert<f64>();
						else
							return Convert<i64>().Convert<f64>();
					}
					else if constexpr (IsF32<U>)
					{
						if constexpr (IsU16<T>)
							return Convert<u32>().Convert<f32>();
						else
							return Convert<i32>().Convert<f32>();
					}
					else if constexpr (IsU64<U> || IsI64<U>)
					{
						if constexpr (IsU16<T>)
							pack.data.sse_m256i = _mm256_cvtepu16_epi64(_mm256_castsi256_si128(data.sse_m256i));
						else
							pack.data.sse_m256i = _mm256_cvtepi16_epi64(_mm256_castsi256_si128(data.sse_m256i));
						return pack;
					}
					else if constexpr (IsU32<U> || IsI32<T>)
					{
						if constexpr (IsU16<T>)
							pack.data.sse_m256i = _mm256_cvtepu16_epi32(_mm256_castsi256_si128(data.sse_m256i));
						else
							pack.data.sse_m256i = _mm256_cvtepi16_epi32(_mm256_castsi256_si128(data.sse_m256i));
						return pack;
					}
					else if constexpr (IsU8<U> || IsI8<U>)
					{
#if HAS_AVX512BW && HAS_AVX512VL
						pack.data.sse_m256i = _mm256_castsi128_si256(_mm256_cvtepi16_epi8(data.sse_m256i));
						return pack;
#elif HAS_AVX2
						__m256i shuffleMask = _mm256_set_epi8(
							0x0E, 0x0C, 0x0A, 0x08, 0x06, 0x04, 0x02, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
							0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0E, 0x0C, 0x0A, 0x08, 0x06, 0x04, 0x02, 0x00
						);
						__m256i shuffled = _mm256_shuffle_epi8(data.sse_m256i, shuffleMask);
						__m256i hi = _mm256_permute2x128_si256(shuffled, shuffled, 0x81);
						__m256i mixed = _mm256_or_si256(hi, shuffled);
						__m256i mask = _mm256_set_epi64x(0, 0, ~0ull, ~0ull);
						pack.data.sse_m256i = _mm256_and_si256(mixed, mask);
						return pack;
#endif
					}
				}
				else if constexpr (IsU8<T> || IsI8<T>)
				{
					if constexpr (IsF64<U>)
					{
						if constexpr (IsU8<T>)
							return Convert<u64>().Convert<f64>();
						else
							return Convert<i64>().Convert<f64>();
					}
					else if constexpr (IsF32<U>)
					{
						if constexpr (IsU8<T>)
							return Convert<u32>().Convert<f32>();
						else
							return Convert<i32>().Convert<f32>();
					}
					else if constexpr (IsU64<U> || IsI64<U>)
					{
#if HAS_AVX2
						if (IsU8<T>)
							pack.data.sse_m256i = _mm256_cvtepu8_epi64(_mm256_castsi256_si128(data.sse_m256i));
						else
							pack.data.sse_m256i = _mm256_cvtepi8_epi64(_mm256_castsi256_si128(data.sse_m256i));
						return pack;
#endif
					}
					else if constexpr (IsU32<U> || IsI32<U>)
					{
#if HAS_AVX2
						if (IsU8<T>)
							pack.data.sse_m256i = _mm256_cvtepu8_epi32(_mm256_castsi256_si128(data.sse_m256i));
						else
							pack.data.sse_m256i = _mm256_cvtepi8_epi32(_mm256_castsi256_si128(data.sse_m256i));
						return pack;
#endif
					}
					else if constexpr (IsU16<U> || IsI16<U>)
					{
#if HAS_AVX2
						if (IsU8<T>)
							pack.data.sse_m256i = _mm256_cvtepu8_epi16(_mm256_castsi256_si128(data.sse_m256i));
						else
							pack.data.sse_m256i = _mm256_cvtepi8_epi16(_mm256_castsi256_si128(data.sse_m256i));
						return pack;
#endif
					}
				}
			}
		}

		return DefPackCvt<U, NewWidth>(*this);
	}
}

#if COMPILER_MSVC
#pragma warning(pop)
#endif