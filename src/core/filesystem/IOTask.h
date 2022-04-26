#pragma once
#include "core/utils/Result.h"
#include "core/containers/ByteBuffer.h"
#include "core/memory/Unique.h"
#include "core/platform/SystemError.h"
#include "core/utils/Delegate.h"

namespace Onca::FileSystem
{
	// TODO: when implementing memory defragmentations, make sure the data in the tasks are pinned to a fixed location

	using AsyncReadCallback = Delegate<void(const ByteBuffer&, const SystemError&)>;
	using AsyncWriteCallback = Delegate<void(const SystemError&)>;

	class CORE_API IOReadTask
	{
	public:

		DEFINE_OPAQUE_HANDLE(NativeHandle);
		DEFINE_SIZED_OPAQUE_HANDLE(NativeDataHandle, 32);

		struct Data
		{
			Data() = default;

			NativeHandle      fileHandle;    ///< File handle
			NativeHandle      waitHandle;    ///< Handle to wait for the task
			ByteBuffer        buffer;        ///< Byte buffer
			SystemError       error;         ///< Error
			NativeDataHandle  nData;         ///< Native data
			AsyncReadCallback callback;      ///< Callback
			
			bool              validData : 1; ///< Whether the data in the I/O read task is valid
		};

		DISABLE_COPY(IOReadTask);

		/**
		 * Create an invalid IO read task
		 */
		IOReadTask() = default;
		~IOReadTask();

		IOReadTask(IOReadTask&& other) noexcept;
		
		auto operator=(IOReadTask&& other) noexcept -> IOReadTask&;

		/**
		 * Await the completion of the async I/O read task
		 * \return Error
		 */
		NO_DISCARD("Error should be checked and handled")
		auto Await() noexcept -> SystemError;
		/**
		 * Check if the task is valid
		 * \return Whether the task is valid
		 */
		auto IsValid() const noexcept -> bool { return !!m_data; }
		/**
		 * Check if the task is completed
		 * \return Whether the task is completed
		 */
		auto IsCompleted() const noexcept -> bool;
		/**
		 * Get the result of the I/O task
		 * \return Result
		 * \note Getting the result will move the value out of the task, therefore this function can only be called once per task
		 */
		NO_DISCARD("Cannot discard the result, as it is moved out of the task!")
		auto GetResult() noexcept -> Result<ByteBuffer, SystemError>;

	private:
		/**
		 * Create a valid IOReadTask
		 */
		explicit IOReadTask(NativeHandle fileHandle, AsyncReadCallback& callback, usize bufferSize);

		friend class File;

		Unique<Data> m_data;
	};

	class CORE_API IOWriteTask
	{
	public:

		DEFINE_OPAQUE_HANDLE(NativeHandle);
		DEFINE_SIZED_OPAQUE_HANDLE(NativeDataHandle, 32);

		struct Data
		{
			Data() = default;

			NativeHandle      fileHandle;    ///< File handle
			NativeHandle      waitHandle;    ///< Handle to wait for the task
			ByteBuffer        buffer;        ///< Byte buffer
			SystemError       error;         ///< Error
			NativeDataHandle  nData;         ///< Native data
			AsyncWriteCallback callback;     ///< Callback
		};

		DISABLE_COPY(IOWriteTask);

		/**
		 * Create an invalid IO read task
		 */
		IOWriteTask() = default;
		~IOWriteTask();
		
		IOWriteTask(IOWriteTask&& other) noexcept;
		
		auto operator=(IOWriteTask&& other) noexcept -> IOWriteTask&;

		/**
		 * Await the completion of the async I/O read task
		 * \return Error
		 */
		NO_DISCARD("Error should be checked and handled")
		auto Await() noexcept -> SystemError;
		/**
		 * Check if the task is valid
		 * \return Whether the task is valid
		 */
		auto IsValid() const noexcept -> bool { return !!m_data; }
		/**
		 * Check if the task is completed
		 * \return Whether the task is completed
		 */
		auto IsCompleted() const noexcept -> bool;
		/**
		 * Get the result of the I/O task
		 * \return Error
		 */
		auto GetResult() noexcept -> SystemError;

	private:
		/**
		 * Create a valid IOWriteTask
		 */
		explicit IOWriteTask(NativeHandle fileHandle, AsyncWriteCallback& callback, const ByteBuffer& buffer);

		friend class File;

		Unique<Data> m_data;
	};

}
