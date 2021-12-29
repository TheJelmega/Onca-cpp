#pragma once
#if __RESHARPER__
#include "Atomic.h"
#endif

namespace Core
{
	template <typename T>
	constexpr Atomic<T>::Atomic() noexcept
	{
		STATIC_ASSERT(std::is_trivially_constructible_v<T>, "Value needs to be default constructable");
	}

	template <typename T>
	constexpr Atomic<T>::Atomic(T val) noexcept
		: m_atomic(val)
	{
	}
	
	template <typename T>
	auto Atomic<T>::operator=(T val) noexcept -> T
	{
		return m_atomic = val;
	}

	template <typename T>
	auto Atomic<T>::Store(T val, MemOrder memOrder) noexcept -> void
	{
		m_atomic.store(val, static_cast<std::memory_order>(memOrder));
	}

	template <typename T>
	auto Atomic<T>::Load(MemOrder memOrder) const noexcept -> T
	{
		return m_atomic.load(static_cast<std::memory_order>(memOrder));
	}

	template <typename T>
	Atomic<T>::operator T() const noexcept
	{
		return Load();
	}

	template <typename T>
	auto Atomic<T>::Exchange(T val, MemOrder memOrder) noexcept -> T
	{
		return m_atomic.exchange(val, static_cast<std::memory_order>(memOrder));
	}

	template <typename T>
	auto Atomic<T>::CompareExchangeStrong(T& cur, T val, MemOrder memOrder) noexcept -> bool
	{
		return m_atomic.compare_exchange_strong(cur, val, static_cast<std::memory_order>(memOrder));
	}

	template <typename T>
	auto Atomic<T>::CompareExchangeWeak(T& cur, T val, MemOrder memOrder) noexcept -> bool
	{
		return m_atomic.compare_exchange_weak(cur, val, static_cast<std::memory_order>(memOrder));
	}

	template <typename T>
	auto Atomic<T>::Wait(T old, MemOrder memOrder) noexcept -> void
	{
		m_atomic.wait(old, static_cast<std::memory_order>(memOrder));
	}

	template <typename T>
	auto Atomic<T>::NotifyOne() noexcept -> void
	{
		m_atomic.notify_one();
	}

	template <typename T>
	auto Atomic<T>::NotifyAll() noexcept -> void
	{
		m_atomic.notify_all();
	}

	template <typename T>
	auto Atomic<T>::FetchAdd(T val, MemOrder memOrder) noexcept -> T
	{
		return m_atomic.fetch_add(val, static_cast<std::memory_order>(memOrder));
	}

	template <typename T>
	auto Atomic<T>::FetchSub(T val, MemOrder memOrder) noexcept -> T
	{
		return m_atomic.fetch_sub(val, static_cast<std::memory_order>(memOrder));
	}

	template <typename T>
	auto Atomic<T>::FetchAnd(T val, MemOrder memOrder) noexcept -> T
	{
		return m_atomic.fetch_and(val, static_cast<std::memory_order>(memOrder));
	}

	template <typename T>
	auto Atomic<T>::FetchOr(T val, MemOrder memOrder) noexcept -> T
	{
		return m_atomic.fetch_or(val, static_cast<std::memory_order>(memOrder));
	}

	template <typename T>
	auto Atomic<T>::FetchXor(T val, MemOrder memOrder) noexcept -> T
	{
		return m_atomic.fetch_xor(val, static_cast<std::memory_order>(memOrder));
	}

	template <typename T>
	auto Atomic<T>::operator++() noexcept -> T
	{
		return FetchAdd(1) + 1;
	}

	template <typename T>
	auto Atomic<T>::operator++(int) noexcept -> T
	{
		return FetchAdd(1);
	}

	template <typename T>
	auto Atomic<T>::operator--() noexcept -> T
	{
		return FetchSub(1) - 1;
	}

	template <typename T>
	auto Atomic<T>::operator--(int) noexcept -> T
	{
		return FetchSub(1);
	}

	template <typename T>
	auto Atomic<T>::operator+=(T val) noexcept -> T
	{
		return FetchAdd(val) + val;
	}

	template <typename T>
	auto Atomic<T>::operator-=(T val) noexcept -> T
	{
		return FetchSub(val) - val;
	}

	template <typename T>
	auto Atomic<T>::operator&=(T val) noexcept -> T
	{
		return FetchAnd(val) & val;
	}

	template <typename T>
	auto Atomic<T>::operator|=(T val) noexcept -> T
	{
		return FetchOr(val) | val;
	}

	template <typename T>
	auto Atomic<T>::operator^=(T val) noexcept -> T
	{
		return FetchXor(val) ^ val;
	}
}
