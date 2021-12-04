#pragma once
#if __RESHARPER__
#include "Result.h"
#endif
#include "core/Assert.h"

namespace Core
{
	template <typename T, NotSameAs<T> E>
	constexpr Result<T, E>::Result(T&& val) noexcept
		: m_value(Move(val))
	{
	}

	template <typename T, NotSameAs<T> E>
	constexpr Result<T, E>::Result(E&& err) noexcept
		: m_value(Move(err))
	{
	}

	template <typename T, NotSameAs<T> E>
	constexpr auto Result<T, E>::Success() const noexcept -> bool
	{
		return std::holds_alternative<T>(m_value);
	}

	template <typename T, NotSameAs<T> E>
	constexpr auto Result<T, E>::Failed() const noexcept -> bool
	{
		return std::holds_alternative<E>(m_value);
	}

	template <typename T, NotSameAs<T> E>
	constexpr auto Result<T, E>::Value() const noexcept -> const T&
	{
		ASSERT(Success(), "Trying to get value when result is not 'success'");
		return std::get<T>(m_value);
	}

	template <typename T, NotSameAs<T> E>
	constexpr auto Result<T, E>::Error() const noexcept -> const E&
	{
		ASSERT(Failed(), "Trying to get value when result is not 'failed'");
		return std::get<E>(m_value);
	}

	template <typename T, NotSameAs<T> E>
	constexpr auto Result<T, E>::ValueOr(const T& def) const noexcept -> const T&
	{
		if (Success())
			return std::get<T>(m_value);
		return def;
	}

	template <typename T, NotSameAs<T> E>
	constexpr auto Result<T, E>::ValueOr(T&& def) noexcept -> T
	{
		if (Success())
			return Move(std::get<T>(m_value));
		return def;
	}

	template <typename T, NotSameAs<T> E>
	Result<T, E>::operator bool() const noexcept
	{
		return Success();
	}
}
