#include "Process.h"

namespace Core
{
	auto Process::GetPath() const noexcept -> const FileSystem::Path&
	{
		return m_path;
	}

	auto Process::GetCmdLine() const noexcept -> const String&
	{
		return m_cmdLine;
	}

	auto Process::GetEnvironment() const noexcept -> const String&
	{
		return m_environment;
	}

	auto Process::GetPriority() const noexcept -> ProcessPriority
	{
		return m_priority;
	}

	auto Process::GetMemoryPriority() const noexcept -> ProcessMemoryPriority
	{
		return m_memPriority;
	}

	auto Process::GetPowerThrottling() const noexcept -> ProcessPowerThrottling
	{
		return m_powerThrottling;
	}

	auto Process::GetProcessId() const noexcept -> ProcessID
	{
		return m_id;
	}

	auto Process::GetWorkingDir() const noexcept -> const FileSystem::Path&
	{
		return m_workingDir;
	}

	auto Process::GetNativeHandle() const noexcept -> NativeHandle
	{
		return m_handle;
	}
}
