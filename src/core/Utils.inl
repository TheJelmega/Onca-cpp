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

	template <ForwardIterator T>
	auto CountElems(const T& begin, const T& end) noexcept -> usize
	{
		usize cnt = 0;
		for (T it = begin; it != end; ++it)
			++cnt;
		return cnt;
	}
}