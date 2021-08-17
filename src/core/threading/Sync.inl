#include "Sync.h"

namespace Core::Threading
{
	template <Lockable T>
	Lock<T>::Lock(T& lock)
		: m_lock(lock)
	{
		m_lock.Lock();
	}

	template <Lockable T>
	Lock<T>::~Lock()
	{
		m_lock.Unlock();
	}
}
