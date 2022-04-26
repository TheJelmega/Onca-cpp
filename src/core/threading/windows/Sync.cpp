#include "core/MinInclude.h"

#if PLATFORM_WINDOWS
#include "core/Assert.h"
#include "core/threading/Sync.h"
#include "core/platform/Platform.h"

namespace Onca::Threading
{
	Mutex::Mutex() noexcept
	{
		InitializeCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>(&m_nativeHandle));
	}

	Mutex::~Mutex() noexcept
	{
		DeleteCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>(&m_nativeHandle));
	}

	void Mutex::Lock() noexcept
	{
		EnterCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>(&m_nativeHandle));
	}

	auto Mutex::TryLock() noexcept -> bool
	{
		return TryEnterCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>(&m_nativeHandle));
	}

	void Mutex::Unlock() noexcept
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

	void TimedMutex::Lock() noexcept
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

	void TimedMutex::Unlock() noexcept
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

	void MultiProcessMutex::Lock() noexcept
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

	void MultiProcessMutex::Unlock() noexcept
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

	void Semaphore::Lock() noexcept
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

	void Semaphore::Unlock() noexcept
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

	void Event::Signal()
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

	void Event::Unsignal()
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

	void Event::Wait()
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