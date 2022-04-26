#pragma once
#if __RESHARPER__
#include "Stringify.h"
#endif

#include "Format.h"
#include "core/math/MathUtils.h"

namespace Onca
{
	inline auto ToString(const String& str) noexcept -> String
	{
		return str;
	}

	template <usize N>
	auto ToString(const ConstString<N>& str) noexcept -> String
	{
		return String{ str.Begin(), str.End() };
	}

	template<ToStringMethod T>
	auto ToString(const T& t) noexcept -> String
	{
		return t.ToString();
	}

	template <CharacterType C>
	auto ToString(const C* cstr) noexcept -> String
	{
		return String{ cstr };
	}

	// TODO: optimize
	template <Integral I>
	auto ToString(I val) noexcept -> String
	{
		return ToFormat(val);
	}
	
	template<FloatingPoint F>
	auto ToString(F val) noexcept -> String
	{
		using Int = SignedOfSameSize<F>;
		return ToFormat(val);
	}

	inline auto ToString(bool val) noexcept -> String
	{
		return ToFormat(val);
	}

	////////////////////////////////////////////////////////////////

	inline auto ToDebugString(const String& str) noexcept -> String
	{
		return str;
	}

	template <usize N>
	auto ToDebugString(const ConstString<N>& str) noexcept -> String
	{
		return String{ str.Begin(), str.End() };
	}

	template <ToDebugStringMethod T>
	auto ToDebugString(const T& t) noexcept -> String
	{
		return t.ToDebugString();
	}

	template <CharacterType C>
	auto ToDebugString(const C* cstr) noexcept -> String
	{
		return String{ cstr };
	}

	template <Integral I>
	auto ToDebugString(I val) noexcept -> String
	{
		return ToFormat(val);
	}

	template <FloatingPoint F>
	auto ToDebugString(F val) noexcept -> String
	{
		return ToFormat(val);
	}

	inline auto ToDebugString(bool val) noexcept -> String
	{
		return ToFormat(val);
	}
}
