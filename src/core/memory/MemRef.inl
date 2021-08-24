#pragma once
#include "MemRef.h"
#include "MemUtils.h"
#include "core/allocator/IAllocator.h"

namespace Core
{
	template <typename T>
	MemRef<T>::MemRef() noexcept
		: m_handle(~usize(0))
		, m_pAlloc(nullptr)
		, m_log2Align(0)
		, m_isBackingMem(false)
		, m_size(0)
	{
	}

	template <typename T>
	MemRef<T>::MemRef(nullptr_t) noexcept
		: m_handle(~usize(0))
		, m_pAlloc(nullptr)
		, m_log2Align(0)
		, m_isBackingMem(false)
		, m_size(0)
	{
	}

	template <typename T>
	MemRef<T>::MemRef(Alloc::IAllocator* pAlloc) noexcept
		: m_handle(~usize(0))
		, m_pAlloc(pAlloc)
		, m_log2Align(0)
		, m_isBackingMem(false)
		, m_size(0)
	{
	}

	template <typename T>
	MemRef<T>::MemRef(usize handle, Alloc::IAllocator* pAlloc, u8 log2Align, usize size, bool isBacking) noexcept
		: m_handle(handle)
		, m_pAlloc(pAlloc)
		, m_log2Align(log2Align)
		, m_isBackingMem(isBacking)
		, m_size(size)
	{
	}

	template <typename T>
	MemRef<T>::MemRef(const MemRef<T>& other) noexcept
		: m_handle(other.m_handle)
		, m_pAlloc(other.m_pAlloc)
		, m_log2Align(other.m_log2Align)
		, m_isBackingMem(other.m_isBackingMem)
		, m_size(other.m_size)
	{
	}

	template <typename T>
	MemRef<T>::MemRef(MemRef<T>&& other) noexcept
		: m_handle(other.m_handle)
		, m_pAlloc(other.m_pAlloc)
		, m_log2Align(other.m_log2Align)
		, m_isBackingMem(other.m_isBackingMem)
		, m_size(other.m_size)
	{
		MemClearData(other);
	}

	template <typename T>
	auto MemRef<T>::operator=(nullptr_t) noexcept -> MemRef<T>&
	{
		MemClearData(*this);
		m_handle = ~usize(0);
		return *this;
	}

	template <typename T>
	auto MemRef<T>::operator=(const MemRef<T>& other) noexcept -> MemRef<T>&
	{
		MemCpy(*this, other);
		return *this;
	}
	
	template <typename T>
	auto MemRef<T>::operator=(MemRef<T>&& other) noexcept -> MemRef<T>&
	{
		MemCpy(*this, other);
		other.m_handle = ~usize(0);
		return *this;
	}

	template <typename T>
	auto MemRef<T>::Ptr() const noexcept -> T*
	{
		if (m_pAlloc) LIKELY
			return m_pAlloc->TranslateToPtr(*this);
		return nullptr;
	}

	template <typename T>
	auto MemRef<T>::GetAlloc() const noexcept -> Alloc::IAllocator*
	{
		return m_pAlloc;
	}

	template <typename T>
	auto MemRef<T>::Align() const noexcept -> u16
	{
		return 1 << m_log2Align;
	}

	template <typename T>
	auto MemRef<T>::Size() const noexcept -> usize
	{
		return m_size;
	}

	template <typename T>
	auto MemRef<T>::GetRawHandle() const noexcept -> usize
	{
		return m_handle;
	}

	template <typename T>
	auto MemRef<T>::IsBackingMem() const noexcept -> bool
	{
		return m_isBackingMem;
	}

	template <typename T>
	auto MemRef<T>::IsValid() const noexcept -> bool
	{
		return m_handle != ~usize(0) && m_pAlloc && m_size != 0;
	}

	template <typename T>
	auto MemRef<T>::Dealloc() noexcept -> void
	{
		if (IsValid())
			m_pAlloc->Deallocate(Move(*this));
	}

	template <typename T>
	template <typename U>
	auto MemRef<T>::As() noexcept -> MemRef<U>
	{
		return { m_handle, m_pAlloc, m_log2Align, m_size, m_isBackingMem };
	}

	template <typename T>
	template <typename U>
	auto MemRef<T>::As() const noexcept -> const MemRef<U>&
	{
		return *reinterpret_cast<const MemRef<U>*>(this);
	}

	template <typename T>
	auto MemRef<T>::operator->() const noexcept -> T*
	{
		return Ptr();
	}
	template <typename T>
	auto MemRef<T>::operator*() const noexcept -> T&
	{
		return *Ptr();
	}
	
	template <typename T>
	MemRef<T>::operator bool() const noexcept
	{
		return IsValid();
	}

	template <typename T>
	template <typename U>
	auto MemRef<T>::operator==(const MemRef<U>& other) const noexcept -> bool
	{
		return m_pAlloc == other.m_pAlloc &&
			   m_handle == other.m_handle ||
			   (!IsValid() && !other.IsValid());
	}

	template <typename T>
	template <typename U>
	auto MemRef<T>::operator!=(const MemRef<U>& other) const noexcept -> bool
	{
		return !(*this == other);
	}
}
