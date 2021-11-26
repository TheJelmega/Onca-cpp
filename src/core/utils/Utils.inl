#pragma once
#if __RESHARPER__
#include "Utils.h"
#endif

#include "hash/Hash.h"

namespace Core
{
	template <typename T>
	auto DefaultDeleter<T>::operator()(MemRef<T>&& ref) noexcept -> void
	{
		~*ref.m_handle;
		ref.Dealloc();
	}

	template <typename T> requires EqualComparable<T, T>
	auto DefaultEqualComparator<T>::operator()(const T& t0, const T& t1) const noexcept -> bool
	{
		return t0 == t1;
	}

	template <typename T> requires OrderedComparable<T>
	auto DefaultComparator<T>::operator()(const T& t0, const T& t1) const noexcept -> i8
	{
		if (t0 < t1) return -1;
		if (t0 > t1) return 1;
		return 0;
	}

	template <typename T>
	auto Hash<T>::operator()(const T& t) const noexcept -> u64
	{
		static Hashing::FVN1A_64 hash;
		return hash(reinterpret_cast<const u8*>(&t), sizeof(T));
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

	template<typename To, typename From>
	constexpr auto Bitcast(From& from) noexcept -> To
	{
		To to;
		MemCpy(to, from);
		return to;
	}
	
}
