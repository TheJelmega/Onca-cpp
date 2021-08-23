#pragma once
#include "Utils.h"

namespace Core
{
	template <typename T>
	auto DefaultDeleter<T>::operator()(MemRef<T>&& ref) noexcept -> void
	{
		~*ref.m_handle;
		ref.Dealloc();
	}

	template<typename T>
	NO_DISCARD("") constexpr auto Move(T&& moved) noexcept -> RemoveReference<T>&&
	{
		return static_cast<RemoveReference<T>&&>(moved);
	}

	template <typename T>
	constexpr auto Forward(RemoveReference<T>& arg) noexcept -> T&&
	{
		return static_cast<T&&>(arg);
	}

	template <typename T>
	constexpr auto Forward(RemoveReference<T>&& arg) noexcept -> T&&
	{
		STATIC_ASSERT(!IsLValueReference<T>, "Bad forward call");
		return static_cast<T&&>(arg);
	}

	template <ForwardIterator T>
	auto CountElems(const T& begin, const T& end) noexcept -> usize
	{
		usize cnt = 0;
		for (T it = begin; it != end; ++it)
			++cnt;
		return cnt;
	}
}