#include "core/platform/Process.h"

#include "core/logging/Logger.h"
#include "core/platform/SystemInfo.h"

#if PLATFORM_WINDOWS
#include "core/platform/Platform.h"

namespace Onca
{
	Process::Process()
		: m_handle(INVALID_HANDLE_VALUE)
		, m_priority(ProcessPriority::Normal)
		, m_id(ProcessID(-1))
		, m_current(false)
	{
	}

	Process::Process(Process&& other) noexcept
		: m_handle(other.m_handle)
		, m_path(Move(other.m_path))
		, m_workingDir(Move(other.m_workingDir))
		, m_cmdLine(Move(other.m_cmdLine))
		, m_environment(Move(other.m_environment))
		, m_priority(other.m_priority)
		, m_memPriority(other.m_memPriority)
		, m_powerThrottling(other.m_powerThrottling)
		, m_id(other.m_id)
		, m_current(other.m_current)
	{
		other.m_handle = INVALID_HANDLE_VALUE;
		other.m_current = false;
	}

	Process::~Process()
	{
		if (m_handle != INVALID_HANDLE_VALUE)
			::CloseHandle(m_handle);
	}

	auto Process::operator=(Process&& other) noexcept -> Process&
	{
		this->~Process();

		m_handle = other.m_handle;
		m_path = Move(other.m_path);
		m_workingDir = Move(other.m_workingDir);
		m_cmdLine = Move(other.m_cmdLine);
		m_environment = Move(other.m_environment);
		m_priority = other.m_priority;
		m_memPriority = other.m_memPriority;
		m_powerThrottling = other.m_powerThrottling;
		m_id = other.m_id;
		m_current = other.m_current;

		other.m_handle = INVALID_HANDLE_VALUE;
		other.m_current = false;
		return *this;
	}

	auto Process::SetPriority(ProcessPriority priority) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE && !m_current)
			return SystemErrorCode::InvalidHandle;

		DWORD priorityClass;
		switch (priority)
		{
		case ProcessPriority::Idle:     priorityClass = IDLE_PRIORITY_CLASS;         break;
		case ProcessPriority::Low:      priorityClass = BELOW_NORMAL_PRIORITY_CLASS; break;
		case ProcessPriority::Normal:   priorityClass = NORMAL_PRIORITY_CLASS;       break;
		case ProcessPriority::High:     priorityClass = ABOVE_NORMAL_PRIORITY_CLASS; break;
		case ProcessPriority::VeryHigh: priorityClass = HIGH_PRIORITY_CLASS;         break;
		case ProcessPriority::Realtime: priorityClass = REALTIME_PRIORITY_CLASS;     break;
		default:                        priorityClass = NORMAL_PRIORITY_CLASS;       break;
		}

		bool res = ::SetPriorityClass(m_handle, priorityClass);
		if (!res)
			return TranslateSystemError();
		return SystemErrorCode::Success;
	}

	auto Process::SetMemoryPriority(ProcessMemoryPriority priority) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE && !m_current)
			return SystemErrorCode::InvalidHandle;

		MEMORY_PRIORITY_INFORMATION info;
		switch (priority)
		{
		case ProcessMemoryPriority::Lowest:  info.MemoryPriority = MEMORY_PRIORITY_LOWEST;       break;
		case ProcessMemoryPriority::VeryLow: info.MemoryPriority = MEMORY_PRIORITY_VERY_LOW;     break;
		case ProcessMemoryPriority::Low:     info.MemoryPriority = MEMORY_PRIORITY_LOW;          break;
		case ProcessMemoryPriority::Medium:  info.MemoryPriority = MEMORY_PRIORITY_MEDIUM;       break;
		case ProcessMemoryPriority::High:    info.MemoryPriority = MEMORY_PRIORITY_BELOW_NORMAL; break;
		case ProcessMemoryPriority::Default: info.MemoryPriority = MEMORY_PRIORITY_NORMAL;       break;
		default:                             info.MemoryPriority = MEMORY_PRIORITY_NORMAL;       break;
		}

		bool res = ::SetProcessInformation(m_handle, _PROCESS_INFORMATION_CLASS::ProcessMemoryPriority, &info, sizeof(MEMORY_PRIORITY_INFORMATION));
		if (!res)
			return TranslateSystemError();
		return SystemErrorCode::Success;
	}

	auto Process::SetPowerThrottling(ProcessPowerThrottling throttling) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE && !m_current)
			return SystemErrorCode::InvalidHandle;

		PROCESS_POWER_THROTTLING_STATE state;
		state.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
		switch (throttling)
		{
		case ProcessPowerThrottling::Disabled:
			state.StateMask = 0;
			state.ControlMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
			break;
		case ProcessPowerThrottling::Auto:
		default:
			state.StateMask = 0;
			state.ControlMask = 0;
			break;
		case ProcessPowerThrottling::Enabled:
			state.StateMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
			state.ControlMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
			break;
		}

		bool res = ::SetProcessInformation(m_handle, _PROCESS_INFORMATION_CLASS::ProcessPowerThrottling, &state, sizeof(PROCESS_POWER_THROTTLING_STATE));
		if (!res)
			return TranslateSystemError();
		return SystemErrorCode::Success;
	}

	auto Process::SetPriorityBoost(bool allow) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE && !m_current)
			return SystemErrorCode::InvalidHandle;

		bool res = ::SetThreadPriorityBoost(m_handle, !allow);
		if (!res)
			return TranslateSystemError();
		return SystemErrorCode::Success;
	}

	auto Process::SetDefaultCpuSetAffinity(const DynArray<u32>& ids) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE && !m_current)
			return SystemErrorCode::InvalidHandle;

		bool res = ::SetProcessDefaultCpuSets(m_handle, reinterpret_cast<const ULONG*>(ids.Data()), ULONG(ids.Size()));
		if (!res)
			return TranslateSystemError();

		return SystemErrorCode::Success;
	}

	auto Process::SetDefaultLogicalAffinity(const DynArray<u32>& cores, u32 processor) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE && !m_current)
			return SystemErrorCode::InvalidHandle;

		DynArray<u32> ids{ cores.Size(), g_GlobalAlloc };
		for (u32 core : cores)
			ids.Add(g_SystemInfo.GetCpuSetIdForCore(core, processor));
		return SetDefaultCpuSetAffinity(ids);
	}

	auto Process::SetDefaultPhysicalAffinity(const DynArray<u32>& cores, u32 processor) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE && !m_current)
			return SystemErrorCode::InvalidHandle;

		DynArray<u32> ids{ cores.Size(), g_GlobalAlloc };
		for (u32 core : cores)
			ids.Add(g_SystemInfo.GetCpuSetIdsForPhysicalCore(core, processor));
		return SetDefaultCpuSetAffinity(ids);
	}

	auto Process::GetHandleCount() const noexcept -> usize
	{
		if (m_handle == INVALID_HANDLE_VALUE && !m_current)
			return 0;

		DWORD count;
		bool res = ::GetProcessHandleCount(m_handle, &count);
		return res ? count : 0;
	}

	auto Process::GetDefaultCpuSetAffinity() const noexcept -> DynArray<u32>
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return DynArray<u32>{};

		ULONG len;
		::GetProcessDefaultCpuSets(m_handle, nullptr, 0, &len);

		DynArray<u32> ids;
		ids.Resize(len);
		::GetProcessDefaultCpuSets(m_handle, reinterpret_cast<PULONG>(ids.Data()), len, &len);
		return ids;
	}

	auto Process::GetDefaultLogicalAffinity() const noexcept -> DynArray<DynArray<u32>>
	{
		DynArray<u32> ids = GetDefaultCpuSetAffinity();
		return g_SystemInfo.GetLogicalCoresForCpuSetIds(ids);
	}

	auto Process::GetDefaultPhysicalAffinity() const noexcept -> DynArray<DynArray<u32>>
	{
		DynArray<u32> ids = GetDefaultCpuSetAffinity();
		return g_SystemInfo.GetPhysicalCoresForCpuSetIds(ids);
	}

	auto Process::GetExitCode() const noexcept -> Optional<u32>
	{
		// no need to check for self pseudo handle, since you cannot get the exit code of the process requesting it
		if (m_handle == INVALID_HANDLE_VALUE)
			return NullOpt;

		DWORD exitCode;
		bool res = ::GetExitCodeProcess(m_handle, &exitCode);
		if (!res || exitCode == STILL_ACTIVE)
			return NullOpt;
		return exitCode;
	}

	auto Process::ToDebugString() const noexcept -> String
	{
		String priority;
		switch (m_priority)
		{
		case ProcessPriority::Idle:     priority = "Idle"_s;     break;
		case ProcessPriority::Low:      priority = "Low"_s;      break;
		case ProcessPriority::Normal:   priority = "Normal"_s;   break;
		case ProcessPriority::High:     priority = "High"_s;     break;
		case ProcessPriority::VeryHigh: priority = "VeryHigh"_s; break;
		case ProcessPriority::Realtime: priority = "Realtime"_s; break;
		default: break;
		}

		String memPriority;
		switch (m_memPriority)
		{
		case ProcessMemoryPriority::Lowest:  memPriority = "Lowest"_s;           break;
		case ProcessMemoryPriority::VeryLow: memPriority = "VeryLow"_s;          break;
		case ProcessMemoryPriority::Low:     memPriority = "Low"_s;              break;
		case ProcessMemoryPriority::Medium:  memPriority = "Medium"_s;           break;
		case ProcessMemoryPriority::High:    memPriority = "High"_s;             break;
		case ProcessMemoryPriority::Default: memPriority = "VeryHigh/Default"_s; break;
		default: break;
		}

		String powerThrottling;
		switch (m_powerThrottling)
		{
		case ProcessPowerThrottling::Disabled: powerThrottling = "Disabled"_s; break;
		case ProcessPowerThrottling::Auto:     powerThrottling = "Auto"_s;     break;
		case ProcessPowerThrottling::Enabled:  powerThrottling = "Enabled"_s;  break;
		default: break;
		}

		return Format("Process: pid={}, cmdLine=\"{}\" workingDir=\"{}\" priority={} memoryPriority={} powerThrottling={}"_s,
					  m_id,
					  m_cmdLine,
					  m_workingDir,
					  priority,
					  memPriority,
					  powerThrottling);
	}

	auto Process::Create(const ProcessCreateInfo& createInfo) noexcept -> Result<Pair<Process, Threading::Thread>, SystemError>
	{
		Process process;
		process.m_priority = createInfo.priority;

		DynArray<char16_t> utf16Path;
		if (!createInfo.path.IsEmpty())
			utf16Path = ("\\\\?\\"_path + createInfo.path.AsAbsolute()).ToNative().GetString().ToUtf16();

		DynArray<char16_t> utf16WorkingDir;
		if (!createInfo.path.IsEmpty())
			utf16WorkingDir = ("\\\\?\\"_path + createInfo.path.AsAbsolute()).ToNative().GetString().ToUtf16();

		DynArray<char16_t> utf16CmdLine = createInfo.cmdLine.ToUtf16();
		DynArray<char16_t> utf16Environment = createInfo.cmdLine.ToUtf16();


		DWORD creationFlags = CREATE_UNICODE_ENVIRONMENT;

		if (createInfo.flags & ProcessCreateFlag::NewConsole)
			creationFlags |= CREATE_NEW_CONSOLE;
		if (createInfo.flags & ProcessCreateFlag::NewProcessGroup)
			creationFlags |= CREATE_NEW_PROCESS_GROUP;
		if (createInfo.flags & ProcessCreateFlag::NoWindow)
			creationFlags |= CREATE_NO_WINDOW;
		if (createInfo.flags & ProcessCreateFlag::CreateSuspended)
			creationFlags |= CREATE_SUSPENDED;
		if (createInfo.flags & ProcessCreateFlag::Detached)
			creationFlags |= DETACHED_PROCESS;

		DWORD priority;
		switch (createInfo.priority)
		{
		case ProcessPriority::Idle:     priority = IDLE_PRIORITY_CLASS;         break;
		case ProcessPriority::Low:      priority = BELOW_NORMAL_PRIORITY_CLASS; break;
		case ProcessPriority::Normal:   priority = NORMAL_PRIORITY_CLASS;       break;
		case ProcessPriority::High:     priority = ABOVE_NORMAL_PRIORITY_CLASS; break;
		case ProcessPriority::VeryHigh: priority = HIGH_PRIORITY_CLASS;         break;
		case ProcessPriority::Realtime: priority = REALTIME_PRIORITY_CLASS;     break;
		default:                        priority = NORMAL_PRIORITY_CLASS;       break;
		}

		PROCESS_INFORMATION processInfo;

		STARTUPINFOW startupInfo = {};
		startupInfo.cb = sizeof(STARTUPINFOEXW);
		startupInfo.dwFlags = STARTF_USESTDHANDLES;


		bool res =::CreateProcessW(reinterpret_cast<LPCWSTR>(utf16Path.Data()),
			                       reinterpret_cast<LPWSTR>(utf16CmdLine.Data()),
			                       nullptr,
			                       nullptr,
			                       createInfo.inheritHandles,
			                       creationFlags | priority,
			                       reinterpret_cast<LPVOID>(utf16Environment.Data()),
			                       reinterpret_cast<LPCWSTR>(utf16WorkingDir.Data()),
			                       &startupInfo,
			                       &processInfo);
		if (!res)
			return TranslateSystemError();

		process.m_handle = processInfo.hProcess;
		process.m_id = processInfo.dwProcessId;

		// Set actual values
		
		utf16Path.Resize(128);
		usize pathSize = ::GetProcessImageFileNameW(process.m_handle, reinterpret_cast<LPWSTR>(utf16Path.Data()), DWORD(utf16Path.Size()));
		while (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			utf16Path.Resize(utf16Path.Size() + 128);
			pathSize = ::GetProcessImageFileNameW(nullptr, reinterpret_cast<LPWSTR>(utf16Path.Data()), DWORD(utf16Path.Size()));
		}
		process.m_path = FileSystem::Path{ String{ utf16Path.Data(), pathSize } };

		LPWSTR wCmdLine = ::GetCommandLineW();
		process.m_cmdLine.Assign(reinterpret_cast<const char16_t*>(wCmdLine));

		if (createInfo.environment.IsEmpty())
		{
			LPWCH pChStart = ::GetEnvironmentStringsW();
			LPWCH pCh = pChStart;
			while (*pCh)
			{
				usize len = StrLen(reinterpret_cast<const char16_t*>(pCh));
				process.m_environment += String{ reinterpret_cast<const char16_t*>(pCh), len } + '\n';
				pCh += len + 1;
			}
			::FreeEnvironmentStringsW(pChStart);
		}
		else
		{
			process.m_environment = createInfo.environment;
		}

		if (createInfo.workingDir.IsEmpty())
		{
			DWORD len = ::GetCurrentDirectoryW(0, nullptr);
			DynArray<char16_t> utf16Dir;
			utf16Dir.Resize(len);
			::GetCurrentDirectoryW(len, reinterpret_cast<LPWSTR>(utf16Dir.Data()));
			process.m_workingDir = FileSystem::Path{ String{ utf16Dir.Data(), len } };
		}
		else
		{
			process.m_workingDir = createInfo.workingDir;
		}

		(void)process.SetMemoryPriority(createInfo.memPriority);
		(void)process.SetPowerThrottling(createInfo.powerThrottling);

		return Pair{ Move(process), Threading::Thread::FromNativeHandle(processInfo.hThread) };
	}

	auto Process::FromCurrent() noexcept -> Process
	{
		Process process;

		process.m_handle = ::GetCurrentProcess();
		process.m_current = process.m_handle == INVALID_HANDLE_VALUE;
		process.m_id = ::GetProcessId(process.m_handle);

		DynArray<char16_t> utf16Path;
		utf16Path.Resize(128);
		usize pathSize = ::GetModuleFileNameW(nullptr, reinterpret_cast<LPWSTR>(utf16Path.Data()), DWORD(utf16Path.Size()));
		while (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			utf16Path.Resize(utf16Path.Size() + 128);
			pathSize = ::GetModuleFileNameW(nullptr, reinterpret_cast<LPWSTR>(utf16Path.Data()), DWORD(utf16Path.Size()));
		}
		process.m_path = FileSystem::Path{ String{ utf16Path.Data(), pathSize } };

		LPWSTR wCmdLine = ::GetCommandLineW();
		process.m_cmdLine.Assign(reinterpret_cast<const char16_t*>(wCmdLine));

		LPWCH pChStart = ::GetEnvironmentStringsW();
		LPWCH pCh = pChStart;
		while (*pCh)
		{
			usize len = StrLen(reinterpret_cast<const char16_t*>(pCh));
			process.m_environment += String{ reinterpret_cast<const char16_t*>(pCh), len } + '\n';
			pCh += len + 1;
		}
		::FreeEnvironmentStringsW(pChStart);

		DWORD len = ::GetCurrentDirectoryW(0, nullptr);
		DynArray<char16_t> utf16Dir;
		utf16Dir.Resize(len);
		::GetCurrentDirectoryW(len, reinterpret_cast<LPWSTR>(utf16Dir.Data()));
		process.m_workingDir = FileSystem::Path{ String{ utf16Dir.Data(), len } };

		DWORD priorityClass = ::GetPriorityClass(process.m_handle);
		switch (priorityClass)
		{
			case IDLE_PRIORITY_CLASS:         process.m_priority = ProcessPriority::Idle;     break;
			case BELOW_NORMAL_PRIORITY_CLASS: process.m_priority = ProcessPriority::Low;      break;
			case NORMAL_PRIORITY_CLASS:       process.m_priority = ProcessPriority::Normal;   break;
			case ABOVE_NORMAL_PRIORITY_CLASS: process.m_priority = ProcessPriority::High;     break;
			case HIGH_PRIORITY_CLASS:         process.m_priority = ProcessPriority::VeryHigh; break;
			case REALTIME_PRIORITY_CLASS:     process.m_priority = ProcessPriority::Realtime; break;
			default:                          process.m_priority = ProcessPriority::Normal;   break;
		}

		MEMORY_PRIORITY_INFORMATION memoryPriority;
		bool res = GetProcessInformation(process.m_handle, _PROCESS_INFORMATION_CLASS::ProcessMemoryPriority, &memoryPriority, sizeof(MEMORY_PRIORITY_INFORMATION));
		if (res)
		{
			switch (memoryPriority.MemoryPriority)
			{
			case MEMORY_PRIORITY_LOWEST:       process.m_memPriority = ProcessMemoryPriority::Lowest;  break;
			case MEMORY_PRIORITY_VERY_LOW:     process.m_memPriority = ProcessMemoryPriority::VeryLow; break;
			case MEMORY_PRIORITY_LOW:          process.m_memPriority = ProcessMemoryPriority::Low;     break;
			case MEMORY_PRIORITY_MEDIUM:       process.m_memPriority = ProcessMemoryPriority::Medium;  break;
			case MEMORY_PRIORITY_BELOW_NORMAL: process.m_memPriority = ProcessMemoryPriority::High;    break;
			case MEMORY_PRIORITY_NORMAL:       process.m_memPriority = ProcessMemoryPriority::Default; break;
			default:                           process.m_memPriority = ProcessMemoryPriority::Default; break;
			}
		}


		return process;
	}
}

#endif
