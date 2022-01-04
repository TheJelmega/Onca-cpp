#pragma once
#include "core/MinInclude.h"
#include "Common.h"
#include "core/platform/SystemError.h"
#include "core/utils/Delegate.h"
#include "core/utils/Result.h"

namespace Core::Threading
{
	/**
	 * Thread priority, hints to the scheduler to when to run the thread
	 */
	enum class ThreadPriority : u8
	{
		Idle        , ///< Idle, when the thread only needs to run occasionally
		VeryLow     , ///< Very low
		Low         , ///< Low
		Normal      , ///< Normal
		High        , ///< High
		VeryHigh    , ///< Very high
		TimeCritical, ///< Time critical, try to avoid if not needed, as it may consume all system resources
	};

	/**
	 * Thread memory priority, hints to the memory manager when to trim pages
	 */
	enum class ThreadMemoryPriority : u8
	{
		Lowest , ///< Lowest
		VeryLow, ///< Very low
		Low    , ///< Low
		Medium , ///< Medium
		High   , ///< High
		Default, ///< Default or very high
	};

	enum class ThreadPowerThrottling : u8
	{
		Disabled, ///< Disable power throttling
		Auto    , ///< Automatic power throttling
		Enabled , ///< Enable power throttling
	};

	struct ThreadAttribs
	{
		bool                  suspended       = false;                         ///< Whether the thread starts in a suspended state
		bool                  priorityBoost   = true;                          ///< Whether to allow thread priority boost (dynamically increase priority)
		usize                 stackSize       = MB(1);                         ///< Stack size of the thread
		String                desc;                                            ///< Thread description
		ThreadPriority        priority        = ThreadPriority::Normal;        ///< Thread priority
		ThreadMemoryPriority  memPriority     = ThreadMemoryPriority::Default; ///< Thread memory priority
		ThreadPowerThrottling powerThrottling = ThreadPowerThrottling::Auto;   ///< Thread power throttling
	};

	/**
	 * Logical thread
	 * \note Pointers to a thread should not be moved over thread boundaries, but should be explicitly moved
	 */
	class CORE_API Thread
	{
	private:

		template<typename... Args>
		struct InvokeData
		{
			Delegate<u32(Args...)> delegate;
			Tuple<Args...> arguments;
		};

	public:
		DEFINE_OPAQUE_HANDLE(NativeHandle);

		/**
		 * Create an invalid thread
		 */
		Thread() noexcept;

		Thread(const Thread&) = default;
		Thread(Thread&& other) noexcept;
		~Thread() noexcept;

		auto operator=(const Thread&) = delete;
		auto operator=(Thread&& other) noexcept -> Thread&;

		/**
		 * Resume the thread from a suspended state
		 * \return Error
		 */
		auto Resume() noexcept -> SystemError;
		/**
		 * Suspend the thread
		 * \return Error
		 * \note Use of this function should be carefully considered, if this thread holds a sync primitive, it could cause a deadlock to appear in the program
		 */
		auto Suspend() noexcept -> SystemError;
		/**
		 * Wait for the thread to exit
		 */
		auto Join() noexcept -> void;
		/**
		 * Detach the thread
		 */
		auto Detach() noexcept -> void;

		/**
		 * Set the description of the thread
		 * \param[in] desc Thread description
		 * \return Error
		 */
		auto SetDescription(const String& desc) noexcept -> SystemError;
		/**
		 * Set the thread priority
		 * \param[in] priority Thread priority
		 * \return Error
		 */
		auto SetPriority(ThreadPriority priority) noexcept -> SystemError;
		/**
		 * Set the thread memory priority
		 * \param[in] priority Thread memory priority
		 * \return Error
		 */
		auto SetMemoryPriority(ThreadMemoryPriority priority) noexcept -> SystemError;
		/**
		 * Set the thread power throttling state
		 * \param[in] throttling Thread power throttling state
		 * \return Error
		 */
		auto SetPowerThrottling(ThreadPowerThrottling throttling) noexcept -> SystemError;
		/**
		 * Set if the thread allows priority boost
		 * \param[in] allow Allow thread priority boost
		 * \return Error
		 * \note Currently only has an effect on windows
		 */
		auto SetPriorityBoost(bool allow) noexcept -> SystemError;
		/**
		 * Set the core affinity only to the given CPU set ids
		 * \param[in] ids CPU set ids
		 * \return Error
		 */
		auto SetCpuSetAffinity(const DynArray<u32>& ids) noexcept -> SystemError;
		/**
		 * Set the core affinity only to the given logical core
		 * \param[in] core Logical core index
		 * \param[in] processor Processor index
		 * \return Error
		 */
		auto SetLogicalAffinity(u32 core, u32 processor) noexcept -> SystemError;
		/**
		 * Set the core affinity to the given logical cores
		 * \param[in] cores Logical core indices
		 * \param[in] processor Processor index
		 * \return Error
		 */
		auto SetLogicalAffinity(const DynArray<u32>& cores, u32 processor) noexcept -> SystemError;
		/**
		 * Set the core affinity only to the given physical core
		 * \param[in] core Logical core index
		 * \param[in] processor Processor index
		 * \return Error
		 */
		auto SetPhysicalAffinity(u32 core, u32 processor) noexcept -> SystemError;
		/**
		 * Set the core affinity to the given physical cores
		 * \param[in] cores Logical core indices
		 * \param[in] processor Processor index
		 * \return Error
		 */
		auto SetPhysicalAffinity(const DynArray<u32>& cores, u32 processor) noexcept -> SystemError;
		/**
		 * Reset the thread to its default core affinity
		 * \return Error
		 */
		auto ResetAffinity() noexcept -> SystemError;
		/**
		 * Get the thread description
		 * \return Thread description
		 */
		auto GetDescription() const noexcept -> String;
		/**
		 * Get the native handle to the thread
		 * \return Native handle
		 */
		auto GetNativeHandle() const noexcept -> NativeHandle;
		/**
		 * Get the thread id
		 * \return Thread id
		 */
		auto GetThreadId() const noexcept -> ThreadID;
		/**
		 * Get the stack size of the thread
		 * \return Stack size of the thread
		 */
		auto GetStackSize() const noexcept -> usize;
		/**
		 * Get the index of the core the thread is currently running on
		 * \return Core index
		 */
		auto GetCoreIndex() const noexcept -> usize;
		/**
		 * Get the thread priority
		 * \return Thread priority
		 */
		auto GetPriority() const noexcept -> ThreadPriority;
		/**
		 * Get the thread memory priority
		 * \return Thread memory priority
		 */
		auto GetMemoryPriority() const noexcept -> ThreadMemoryPriority;
		/**
		 * Get the thread power throttling state
		 * \return Thread power throttling state
		 */
		auto GetPowerThrottling() const noexcept -> ThreadPowerThrottling;
		/**
		 * Get the CPU set ids of the assigned CPU set ids
		 * \return CPU set ids of the assigned CPU set ids
		 */
		auto GetCpuSetAffinity() const noexcept -> DynArray<u32>;
		/**
		 * Get the CPU set ids of the assigned logical cores
		 * \return CPU set ids of the assigned logical cores
		 */
		auto GetLogicalAffinity() const noexcept -> DynArray<DynArray<u32>>;
		/**
		 * Get the CPU set ids of the assigned physical cores
		 * \return CPU set ids of the assigned physical cores
		 */
		auto GetPhysicalAffinity() const noexcept -> DynArray<DynArray<u32>>;
		/**
		 * Get the exit code of the thread, a null optional will be returned if the thread has not yet exited
		 * \return Optional with value
		 */
		auto GetExitCode() const noexcept -> Optional<u32>;
		/**
		 * Get the process id of the process associated with the thread
		 * \return Process id of the process associated with the thread
		 */
		auto GetProcessId() const noexcept -> u32;
		/**
		 * Check if the thread is valid
		 * \return Whether the thread is valid
		 */
		auto IsValid() const noexcept -> bool;
		/**
		 * Check if the thread is suspended
		 * \return Whether the thread is suspended
		 */
		auto IsSuspended() const noexcept -> bool;
		/**
		 * Check if the thread has any pending I/O
		 * \return Whether the thread has any pending I/O
		 */
		auto HasIOPending() const noexcept -> bool;
		/**
		 * Check if the thread allows priority boost
		 * \return Whether the thread allows priority boost
		 * \note Currently can only return true on windows
		 */
		auto HasPriorityBoost() const noexcept -> bool;

		/**
		 * Get a string with the debug representation of the thread
		 * \return Debug string
		 */
		auto ToDebugString() const noexcept -> String;

		/**
		 * Create a thread
		 * \tparam Args Argument types
		 * \param[in] attribs Attributes
		 * \param[in] delegate Delegate to function to call
		 * \param[in] args Arguments
		 * \return A result with the thread or error
		 */
		template<typename... Args>
		static auto Create(ThreadAttribs attribs, const Delegate<u32(Args...)>& delegate, Args&&... args) noexcept -> Result<Thread, SystemError>;
		/**
		 * Create a a thread based on an existing handle
		 * \return Thread
		 */
		static auto FromNativeHandle(NativeHandle handle) noexcept -> Thread;
		/**
		 * Get the current thread
		 * \return Current thread
		 * \note not all functionality is allowed on the returned thread
		 */
		static auto FromCurrent() noexcept -> Thread;

	private:
		auto Init(void* pInvoke, void* pData) noexcept -> void;

		template<typename... Args>
		static auto Invoke(void* pData) noexcept -> u32;

		ThreadAttribs    m_attribs;  ///< Thread attributes
		NativeHandle     m_handle;   ///< Native handle
		ThreadID         m_threadId; ///< Thread id
		bool             m_current;  ///< Whether the thread represents the current thread
	};


	/**
	 * Get the current thread id
	 * \return Current thread id
	 */
	CORE_API auto GetCurrentThreadId() noexcept -> ThreadID;
}

#include "Thread.inl"
