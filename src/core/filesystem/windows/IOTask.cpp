#include "../IOTask.h"
#if PLATFORM_WINDOWS

#include "core/platform/Platform.h"

namespace Core::FileSystem
{
	IOReadTask::~IOReadTask()
	{
		if (!m_data)
			return;

		if (m_data->waitHandle != INVALID_HANDLE_VALUE)
			::CloseHandle(m_data->waitHandle);
	}

	IOReadTask::IOReadTask(IOReadTask&& other) noexcept
		: m_data(Move(other.m_data))
	{
	}

	auto IOReadTask::operator=(IOReadTask&& other) noexcept -> IOReadTask&
	{
		m_data = Move(m_data);
		return *this;
	}

	auto IOReadTask::Await() noexcept -> SystemError
	{
		ASSERT(IsValid(), "Cannot call Await() on an invalid IOReadTask");
		u32 res = WAIT_IO_COMPLETION;
		while (res == WAIT_IO_COMPLETION)
			res = ::WaitForSingleObjectEx(m_data->waitHandle, INFINITE, true);
		if (res == WAIT_OBJECT_0)
			return SystemErrorCode::Success;
		if (res == WAIT_ABANDONED)
			return SystemErrorCode::AsyncAbandoned;
		return TranslateSystemError();
	}

	auto IOReadTask::IsCompleted() const noexcept -> bool
	{
		ASSERT(IsValid(), "Cannot call IsComplete() on an invalid IOReadTask");
		const u32 res = ::WaitForSingleObject(m_data->waitHandle, 0);
		return res == WAIT_OBJECT_0;
	}

	auto IOReadTask::GetResult() noexcept -> Result<ByteBuffer, SystemError>
	{
		ASSERT(IsValid(), "Cannot call GetResult() on an invalid IOReadTask");
		ASSERT(IsCompleted(), "Cannot get the result of a task when it hasn't completed");
		ASSERT(m_data->validData, "Cannot get the result of a task when it hasn't completed");
		if (m_data->error.code == SystemErrorCode::Success)
			return Move(m_data->buffer);
		return Move(m_data->error);
	}

	IOReadTask::IOReadTask(NativeHandle fileHandle, AsyncReadCallback& callback, usize bufferSize)
		: m_data(Unique<Data>::Create())
	{
		m_data->fileHandle = fileHandle;
		m_data->waitHandle = ::CreateEventW(nullptr, true, false, nullptr);
		m_data->buffer.Resize(bufferSize);
		m_data->callback = callback;
		m_data->error.code = SystemErrorCode::AsyncIncomplete;
	}
	
	IOWriteTask::~IOWriteTask()
	{
		if (!m_data)
			return;

		if (m_data->waitHandle != INVALID_HANDLE_VALUE)
			::CloseHandle(m_data->waitHandle);
	}

	IOWriteTask::IOWriteTask(IOWriteTask&& other) noexcept
		: m_data(Move(other.m_data))
	{
	}

	auto IOWriteTask::operator=(IOWriteTask&& other) noexcept -> IOWriteTask&
	{
		m_data = Move(m_data);
		return *this;
	}

	auto IOWriteTask::Await() noexcept -> SystemError
	{
		ASSERT(IsValid(), "Cannot call Await() on an invalid IOWriteTask");
		u32 res = WAIT_IO_COMPLETION;
		while (res == WAIT_IO_COMPLETION)
			res = ::WaitForSingleObjectEx(m_data->waitHandle, INFINITE, true);
		if (res == WAIT_OBJECT_0)
			return SystemErrorCode::Success;
		if (res == WAIT_ABANDONED)
			return SystemErrorCode::AsyncAbandoned;
		return TranslateSystemError();
	}

	auto IOWriteTask::IsCompleted() const noexcept -> bool
	{
		ASSERT(IsValid(), "Cannot call IsComplete() on an invalid IOWriteTask");
		const u32 res = ::WaitForSingleObject(m_data->waitHandle, 0);
		return res == WAIT_OBJECT_0;
	}

	auto IOWriteTask::GetResult() noexcept -> SystemError
	{
		ASSERT(IsValid(), "Cannot call GetResult() on an invalid IOWriteTask");
		ASSERT(IsCompleted(), "Cannot get the result of a task when it hasn't completed");
		return m_data->error;
	}

	IOWriteTask::IOWriteTask(NativeHandle fileHandle, AsyncWriteCallback& callback, const ByteBuffer& buffer)
		: m_data(Unique<Data>::Create())
	{
		m_data->fileHandle = fileHandle;
		m_data->waitHandle = ::CreateEventW(nullptr, true, false, nullptr);
		m_data->buffer = buffer;
		m_data->callback = callback;
		m_data->error.code = SystemErrorCode::AsyncIncomplete;
	}
}

#endif