#pragma once
#include "Base.h"
#include "Concepts.h"
#include "core/Assert.h"
#include "core/memory/MemUtils.h"
#include "core/utils/Meta.h"

namespace Core::Intrin
{
	namespace Detail
	{
		template<SimdBaseType T>
		union PackData128
		{
			using Unsigned = UnsignedOfSameSize<T>;
			using Signed = SignedOfSameSize<T>;

			T        raw  [16 / sizeof(T)];
			Unsigned bits [16 / sizeof(T)];
			Signed   sbits[16 / sizeof(T)];
			f64      f64_ [2];
			f32      f32_ [4];
			u64      u64_ [2];
			u32      u32_ [4];
			u16      u16_ [8];
			u8       u8_  [16];
			i64      i64_ [2];
			i32      i32_ [4];
			i16      i16_ [8];
			i8       i8_  [16];
#if HAS_SSE_SUPPORT
			__m128   sse_m128;
			__m128d  sse_m128d;
			__m128i  sse_m128i;
#endif
		};
		STATIC_ASSERT(sizeof(PackData128<f32>) == 16, "PackData256 size is incorrect, expected 16");
			
		template<SimdBaseType T>
		union PackData256 // <32> 256-bit
		{
			using Unsigned = UnsignedOfSameSize<T>;
			using Signed = SignedOfSameSize<T>;

			T              raw  [32 / sizeof(T)];
			Unsigned       bits [32 / sizeof(T)];
			Signed         sbits[32 / sizeof(T)];
			f64            f64_ [4];
			f32            f32_ [8];
			u64            u64_ [4];
			u32            u32_ [8];
			u16            u16_ [16];
			u8             u8_  [32];
			i64            i64_ [4];
			i32            i32_ [8];
			i16            i16_ [16];
			i8             i8_  [32];

			PackData128<T> m128 [2];
#if HAS_SSE_SUPPORT
			__m128         sse_m128[2];
			__m128d        sse_m128d[2];
			__m128i        sse_m128i[2];
#endif
#if HAS_AVX
			__m256         sse_m256;
			__m256d        sse_m256d;
			__m256i        sse_m256i;
#endif
		};
		STATIC_ASSERT(sizeof(PackData256<f32>) == 32, "PackData256 size is incorrect, expected 32");

		template<SimdBaseType T, usize DataSize>
		struct PackDataSelector
		{
			STATIC_ASSERT(DataSize == 32, "Invalid data size");
			using Type = PackData256<T>;
		};
		template<SimdBaseType T>
		struct PackDataSelector<T, 16>
		{
			using Type = PackData128<T>;
		};

		template<SimdBaseType T, usize Width>
		using PackData = typename PackDataSelector<T, Width * sizeof(T)>::Type;
		
	}
	
	/**
	 * SIMD comparison operator
	 */
	enum class ComparisonOp
	{
		Eq,    ///< Equal
		Ge,    ///< Greater or equal
		Gt,    ///< Greater than
		Le,    ///< Less or equal
		Lt,    ///< Less then
		NEq,   ///< Not equal
		Ord,   ///< Values are not NaN
		Unord, ///< Either value is NaN
	};

	template<SimdBaseType T, usize Width>
	struct alignas(sizeof(T)* Width) Pack
	{
		static constexpr usize DataSize = sizeof(T) * Width;
		static constexpr usize Align = DataSize;
		STATIC_ASSERT(DataSize == 16 || DataSize == 32, "Invalid 'Width': sizeof(T) * Width needs to be 16 (128-bit) or 32 (256-bit)");

		// Special case for autocomplete
#if defined(__INTELLISENSE__) || defined(__RESHARPER__)
		Detail::PackData256<T> data;
#else
		Detail::PackData<T, Width> data;
#endif

		/**
		 * Check if the pack if 128 bits wide
		 * \return Whether the pack is 128 bits wide
		 */
		static constexpr auto Is128Bit() noexcept -> bool
		{
			return DataSize == 16;
		}
		/**
		 * Check if the pack if 256 bits wide
		 * \return Whether the pack is 256 bits wide
		 */
		static constexpr auto Is256Bit() noexcept -> bool
		{
			return DataSize == 32;
		}

		/**
		 * Check if the pack is stored in a native register types
		 * \return Whether the pack is stored in a native register types
		 * \note The register is guaranteed to be at least 128-bit
		 */
		static constexpr auto IsNative() noexcept -> bool
		{
			return HAS_SSE_SUPPORT;
		}

		/**
		 * Check if the pack is stored in a native 256-bit register types
		 * \return Whether the pack is stored in a native register types
		 */
		static constexpr auto IsNative256() noexcept -> bool
		{
			return (FloatingPoint<T> && HAS_AVX) || HAS_AVX2;
		}

		/**
		 * Create a pack with all its elements set to 0s
		 * \return Zeroed pack
		 */
		static constexpr auto Zero() noexcept -> Pack;
		/**
		 * Create a pack with all its bits set to 1
		 * \return Zeroed pack
		 */
		static constexpr auto Ones() noexcept -> Pack;
		/**
		 * Create a pack with all its elements set to a given value
		 * \param val Value
		 * \return Pack with value
		 */
		static constexpr auto Set(T val) noexcept -> Pack;
		/**
		 * Create a pack with its elements set the given values
		 * \tparam Args Value types (should be the same as T)
		 * \param vals Values
		 * \return Pack with values
		 */
		template<SameAs<T>... Args>
		static constexpr auto Set(Args... vals) noexcept -> Pack;
		/**
		 * Create a pack with its elements set the given values (in reverse order)
		 * \tparam Args Value types (should be the same as T)
		 * \param vals Values
		 * \return Pack with values
		 */
		template<SameAs<T>... Args>
		static constexpr auto SetR(Args... vals) noexcept -> Pack;

		/**
		 * Create a pack with its elements loaded from memory
		 * \param addr Address to first value
		 * \return Pack with loaded values
		 * \note memory does NOT have to be aligned
		 */
		static constexpr auto Load(const T* addr) noexcept -> Pack;
		/**
		 * Create a pack with its elements loaded from memory
		 * \param addr Address to first value
		 * \return Pack with loaded values
		 * \note memory NEEDS to be aligned (see Pack::Align)
		 */
		static constexpr auto AlignedLoad(const T* addr) noexcept -> Pack;

		/**
		 * Create a pack with all elements set to 0
		 */
		constexpr Pack() noexcept;
		/**
		 * Create an uninitialized Pack
		 */
		explicit constexpr Pack(UnInitTag) noexcept;
		/**
		 * Create a pack with all elements set to 'val'
		 * \param val Value
		 */
		explicit constexpr Pack(T val) noexcept;
		/**
		 * Create a pack with the elements set to the given values
		 * \tparam Args Value types (should be the same as T)
		 * \param vals Values
		 */
		template<SameAs<T>... Args>
		explicit constexpr Pack(Args... vals) noexcept;

		constexpr auto operator++() noexcept -> Pack&;
		constexpr auto operator++(int) noexcept -> Pack;
		constexpr auto operator--() noexcept -> Pack&;
		constexpr auto operator--(int) noexcept -> Pack;

		constexpr auto operator+() const noexcept -> Pack;
		constexpr auto operator-() const noexcept -> Pack;
		constexpr auto operator~() const noexcept -> Pack;

		constexpr auto operator+(const Pack& other) const noexcept -> Pack;
		constexpr auto operator-(const Pack& other) const noexcept -> Pack;
		constexpr auto operator*(const Pack& other) const noexcept -> Pack;
		constexpr auto operator/(const Pack& other) const noexcept -> Pack;

		constexpr auto operator&(const Pack& other) const noexcept -> Pack;
		constexpr auto operator^(const Pack& other) const noexcept -> Pack;
		constexpr auto operator|(const Pack& other) const noexcept -> Pack;

		constexpr auto operator>>(const Pack& count) const noexcept -> Pack;
		template<UnsignedIntegral U>
		constexpr auto operator>>(U count) const noexcept -> Pack;
		constexpr auto operator<<(const Pack& count) const noexcept -> Pack;
		template<UnsignedIntegral U>
		constexpr auto operator<<(U count) const noexcept -> Pack;

		constexpr auto operator+=(const Pack& other) noexcept -> Pack&;
		constexpr auto operator-=(const Pack& other) noexcept -> Pack&;
		constexpr auto operator*=(const Pack& other) noexcept -> Pack&;
		constexpr auto operator/=(const Pack& other) noexcept -> Pack&;

		constexpr auto operator&=(const Pack& other) noexcept -> Pack&;
		constexpr auto operator^=(const Pack& other) noexcept -> Pack&;
		constexpr auto operator|=(const Pack& other) noexcept -> Pack&;

		constexpr auto operator>>=(const Pack& count) const noexcept -> Pack&;
		template<UnsignedIntegral U>
		constexpr auto operator>>=(U count) const noexcept -> Pack&;
		constexpr auto operator<<=(const Pack& count) const noexcept -> Pack&;
		template<UnsignedIntegral U>
		constexpr auto operator<<=(U count) const noexcept -> Pack&;

		constexpr auto operator==(const Pack& other) const noexcept -> Pack;
		constexpr auto operator!=(const Pack& other) const noexcept -> Pack;
		constexpr auto operator<(const Pack& other) const noexcept -> Pack;
		constexpr auto operator<=(const Pack& other) const noexcept -> Pack;
		constexpr auto operator>(const Pack& other) const noexcept -> Pack;
		constexpr auto operator>=(const Pack& other) const noexcept -> Pack;

		/**
		 * Store the elements into unaligned memory
		 * \param addr Address to store elements to
		 */
		constexpr auto Store(T* addr) const noexcept -> void;
		/**
		 * Store the elements into unaligned memory
		 * \param addr Address to store elements to
		 */
		constexpr auto AlignedStore(T* addr) const noexcept -> void;

		/**
		 * Create a copy of the pack with a value inserted into it
		 * \tparam Index Index of the element to insert to
		 * \param val Value to insert
		 * \return Pack with the value inserted
		 */
		template<usize Index>
		constexpr auto Insert(T val) const noexcept -> Pack;
		/**
		 * Extract the value of an element from the pack
		 * \tparam Index Index of element to extract
		 * \return Extracted element
		 */
		template<usize Index>
		constexpr auto Extract() const noexcept -> T;

		/**
		 * Convert a pack to another type
		 * \tparam To Type to convert to
		 * \return Converted register
		 * \note Conversion to smaller types will truncate the value
		 * \note Signed/Unsigned conversion are bitcasted
		 */
		template<typename U>
		constexpr auto Convert() const noexcept -> Pack<U, DataSize / sizeof(U)>;

		/**
		 * Compare the current elements with the elements of the given pack, using the Comparison operator
		 * \tparam Op Comparison operator
		 * \param other Pack to compare with
		 * \return All 1s if comparison is successful, false otherwise
		 */
		template<ComparisonOp Op>
		constexpr auto Compare(const Pack& other) const noexcept -> Pack;

		/**
		 * Get a pack with negated values
		 * \return Pack with negated elements
		 */
		constexpr auto Neg() const noexcept -> Pack;
		/**
		 * Add the elements of the given pack to the current elements
		 * \param other Pack to add
		 * \return Pack with result
		 */
		constexpr auto Add(const Pack& other) const noexcept -> Pack;
		/**
		 * Subtract the elements of the given pack from the current elements
		 * \param other Pack to add
		 * \return Pack with result
		 */
		constexpr auto Sub(const Pack& other) const noexcept -> Pack;
		/**
		 * Multiply the elements of the given pack with the current elements
		 * \param other Pack to subtract
		 * \return Pack with result
		 */
		constexpr auto Mul(const Pack& other) const noexcept -> Pack;
		/**
		 * Divide the current elements by the elements of the given pack
		 * \param other Pack to multiply with
		 * \return Pack with result
		 */
		// TODO: Fast version when dividing by scalar
		constexpr auto Div(const Pack& other) const noexcept -> Pack;

		/**
		 * Add the elements of hte fiber pack to the current elements, if the result were to overflow, saturate the result to the max value
		 * \param other Pack to add
		 * \return Pack with result
		 * \note When using a floating point, this will be a normal add, i.e. +INF on overflow
		 */
		constexpr auto AddSaturated(const Pack& other) const noexcept -> Pack;
		/**
		 * Add the elements of hte fiber pack to the current elements, if the result were to overflow, saturate the result to the max value
		 * \param other Pack to add
		 * \return Pack with result
		 * \note When using a floating point, this will be a normal add, i.e. -INF on overflow
		 */
		constexpr auto SubSaturated(const Pack& other) const noexcept -> Pack;

		/**
		 * AND the current elements to the elements of the given pack
		 * \param other Pack to devide with
		 * \return Pack with result
		 */
		constexpr auto And(const Pack& other) const noexcept -> Pack;
		/**
		 * AND the current elements with the NOT of the elements of the given pack
		 * \param other Pack to and with
		 * \return Pack with result
		 */
		constexpr auto AndNot(const Pack& other) const noexcept -> Pack;
		/**
		 * XOR the current elements with the elements of the given pack
		 * \param other Pack to xor with
		 * \return Pack with result
		 */
		constexpr auto Xor(const Pack& other) const noexcept -> Pack;
		/**
		 * OR the current elements with the elements of the given pack
		 * \param other Pack to or with
		 * \return Pack with result
		 */
		constexpr auto Or(const Pack& other) const noexcept -> Pack;
		/**
		 * NOT the current elements
		 * \return Pack with result
		 */
		constexpr auto Not() const noexcept -> Pack;

		/**
		 * Blend 2 packs using a mask
		 * \param other Other pack to blend
		 * \param mask Blend mask (0 == this, 1 == other)
		 * \return Blended pack
		 */
		constexpr auto Blend(const Pack& other, const Pack& mask) const noexcept -> Pack;

		/**
		 * Shift each element of the pack to the left by the corresponding element in the given count
		 * \param count Shift count
		 * \return Shifted pack
		 */
		template<IntegralOfSameSize<T> U>
		constexpr auto ShiftL(const Pack<U, Width>& count) const noexcept -> Pack;
		/**
		 * Shift each element of the pack to the left by the given count
		 * \param count Shift count
		 * \return Shifted pack
		 */
		template<UnsignedIntegral U>
		constexpr auto ShiftL(U count) const noexcept -> Pack;
		/**
		 * Shift each element of the pack to the left by the given count
		 * \tparam Count Shift count
		 * \return Shifted pack
		 */
		template<usize Count>
		constexpr auto ShiftL() const noexcept -> Pack;
		/**
		 * Arithmetically shift (sign extended) each element of the pack to the right by the corresponding element in the given count
		 * \param count Shift count
		 * \return Shifted pack
		 */
		template<IntegralOfSameSize<T> U>
		constexpr auto ShiftRA(const Pack<U, Width>& count) const noexcept -> Pack;
		/**
		 * Arithmetically shift (sign extended) each element of the pack to the right by the given count
		 * \param count Shift count
		 * \return Shifted pack
		 */
		template<UnsignedIntegral U>
		constexpr auto ShiftRA(U count) const noexcept -> Pack;
		/**
		 * Arithmetically shift (sign extended) each element of the pack to the right by the given count
		 * \tparam Count Shift count
		 * \return Shifted pack
		 */
		template<usize Count>
		constexpr auto ShiftRA() const noexcept -> Pack;
		/**
		 * Logically shift (zero extended) each element of the pack to the right by the corresponding element in the given count
		 * \param count Shift count
		 * \return Shifted pack
		 */
		template<IntegralOfSameSize<T> U>
		constexpr auto ShiftRL(const Pack<U, Width>& count) const noexcept -> Pack;
		/**
		 * Logically shift (zero extended) each element of the pack to the right by the given count
		 * \param count Shift count
		 * \return Shifted pack
		 */
		template<UnsignedIntegral U>
		constexpr auto ShiftRL(U count) const noexcept -> Pack;
		/**
		 * Logically shift (zero extended) each element of the pack to the right by the given count
		 * \tparam Count Shift count
		 * \return Shifted pack
		 */
		template<usize Count>
		constexpr auto ShiftRL() const noexcept -> Pack;

		/**
		 * Creates a pack with the minimum value of this and the given value
		 * \param other Value to compare with
		 * \return Pack with minimum values
		 * \note If either value is NaN, the value returned for that element is undefined
		 */
		constexpr auto Min(const Pack& other) const noexcept -> Pack;
		/**
		 * Creates a pack with the maximum value of this and the given value
		 * \param other Value to compare with
		 * \return Pack with maximum values
		 * \note If either value is NaN, the value returned for that element is undefined
		 */
		constexpr auto Max(const Pack& other) const noexcept -> Pack;

		/**
		 * Creates a pack with elements equal to the nearest integer that is greater or equal to that element
		 * \return Pack with the ceil of the elements
		 */
		constexpr auto Ceil() const noexcept -> Pack;
		/**
		 * Creates a pack with elements equal to the nearest integer that is less or equal to that element
		 * \return Pack with the floor of the elements
		 */
		constexpr auto Floor() const noexcept -> Pack;
		/**
		 * Creates a pack with truncated elements (without fraction)
		 * \return Pack with truncated elements
		 */
		constexpr auto Trunc() const noexcept -> Pack;
		/**
		 * Creates a pack with elements rounded to the nearest integer
		 * \return Pack with the floor of the elements
		 * \note Round is a sequence of instructions, as the intrinsic round results in RoundEven
		 */
		constexpr auto Round() const noexcept -> Pack;
		/**
		 * Creates a pack with elements rounded to the nearest even integer
		 * \return Pack with the floor of the elements
		 */
		constexpr auto RoundEven() const noexcept -> Pack;

		/**
		 * Calculate the reciprocal (1/x) of each element in the Pack
		 * \return Reciprocal of the elements in the Pack
		 * \note Result is undefined if an element is 0
		 */
		constexpr auto Rcp() const noexcept -> Pack;
		/**
		 * Calculate the square root of each element in the Pack
		 * \return Square root of the elements in the Pack
		 */
		constexpr auto Sqrt() const noexcept -> Pack;
		/**
		 * Calculate the reverse square root of each element in the Pack
		 * \return Reverse square root of the elements in the Pack
		 */
		constexpr auto RSqrt() const noexcept -> Pack;

		/**
		 * Create a pack with the absolute value of each element
		 * \return Pack with absolute values
		 */
		constexpr auto Abs() const noexcept -> Pack;
		/**
		 * Create a pack with the sign of each element (neg: -1, 0: 0, pos: 1)
		 * \return Pack with the sign of each element
		 */
		constexpr auto Sign() const noexcept -> Pack;


		/**
		 * Sum all elements in the pack
		 * \tparam NumElements Number of elements to sum up
		 * \return Pack set to the sum of all elements
		 */
		template<usize NumElements = Width>
		constexpr auto HAdd() const noexcept -> Pack;
		/**
		 * Calculate a dot product of a number of elements and get a register with each element set to that dot product
		 * \tparam NumElements Number of elements from the start to calculate the dot product for
		 * \param other Other pack
		 * \return A pack with each element set to the dot product
		 */
		template<usize NumElements = Width>
		constexpr auto Dot(const Pack& other) const noexcept -> Pack;

	private:

		template<SimdBaseType U, usize W>
		friend struct Pack;

		template<typename U>
		Pack(const Detail::PackData128<U>& data)
		{
			STATIC_ASSERT(DataSize == 16, "Invalid data size for pack");
			MemCpy(&this->data, &data, sizeof(data));
		}
		template<typename U>
		Pack(const Detail::PackData256<U>& data)
		{
			STATIC_ASSERT(DataSize == 32, "Invalid data size for pack");
			MemCpy(&this->data, &data, sizeof(data));
		}

		auto HalfPack(usize idx) const noexcept -> Pack<T, Width / 2>
		{
			if constexpr (Is128Bit())
				return Pack<T, Width / 2>{ UnInit };
			else
				return Pack<T, Width / 2>{ data.m128[idx] };
		}
	};
}

// Files are order, so they can use functionality defined in the .inl files before them
#include "PackMemory.inl"
#include "PackLogic.inl"
#include "PackComp.inl"
#include "PackArith.inl"
#include "PackConvert.inl"
#include "Pack.inl"

namespace Core
{
	using f64x2 = Intrin::Pack<f64, 2>;
	using f64x4 = Intrin::Pack<f64, 4>;
	//using f64x8 = Intrin::Pack<f64, 8>;

	using f32x4 = Intrin::Pack<f32, 4>;
	using f32x8 = Intrin::Pack<f32, 8>;
	//using f32x16 = Intrin::Pack<f32, 16>;

	using u64x2 = Intrin::Pack<u64, 2>;
	using u64x4 = Intrin::Pack<u64, 4>;
	//using u64x8 = Intrin::Pack<u64, 8>;

	using u32x4 = Intrin::Pack<u32, 4>;
	using u32x8 = Intrin::Pack<u32, 8>;
	//using u32x16 = Intrin::Pack<u32, 16>;

	using u16x8 = Intrin::Pack<u16, 8>;
	using u16x16 = Intrin::Pack<u16, 16>;
	//using u16x32 = Intrin::Pack<u16, 32>;

	using u8x16 = Intrin::Pack<u8, 16>;
	using u8x32 = Intrin::Pack<u8, 32>;
	//using u8x64 = Intrin::Pack<u8, 64>;

	using i64x2 = Intrin::Pack<i64, 2>;
	using i64x4 = Intrin::Pack<i64, 4>;
	//using i64x8 = Intrin::Pack<i64, 8>;

	using i32x4 = Intrin::Pack<i32, 4>;
	using i32x8 = Intrin::Pack<i32, 8>;
	//using i32x16 = Intrin::Pack<i32, 16>;

	using i16x8 = Intrin::Pack<i16, 8>;
	using i16x16 = Intrin::Pack<i16, 16>;
	//using i16x32 = Intrin::Pack<i16, 32>;

	using i8x16 = Intrin::Pack<i8, 16>;
	using i8x32 = Intrin::Pack<i8, 32>;
	//using i8x64 = Intrin::Pack<i8, 64>;
}
