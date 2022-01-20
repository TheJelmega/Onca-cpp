#pragma once
#if __RESHARPER__
#include "Unique.h"
#endif

#include "core/allocator/GlobalAlloc.h"

#ifdef __INTELLISENSE__
#pragma diag_suppress 438 // supress bogus "expected a ''" due to concepts
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
		: m_mem(Move(ref))
	{
	}

	template <typename T, MemRefDeleter<T> D>
	template <typename U, MemRefDeleter<U> D2>
	Unique<T, D>::Unique(Unique<U, D2>&& unique)
		: m_mem(unique.m_mem.As<T>())
		, m_deleter(Move(unique.m_deleter))
	{
		unique.m_mem = MemRef<U>{};
	}

	template <typename T, MemRefDeleter<T> D>
	Unique<T, D>::~Unique() noexcept
	{
		m_deleter(Move(m_mem));
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::operator=(nullptr_t) noexcept -> Unique<T, D>&
	{
		m_deleter(Move(m_mem));
		return *this;
	}

	template <typename T, MemRefDeleter<T> D>
	template <typename U, MemRefDeleter<U> D2>
	auto Unique<T, D>::operator=(Unique<U, D2>&& unique) noexcept -> Unique<T, D>&
	{
		m_deleter(Move(m_mem));
		m_mem = unique.m_mem.template As<T>();
		m_deleter = Move(unique.m_deleter);
		unique.m_mem = MemRef<U>{};
		return *this;
	}

	template <typename T, MemRefDeleter<T> D>
	Unique<T, D>::operator bool() const noexcept
	{
		return m_mem.IsValid();
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::Release() noexcept -> MemRef<T>
	{
		MemRef<T> tmp = Move(m_mem);
		return tmp;
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::Reset(MemRef<T>&& ref) noexcept -> void
	{
		m_deleter(Move(m_mem));
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
	auto Unique<T, D>::Get() const noexcept -> const T*
	{
		return m_mem.Ptr();
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::GetMemRef() const noexcept -> const MemRef<T>&
	{
		return m_mem;
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::GetDeleter() const noexcept -> D
	{
		return m_deleter;
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
		return m_mem.Ptr();
	}

	template <typename T, MemRefDeleter<T> D>
	auto Unique<T, D>::operator==(const Unique<T>& other) const noexcept -> bool
	{
		return m_mem == other.m_mem;
	}
	
	template <typename T, MemRefDeleter<T> D>
	template <typename ... Args>
	auto Unique<T, D>::Create(Args&&... args) noexcept -> Unique<T, D>
	{
		return CreateWitAlloc(g_GlobalAlloc, Forward<Args>(args)...);
	}

	template <typename T, MemRefDeleter<T> D>
	template <typename ... Args>
	auto Unique<T, D>::CreateWitAlloc(Alloc::IAllocator& alloc, Args&&... args) noexcept -> Unique<T, D>
	{
		Unique<T, D> unique{ alloc.Allocate<T>() };
		new (unique.m_mem.Ptr()) T{ Forward<Args>(args)... };
		return unique;
	}
}
