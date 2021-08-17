#pragma once
#include "Unique.h"
#include "core/allocator/GlobalAlloc.h"

#ifdef __INTELLISENSE__
#pragma diag_suppress 438 // supress bpgus "expected a ''" due to concepts
#endif

namespace Core
{
	template <typename T, MemRefDeleter<T> D>
	constexpr Unique<T, D>::Unique() noexcept
		: m_mem(nullptr)
	{
	}

	template <typename T, MemRefDeleter<T> D>
	constexpr Unique<T, D>::Unique(nullptr_t) noexcept
		: m_mem(nullptr)
	{
	}

	template <typename T, MemRefDeleter<T> D>
	Unique<T, D>::Unique(MemRef<T>&& ref) noexcept
		: m_mem(StdMove(ref))
	{
	}

	template <typename T, MemRefDeleter<T> D>
	template <typename U>
	Unique<T, D>::Unique(Unique<U, D>&& unique)
		: m_mem(StdMove(unique.m_mem))
	{
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::operator=(nullptr_t) noexcept -> Unique<T, D>&
	{
		m_Deleter(m_mem);
		return *this;
	}

	template <typename T, MemRefDeleter<T> D>
	template <typename U>
	auto Unique<T, D>::operator=(Unique<U, D>&& unique) noexcept -> Unique<T, D>&
	{
		m_Deleter(m_mem);
		m_mem = StdMove(unique.m_mem);
		return *this;
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::Release() noexcept -> MemRef<T>
	{
		MemRef<T> tmp = StdMove(m_mem);
		return tmp;
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::Reset(MemRef<T>&& ref) noexcept -> void
	{
		m_Deleter(m_mem);
		m_mem = std::move(ref);
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::Swap(Unique<T>& other) noexcept -> void
	{
		MemRef<T> tmp = std::move(other);
		other = std::move(*this);
		*this = std::move(tmp);
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::Get() noexcept -> T*
	{
		return m_mem.Ptr();
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::GetRef() noexcept -> const MemRef<T>&
	{
		return m_mem;
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::GetDeleter() noexcept -> D
	{
		return m_Deleter;
	}

	template <typename T, MemRefDeleter<T> D>
	Unique<T, D>::operator bool()
	{
		return m_mem.IsValid();
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::operator->() const noexcept -> const T*
	{
		return m_mem.Ptr();
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::operator->() noexcept -> T*
	{
		return m_mem.Ptr();
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::operator*() const noexcept -> const T&
	{
		return *m_mem.Ptr();
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::operator*() noexcept -> T&
	{
		return *m_mem.Ptr();
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::operator<=>(const Unique<T>& other) const noexcept -> std::strong_ordering
	{
		T* pThis = m_mem.Ptr();
		T* pOther = other.m_mem.Ptr();
		if (pThis == pOther) return std::strong_ordering::equal;
		if (pThis <  pOther) return std::strong_ordering::less;
		return std::strong_ordering::greater;
	}

	template <typename T, MemRefDeleter<T> D>
	template <typename ... Args>
	auto Unique<T, D>::Create(const Args&... args) noexcept -> Unique<T, D>
	{
		return CreateWitAlloc(g_GlobalAlloc, args...);
	}

	template <typename T, MemRefDeleter<T> D>
	template <typename ... Args>
	auto Unique<T, D>::CreateWitAlloc(Alloc::IAllocator& alloc, Args&&... args) noexcept -> Unique<T, D>
	{
		Unique<T, D> unique{ alloc.Allocate<T>() };
		new (unique.m_mem.Ptr()) T{ StdForward<Args>(args)... };
		return unique;
	}
}
