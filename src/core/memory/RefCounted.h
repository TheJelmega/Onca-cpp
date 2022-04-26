#pragma once
#include "MemRef.h"
#include "core/utils/Atomic.h"
#include "core/utils/Delegate.h"
#include "core/utils/Utils.h"

namespace Onca
{
	namespace Detail
	{
		struct RefCountedControlBlock
		{
			u32 strongCount;
			u32 weakCount;
			u32 controlCount;

			/**
			 * Increment the strong ref count and check if the data needs to be deleted
			 * \return Whether the data should be deleted
			 */
			void IncStrongRef() noexcept;
			/**
			 * Increment the weal ref count and check if the data needs to be deleted
			 */
			void IncWeakRef() noexcept;
			/**
			 * Acquire a reference to the control block
			 * \return 
			 */
			void Acquire() noexcept;

			/**
			 * Decrement the strong ref count and check if the data needs to be deleted
			 * \return Whether the data should be deleted
			 */
			auto DecStrongRef() noexcept -> bool;
			/**
			 * Decrement the weal ref count and check if the data needs to be deleted
			 */
			void DecWeakRef() noexcept;
			/**
			 * Release a reference to the control block and check if the control block need to be deleted
			 * \return Whether the control block should be deleted
			 */
			auto Release() noexcept -> bool;

			/**
			 * Check if the data is valid (mostly used for weak pointers)
			 * \return Whether the data is still valid
			 */
			auto IsValid() const noexcept -> bool;

			/**
			 * Set the strong to 0, allowing the data to be invalidated
			 */
			void ReleaseData() noexcept;
		};

		struct AtomicRefCountedControlBlock
		{
			Atomic<u32> strongCount;
			Atomic<u32> weakCount;
			Atomic<u32> controlCount;

			/**
			 * Increment the strong ref count and check if the data needs to be deleted
			 * \return Whether the data should be deleted
			 */
			void IncStrongRef() noexcept;
			/**
			 * Increment the weal ref count and check if the data needs to be deleted
			 */
			void IncWeakRef() noexcept;
			/**
			 * Acquire a reference to the control block
			 * \return
			 */
			void Acquire() noexcept;

			/**
			 * Decrement the strong ref count and check if the data needs to be deleted
			 * \return Whether the data should be deleted
			 */
			auto DecStrongRef() noexcept -> bool;
			/**
			 * Decrement the weal ref count and check if the data needs to be deleted
			 */
			void DecWeakRef() noexcept;
			/**
			 * Release a reference to the control block and check if the control block need to be deleted
			 * \return Whether the control block should be deleted
			 */
			auto Release() noexcept -> bool;

			/**
			 * Check if the data is valid (mostly used for weak pointers)
			 * \return Whether the data is still valid
			 */
			auto IsValid() const noexcept -> bool;

			/**
			 * Set the strong to 0, allowing the data to be invalidated
			 */
			void ReleaseData() noexcept;
		};

		template<typename T>
		void DefaultRefCountedDeleter(MemRef<T>&& memref) noexcept;

		template<typename T, typename ControlBlock>
		class RefCounted
		{
		public:
			/**
			 * Create a null ref counted pointer
			 */
			RefCounted() noexcept;
			/**
			 * Create a null ref counted pointer
			 */
			RefCounted(nullptr_t) noexcept;
			/**
			 * Create a ref counted pointer from a MemRef
			 * \param[in] memref MemRef
			 */
			RefCounted(MemRef<T>&& memref) noexcept;
			/**
			 * Create a ref counted pointer from a MemRef and a custom deleter
			 * \param[in] memref MemRef
			 * \param[in] deleter Deleter
			 */
			RefCounted(MemRef<T>&& memref, Delegate<void(MemRef<T>&)> deleter) noexcept;

			template<typename U>
			explicit RefCounted(RefCounted<U, ControlBlock>&& rc) noexcept;
			RefCounted(const RefCounted& rc) noexcept;
			RefCounted(RefCounted&& rc) noexcept;

			~RefCounted() noexcept;

			auto operator=(const RefCounted& rc) noexcept -> RefCounted&;
			auto operator=(RefCounted&& rc) noexcept -> RefCounted&;

			/**
			 * \brief Release control of the underlying MemRef and return it
			 * \return Underlying MemRef
			 */
			NO_DISCARD("Can cause memory leaks, since handle to memory will be lost")
			auto Release() noexcept -> MemRef<T>;
			/**
			 * \brief Reset the internal memory of the Unique<T>
			 * \param[in] ref Memory reference to reset the Unique<T> with
			 */
			void Reset(MemRef<T>&& ref) noexcept;
			/**
			 * \brief Swap the contents of this Unique with another
			 * \param[in] other Unique to swap contents with
			 */
			void Swap(RefCounted& other) noexcept;

			/**
			 * \brief Get a pointer to the managed memory
			 * \return Pointer to managed memory
			 */
			auto Get() const noexcept -> const T*;
			/**
			 * \brief Get a pointer to the managed memory
			 * \return Pointer to managed memory
			 */
			auto Get() noexcept -> T*;
			/**
			 * \brief Get a const reference to the managed MemRef
			 * \return Managed MemRef
			 */
			auto GetMemRef() const noexcept -> const MemRef<T>&;
			/**
			 * \brief Get the deleter for this unique
			 * \return Deleter for this unique
			 */
			auto GetDeleter() const noexcept -> Delegate<void(MemRef<T>&&)>;
			/**
			 * Get the number of ref counted pointer that reference the data
			 * \return Use count
			 */
			auto UseCount() const noexcept -> u32;

			explicit operator bool() const noexcept;

			auto operator->() const noexcept -> const T*;
			auto operator->() noexcept -> T*;
			auto operator*() const noexcept -> const T&;
			auto operator*() noexcept -> T&;
			
			auto operator==(const RefCounted& other) const noexcept -> bool;

			/**
			 * Create a ref counted pointer with a constructed type and the global allocator
			 * \tparam Args Argument types
			 * \param[in] args Arguments to constuct type
			 * \return Ref counted pointer with the constructed type
			 */
			template<typename ...Args>
			static auto Create(Args&&... args) noexcept -> RefCounted;
			/**
			 * Create a ref counted pointer with a constructed type
			 * \tparam Args Argument types
			 * \param[in] alloc Allocator to use
			 * \param[in] args Arguments to constuct type
			 * \return Ref counted pointer with the constructed type
			 */
			template<typename ...Args>
			static auto CreateWitAlloc(Alloc::IAllocator& alloc, Args&&... args) noexcept -> RefCounted;
		private:
			template<typename U, typename C>
			friend class Weak;

			/**
			 * Acquire the control block and increment the reference count of the object
			 */
			void AcquireAndIncRef() noexcept;
			/**
			 * Decrement the reference count of the object and release the control block
			 */
			void DecRefAndRelease() noexcept;

			MemRef<T>                   m_data;    ///< Data
			MemRef<ControlBlock>        m_control; ///< Control block
			Delegate<void(MemRef<T>&&)> m_deleter;  ///< Deleter
		};

		template<typename T, typename ControlBlock>
		class Weak
		{
		public:
			/**
			 * Create a null ref counted pointer
			 */
			Weak() noexcept;
			/**
			 * Create a null ref counted pointer
			 */
			Weak(nullptr_t) noexcept;
			/**
			 * Create a ref counted pointer from a MemRef
			 * \param[in] rc Ref counted pointer
			 */
			Weak(const RefCounted<T, ControlBlock>& rc) noexcept;

			template<typename U>
			explicit Weak(Weak<U, ControlBlock>&& weak) noexcept;
			Weak(const Weak& weak) noexcept;
			Weak(Weak&& weak) noexcept;

			~Weak() noexcept;

			auto operator=(const Weak& weak) noexcept -> Weak&;
			auto operator=(Weak&& weak) noexcept -> Weak&;
			
			/**
			 * \brief Swap the contents of this Unique with another
			 * \param[in] other Unique to swap contents with
			 */
			void Swap(Weak& other) noexcept;

			/**
			 * \brief Get a pointer to the managed memory
			 * \return Pointer to managed memory
			 */
			auto Get() const noexcept -> const T*;
			/**
			 * \brief Get a pointer to the managed memory
			 * \return Pointer to managed memory
			 */
			auto Get() noexcept -> T*;
			/**
			 * \brief Get a const reference to the managed MemRef
			 * \return Managed MemRef
			 */
			auto GetMemRef() const noexcept -> const MemRef<T>&;
			/**
			 * Get the number of ref counted pointer that reference the data
			 * \return Use count
			 */
			auto UseCount() const noexcept -> u32;
			/**
			 * Check if the weak pointer is valid
			 * \return Whether the weak pointer is valid
			 */
			auto IsValid() const noexcept -> bool;

			explicit operator bool() const noexcept;

			auto operator->() const noexcept -> const T*;
			auto operator->() noexcept -> T*;
			auto operator*() const noexcept -> const T&;
			auto operator*() noexcept -> T&;

			auto operator==(const Weak& other) const noexcept -> bool;
		private:
			template<typename U, typename C>
			friend class RefCounted;

			/**
			 * Acquire the control block and increment the reference count of the object
			 */
			void AcquireAndIncRef() noexcept;
			/**
			 * Decrement the reference count of the object and release the control block
			 */
			void DecRefAndRelease() noexcept;

			MemRef<T>            m_data;    ///< Data
			MemRef<ControlBlock> m_control; ///< Control block
		};
	}

	template<typename T>
	using Rc = Detail::RefCounted<T, Detail::RefCountedControlBlock>;
	template<typename T>
	using Weak = Detail::Weak<T, Detail::RefCountedControlBlock>;

	template<typename T>
	using Arc = Detail::RefCounted<T, Detail::AtomicRefCountedControlBlock>;
	template<typename T>
	using AWeak = Detail::Weak<T, Detail::AtomicRefCountedControlBlock>;

	
}

#include "RefCounted.inl"