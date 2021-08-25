#pragma once
#include "Array.h"
#include "core/Assert.h"

namespace Core
{
	template <typename T, usize N>
	auto Array<T, N>::At(usize idx) const noexcept -> Optional<T>
	{
		if (idx > N)
			return NullOpt;
		return Optional<T>{ m_data[idx] };
	}

	template <typename T, usize N>
	constexpr auto Array<T, N>::operator[](usize idx) noexcept -> T&
	{
#ifdef __cpp_if_consteval
		if consteval
		{
			STATIC_ASSERT(idx < N, "Index out of range");
		}
		else
		{
			ASSERT(idx < N, "Index out of range");
		}
#elif FORCE_ARRAY_RUNTIME_ASSERT
		ASSERT(idx < N, "Index out of range");
#endif
		return m_data[idx];
	}

	template <typename T, usize N>
	constexpr auto Array<T, N>::operator[](usize idx) const noexcept -> const T&
	{
#ifdef __cpp_if_consteval
		if consteval
		{
			STATIC_ASSERT(idx < N, "Index out of range");
		}
		else
		{
			ASSERT(idx < N, "Index out of range");
		}
#elif FORCE_ARRAY_RUNTIME_ASSERT
		ASSERT(idx < N, "Index out of range");
#endif
		return m_data[idx];
	}

	template <typename T, usize N>
	constexpr auto Array<T, N>::Size() const noexcept -> usize
	{
		return N;
	}

	template <typename T, usize N>
	constexpr auto Array<T, N>::IsEmpty() const noexcept -> bool
	{
		return N == 0;
	}

	template <typename T, usize N>
	constexpr auto Array<T, N>::Front() noexcept -> T&
	{
		STATIC_ASSERT(N > 0, "Array can't be empty");
		return *m_data;
	}

	template <typename T, usize N>
	constexpr auto Array<T, N>::Front() const noexcept -> const T&
	{
		STATIC_ASSERT(N > 0, "Array can't be empty");
		return *m_data;
	}

	template <typename T, usize N>
	constexpr auto Array<T, N>::Back() noexcept -> T&
	{
		STATIC_ASSERT(N > 0, "Array can't be empty");
		return m_data[N - 1];
	}

	template <typename T, usize N>
	constexpr auto Array<T, N>::Back() const noexcept -> const T&
	{
		STATIC_ASSERT(N > 0, "Array can't be empty");
		return m_data[N - 1];
	}

	template <typename T, usize N>
	constexpr auto Array<T, N>::Data() noexcept -> T*
	{
		return m_data;
	}

	template <typename T, usize N>
	constexpr auto Array<T, N>::Data() const noexcept -> const T*
	{
		return m_data;
	}

	template <typename T, usize N>
	constexpr auto Array<T, N>::Begin() noexcept -> T*
	{
		return m_data;
	}

	template <typename T, usize N>
	constexpr auto Array<T, N>::Begin() const noexcept -> const T*
	{
		return m_data;
	}

	template <typename T, usize N>
	constexpr auto Array<T, N>::End() noexcept -> T*
	{
		return m_data + N - 1;
	}

	template <typename T, usize N>
	constexpr auto Array<T, N>::End() const noexcept -> const T*
	{
		return m_data + N - 1;
	}
}
