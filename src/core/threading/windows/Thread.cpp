#include "../Thread.h"

#include "core/platform/SystemInfo.h"

#if PLATFORM_WINDOWS
#include "core/platform/Platform.h"

namespace Core::Threading
{
	Thread::Thread() noexcept
		: m_handle(INVALID_HANDLE_VALUE)
		, m_threadId(ThreadID(-1))
		, m_current(false)
	{
	}

	Thread::Thread(Thread&& other) noexcept
		: m_attribs(other.m_attribs)
		, m_handle(other.m_handle)
		, m_threadId(other.m_threadId)
	{
		m_current = m_handle == ::GetCurrentThread();
		other.m_handle = INVALID_HANDLE_VALUE;
	}

	Thread::~Thread() noexcept
	{
		// We expect the user to terminate correctly, so just detach it here
		Detach();
	}

	auto Thread::operator=(Thread&& other) noexcept -> Thread&
	{
		this->~Thread();

		m_attribs = other.m_attribs;
		m_handle = other.m_handle;
		m_threadId = other.m_threadId;

		m_current = m_handle == ::GetCurrentThread();
		other.m_handle = INVALID_HANDLE_VALUE;

		return *this;
	}

	auto Thread::Resume() noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		DWORD count = ::ResumeThread(m_handle);
		if (count == DWORD(-1))
			return TranslateSystemError();

		m_attribs.suspended = count > 1;
		return SystemErrorCode::Success;
	}

	auto Thread::Suspend() noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		// We are writing for 64-bit only, so use the 64-bit version
		u32 count = ::Wow64SuspendThread(m_handle);
		if (count == DWORD(-1))
			return TranslateSystemError();

		m_attribs.suspended = count > 1;
		return SystemErrorCode::Success;

	}

	void Thread::Join() noexcept
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return;

		WaitForSingleObject(m_handle, INFINITE);
	}

	void Thread::Detach() noexcept
	{
		if (m_handle != INVALID_HANDLE_VALUE)
			::CloseHandle(m_handle);
	}

	auto Thread::SetDescription(const String& desc) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		m_attribs.desc = desc;
		if (!desc.IsEmpty())
		{
			DynArray<char16_t> utf16 = m_attribs.desc.ToUtf16();
			HRESULT hr = ::SetThreadDescription(m_handle, reinterpret_cast<PCWSTR>(utf16.Data()));
			if (FAILED(hr))
				return { SystemErrorCode::CouldNotSetDesc, "Could not set the thread description" };
		}
		return SystemErrorCode::Success;
	}

	auto Thread::SetPriority(ThreadPriority priority) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		i32 winPriority;
		switch (priority)
		{
		case ThreadPriority::Idle:         winPriority = THREAD_PRIORITY_IDLE;          break;
		case ThreadPriority::VeryLow:      winPriority = THREAD_PRIORITY_LOWEST;        break;
		case ThreadPriority::Low:          winPriority = THREAD_PRIORITY_BELOW_NORMAL;  break;
		case ThreadPriority::Normal:       winPriority = THREAD_PRIORITY_NORMAL;        break;
		case ThreadPriority::High:         winPriority = THREAD_PRIORITY_ABOVE_NORMAL;  break;
		case ThreadPriority::VeryHigh:     winPriority = THREAD_PRIORITY_HIGHEST;       break;
		case ThreadPriority::TimeCritical: winPriority = THREAD_PRIORITY_TIME_CRITICAL; break;
		default:                           winPriority = THREAD_PRIORITY_NORMAL;        break;
		}

		bool res = ::SetThreadPriority(m_handle, winPriority);
		if (!res)
			return TranslateSystemError();

		m_attribs.priority = priority;
		return SystemErrorCode::Success;
	}

	auto Thread::SetMemoryPriority(ThreadMemoryPriority priority) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		MEMORY_PRIORITY_INFORMATION info;
		switch (priority)
		{
		case ThreadMemoryPriority::Lowest:  info.MemoryPriority = MEMORY_PRIORITY_LOWEST;       break;
		case ThreadMemoryPriority::VeryLow: info.MemoryPriority = MEMORY_PRIORITY_VERY_LOW;     break;
		case ThreadMemoryPriority::Low:     info.MemoryPriority = MEMORY_PRIORITY_LOW;          break;
		case ThreadMemoryPriority::Medium:  info.MemoryPriority = MEMORY_PRIORITY_MEDIUM;       break;
		case ThreadMemoryPriority::High:    info.MemoryPriority = MEMORY_PRIORITY_BELOW_NORMAL; break;
		case ThreadMemoryPriority::Default: info.MemoryPriority = MEMORY_PRIORITY_NORMAL;       break;
		default:                            info.MemoryPriority = MEMORY_PRIORITY_NORMAL;       break;
		}

		bool res = ::SetThreadInformation(m_handle, _THREAD_INFORMATION_CLASS::ThreadMemoryPriority, &info, sizeof(MEMORY_PRIORITY_INFORMATION));
		if (!res)
			return TranslateSystemError();

		m_attribs.memPriority = priority;
		return SystemErrorCode::Success;
	}

	auto Thread::SetPowerThrottling(ThreadPowerThrottling throttling) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		THREAD_POWER_THROTTLING_STATE info;
		info.Version = THREAD_POWER_THROTTLING_CURRENT_VERSION;

		switch (throttling)
		{
		case ThreadPowerThrottling::Disabled:
			info.ControlMask = THREAD_POWER_THROTTLING_EXECUTION_SPEED;
			info.StateMask   = 0;
			break;
		case ThreadPowerThrottling::Enabled:
			info.ControlMask = THREAD_POWER_THROTTLING_EXECUTION_SPEED;
			info.StateMask   = THREAD_POWER_THROTTLING_EXECUTION_SPEED;
			break;
		case ThreadPowerThrottling::Auto:
		default:
			info.ControlMask = 0;
			info.StateMask   = 0;
			break;
		}

		bool res = ::SetThreadInformation(m_handle, _THREAD_INFORMATION_CLASS::ThreadPowerThrottling, &info, sizeof(THREAD_POWER_THROTTLING_STATE));
		if (!res)
			return TranslateSystemError();

		m_attribs.powerThrottling = throttling;
		return SystemErrorCode::Success;
	}

	auto Thread::SetPriorityBoost(bool allow) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		bool res = ::SetThreadPriorityBoost(m_handle, !allow);
		if (!res)
			return TranslateSystemError();

		m_attribs.priorityBoost = allow;
		return SystemErrorCode::Success;
	}

	auto Thread::SetCpuSetAffinity(const DynArray<u32>& ids) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		bool res = ::SetThreadSelectedCpuSets(m_handle, reinterpret_cast<const ULONG*>(ids.Data()), ULONG(ids.Size()));
		if (!res)
			return TranslateSystemError();

		return SystemErrorCode::Success;
	}

	auto Thread::SetLogicalAffinity(u32 core, u32 processor) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		ULONG id = g_SystemInfo.GetCpuSetIdForCore(core, processor);
		bool res = ::SetThreadSelectedCpuSets(m_handle, &id, 1);
		if (!res)
			return TranslateSystemError();

		return SystemErrorCode::Success;
	}

	auto Thread::SetLogicalAffinity(const DynArray<u32>& cores, u32 processor) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		DynArray<u32> ids{ cores.Size(), g_GlobalAlloc };
		for (u32 core : cores)
			ids.Add(g_SystemInfo.GetCpuSetIdForCore(core, processor));
		return SetCpuSetAffinity(ids);
	}

	auto Thread::SetPhysicalAffinity(u32 core, u32 processor) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		STATIC_ASSERT(sizeof(u32) == sizeof(ULONG), "types need to match");
		DynArray<u32> ids = g_SystemInfo.GetCpuSetIdsForPhysicalCore(core, processor);
		return SetCpuSetAffinity(ids);
	}

	auto Thread::SetPhysicalAffinity(const DynArray<u32>& cores, u32 processor) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		STATIC_ASSERT(sizeof(u32) == sizeof(ULONG), "types need to match");

		DynArray<u32> ids{ cores.Size() * 2, g_GlobalAlloc };
		for (u32 core : cores)
			ids.Add(g_SystemInfo.GetCpuSetIdsForPhysicalCore(core, processor));
		return SetCpuSetAffinity(ids);
	}

	auto Thread::ResetAffinity() noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		bool res = ::SetThreadSelectedCpuSets(m_handle, nullptr, 0);
		if (!res)
			return TranslateSystemError();

		return SystemErrorCode::Success;
	}

	auto Thread::GetCoreIndex() const noexcept -> usize
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return usize(-1);

		PROCESSOR_NUMBER processorNumber;
		::GetCurrentProcessorNumberEx(&processorNumber);
		return g_SystemInfo.GroupRelativeToCoreIndex(processorNumber.Group, processorNumber.Number);
	}

	auto Thread::GetCpuSetAffinity() const noexcept -> DynArray<u32>
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return DynArray<u32>{};

		ULONG len;
		::GetThreadSelectedCpuSets(m_handle, nullptr, 0, &len);

		DynArray<u32> ids;
		ids.Resize(len);
		::GetThreadSelectedCpuSets(m_handle, reinterpret_cast<PULONG>(ids.Data()), len, &len);
		return ids;
	}

	auto Thread::GetLogicalAffinity() const noexcept -> DynArray<DynArray<u32>>
	{
		DynArray<u32> ids = GetCpuSetAffinity();
		return g_SystemInfo.GetLogicalCoresForCpuSetIds(ids);
	}

	auto Thread::GetPhysicalAffinity() const noexcept -> DynArray<DynArray<u32>>
	{
		DynArray<u32> ids = GetCpuSetAffinity();
		return g_SystemInfo.GetPhysicalCoresForCpuSetIds(ids);
	}

	auto Thread::GetExitCode() const noexcept -> Optional<u32>
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return NullOpt;

		DWORD exitCode;
		bool res = ::GetExitCodeThread(m_handle, &exitCode);
		if (!res || exitCode == STILL_ACTIVE)
			return NullOpt;
		return exitCode;
	}

	auto Thread::GetProcessId() const noexcept -> u32
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return u32(-1);

		return ::GetProcessIdOfThread(m_handle);
	}

	inline auto Thread::IsValid() const noexcept -> bool
	{
		return m_handle != INVALID_HANDLE_VALUE;
	}

	auto Thread::ToDebugString() const noexcept -> String
	{
		String coreInfo;
		DynArray<DynArray<u32>> cores = GetLogicalAffinity();
		for (usize i = 0; i < cores.Size(); ++i)
		{
			if (i != 0)
				coreInfo += ' ';

			coreInfo += Format("processor={} cores=["_s, i);
			for (usize j = 0; j < cores[i].Size(); ++j)
			{
				if (j != 0)
					coreInfo += ", "_s;
				coreInfo += ToString(cores[i][j]);
			}
			coreInfo += ']';
		}

		String priority;
		switch (m_attribs.priority)
		{
		case ThreadPriority::Idle:         priority = "Idle"_s;         break;
		case ThreadPriority::VeryLow:      priority = "VeryLow"_s;      break;
		case ThreadPriority::Low:          priority = "Low"_s;          break;
		case ThreadPriority::Normal:       priority = "Normal"_s;       break;
		case ThreadPriority::High:         priority = "High"_s;         break;
		case ThreadPriority::VeryHigh:     priority = "VeryHigh"_s;     break;
		case ThreadPriority::TimeCritical: priority = "TimeCritical"_s; break;
		default: break;
		}

		String memPriority;
		switch (m_attribs.memPriority)
		{
		case ThreadMemoryPriority::Lowest:  memPriority = "Lowest"_s;           break;
		case ThreadMemoryPriority::VeryLow: memPriority = "VeryLow"_s;          break;
		case ThreadMemoryPriority::Low:     memPriority = "Low"_s;              break;
		case ThreadMemoryPriority::Medium:  memPriority = "Medium"_s;           break;
		case ThreadMemoryPriority::High:    memPriority = "High"_s;             break;
		case ThreadMemoryPriority::Default: memPriority = "VeryHigh/Default"_s; break;
		default: break;
		}

		String powerThrottling;
		switch (m_attribs.powerThrottling)
		{
		case ThreadPowerThrottling::Disabled: powerThrottling = "Disabled"_s; break;
		case ThreadPowerThrottling::Auto:     powerThrottling = "Auto"_s;     break;
		case ThreadPowerThrottling::Enabled:  powerThrottling = "Enabled"_s;  break;
		default: break;
		}

		return Format("Thread: tid={} desc=\"{}\" {} stacksize={} suspended={} priorityBost={} priority={} memoryPriority={} powerThrottling={}"_s,
		              m_threadId,
		              m_attribs.desc,
		              coreInfo,
		              m_attribs.stackSize,
		              m_attribs.suspended,
		              m_attribs.priorityBoost,
		              priority,
					  memPriority,
					  powerThrottling);
	}

	auto Thread::FromNativeHandle(NativeHandle handle) noexcept -> Thread
	{
		Thread thread;
		thread.m_handle = handle;
		thread.m_threadId = ::GetThreadId(handle);
		thread.m_current = handle == ::GetCurrentThread();

		UINT_PTR stackStart;
		UINT_PTR stackEnd;
		::GetCurrentThreadStackLimits(&stackStart, &stackEnd);
		thread.m_attribs.stackSize = usize(stackEnd - stackStart);

		PWSTR pDesc;
		HRESULT hr = ::GetThreadDescription(handle, &pDesc);
		if (SUCCEEDED(hr))
		{
			thread.m_attribs.desc.Assign(reinterpret_cast<char16_t*>(pDesc));
			LocalFree(pDesc);
		}

		i32 threadPriority = ::GetThreadPriority(handle);
		switch (threadPriority)
		{
		case THREAD_PRIORITY_IDLE:          thread.m_attribs.priority = ThreadPriority::Idle;         break;
		case THREAD_PRIORITY_LOWEST:        thread.m_attribs.priority = ThreadPriority::VeryLow;      break;
		case THREAD_PRIORITY_BELOW_NORMAL:  thread.m_attribs.priority = ThreadPriority::Low;          break;
		case THREAD_PRIORITY_NORMAL:        thread.m_attribs.priority = ThreadPriority::Normal;       break;
		case THREAD_PRIORITY_ABOVE_NORMAL:  thread.m_attribs.priority = ThreadPriority::High;         break;
		case THREAD_PRIORITY_HIGHEST:       thread.m_attribs.priority = ThreadPriority::VeryHigh;     break;
		case THREAD_PRIORITY_TIME_CRITICAL: thread.m_attribs.priority = ThreadPriority::TimeCritical; break;
		default: break;
		}

		MEMORY_PRIORITY_INFORMATION memInfo;
		bool res = ::GetThreadInformation(handle, _THREAD_INFORMATION_CLASS::ThreadMemoryPriority, &memInfo, sizeof(MEMORY_PRIORITY_INFORMATION));
		if (res)
		{
			switch (memInfo.MemoryPriority)
			{
			case MEMORY_PRIORITY_LOWEST:       thread.m_attribs.memPriority = ThreadMemoryPriority::Lowest;  break;
			case MEMORY_PRIORITY_VERY_LOW:     thread.m_attribs.memPriority = ThreadMemoryPriority::VeryLow; break;
			case MEMORY_PRIORITY_LOW:          thread.m_attribs.memPriority = ThreadMemoryPriority::Low;     break;
			case MEMORY_PRIORITY_MEDIUM:       thread.m_attribs.memPriority = ThreadMemoryPriority::Medium;  break;
			case MEMORY_PRIORITY_BELOW_NORMAL: thread.m_attribs.memPriority = ThreadMemoryPriority::High;    break;
			case MEMORY_PRIORITY_NORMAL:       thread.m_attribs.memPriority = ThreadMemoryPriority::Default; break;
			default:;
			}
		}

		THREAD_POWER_THROTTLING_STATE powerState;
		res = ::GetThreadInformation(handle, _THREAD_INFORMATION_CLASS::ThreadPowerThrottling, &powerState, sizeof(THREAD_POWER_THROTTLING_STATE));
		if (res)
		{
			if (powerState.ControlMask == 0)
				thread.m_attribs.powerThrottling = ThreadPowerThrottling::Auto;
			else if (powerState.StateMask == 0)
				thread.m_attribs.powerThrottling = ThreadPowerThrottling::Disabled;
			else
				thread.m_attribs.powerThrottling = ThreadPowerThrottling::Enabled;
		}

		return thread;
	}

	auto Thread::FromCurrent() noexcept -> Thread
	{
		return FromNativeHandle(::GetCurrentThread());
	}

	void Thread::Init(void* pInvoke, void* pData) noexcept
	{
		m_handle = ::CreateThread(nullptr,
								  m_attribs.stackSize,
								  reinterpret_cast<LPTHREAD_START_ROUTINE>(pInvoke),
								  pData,
								  m_attribs.suspended ? CREATE_SUSPENDED : 0,
								  reinterpret_cast<LPDWORD>(&m_threadId));

		if (m_handle == INVALID_HANDLE_VALUE)
			return;
		
		if (!m_attribs.desc.IsEmpty())
		{
			DynArray<char16_t> utf16 = m_attribs.desc.ToUtf16();
			::SetThreadDescription(m_handle, reinterpret_cast<PCWSTR>(utf16.Data()));
		}

		SetPriority(m_attribs.priority);
		SetPriorityBoost(m_attribs.priorityBoost);
		SetMemoryPriority(m_attribs.memPriority);
		SetPowerThrottling(m_attribs.powerThrottling);
	}

	auto Thread::HasIOPending() const noexcept -> bool
	{
		BOOL pending;
		bool res =::GetThreadIOPendingFlag(m_handle, &pending);
		return res && pending;
	}

	auto GetCurrentThreadId() noexcept -> ThreadID
	{
		return ThreadID(::GetCurrentThreadId());
	}

	void ExitThread(u32 exitCode) noexcept
	{
		::ExitThread(exitCode);
	}
}

#endif