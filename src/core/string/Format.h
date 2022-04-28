#pragma once
#include "String.h"
#include "core/math/IntUtils.h"
#include "Interfaces.h"

namespace Onca
{
	namespace Detail::Format
	{
		constexpr u8 Log2Digits10[] =
		{
			1, 1, 1,
			2, 2, 2,
			3, 3, 3,
			4, 4, 4, 4,
			5, 5, 5,
			6, 6, 6,
			7, 7, 7, 7,
			8, 8, 8,
			9, 9, 9,
			10, 10, 10, 10,
			11, 11, 11,
			12, 12, 12,
			13, 13, 13, 13,
			14, 14, 14,
			15, 15, 15,
			16, 16, 16, 16,
			17, 17, 17,
			18, 18, 18,
			19, 19, 19, 19,
			20
		};

		constexpr u64 PowOf10[] =
		{
			0,
			0,
			10,
			100,
			1000,
			10000,
			100000,
			1000000,
			10000000,
			100000000,
			1000000000,
			10000000000,
			100000000000,
			1000000000000,
			10000000000000,
			100000000000000,
			1000000000000000,
			10000000000000000,
			100000000000000000,
			1000000000000000000,
		};

		constexpr const char* DecPrefix[]
		{
			"",
			"0.",
			"0.0",
			"0.00",
			"0.000",
			"0.0000",
			"0.00000",
			"0.000000",
		};

		constexpr char Digits100[] =
			"00010203040506070809"
			"10111213141516171819"
			"20212223242526272829"
			"30313233343536373839"
			"40414243444546474849"
			"50515253545556575859"
			"60616263646566676869"
			"70717273747576777879"
			"80818283848586878889"
			"90919293949596979899";

		static constexpr i8 TrailingZeroCountTable[] = {
			2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};

		struct QuotientRemainderPair
		{
			u32 quotient;
			u32 remainder;
		};

		template<u32 Divisor, u32 MaxPrecision, u32 AdditionalPrecision>
		constexpr auto FastDiv(u32 n) noexcept -> QuotientRemainderPair;

		/**
		 * Calculate the number of decimal digits in an integer
		 * \tparam I Integral type
		 * \param[in] i value
		 * \return Number of decimal digits
		 */
		template<Integral I>
		auto CountDecDigits(I i) noexcept -> i32;

		/**
		 * Format an integer as a decimal number to a buffer (from back to front)
		 * \tparam I Integral type
		 * \param[in] val Value
		 * \param[in] buffer Pointer to the buffer
		 * \return Pointer to element after the last written element in the buffer
		 */
		template<Integral I>
		auto FormatIntegerDecToBuf(I val, char* buffer) noexcept -> char*;
		/**
		 * Format an integer as a hexadecimal number
		 * \tparam I Integral type
		 * \tparam NumBits Number of bits to write
		 * \param[in] val Value
		 * \param[in] upper Whether the hexadecimal should be in upper case
		 * \param[in] buffer Pointer to the buffer
		 * \return Pointer to element after the last written element in the buffer
		 */
		template<Integral I, u8 NumBits = sizeof(I) * 8>
		auto FormatIntegerHexToBuf(I val, bool upper, char* buffer) noexcept -> char*;

		/**
		 * Format an integer as a hexadecimal number
		 * \tparam I Integral type
		 * \param[in] val Value
		 * \param[in] upper Whether the hexadecimal should be in upper case
		 * \return String
		 */
		template<Integral I>
		auto FormatIntegerHex(I val, bool upper) noexcept -> String;
		/**
		 * Format an integer as an octal number
		 * \tparam I Integral type
		 * \param[in] val Value
		 * \return String
		 */
		template<Integral I>
		auto FormatIntegerOct(I val) noexcept -> String;
		/**
		 * Format an integer as a binary number
		 * \tparam I Integral type
		 * \param[in] val Value
		 * \return String
		 */
		template<Integral I>
		auto FormatIntegerBin(I val) noexcept -> String;

		
		/**
		 * Format an integer as a decimal number
		 * \tparam I Integral type
		 * \param[in] val Value
		 * \return String
		 */
		template<Integral I>
		auto FormatIntegerDec(I val) noexcept -> String;
		/**
		 * Format an integer as a scientific notation number
		 * \tparam I Integral type
		 * \param[in] val Value
		 * \return String
		 */
		template<Integral I>
		auto FormatIntegerSci(I val, u8 precision) noexcept -> String;

		template<FloatingPoint F>
		constexpr usize MaxFloatSciOutputStringLen = IsF32<F> ?
			// sign(1) + significand(9) + decimal point(1) + exp marker(1) + exp sign(1) + exp(2)
			(1 + 9 + 1 + 1 + 1 + 2) :
			// sign(1) + significand(17) + decimal point(1) + exp marker(1) + exp sign(1) + exp(3)
			(1 + 17 + 1 + 1 + 1 + 3);

		template<FloatingPoint F>
		constexpr usize MaxFloatDecOutputStringLen = IsF32<F> ?
			// sign(1) + decPrefix(8) + significand(9) + decimal point(1)
			(1 + 8 + 9 + 1) :
			// sign(1)  + decPrefix(8) + significand(17) + decimal point(1)
			(1 + 17 + 9 + 1);

		template<FloatingPoint F>
		auto FormatFloatHex(F val, bool upper) noexcept -> String;
		template<FloatingPoint F>
		auto FormatFloatSci(UnsignedOfSameSize<F> significand, i32 exp, u8 precision) noexcept -> String;

		template<FloatingPoint F>
		auto FormatFloatingPoint(UnsignedOfSameSize<F> significand, i32 exp, u8 precision) noexcept -> String;

		template<typename First, typename... Args>
		auto FormatArg(usize idx, const String& options, const First& first, const Args&... other) noexcept -> String;
		template<typename First, typename... Args>
		auto FormatDebugArg(usize idx, const First& first, const Args&... other) noexcept -> String;
	}

	/**
	 * Convert any type that has a ToString() method to a string via a free function
	 * \tparam T Type
	 * \param[in] t Value
	 * \return String
	 */
	template<ToFormatableMethod T>
	auto ToFormat(const T& t, const String& options = ""_s) noexcept -> String;

	/**
	 * Default format case for string
	 * \param[in] str String
	 * \return String
	 */
	auto ToFormat(const String& str, const String& = ""_s) noexcept -> String;
	/**
	 * Default format case for string
	 * \param[in] str String
	 * \return String
	 */
	template<usize N>
	auto ToFormat(const ConstString<N>& str, const String & = ""_s) noexcept -> String;

	/**
	 * Convert a c-string to a string
	 * \tparam C Character type
	 * \param[in] cstr c-string
	 * \return String
	 */
	template<CharacterType C>
	auto ToFormat(const C* cstr, const String& options = ""_s) noexcept -> String;

	template<Integral I>
	auto ToFormat(I val, const String& options = ""_s) noexcept -> String;

	template<FloatingPoint F>
	auto ToFormat(F val, const String& options = ""_s) noexcept -> String;
	
	auto ToFormat(bool val, const String& options = ""_s) noexcept -> String;

	template<ToFormatableMethod T>
	auto ToFormat(const T* val, const String& options = ""_s) noexcept -> String;

	/**
	 * Format a string using the given arguments, based on the following format
	 *
	 * {[<index>][,<alignment>]:[<format>]}
	 * - index of the argument, if not provided, the index will either be 0 at the start of the format, or the proceeding index + 1
	 * - alignment of the formatted argument, this is the minimum amount of characters the formatted argument will take in, a negative value will pad at the front, a positive argument will pad at the back
	 * - format string of the arguments, below is a list of formatting options based on the type of the argument
	 *
	 * Alignment options:
	 * - 'N' where N is an integer, pad the argument to at least N characters, with the argument itself being centered
	 * - '+N' where N is an integer, pad the argument to at least N characters, with the padding added behind the argument
	 * - '-N' where N is an integer, pad the argument to at least N characters, with the padding added before the argument
	 *
	 * Global formatting options:
	 * - '?' : Format as a debug string, using ToDebugString. Has to be used alone (expect for pointers)
	 *
	 * Integer formatting options:
	 * - '+' : Always show the sign, can be used in combination with other options by putting it as the first character
	 * - 'x' : Show as lower case hexadecimal
	 * - 'X' : Show as upper case hexadecimal
	 * - 'b' : Show as binary
	 * - 'o' : Show as octal
	 * - 'e.<precision>': where '<precision>' is an integer, format as scientific notation, with a given precision
	 *
	 * Floating point formatting options:
	 * - '+' : Always show the sign
	 * - 'x' : Show as a lower case hex float
	 * - 'X' : Show as a upper case hex float
	 * - 'e' : Show in scientific notation
	 * - '.<precision>': where '<precision>' is an integer, format with <precision> decimals after the dot
	 * - 'e.<precision>': where '<precision>' is an integer, format as scientific notation, with <precision> decimals after the dot
	 *
	 * when it comes to floating point precision, a 'f32' supports up to 9 digits and a 'f64' support up to 17 digits
	 *
	 * Pointer formatting options (all pointers, except for character pointers, as they are handled as c-strings):
	 * - '*'  : Format the content of the dereferenced pointer
	 * - '*?' : Format as the debug string of the dereferenced pointer
	 * - 'x'  : Format as lower case hexadecimal
	 * - 'X'  : Show as upper case hexadecimal (default pointer option)
	 *
	 * If formatting options are not provided and the argument supports ToFormat, pass the options to the function. For more info about a specific types formatting options, see its ToFormat() documentation
	 *
	 * To use '{' or '}', an escaped version needs to be used, i.e. '{{' or '}}'
	 *
	 * \brief Format a string using the given arguments
	 * \tparam Args Argument types
	 * \param[in] format Format template
	 * \param[in] args Arguments
	 * \return Formatted string
	 * \note If no arguments are passed, the format string will be returned
	 */
	template<typename... Args>
	auto Format(const String& format, const Args&... args) noexcept -> String;
}

#include "Format.inl"