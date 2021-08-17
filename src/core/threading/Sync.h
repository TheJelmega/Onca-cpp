#pragma once
#include "core/MinInclude.h"

namespace Core::Threading
{
	/**
	 * Synchronization primitive to only allow access to a mutually exclusive resource
	 * \note Primitive CANNOT cross process boundaries
	 * \note Mutex does not support timeout, as it can be faster than a TimedSection on certain systems
	 */
	class CORE_API Mutex
	{
	private:
		constexpr static usize HandleSize = 40; ///< Minimum space needed for Win32 CRITICAL_SECTION
		struct alignas(8) NativeHandle { u8 data[HandleSize]; };
		
	public:
		/**
		 * Create a Mutex
		 */
		Mutex() noexcept;
		~Mutex() noexcept;

		/**
		 * Lock the Mutex, blocks the thread if the Mutex is not available
		 */
		auto Lock() noexcept -> void;
		/**
		 * Try to lock the Mutex, returns if the Mutex is not available
		 * \return If the Mutex has been successfully locked
		 */
		auto TryLock() noexcept -> bool;
		/**
		 * Unlock the mutex
		 */
		auto Unlock() noexcept -> void;

		/**
		 * \brief Get the native handle to the OS object
		 * \return Native handle
		 */
		auto GetNativeHandle() noexcept -> NativeHandle { return m_nativeHandle; }

	private:
		NativeHandle m_nativeHandle; ///< Handle to native OS handle
	};

	/**
	 * Synchronization primitive to only allow access to a mutually exclusive resource, which also allows a timeout to be set when trying to lock
	 * \note Primitive CANNOT cross process boundaries
	 */
	class CORE_API TimedMutex
	{
	private:
		constexpr static usize HandleSize = sizeof(usize);
		struct alignas(8) NativeHandle { u8 data[HandleSize]; };

	public:
		/**
		 * Create a Mutex
		 */
		TimedMutex() noexcept;
		~TimedMutex() noexcept;

		/**
		 * Lock the Mutex, blocks the thread if the Mutex is not available
		 */
		auto Lock() noexcept -> void;
		/**
		 * Try to lock the Mutex, returns if the Mutex is not available
		 * \return If the Mutex has been successfully locked
		 */
		auto TryLock() noexcept -> bool;
		/**
		 * Try to lock the Mutex, returns if the Mutex is not available
		 * \param[in] timeout Timeout in ms when trying to lock the TimedMutex
		 * \return If the Mutex has been successfully locked
		 */
		auto TryLockTimout(usize timeout) noexcept -> bool;
		/**
		 * Unlock the mutex
		 */
		auto Unlock() noexcept -> void;

		/**
		 * \brief Get the native handle to the OS object
		 * \return Native handle
		 */
		auto GetNativeHandle() noexcept -> NativeHandle { return m_nativeHandle; }

	private:
		NativeHandle m_nativeHandle; ///< Handle to native OS handle
	};

	/**
	 * Synchronization primitive to only allow access to a mutually exclusive resource
	 * \note Primitive CAN cross process boundaries
	 */
	class CORE_API MultiProcessMutex
	{
		constexpr static usize HandleSize = sizeof(usize);
		struct alignas(8) NativeHandle { u8 data[HandleSize]; };
	public:
		/**
		 * Create a MultiProcessMutex
		 * \param[in] identifier Mutex identifier
		 * \note Identifier can be a nullptr, but mutex will not be accessible from other processes
		 */
		explicit MultiProcessMutex(const char* identifier) noexcept;
		~MultiProcessMutex() noexcept;
		
		/**
		 * Lock the Mutex, blocks the thread if the Mutex is not available
		 */
		auto Lock() noexcept -> void;
		/**
		 * Try to lock the Mutex, returns if the Mutex is not available
		 * \return If the Mutex has been successfully locked
		 */
		auto TryLock() noexcept -> bool;
		/**
		 * Unlock the mutex
		 */
		auto Unlock() noexcept -> void;

		/**
		 * \brief Get the native handle to the OS object
		 * \return Native handle
		 */
		auto GetNativeHandle() noexcept -> NativeHandle { return m_nativeHandle; }
		/**
		 * Get the identifier of the MultiProcessMutex
		 * \return Identifier of the MultiProcessMutex
		 */
		auto GetIdentifier() const noexcept -> const char* { return m_identifier; }

		/**
		 * Open an already existing MultiProcessMutex
		 * \param[in] identifier Identifier of MultiProcessMutex to open
		 * \return Opened MultiProcessMutex
		 */
		static auto Open(const char* identifier) noexcept -> MultiProcessMutex;

	private:
		explicit MultiProcessMutex(NativeHandle handle, const char* identifier) noexcept;
		
		NativeHandle m_nativeHandle; ///< Handle to native OS handle
		const char*  m_identifier;   ///< Identifier from cross process use
		bool         m_owned;        ///< Whether the primitive
	};


	/**
	 * Synchronization primitive to allow access to a certain amount of threads
	 * \note Primitive CAN cross process boundaries
	 */
	class CORE_API Semaphore
	{
		constexpr static usize HandleSize = sizeof(usize);
		struct alignas(8) NativeHandle { u8 data[HandleSize]; };
	public:
		/**
		 * Create a Semaphore
		 * \param[in] count 
		 * \param[in] identifier Mutex identifier
		 * \note Identifier can be a nullptr, but mutex will not be accessible from other processes
		 */
		explicit Semaphore(usize count, const char* identifier = nullptr) noexcept;
		~Semaphore() noexcept;

		/**
		 * Lock the Mutex, blocks the thread if the Mutex is not available
		 */
		auto Lock() noexcept -> void;
		/**
		 * Try to lock the Mutex, returns if the Mutex is not available
		 * \return If the Mutex has been successfully locked
		 */
		auto TryLock() noexcept -> bool;
		/**
		 * Unlock the mutex
		 */
		auto Unlock() noexcept -> void;

		/**
		 * \brief Get the native handle to the OS object
		 * \return Native handle
		 */
		auto GetNativeHandle() noexcept -> NativeHandle { return m_nativeHandle; }
		/**
		 * Get the identifier of the Semaphore
		 * \return Identifier of the Semaphore
		 */
		auto GetIdentifier() const noexcept -> const char* { return m_identifier; }

		/**
		 * Open an already existing Semaphore
		 * \param[in] identifier Identifier of Semaphore to open
		 * \return Opened Semaphore
		 */
		static auto Open(const char* identifier) noexcept -> Semaphore;

	private:
		explicit Semaphore(NativeHandle handle, const char* identifier) noexcept;

		NativeHandle m_nativeHandle; ///< Handle to native OS handle
		const char*  m_identifier;   ///< Identifier from cross process use
		bool         m_owned;        ///< Whether the primitive
	};


	class CORE_API Event
	{
		constexpr static usize HandleSize = sizeof(usize);
		struct alignas(8) NativeHandle { u8 data[HandleSize]; };
	public:
		/**
		 * Create an Event
		 * \param[in] initialState Initial state of the event, i.e. whether is starts signaled
		 * \param[in] manualReset If set to true, the event will be signaled, until a call to Unsignal is made,
		 *                        if set to false, the signal will be unsignaled once the first thread waiting on it continues
		 *                        (no guarantee is made about what thread will run)
		 * \param[in] identifier Mutex identifier
		 * \note Identifier can be a nullptr, but mutex will not be accessible from other processes
		 */
		explicit Event(bool initialState, bool manualReset, const char* identifier = nullptr) noexcept;
		~Event() noexcept;

		/**
		 * Signal the Event
		 */
		auto Signal() -> void;
		/**
		 * Set the Event as not signaled
		 */
		auto Unsignal() -> void;
		/**
		 * Wait until the event has been signaled
		 */
		auto Wait() -> void;

		/**
		 * \brief Get the native handle to the OS object
		 * \return Native handle
		 */
		auto GetNativeHandle() noexcept -> NativeHandle { return m_nativeHandle; }
		/**
		 * Get the identifier of the Event
		 * \return Identifier of the Event
		 */
		auto GetIdentifier() const noexcept -> const char* { return m_identifier; }

		/**
		 * Open an already existing Event
		 * \param[in] identifier Identifier of Event to open
		 * \return Opened Event
		 */
		static auto Open(const char* identifier) noexcept -> Event;

	private:
		explicit Event(NativeHandle handle, const char* identifier) noexcept;
		
		NativeHandle m_nativeHandle; ///< Handle to native OS handle
		const char*  m_identifier;   ///< Identifier from cross process use
		bool         m_owned;        ///< Whether the primitive
	};

	/**
	 * Defines a lockable type
	 */
	template<typename T>
	concept Lockable = requires(T t)
	{
		{ t.Lock() } noexcept;
		{ t.Unlock() } noexcept;
	};

	/**
	 * Lock a Lockable object for the current scope
	 * \tparam T Lockable object to lock
	 */
	template<Lockable T>
	class Lock
	{
	public:
		explicit Lock(T& lock);
		~Lock();

		Lock(Lock<T>&) = delete;
		auto operator=(Lock<T>&) = delete;
	private:
		T& m_lock;
	};

	// TODO: Add version of Lock for multiple Lockables ???
	
}

#include "Sync.inl"
