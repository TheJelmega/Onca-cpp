#pragma once
#include "MemRef.h"
#include "Utils.h"

#ifdef __INTELLISENSE__
#pragma diag_suppress 438 // supress bpgus "expected a ''" due to concepts
#endif

namespace Core
{
	template<typename T, MemRefDeleter<T> D = DefaultDeleter<T>>
	class Unique
	{
	public:
		constexpr Unique() noexcept;
		constexpr Unique(nullptr_t) noexcept;
		explicit Unique(MemRef<T>&& ref) noexcept;
		template<typename U>
		explicit Unique(Unique<U, D>&& unique);

		Unique(Unique<T, D>&&) noexcept = default;
		Unique(const Unique<T, D>&) = delete;

		auto operator=(nullptr_t) noexcept -> Unique<T, D>&;
		template<typename U>
		auto operator=(Unique<U, D>&& unique) noexcept -> Unique<T, D>&;
		
		auto operator=(Unique<T, D>&&) noexcept -> Unique<T, D>& = default;
		auto operator=(const Unique<T, D>&) -> Unique<T, D>& = delete;

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
		auto Reset(MemRef<T>&& ref) noexcept -> void;
		/**
		 * \brief Swap the contents of this Unique with another
		 * \param[in] other Unique to swap contents with
		 */
		auto Swap(Unique<T>& other) noexcept -> void;

		/**
		 * \brief Get a pointer to the managed memory
		 * \return Pointer to managed memory
		 */
		auto Get() noexcept -> T*;
		/**
		 * \brief Get a const reference to the managed MemRef
		 * \return Managed MemRef
		 */
		auto GetRef() noexcept -> const MemRef<T>&;
		/**
		 * \brief Get the deleter for this unique
		 * \return Deleter for this unique
		 */
		auto GetDeleter() noexcept -> D;

		explicit operator bool();

		auto operator->() const noexcept -> const T*;
		auto operator->() noexcept -> T*;
		auto operator*() const noexcept -> const T&;
		auto operator*() noexcept -> T&;

		auto operator<=>(const Unique<T>& other) const noexcept -> std::strong_ordering;

		/**
		 * Create a Unique with a constructed type and the global allocator
		 * \tparam Args Argument types
		 * \param[in] args Arguments to constuct type
		 * \return Unique with the constructed type
		 */
		template<typename ...Args>
		static auto Create(const Args&... args) noexcept -> Unique<T, D>;
		/**
		 * Create a Unique with a constructed type
		 * \tparam Args Argument types
		 * \param[in] alloc Allocator to use
		 * \param[in] args Arguments to constuct type
		 * \return Unique with the constructed type
		 */
		template<typename ...Args>
		static auto CreateWitAlloc(Alloc::IAllocator& alloc, Args&&... args) noexcept -> Unique<T, D>;

	private:
		MemRef<T> m_mem;     ///< Managed memory
		D         m_Deleter; ///< Deleter when memory is discarded
	};
	
}

#include "Unique.inl"
