#pragma once
#include "MinInclude.h"
#include <atomic>

namespace Core
{
	/**
	 * Wrapper around std::memory_order
	 */
	enum class MemOrder : u8
	{
		Relaxed = u8(std::memory_order::relaxed),
		Consume = u8(std::memory_order::consume),
		Acquire = u8(std::memory_order::acquire),
		Release = u8(std::memory_order::release),
		AcqRel  = u8(std::memory_order::acq_rel),
		SeqCst  = u8(std::memory_order::seq_cst),
	};

	/**
	 * Wrapper around std::atomic
	 * \tparam T Underlying type
	 */
	template<typename T>
	class Atomic
	{
		STATIC_ASSERT(sizeof(T) <= sizeof(usize), "Cannot manage a type larger than usize");
		STATIC_ASSERT(std::atomic<T>::is_always_lock_free, "Underlying atomic implementation needs to be always lock free");
	public:
		/**
		 * Initialize the atomic with it's default value 'T{}'
		 */
		constexpr Atomic() noexcept;
		/**
		 * Initialize the atomic with a value
		 * \param[in] val Initial value
		 */
		constexpr Atomic(T val) noexcept;
		Atomic(Atomic<T>&) = delete;

		auto operator=(T val) noexcept -> T;
		auto operator=(Atomic<T>&) = delete;

		/**
		 * Atomically replace the current value with a new value
		 * \param[in] val Value to store
		 * \param[in] memOrder Memory order constraints to enforce
		 * \note 'memOrder' must be one of the following: Relaxed, Consume, Acquire, or SeqCst
		 */
		auto Store(T val, MemOrder memOrder = MemOrder::SeqCst) noexcept -> void;
		/**
		 * Atomically load and return the current value
		 * \param[in] memOrder Memory order constraints to enforce
		 * \return Current value
		 * \note 'memOrder' must be one of the following: Relaxed, Consume, Acquire, or SeqCst
		 */
		auto Load(MemOrder memOrder = MemOrder::SeqCst) noexcept -> T;
		operator T() noexcept;

		
		/**
		 * Atomically replace the current value with a new value
		 * \param[in] val Value to store
		 * \param[in] memOrder Memory order constraints to enforce
		 * \return Value of the atomic before this call
		 * \note 'memOrder' must be one of the following: Relaxed, Consume, Acquire, or SeqCst
		 */
		auto Exchange(T val, MemOrder memOrder = MemOrder::SeqCst) noexcept -> T;

		/**
		 * Atomically compare the stored value with an expected value, and replace the if with a new value, if they match
		 * \param[in, out] cur Value expected to be stored in the atomic value. Gets replaced with the actual value when the comparison fails
		 * \param[in] val Value to store if the comparison matches
		 * \param[in] memOrder Memory synchronization ordering for both operations
		 * \return Whether the atomic value was successfully changed
		 */
		auto CompareExchangeStrong(T& cur, T val, MemOrder memOrder = MemOrder::SeqCst) noexcept -> bool;
		/**
		 * Atomically compare the stored value with an expected value, and replace the if with a new value, if they match
		 * \param[in, out] cur Value expected to be stored in the atomic value. Gets replaced with the actual value when the comparison fails
		 * \param[in] val Value to store if the comparison matches
		 * \param[in] memOrder Memory synchronization ordering for both operations
		 * \return Whether the atomic value was successfully changed
		 * \note This function may fail, even when both values match. On certain platforms, this may give better performance than the strong version
		 */
		auto CompareExchangeWeak(T& cur, T val, MemOrder memOrder = MemOrder::SeqCst) noexcept -> bool;

		/**
		 * When the value of the atomic differs from the 'old' value, the function returns,
		 * if not, the thread will be blocked until either 'NotifyOne' or 'NotifyAll' has been called, or the thread is unblocked spuriously
		 * \param[in] old Value that the atomic should not contain
		 * \param[in] memOrder Memory order constraints to enforce
		 * \note 'memOrder' cannot be Release of AcqRel
		 */
		auto Wait(T old, MemOrder memOrder = MemOrder::SeqCst) noexcept -> void;

		/**
		 * If a thread is waiting on the atomic, notify a single thread that is waiting for it, otherwise do nothing
		 */
		auto NotifyOne() noexcept -> void;
		/**
		 * If a thread is waiting on the atomic, notify all threads that is waiting for it, otherwise do nothing
		 */
		auto NotifyAll() noexcept -> void;

		/**
		 * Atomically add a value to the atomic and return the value it had before modification
		 * \param[in] val Value to add
		 * \param[in] memOrder Memory order constraints to enforce
		 * \return Value before the modification
		 */
		auto FetchAdd(T val, MemOrder memOrder = MemOrder::SeqCst) noexcept -> T;
		/**
		 * Atomically subtract a value to the atomic and return the value it had before modification
		 * \param[in] val Value to add
		 * \param[in] memOrder Memory order constraints to enforce
		 * \return Value before the modification
		 */
		auto FetchSub(T val, MemOrder memOrder = MemOrder::SeqCst) noexcept -> T;
		/**
		 * Atomically and a value to the atomic and return the value it had before modification
		 * \param[in] val Value to add
		 * \param[in] memOrder Memory order constraints to enforce
		 * \return Value before the modification
		 */
		auto FetchAnd(T val, MemOrder memOrder = MemOrder::SeqCst) noexcept -> T;
		/**
		 * Atomically or a value to the atomic and return the value it had before modification
		 * \param[in] val Value to add
		 * \param[in] memOrder Memory order constraints to enforce
		 * \return Value before the modification
		 */
		auto FetchOr(T val, MemOrder memOrder = MemOrder::SeqCst) noexcept -> T;
		/**
		 * Atomically xor a value to the atomic and return the value it had before modification
		 * \param[in] val Value to add
		 * \param[in] memOrder Memory order constraints to enforce
		 * \return Value before the modification
		 */
		auto FetchXor(T val, MemOrder memOrder = MemOrder::SeqCst) noexcept -> T;

		auto operator++()    noexcept -> T;
		auto operator++(int) noexcept -> T;
		auto operator--()    noexcept -> T;
		auto operator--(int) noexcept -> T;

		auto operator+=(T val) noexcept -> T;
		auto operator-=(T val) noexcept -> T;
		auto operator&=(T val) noexcept -> T;
		auto operator|=(T val) noexcept -> T;
		auto operator^=(T val) noexcept -> T;

	private:
		std::atomic<T> m_atomic; ///< Wrapped atomic
	};
}

#include "Atomic.inl"