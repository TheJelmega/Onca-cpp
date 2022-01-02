#pragma once

#include "Thread.h"
#include "core/utils/Meta.h"

namespace Core::Threading
{
	template <typename ... Args>
	auto Thread::Invoke(void* pData) noexcept -> void
	{
		InvokeData<Args...>& data = *static_cast<InvokeData<Args...>*>(pData);
		data.delegate(data.arguments);
	}

	inline auto Thread::GetDescription() const noexcept -> String
	{
		return m_attribs.desc;
	}

	inline auto Thread::GetNativeHandle() const noexcept -> NativeHandle
	{
		return m_handle;
	}

	inline auto Thread::GetThreadId() const noexcept -> ThreadID
	{
		return m_threadId;
	}

	inline auto Thread::GetStackSize() const noexcept -> usize
	{
		return m_attribs.stackSize;
	}

	inline auto Thread::GetPriority() const noexcept -> ThreadPriority
	{
		return m_attribs.priority;
	}

	inline auto Thread::GetMemoryPriority() const noexcept -> ThreadMemoryPriority
	{
		return m_attribs.memPriority;
	}

	inline auto Thread::GetPowerThrottling() const noexcept -> ThreadPowerThrottling
	{
		return m_attribs.powerThrottling;
	}

	inline auto Thread::IsSuspended() const noexcept -> bool
	{
		return m_attribs.suspended;
	}

	inline auto Thread::HasPriorityBoost() const noexcept -> bool
	{
		return m_attribs.priorityBoost;
	}

	template <typename ... Args>
	auto Thread::Create(ThreadAttribs attribs, const Delegate<void(Args...)>& delegate, Args&&... args) noexcept -> Result<Thread, Error>
	{
		Thread thread;
		thread.m_attribs = attribs;
		InvokeData<Args...> data;
		data.delegate = delegate;
		data.arguments = { Forward<Args>(args)... };
		thread.Init(&Thread::Invoke<Args...>, &data);

		if (thread.IsValid())
			return thread;
		return TranslateError();
	}
}
