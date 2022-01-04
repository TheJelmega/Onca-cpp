#pragma once
#include "SystemError.h"
#include "core/MinInclude.h"
#include "core/filesystem/Path.h"
#include "core/string/Include.h"
#include "core/threading/Thread.h"
#include "core/utils/Result.h"

namespace Core
{
	using ProcessID = u32;

	enum class ProcessCreateFlag
	{
		None            = 0     , ///< None
		NewConsole      = BIT(0), ///< Create a new console window/terminal for this process
		NewProcessGroup = BIT(1), ///< The process is the new root process for a process group
		NoWindow        = BIT(2), ///< The process is a console application, but run it without a creating a console window/terminal, but can still attach to its parent's console
		CreateSuspended = BIT(3), ///< Create the process with it's initial thread in a suspended state, which needs to be manually resumed
		Detached        = BIT(4), ///< THe process does not inherit the same console/terminal as its parent, cannot attach to its parent's console
	};
	DEFINE_FLAGS(ProcessCreateFlag);

	enum class ProcessPriority
	{
		Idle    , ///< Idle, process will only run when the system is idling and no process with a higher priority needs to be run
		Low     , ///< Low
		Normal  , ///< Normal
		High    , ///< High
		VeryHigh, ///< Very high, intended for time-critical applications, CPU bound applications can use up most available cycles
		Realtime, ///< Realtime, highest possible priority, will run even before OS processes, so might cause system hitches or other issues, like caches not being flushed or unresponsive input
	};

	enum class ProcessMemoryPriority : u8
	{
		Lowest , ///< Lowest
		VeryLow, ///< Very low
		Low    , ///< Low
		Medium , ///< Medium
		High   , ///< High
		Default, ///< Default or very high
	};

	enum class ProcessPowerThrottling : u8
	{
		Disabled, ///< Disable power throttling
		Auto    , ///< Automatic power throttling
		Enabled , ///< Enable power throttling
	};

	struct ProcessCreateInfo
	{
		FileSystem::Path       path;                                             ///< Path to process
		FileSystem::Path       workingDir;                                       ///< Working directory for the process, when this string is empty, inherit it from the parent process
		String                 cmdLine;                                          ///< Command line arguments
		String                 environment;                                      ///< Environment setting, use the parent process' when this string is empty, inherit it from the parent process
		ProcessCreateFlags     flags           = ProcessCreateFlag::None;        ///< Flags
		ProcessPriority        priority        = ProcessPriority::Normal;        ///< Process priority
		ProcessMemoryPriority  memPriority     = ProcessMemoryPriority::Default; ///< Process memory priority
		ProcessPowerThrottling powerThrottling = ProcessPowerThrottling::Auto;   ///< Thread power throttling
		bool                   inheritHandles  = false;                          ///< Inherit handles from the parent process
	};

	class CORE_API Process
	{
	public:
		DEFINE_OPAQUE_HANDLE(NativeHandle);

		Process();

		Process(const Process&) = delete;
		Process(Process&& other) noexcept;
		~Process();

		auto operator=(const Process&) = delete;
		auto operator=(Process&& other) noexcept -> Process&;

		/**
		 * Set the process' priority
		 * \param[in] priority Priority
		 * \return Error
		 */
		auto SetPriority(ProcessPriority priority) noexcept -> SystemError;
		/**
		 * Set the process' memory priority
		 * \param[in] priority Memory priority
		 * \return Error
		 */
		auto SetMemoryPriority(ProcessMemoryPriority priority) noexcept -> SystemError;
		/**
		 * Set the process power throttling state
		 * \param[in] throttling Process power throttling state
		 * \return Error
		 */
		auto SetPowerThrottling(ProcessPowerThrottling throttling) noexcept -> SystemError;
		/**
		 * Set if the process allows priority boost
		 * \param[in] allow Allow process priority boost
		 * \return Error
		 * \note Currently only has an effect on windows
		 */
		auto SetPriorityBoost(bool allow) noexcept -> SystemError;

		/**
		 * Set the core affinity only to the given CPU set ids for any new threads created in this process
		 * \param[in] ids CPU set ids
		 * \return Error
		 */
		auto SetDefaultCpuSetAffinity(const DynArray<u32>& ids) noexcept -> SystemError;
		/**
		 * Set the core affinity to the given logical cores for any new threads created in this process
		 * \param[in] cores Logical core indices
		 * \param[in] processor Processor index
		 * \return Error
		 */
		auto SetDefaultLogicalAffinity(const DynArray<u32>& cores, u32 processor) noexcept -> SystemError;
		/**
		 * Set the core affinity to the given physical cores for any new threads created in this process
		 * \param[in] cores Logical core indices
		 * \param[in] processor Processor index
		 * \return Error
		 */
		auto SetDefaultPhysicalAffinity(const DynArray<u32>& cores, u32 processor) noexcept -> SystemError;

		/**
		 * Get the path to the executable
		 * \return Path to the executable
		 */
		auto GetPath() const noexcept -> const FileSystem::Path&;
		/**
		 * Get the path to the process' working directory
		 * \return Path to the process' working directory
		 */
		auto GetWorkingDir() const noexcept -> const FileSystem::Path&;
		/**
		 * Get the command line arguments
		 * \return Command line arguments
		 */
		auto GetCmdLine() const noexcept -> const String&;
		/**
		 * Get the path to the process' working environment
		 * \return Path to the process' working environment
		 */
		auto GetEnvironment() const noexcept -> const String&;
		/**
		 * Get the path to the process' priority
		 * \return Path to the process' priority
		 */
		auto GetPriority() const noexcept -> ProcessPriority;
		/**
		 * Get the path to the process' memory priority
		 * \return Path to the process' memory priority
		 */
		auto GetMemoryPriority() const noexcept -> ProcessMemoryPriority;
		/**
		 * Get the path to the process' power throttling state
		 * \return Path to the process' power throttling state
		 */
		auto GetPowerThrottling() const noexcept -> ProcessPowerThrottling;
		/**
		 * Get the number of handles opened by the process
		 * \return Number of handles opened by the process
		 */
		auto GetHandleCount() const noexcept -> usize;
		/**
		 * Get the path to the process' id
		 * \return Path to the process' id
		 */
		auto GetProcessId() const noexcept -> ProcessID;
		/**
		 * Get the native handle
		 * \return Native handle
		 */
		auto GetNativeHandle() const noexcept -> NativeHandle;
		/**
		 * Get the CPU set ids of the assigned CPU set ids
		 * \return CPU set ids of the assigned CPU set ids
		 */
		auto GetDefaultCpuSetAffinity() const noexcept -> DynArray<u32>;
		/**
		 * Get the CPU set ids of the assigned logical cores
		 * \return CPU set ids of the assigned logical cores
		 */
		auto GetDefaultLogicalAffinity() const noexcept -> DynArray<DynArray<u32>>;
		/**
		 * Get the CPU set ids of the assigned physical cores
		 * \return CPU set ids of the assigned physical cores
		 */
		auto GetDefaultPhysicalAffinity() const noexcept -> DynArray<DynArray<u32>>;
		/**
		 * Get the exit code of the process, a null optional will be returned if the process has not yet exited
		 * \return Optional with value
		 */
		auto GetExitCode() const noexcept -> Optional<u32>;

		/**
		 * Get a string with the debug representation of the process
		 * \return Debug string
		 */
		auto ToDebugString() const noexcept -> String;

		/**
		 * Create a new process
		 * \param[in] createInfo Create info
		 * \return Result with a pair, containing the process and it's main thread, or an error
		 */
		static auto Create(const ProcessCreateInfo& createInfo) noexcept -> Result<Pair<Process, Threading::Thread>, SystemError>;

		/**
		 * Get the current process
		 * \return Current process
		 */
		static auto FromCurrent() noexcept -> Process;

	private:
		NativeHandle           m_handle;           ///< Native handle to process
		FileSystem::Path       m_path;             ///< Path to process executable
		FileSystem::Path       m_workingDir;       ///< Working directory
		String                 m_cmdLine;          ///< Command line arguments
		String                 m_environment;      ///< Environment settings
		ProcessPriority        m_priority;         ///< Process priority
		ProcessMemoryPriority  m_memPriority;      ///< Process memory priority
		ProcessPowerThrottling m_powerThrottling;  ///< Process power throttling

		ProcessID              m_id;               ///< Process ID

		bool                   m_current;          ///< Whether the process represents the current process
	};
}

DEFINE_ENUM_FLAG_OPS(Core::ProcessCreateFlag);
