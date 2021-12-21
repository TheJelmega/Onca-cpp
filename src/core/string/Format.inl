#pragma once
#if __RESHARPER__
#include "Format.h"
#endif

#include "Format.h"
#include "core/math/MathUtils.h"

namespace Core
{
	namespace Detail::Format
	{
		template<u32 Divisor, u32 MaxPrecision, u32 AdditionalPrecision>
		constexpr auto FastDiv(u32 n) noexcept -> QuotientRemainderPair
		{
			constexpr u32 leftEnd = u32(((1u << (MaxPrecision + AdditionalPrecision)) + Divisor - 1) / Divisor);
			constexpr u32 rightEnd = u32(((1u << AdditionalPrecision) * ((1 << MaxPrecision) + 1)) / Divisor);

			// Ensure sufficient precision
			STATIC_ASSERT(leftEnd <= rightEnd, "DragonBox: Not enough precision");
			// Ensure no overflow
			STATIC_ASSERT(leftEnd <= u32(1u << (32 - MaxPrecision)), "DragonBox: Overflow");

			u32 quotient = (n * leftEnd) >> (MaxPrecision + AdditionalPrecision);
			u32 remainder = n - Divisor * quotient;
			return { quotient, remainder };
		}

		template <Integral I>
		auto CountDecDigits(I i) noexcept -> i32
		{
			u8 log2_10 = Log2Digits10[Intrin::BitScanMSB(i)];
			return log2_10 - (i < I(PowOf10[log2_10]));
		}

		template <Integral I>
		auto FormatIntegerDecToBuf(I val, char* buffer) noexcept -> char*
		{
			if (val == 0)
			{
				*buffer++ = '0';
				return buffer;
			}

			usize digits = CountDecDigits(val);
			char* bufferPtr = buffer + digits;
			char* bufferEnd = bufferPtr;

			while (val >= 1'000)
			{
				I rem = val % 10'000;
				val /= I(10'000);

				// c1 = rem / 100; c2 = rem % 100
				auto [c1, c2] = FastDiv<100, 14, 5>(u32(rem));

				bufferPtr -= 4;
				MemCpy(bufferPtr + 2, &Digits100[c2 * 2], 2);
				MemCpy(bufferPtr, &Digits100[c1 * 2], 2);
			}

			while (val >= 10)
			{
				usize idx = (val % 100) * 2;
				bufferPtr -= 2;
				MemCpy(bufferPtr, &Digits100[idx], 2);
				val /= 100;
			}
			if (val > 0)
				*--bufferPtr = char('0' + val);
			return bufferEnd;
		}

		template <Integral I, u8 NumBits>
		auto FormatIntegerHexToBuf(I val, bool upper, char* buffer) noexcept -> char*
		{
			constexpr u8 SizeOfI = sizeof(I) * 8;
			constexpr u8 NumNibbles = (NumBits + 3) / 4;
			constexpr u8 FrontOffset = SizeOfI - (sizeof(I) * 2 - NumNibbles) * 4 - 4;
			constexpr u8 FirstMask = 0xF >> (NumNibbles * 4 - NumBits);

			char* buffPtr = buffer + NumNibbles;

			const char* chars = upper ? "0123456789ABCDEF" : "0123456789abcdef";
			for (u8 toWrite = 0; toWrite < NumNibbles - 1; ++toWrite)
			{
				*--buffPtr = chars[(val >> (toWrite * 4)) & 0x0F];
			}
			*--buffPtr = chars[(val >> FrontOffset) & FirstMask];
			return buffer + NumNibbles;
		}


		template <Integral I>
		auto FormatIntegerHex(I val, bool upper) noexcept -> String
		{
			constexpr usize bufferSize = sizeof(I) * 2 + 2;
			char buffer[bufferSize];
			MemSet(buffer, '0', bufferSize);
			buffer[1] = 'x';

			FormatIntegerHexToBuf(val, upper, buffer + 2);
			return String{ buffer, bufferSize };
		}

		template <Integral I>
		auto FormatIntegerOct(I val) noexcept -> String
		{
			constexpr u8 bufSizes[] = { 3, 6, 0, 11, 0, 0, 0, 22 };
			constexpr u8 bufferSize = bufSizes[sizeof(I) - 1] + 1;
			char buffer[bufferSize];
			MemSet(buffer, '0', bufferSize);
			buffer[1] = 'o';

			char* bufferPtr = buffer + bufferSize - 1;
			do
			{
				const char* chars = "01234567";
				*bufferPtr-- = chars[val & 0x07];
			}
			while (val >>= 3);
			return String{ buffer, bufferSize };
		}

		template <Integral I>
		auto FormatIntegerBin(I val) noexcept -> String
		{
			const char* nibbles[] =
			{
				"0000", "0001", "0010", "0011"
				"0100", "0101", "0110", "0111",
				"1000", "1001", "1010", "1011",
				"1100", "1101", "1110", "1111",
			};

			constexpr u8 bufferSize = sizeof(I) * 8 + 2;
			char buffer[bufferSize];
			MemSet(buffer, '0', bufferSize);
			buffer[1] = 'b';

			char* bufferPtr = buffer + bufferSize - 4;
			do
			{
				MemCpy(bufferPtr, nibbles[val & 0x0F], 4);
				bufferPtr -= 4;
			} while (val >>= 4);
			return String{ buffer, bufferSize };
		}

		template <Integral I>
		auto FormatIntegerDec(I val) noexcept -> String
		{
			constexpr u8 bufferSize = Math::Consts::Digits10<I>;
			char buffer[bufferSize];

			char* bufferEnd = FormatIntegerDecToBuf(val, buffer);
			return String{ reinterpret_cast<char*>(buffer), bufferEnd };
		}

		template <Integral I>
		auto FormatIntegerSci(I val, u8 precision) noexcept -> String
		{
			if (val == 0)
				return "0e0"_s;

			constexpr u8 bufferSize = Math::Consts::Digits10<I> + 3;
			char buffer[bufferSize];

			usize digits = CountDecDigits(val);
			precision = Math::Min(precision, digits);

			usize expIdx = precision;
			char* bufferPtr = buffer + precision;

			val /= Format::PowOf10[digits - precision + 1];
			while (val > 10)
			{
				usize idx = (val % 100) * 2;
				bufferPtr -= 2;
				MemCpy(buffer, &Digits100[idx], 2);
				val /= 100;
			}
			if (val > 0)
				*--bufferPtr = char('0' + val);

			buffer[expIdx++] = 'e';

			if (digits >= 10)
			{
				MemCpy(*buffer[precision], &Digits100[digits + 2], 2);
				expIdx += 2;
			}
			else
			{
				buffer[expIdx++] = char('0' + digits);
			}
			return String{ buffer, expIdx };
		}


		template <FloatingPoint F>
		auto FormatFloatHex(F val, bool upper) noexcept -> String
		{
			FloatUtils::Carrier<F> bits = Math::FloatBitsToUInt(val);
			i32 exp = FloatUtils::ExtractExpBits(bits);
			FloatUtils::Carrier<F> sig = FloatUtils::RemoveExpBits(bits, exp);

			constexpr u8 expOffset = IsF32<F> ? 10 : 17;

			constexpr usize bufferSize = sizeof(F) * 2 + 6;
			char buffer[bufferSize];
			MemSet(buffer, '0', bufferSize);
			buffer[1] = 'x';
			buffer[2] = '1';
			buffer[3] = '.';
			buffer[expOffset] = 'p';

			FormatIntegerHexToBuf<FloatUtils::Carrier<F>, Math::Consts::SignificandBits<F>>(sig, true, buffer + 4);

			exp += Math::Consts::ExpBias<F>;
			buffer[expOffset + 1] = exp < 0 ? '-' : '+';
			FormatIntegerHexToBuf<FloatUtils::Carrier<F>, Math::Consts::ExpDigits<F>>(exp, upper, buffer + expOffset + 2);

			return String{ buffer, bufferSize };
		}

		template <FloatingPoint F>
		auto FormatFloatSci(UnsignedOfSameSize<F> significand, i32 exp, u8 precision) noexcept -> String
		{
			char buffer[MaxFloatSciOutputStringLen<F>];
			char* end = buffer;

			if constexpr (IsF32<F>)
			{
				i32 remainingDigitsMinus1 = significand == 0 ? 0 : CountDecDigits(significand) - 1;
				exp += remainingDigitsMinus1;
				if (remainingDigitsMinus1 > precision)
				{
					significand /= UnsignedOfSameSize<F>(PowOf10[remainingDigitsMinus1 - precision + 1]);
					remainingDigitsMinus1 = precision;
				}

				i32 expPos = remainingDigitsMinus1 + 2;

				while (remainingDigitsMinus1 >= 4)
				{
					u32 c = significand % 10'000;
					significand /= 10'000;

					// c1 = c / 100, c2 = c % 100
					auto [c1, c2] = FastDiv<100, 14, 5>(c);

					MemCpy(end + remainingDigitsMinus1, &Digits100[c2 * 2], 2);
					MemCpy(end + remainingDigitsMinus1 - 2, &Digits100[c1 * 2], 2);
					remainingDigitsMinus1 -= 4;
				}

				if (remainingDigitsMinus1 >= 2)
				{
					// c1 = c / 100, c2 = c % 100
					auto [c1, c2] = FastDiv<100, 14, 5>(significand);
					significand = c1;

					MemCpy(end + remainingDigitsMinus1, &Digits100[c2 * 2], 2);
					remainingDigitsMinus1 -= 2;
				}

				if (remainingDigitsMinus1 > 0)
				{
					ASSERT(remainingDigitsMinus1 == 1, "Format: Invalid remaining digits");

					// c1 = c / 10, c2 = c % 10
					auto [d1, d2] = FastDiv<10, 7, 3>(significand);

					end[0] = char('0' + d1);
					end[1] = '.';
					end[2] = char('0' + d2);
					end += expPos;
				}
				else
				{
					end[0] = char('0' + significand);

					// If the significand is of 1 digit, do not print the decimal dot
					if (expPos != 2)
					{
						end[1] = '.';
						end += expPos;
					}
					else
					{
						++end;
					}
				}

				// exp
				if (exp < 0)
				{
					*end++ = 'e';
					*end++ = '-';
					exp = -exp;
				}
				else
				{
					*end++ = 'e';
				}

				if (exp > 10)
				{
					MemCpy(end, &Digits100[exp * 2], 2);
					end += 2;
				}
				else
				{
					*end++ = char('0' + exp);
				}

				return String{ reinterpret_cast<char*>(buffer), end };
			}
			else
			{
				u32 s32;
				i32 remainingDigitsMinus1;
				i32 expPos;
				bool mayHaveMoreTrailingZeros = false;

				if ((significand >> 32) != 0)
				{
					// Significand is at most 10^17, the quotient is at most 10^9,
					// so it fits inside a 32-bit integer
					s32 = u32(significand / 100'000'000);
					u32 r = significand - s32 * 100'000'000;
					
					remainingDigitsMinus1 = CountDecDigits(s32) - 1;
					exp += remainingDigitsMinus1;
					expPos = remainingDigitsMinus1 + 2;

					if (r != 0)
					{
						u32 c = r % 10'000;
						r /= 10'000;

						// c1 = r / 100; c2 = r % 100
						auto [c1, c2] = FastDiv<100, 14, 5>(r);
						// c2 = s32 c / 100; c4 = c % 100
						auto [c3, c4] = FastDiv<100, 14, 5>(c);

						i8 tz = TrailingZeroCountTable[c4];
						if (tz == 0)
						{
							goto PrintC4Label_;
						}
						else if (tz == 1)
						{
							MemCpy(end + remainingDigitsMinus1, &Digits100[c4 * 2], 1);
							expPos -= 1;
							goto PrintC3Label_;
						}

						tz = TrailingZeroCountTable[c3];
						if (tz == 0)
						{
							goto PrintC3Label_;
						}
						else if (tz == 1)
						{
							MemCpy(end + remainingDigitsMinus1, &Digits100[c4 * 2], 1);
							expPos -= 1;
							goto PrintC2Label_;
						}

						tz = TrailingZeroCountTable[c2];
						if (tz == 0)
						{
							goto PrintC2Label_;
						}
						else if (tz == 1)
						{
							MemCpy(end + remainingDigitsMinus1, &Digits100[c4 * 2], 1);
							expPos -= 1;
							goto PrintC1Label_;
						}

						tz = TrailingZeroCountTable[c2];
						if (tz == 0)
						{
							goto PrintC1Label_;
						}

						ASSERT("tz == 1", "DragonBox: Invalid value");
						MemCpy(end + remainingDigitsMinus1 - 6, &Digits100[c1 * 2], 1);
						expPos -= 7;
						goto AfterPrintLable;


					PrintC4Label_:
						MemCpy(end + remainingDigitsMinus1, &Digits100[c4 * 2], 2);

					PrintC3Label_:
						MemCpy(end + remainingDigitsMinus1 - 2, &Digits100[c3 * 2], 2);

					PrintC2Label_:
						MemCpy(end + remainingDigitsMinus1 - 4, &Digits100[c2 * 2], 2);

					PrintC1Label_:
						MemCpy(end + remainingDigitsMinus1 - 6, &Digits100[c1 * 2], 2);

					AfterPrintLable:;
					}
					else
					{
						expPos -= 8;
						mayHaveMoreTrailingZeros = true;
					}
					remainingDigitsMinus1 -= 8;
				}
				else
				{
					s32 = u32(significand);
					remainingDigitsMinus1 = s32 >= 1'000'000'000 ? 9 : CountDecDigits(s32) - 1;
					exp += remainingDigitsMinus1;
					expPos = remainingDigitsMinus1 + 2;
					mayHaveMoreTrailingZeros = true;
				}

				while (remainingDigitsMinus1 >= 4)
				{
					u32 c = s32 % 10'000;
					s32 /= 10'000;

					// c1 = c / 100; c2 = c % 100
					auto [c1, c2] = FastDiv<100, 14, 5>(c);

					if (mayHaveMoreTrailingZeros)
					{
						i8 tz = TrailingZeroCountTable[c2];
						if (tz == 0)
						{
							mayHaveMoreTrailingZeros = false;
							goto InsideLoopPrintC2Label_;
						}
						else if (tz == 1)
						{
							mayHaveMoreTrailingZeros = false;
							expPos -= 2;
							MemCpy(end + remainingDigitsMinus1, &Digits100[c2 * 2], 1);
							goto InsideLoopPrintC1Label_;
						}

						i8 tz = TrailingZeroCountTable[c1];
						if (tz == 0)
						{
							mayHaveMoreTrailingZeros = false;
							goto InsideLoopPrintC1Label_;
						}
						else if (tz == 1)
						{
							mayHaveMoreTrailingZeros = false;
							expPos -= 3;
							MemCpy(end + remainingDigitsMinus1 - 2, &Digits100[c1 * 2], 1);
							goto InsideLoopAfterPrintLabel_;
						}
						expPos -= 4;
						goto InsideLoopAfterPrintLabel_;
					}

				InsideLoopPrintC2Label_:
					MemCpy(end + remainingDigitsMinus1, &Digits100[c2 * 2], 2);

				InsideLoopPrintC1Label_:
					MemCpy(end + remainingDigitsMinus1 - 2, &Digits100[c1 * 2], 2);

				InsideLoopAfterPrintLabel_:
					remainingDigitsMinus1 -= 4;
				}

				if (remainingDigitsMinus1 >= 2)
				{
					// c1 = s32 / 100; c2 = s32 % 100
					auto [c1, c2] = FastDiv<100, 14, 5>(s32);
					s32 = c1;

					if (mayHaveMoreTrailingZeros)
					{
						i8 tz = TrailingZeroCountTable[c2];
						expPos -= 2;
						if (tz == 0)
						{
							MemCpy(end + remainingDigitsMinus1, &Digits100[c2 * 2], 2);
							mayHaveMoreTrailingZeros = false;
						}
						else if (tz == 1)
						{
							MemCpy(end + remainingDigitsMinus1, &Digits100[c2 * 2], 1);
							mayHaveMoreTrailingZeros = false;
						}
					}
					else
					{
						MemCpy(end + remainingDigitsMinus1, &Digits100[c2 * 2], 2);
					}
					remainingDigitsMinus1 -= 2;
				}

				if (remainingDigitsMinus1 > 0)
				{
					ASSERT(remainingDigitsMinus1 == 1, "DragonBox: Invalid value");

					// d1 = s32 / 10, d2 = s32 % 10
					auto [d1, d2] = FastDiv<10, 7, 3>(s32);

					end[0] = char('0' + d1);
					if (mayHaveMoreTrailingZeros)
					{
						++end;
					}
					else
					{
						end[1] = '.';
						end[2] = char('0' - d2);
						end += expPos;
					}
				}
				else
				{
					end[0] = char('0' + s32);

					if (mayHaveMoreTrailingZeros)
					{
						++end;
					}
					else
					{
						end[1] = '.';
						end += expPos;
					}
				}

				// exp
				if (exp < 0)
				{
					*end++ = 'e';
					*end++ = '-';
					exp = -exp;
				}
				else
				{
					*end++ = 'e';
				}

				if (exp >= 100)
				{
					// d1 = exp / 100; d2 = exp % 100
					auto [d1, d2] = FastDiv<100, 10, 3>(exp);
					MemCpy(end, &Digits100[d1 * 2], 2);
					end[2] = char('0' + d2);
					end += 3;
				}
				else if (exp > 10)
				{
					MemCpy(end, &Digits100[exp * 2], 2);
					end += 2;
				}
				else
				{
					*end++ = char('0' + exp);
				}

				return String{ buffer, end };
			}
		}
		
		template <FloatingPoint F>
		auto FormatFloatingPoint(UnsignedOfSameSize<F> significand, i32 exp, u8 precision) noexcept -> String
		{
			if (significand == 0)
				return 0;

			if (exp > 9 || exp < -7)
				return FormatFloatSci<F>(significand, exp, precision);

			char buffer[MaxFloatDecOutputStringLen<F>] = {};
			char* bufferPtr = buffer;

			i32 digits = CountDecDigits(significand);
			exp += digits;
			if (exp >= 0)
			{
				u64 pow10 = PowOf10[digits - exp + 1];
				u64 decimal = significand % pow10;
				u64 integral = significand / pow10;

				bufferPtr = FormatIntegerDecToBuf(integral, bufferPtr);

				if (precision == u8(-1))
				{
					*bufferPtr++ = '.';
					bufferPtr = FormatIntegerDecToBuf(decimal, bufferPtr);
				}
				else if (precision > 0)
				{
					*bufferPtr++ = '.';

					i32 decDigits = CountDecDigits(decimal);
					if (decDigits > precision)
						decimal /= PowOf10[decDigits - precision + 1];
					bufferPtr = FormatIntegerDecToBuf(decimal, bufferPtr);
				}
				return String{ reinterpret_cast<char*>(buffer), bufferPtr };
			}
			else
			{
 				return String{ DecPrefix[-exp], usize(2 - exp) } + FormatIntegerDec(significand);
			}
		}

		template <typename First, typename ... Args>
		auto FormatArg(usize idx, const String& options, const First& first, const Args&... other) noexcept -> String
		{
			if (idx == 0)
				return ToFormat(first, options);

			if constexpr (sizeof...(Args) == 0)
				return "<INVALID>";
			else
				return FormatArg(idx - 1, options, other...);
		}

		template <typename First, typename ... Args>
		auto FormatDebugArg(usize idx, const First& first, const Args&... other) noexcept -> String
		{
			if (idx == 0)
			{
				if constexpr (DebugStringifyable<First>)
					return ToDebugString(first);
				else if constexpr (Stringifyable<First>)
					return ToString(first);
				else
					return "<NO_DEBUG_STRING>";
			}

			if constexpr (sizeof...(Args) == 0)
				return "<INVALID>";
			else
				return FormatDebugArg(idx - 1, other...);
		}

		template <FloatingPoint F>
		auto FormatFloatSci(F val) noexcept -> String
		{
			auto res = DragonBox::ToDecimal(val);
			return FormatFloatingPoint(res.significand, res.exponent);
		}
	}

	template<ToFormatableMethod T>
	auto ToFormat(const T& t, const String& options) noexcept -> String
	{
		return t.ToFormat(options);
	}

	inline auto ToFormat(const String& str, const String&) noexcept -> String
	{
		return str;
	}

	inline auto ToFormat(bool val, const String& options) noexcept -> String
	{
		return val ? "true"_s : "false"_s;
	}

	template <usize N>
	auto ToFormat(const ConstString<N>& str, const String&) noexcept -> String
	{
		return String{ str.Begin(), str.End() };
	}

	template <CharacterType C>
	auto ToFormat(const C* cstr, const String& options) noexcept -> String
	{
		return String{ cstr };
	}

	template <Integral I>
	auto ToFormat(I val, const String& options) noexcept -> String
	{
		bool neg = val < 0;
		bool showSign = options.StartsWith('+');
		String str = neg ? "-"_s : showSign ? "+"_s : ""_s;

		if (options.Length() > usize(showSign))
		{
			if (options[showSign] == 'x' || options[showSign] == 'X')
				return str + Detail::Format::FormatIntegerHex(val, options[showSign] == 'X');
			if (options[showSign] == '0')
				return str + Detail::Format::FormatIntegerOct(val);
			if (options[showSign] == 'b')
				return str + Detail::Format::FormatIntegerBin(val);
		}

		return str + Detail::Format::FormatIntegerDec(val);
	}

	template <FloatingPoint F>
	auto ToFormat(F val, const String& options) noexcept -> String
	{
		using Carrier = UnsignedOfSameSize<F>;
		const Carrier br = Math::FloatBitsToUInt(val);
		const u32 exp = FloatUtils::ExtractExpBits(br);
		const Carrier s = FloatUtils::RemoveExpBits(br, exp);

		if (FloatUtils::IsFinite(exp))
		{
			String signStr;
			bool showSign = options.StartsWith('+');
			if (FloatUtils::IsNegative(br))
				signStr = "-"_s;
			else if (showSign)
				signStr = "+"_s;

			if (FloatUtils::IsNonZero(br))
			{
				if (options.Length() > usize(showSign))
				{
					if (options[showSign] == 'x' || options[showSign] == 'X')
						return "0"_s; // TODO
					if (options[showSign] == 'e')
					{
						u8 precision = u8(-1);
						if (options.Length() > showSign + 1)
							precision = Parse<u8>(options.SubString(showSign + 2));
						DragonBox::DecimalFP<FloatUtils::Carrier<F>> res = DragonBox::ToDecimal<F>(s, exp);
						return Detail::Format::FormatFloatSci<F>(res.significand, res.exponent, precision);
					}
				}

				u8 precision = u8(-1);
				if (options.Length() > showSign + 1 && options[showSign] == '.')
					precision = Parse<u8>(options.SubString(showSign + 1));

				DragonBox::DecimalFP<FloatUtils::Carrier<F>> res = DragonBox::ToDecimal<F>(s, exp);
				return Detail::Format::FormatFloatingPoint<F>(res.significand, res.exponent, precision);
			}
			return "0"_s;
		}
		else if (FloatUtils::HasAllZeroSignificandBits(br))
		{
			if (FloatUtils::IsNegative(br))
				return "-inf"_s;
			return "inf"_s;
		}
		else
		{
			return "NaN"_s;
		}
	}

	template <ToFormatableMethod T>
	auto ToFormat(const T* val, const String& options) noexcept -> String
	{
		if (options.StartsWith('*'))
		{
			if (val)
				return ToFormat(val, options.SubString(1));
			return "<NULL>"_s;
		}

		return Detail::Format::FormatIntegerHex(usize(val), options.Length() != 1 || options[0] != 'x');
	}

	template <Formatable ... Args>
	auto Format(const String& format, Args... args) noexcept -> String
	{
		if constexpr (sizeof...(Args) == 0)
			return format;

		String res;

		usize pos = format.Find('{');
		if (pos == String::NPos)
			return format;

		usize len = format.Length();
		usize lastPos = 0;
		usize nextArgIdx = 0;
		do
		{
			if (len > pos + 1 && format[pos + 1] == '{')
			{
				res += format.SubString(lastPos, pos - lastPos + 1);
				lastPos = pos + 2;
				pos = format.Find('{', lastPos);
				continue;
			}

			usize end = format.Find('}', pos + 1);
			if (end == String::NPos)
			{
				// Should not happen
				ASSERT(false, "String Format, could not find '}'");
				continue;
			}

			res += format.SubString(lastPos, pos - lastPos);

			usize optLen = end - pos - 1;
			String options = format.SubString(pos + 1, optLen);
			usize optCommaIdx = options.Find(',');
			usize optColonIdx = options.Find(':');

			usize argIdx = nextArgIdx;
			usize padding = 0;
			bool padFront = false, padBack = false;
			if (optColonIdx != String::NPos)
			{
				if (optCommaIdx != String::NPos)
				{
					String tmp = options.SubString(0, optCommaIdx);
					if (!tmp.IsEmpty())
						argIdx = Parse<usize>(tmp);

					tmp = options.SubString(optCommaIdx + 1, optColonIdx - optCommaIdx - 1);
					if (!tmp.IsEmpty())
					{
						if (tmp.StartsWith('+'))
						{
							padBack = true;
							tmp.Erase(0, 1);
						}
						else if (tmp.StartsWith('-'))
						{
							padFront = true;
							tmp.Erase(0, 1);
						}

						padding = Parse<usize>(tmp);
					}
				}
				else
				{
					String tmp = options.SubString(0, optColonIdx);
					if (!tmp.IsEmpty())
						argIdx = Parse<usize>(tmp);
				}

				options.Erase(0, optColonIdx + 1);
			}
			else
			{
				if (optCommaIdx != String::NPos)
				{
					String tmp = options.SubString(0, optCommaIdx);
					if (!tmp.IsEmpty())
						argIdx = Parse<usize>(tmp);

					tmp = options.SubString(optCommaIdx + 1, optLen - optCommaIdx - 1);
					if (!tmp.IsEmpty())
					{
						if (tmp.StartsWith('+'))
						{
							padBack = true;
							tmp.Erase(0, 1);
						}
						else if (tmp.StartsWith('-'))
						{
							padFront = true;
							tmp.Erase(0, 1);
						}

						padding = Parse<usize>(tmp);
					}
				}
				else
				{
					String tmp = options.SubString(0);
					if (!tmp.IsEmpty())
						argIdx = Parse<usize>(tmp);
				}

				options = ""_s;
			}

			String formattedArg;
			if (options.Length() == 1 && options[0] == '?')
				formattedArg = Detail::Format::FormatDebugArg(argIdx, args...);
			else
				formattedArg = Detail::Format::FormatArg(argIdx, options, args...);

			if (padding > formattedArg.Length())
			{
				padding -= formattedArg.Length();
				if (padFront)
				{
					formattedArg.PadLeft(padding);
				}
				else if (padBack)
				{
					formattedArg.PadRight(padding);
				}
				else
				{
					usize frontPadding = padding / 2;
					usize backPadding = padding - frontPadding;
					formattedArg.PadLeft(frontPadding);
					formattedArg.PadRight(backPadding);
				}
			}
			res += formattedArg;

			lastPos = end + 1;
			pos = format.Find('{', lastPos);
			nextArgIdx = argIdx + 1;
		}
		while (pos != String::NPos);

		return res + format.SubString(lastPos).Replace("}}"_s, '}');
	}
}
