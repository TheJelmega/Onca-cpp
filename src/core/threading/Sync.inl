#pragma once
#if __RESHARPER__
#include "Sync.h"
#endif

namespace Onca::Threading
{
	namespace Detail
	{
		template<Lockable T, Lockable... Others>
		ManualLock<T, Others...>::ManualLock(T& lock, Others&... others) noexcept
			: m_lock(lock)
			, m_others(others...)
		{
			m_lock.Lock();
		}

		template<Lockable T, Lockable... Others>
		ManualLock<T, Others...>::~ManualLock() noexcept
		{
			m_lock.Unlock();
		}

		template <Lockable T, Lockable ... Others>
		void ManualLock<T, Others...>::Lock() noexcept
		{
			m_lock.Lock();
			m_others.Lock();
		}

		template <Lockable T, Lockable ... Others>
		void ManualLock<T, Others...>::Unlock() noexcept
		{
			m_others.Unlock();
			m_lock.Unlock();
		}

		template <Lockable T, Lockable ... Others>
		ManualLock<T, Others...>::ManualLock() noexcept
			: m_lock(nullptr)
		{
		}

		template <Lockable T>
		ManualLock<T>::ManualLock(T& lock) noexcept
			: m_lock(lock)
		{
			m_lock.Lock();
		}

		template <Lockable T>
		ManualLock<T>::~ManualLock() noexcept
		{
			m_lock.Unlock();
		}

		template <Lockable T>
		void ManualLock<T>::Lock() noexcept
		{
			m_lock.Lock();
		}

		template <Lockable T>
		void ManualLock<T>::Unlock() noexcept
		{
			m_lock.Unlock();
		}

		template <Lockable T>
		ManualLock<T>::ManualLock() noexcept
			: m_lock(nullptr)
		{
		}
	}

	template <Lockable T, Lockable ... Others>
	Lock<T, Others...>::Lock(T& lock, Others&... others) noexcept
		: m_lock(lock, others...)
	{
		m_lock.Lock();
	}

	template <Lockable T, Lockable ... Others>
	Lock<T, Others...>::~Lock() noexcept
	{
		m_lock.Unlock();
	}
}
