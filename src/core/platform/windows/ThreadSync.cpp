#include "core/MinInclude.h"

#if PLATFORM_WINDOWS
#include "core/Assert.h"
#include "core/threading/Sync.h"
#include "Win.h"

namespace Core::Threading
{
	Mutex::Mutex() noexcept
	{
		InitializeCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>(&m_nativeHandle));
	}

	Mutex::~Mutex() noexcept
	{
		DeleteCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>(&m_nativeHandle));
	}

	auto Mutex::Lock() noexcept -> void
	{
		EnterCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>(&m_nativeHandle));
	}

	auto Mutex::TryLock() noexcept -> bool
	{
		return TryEnterCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>(&m_nativeHandle));
	}

	auto Mutex::Unlock() noexcept -> void
	{
		LeaveCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>(&m_nativeHandle));
	}

	TimedMutex::TimedMutex() noexcept
	{
		HANDLE handle = CreateMutexA(nullptr, FALSE, nullptr);
		ASSERT(handle, "Failed to create TimedMutex");
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.handle = handle;
		m_nativeHandle = handleUnion.native;
	}

	TimedMutex::~TimedMutex() noexcept
	{
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		CloseHandle(handleUnion.handle);
	}

	auto TimedMutex::Lock() noexcept -> void
	{
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		DWORD res = WaitForSingleObject(handleUnion.handle, INFINITE);
		ASSERT(res == WAIT_OBJECT_0, "Multiprocess::Lock using WaitForSingleObject failed");
	}

	auto TimedMutex::TryLock() noexcept -> bool
	{
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		DWORD res = WaitForSingleObject(handleUnion.handle, 0);
		ASSERT(res == WAIT_OBJECT_0 || res == WAIT_TIMEOUT, "Multiprocess::TryLock using WaitForSingleObject failed");
		return res == WAIT_OBJECT_0;
	}

	auto TimedMutex::TryLockTimout(usize timeout) noexcept -> bool
	{
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		DWORD res = WaitForSingleObject(handleUnion.handle, DWORD(timeout));
		ASSERT(res == WAIT_OBJECT_0 || res == WAIT_TIMEOUT, "Multiprocess::TryLock using WaitForSingleObject failed");
		return res == WAIT_OBJECT_0;
	}

	auto TimedMutex::Unlock() noexcept -> void
	{
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		ReleaseMutex(handleUnion.handle);
	}

	MultiProcessMutex::MultiProcessMutex(const char* identifier) noexcept
		: m_identifier(identifier)
		, m_owned(true)
	{
		HANDLE handle =	CreateMutexA(nullptr, FALSE, identifier);
		ASSERT(handle, "Failed to create MultiProcessMutex");
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.handle = handle;
		m_nativeHandle = handleUnion.native;
	}
	
	MultiProcessMutex::~MultiProcessMutex() noexcept
	{
		if (!m_owned)
			return;

		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		CloseHandle(handleUnion.handle);
	}

	auto MultiProcessMutex::Lock() noexcept -> void
	{
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		DWORD res = WaitForSingleObject(handleUnion.handle, INFINITE);
		ASSERT(res == WAIT_OBJECT_0, "Multiprocess::Lock using WaitForSingleObject failed");
	}

	auto MultiProcessMutex::TryLock() noexcept -> bool
	{
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		DWORD res = WaitForSingleObject(handleUnion.handle, 0);
		ASSERT(res == WAIT_OBJECT_0 || res == WAIT_TIMEOUT, "Multiprocess::TryLock using WaitForSingleObject failed");
		return res == WAIT_OBJECT_0;
	}

	auto MultiProcessMutex::Unlock() noexcept -> void
	{
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		ReleaseMutex(handleUnion.handle);
	}

	auto MultiProcessMutex::Open(const char* identifier) noexcept -> MultiProcessMutex
	{
		HANDLE handle = OpenMutexA(SYNCHRONIZE, false, identifier);
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.handle = handle;
		return MultiProcessMutex{ handleUnion.native, identifier };
	}

	MultiProcessMutex::MultiProcessMutex(NativeHandle handle, const char* identifier) noexcept
		: m_nativeHandle(handle)
		, m_identifier(identifier)
		, m_owned(false)
	{
	}

	Semaphore::Semaphore(usize count, const char* identifier) noexcept
		: m_identifier(identifier)
		, m_owned(true)
	{
		HANDLE handle = CreateSemaphoreA(nullptr, LONG(count), LONG(count), identifier);
		ASSERT(handle, "Failed to create Semaphore");
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.handle = handle;
		m_nativeHandle = handleUnion.native;
	}

	Semaphore::~Semaphore() noexcept
	{
		if (!m_owned)
			return;

		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		CloseHandle(handleUnion.handle);
	}

	auto Semaphore::Lock() noexcept -> void
	{
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		DWORD res = WaitForSingleObject(handleUnion.handle, INFINITE);
		ASSERT(res == WAIT_OBJECT_0, "Semaphore::Lock using WaitForSingleObject failed");
	}

	auto Semaphore::TryLock() noexcept -> bool
	{
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		DWORD res = WaitForSingleObject(handleUnion.handle, 0);
		ASSERT(res == WAIT_OBJECT_0 || res == WAIT_TIMEOUT, "Semaphore::TryLock using WaitForSingleObject failed");
		return res == WAIT_OBJECT_0;
	}

	auto Semaphore::Unlock() noexcept -> void
	{
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		BOOL res = ReleaseSemaphore(handleUnion.handle, 1, nullptr);
		ASSERT(res, "Semaphore::Unlock failed to release using ReleaseSemaphore");
	}

	auto Semaphore::Open(const char* identifier) noexcept -> Semaphore
	{
		HANDLE handle = OpenSemaphoreA(SYNCHRONIZE, false, identifier);
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.handle = handle;
		return Semaphore{ handleUnion.native, identifier };
	}

	Semaphore::Semaphore(NativeHandle handle, const char* identifier) noexcept
		: m_nativeHandle(handle)
		, m_identifier(identifier)
		, m_owned(false)
	{
	}

	Event::Event(bool initialState, bool manualReset, const char* identifier) noexcept
		: m_identifier(identifier)
		, m_owned(true)
	{
		HANDLE handle = CreateEventA(nullptr, BOOL(manualReset),  BOOL(initialState), identifier);
		ASSERT(handle, "Failed to create Semaphore");
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.handle = handle;
		m_nativeHandle = handleUnion.native;
	}

	Event::~Event() noexcept
	{
		if (!m_owned)
			return;
		
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		CloseHandle(handleUnion.handle);
	}

	auto Event::Signal() -> void
	{
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		BOOL res = SetEvent(handleUnion.handle);
		ASSERT(res, "Signaling Event using SetEvent failed");
	}

	auto Event::Unsignal() -> void
	{
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		BOOL res = ResetEvent(handleUnion.handle);
		ASSERT(res, "Unsignaling Event using ResetEvent failed");
	}

	auto Event::Wait() -> void
	{
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.native = m_nativeHandle;
		DWORD res = WaitForSingleObject(handleUnion.handle, INFINITE);
		ASSERT(res == WAIT_OBJECT_0, "Event::Wait using WaitForSingleObject failed");
	}

	auto Event::Open(const char* identifier) noexcept -> Event
	{
		HANDLE handle = OpenEventA(SYNCHRONIZE, false, identifier);
		union
		{
			HANDLE handle;
			NativeHandle native;
		} handleUnion;
		handleUnion.handle = handle;
		return Event{ handleUnion.native, identifier };
	}

	Event::Event(NativeHandle handle, const char* identifier) noexcept
		: m_nativeHandle(handle)
		, m_identifier(identifier)
		, m_owned(false)
	{
	}
}

#endif