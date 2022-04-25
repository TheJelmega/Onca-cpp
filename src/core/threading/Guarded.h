#pragma once
#include "Sync.h"

namespace Core::Threading
{
	template<typename T, Lockable LockT = Mutex>
	class Guarded;

	template<typename T, Lockable LockT>
	class LockedGuard
	{
	public:
		DISABLE_COPY(LockedGuard);

		LockedGuard(LockedGuard&& other) noexcept;
		auto operator=(LockedGuard&& other) noexcept -> LockedGuard&;

		~LockedGuard();

		/**
		 * Get a reference to the data
		 * \return Reference to the data
		 */
		auto Get() const noexcept -> const T&;
		/**
		 * Get a reference to the data
		 * \return Reference to the data
		 */
		auto Get() noexcept -> T&;

		/**
		 * Check if the LockedGuard is still valid
		 * \return Whether the LockedGuard is still valid
		 */
		auto IsValid() const noexcept -> bool;

		auto operator->() const noexcept -> const T*;
		auto operator->() noexcept -> T*;
		auto operator*() const noexcept -> const T&;
		auto operator*() noexcept -> T&;

	private:
		friend class Guarded<T, LockT>;

		LockedGuard(T& data, LockT& lock);

		T&     m_data;  ///< Reference to the data
		LockT& m_lock;  ///< Reference to the lock
		bool   m_valid; ///< The locked guard is valid (not moved or unlocked)
	};

	template<typename T, Lockable LockT>
	class Guarded
	{
	public:
		/**
		 * Create default initialized data
		 */
		Guarded() noexcept requires DefaultConstructible<T>;

		/**
		 * Create guarded data with the given arguments
		 * \tparam Args Argument types
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		explicit Guarded(Args&&... args) noexcept;

		/**
		 * Lock the data for access
		 * \return A locked guard, which can be used to access the data
		 */
		auto Lock() noexcept -> LockedGuard<T, LockT>;
		/**
		 * Try to lock the guarded data
		 * \return An optional with a locked guard (if the lock could be locked), which can be used to access the data
		 */
		auto TryLock() noexcept -> Optional<LockedGuard<T, LockT>>;
		/**
		 * Unlock the guarded data
		 * \param[in] lockedGuard Locked guard to unlock
		 */
		auto Unlock(LockedGuard<T, LockT>&& lockedGuard) noexcept -> void;

	private:
		T     m_data; // Guarded data
		LockT m_lock; // Locking primitive/guard
		
	};

}

#include "Guarded.inl"