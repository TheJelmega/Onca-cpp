#pragma once
#if __RESHARPER__
#endif
#include "Guarded.h"

namespace Onca::Threading
{
	template <typename T, Lockable LockT>
	LockedGuard<T, LockT>::LockedGuard(LockedGuard&& other) noexcept
		: m_data(other.m_data)
		, m_lock(other.m_lock)
	{
		other.m_valid = false;
	}

	template <typename T, Lockable LockT>
	auto LockedGuard<T, LockT>::operator=(LockedGuard&& other) noexcept -> LockedGuard&
	{
		MemCpy(*this, other);
		other.m_valid = false;
		return *this;
	}

	template <typename T, Lockable LockT>
	LockedGuard<T, LockT>::~LockedGuard()
	{
		if (!m_valid)
			return;

		m_lock.Unlock();
		m_valid = false;
	}

	template <typename T, Lockable LockT>
	auto LockedGuard<T, LockT>::Get() const noexcept -> const T&
	{
		ASSERT(m_valid, "Accessing invalid locked guard");
		return m_data;
	}

	template <typename T, Lockable LockT>
	auto LockedGuard<T, LockT>::Get() noexcept -> T&
	{
		ASSERT(m_valid, "Accessing invalid locked guard");
		return m_data;
	}

	template <typename T, Lockable LockT>
	auto LockedGuard<T, LockT>::IsValid() const noexcept -> bool
	{
		return m_valid;
	}

	template <typename T, Lockable LockT>
	auto LockedGuard<T, LockT>::operator->() const noexcept -> const T*
	{
		ASSERT(m_valid, "Accessing invalid locked guard");
		return &m_data;
	}

	template <typename T, Lockable LockT>
	auto LockedGuard<T, LockT>::operator->() noexcept -> T*
	{
		ASSERT(m_valid, "Accessing invalid locked guard");
		return &m_data;
	}

	template <typename T, Lockable LockT>
	auto LockedGuard<T, LockT>::operator*() const noexcept -> const T&
	{
		ASSERT(m_valid, "Accessing invalid locked guard");
		return m_data;
	}

	template <typename T, Lockable LockT>
	auto LockedGuard<T, LockT>::operator*() noexcept -> T&
	{
		ASSERT(m_valid, "Accessing invalid locked guard");
		return m_data;
	}

	template <typename T, Lockable LockT>
	LockedGuard<T, LockT>::LockedGuard(T& data, LockT& lock)
		: m_data(data)
		, m_lock(lock)
		, m_valid(true)
	{
	}

	template <typename T, Lockable Lock>
	Guarded<T, Lock>::Guarded() noexcept requires DefaultConstructible<T>
	{
	}

	template <typename T, Lockable Lock>
	template <typename ... Args>
	Guarded<T, Lock>::Guarded(Args&&... args) noexcept
		: m_data(Forward<Args>(args)...)
	{
	}

	template <typename T, Lockable LockT>
	auto Guarded<T, LockT>::Lock() noexcept -> LockedGuard<T, LockT>
	{
		m_lock.Lock();
		return { m_data, m_lock };
	}

	template <typename T, Lockable LockT>
	auto Guarded<T, LockT>::TryLock() noexcept -> Optional<LockedGuard<T, LockT>>
	{
		if (m_lock.TryLock())
			return {  };
		return NullOpt;
	}

	template <typename T, Lockable LockT>
	void Guarded<T, LockT>::Unlock(LockedGuard<T, LockT>&& lockedGuard) noexcept
	{
		lockedGuard.~LockedGuard();
	}
}
