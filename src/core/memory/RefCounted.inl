#pragma once
#if __RESHARPER
#include "RefCounted.h"
#endif
#include "core/utils/Algo.h"
#include "core/allocator/GlobalAlloc.h"

namespace Core
{
	namespace Detail
	{
		inline auto RefCountedControlBlock::IncStrongRef() noexcept -> void
		{
			++strongCount;
		}

		inline auto RefCountedControlBlock::IncWeakRef() noexcept -> void
		{
			++strongCount;
		}

		inline auto RefCountedControlBlock::Acquire() noexcept -> void
		{
			++controlCount;
		}

		inline auto RefCountedControlBlock::DecStrongRef() noexcept -> bool
		{
			if (strongCount > 0)
			{
				--strongCount;
				return strongCount == 0;
			}
			return false;
		}

		inline auto RefCountedControlBlock::DecWeakRef() noexcept -> void
		{
			if (weakCount > 0)
				--weakCount;
		}

		inline auto RefCountedControlBlock::Release() noexcept -> bool
		{
			--controlCount;
			return controlCount == 0;
		}

		inline auto RefCountedControlBlock::IsValid() const noexcept -> bool
		{
			return strongCount > 0;
		}

		inline auto RefCountedControlBlock::ReleaseData() noexcept -> void
		{
			strongCount = 0;
		}

		inline auto AtomicRefCountedControlBlock::IncStrongRef() noexcept -> void
		{
			++strongCount;
		}

		inline auto AtomicRefCountedControlBlock::IncWeakRef() noexcept -> void
		{
			++weakCount;
		}

		inline auto AtomicRefCountedControlBlock::Acquire() noexcept -> void
		{
			++controlCount;
		}

		inline auto AtomicRefCountedControlBlock::DecStrongRef() noexcept -> bool
		{
			u32 count = strongCount;
			if (count == 0)
				return false;
			do
			{
				--count;
			}
			while (strongCount.CompareExchangeWeak(count, count) && count > 0);
			return count == 0;
		}

		inline auto AtomicRefCountedControlBlock::DecWeakRef() noexcept -> void
		{
			u32 count = weakCount;
			if (count == 0)
				return;
			do
			{
				--count;
			}
			while (weakCount.CompareExchangeWeak(count, count) && count > 0);
		}

		inline auto AtomicRefCountedControlBlock::Release() noexcept -> bool
		{
			return --controlCount == 0;
		}

		inline auto AtomicRefCountedControlBlock::IsValid() const noexcept -> bool
		{
			return strongCount > 0;
		}

		inline auto AtomicRefCountedControlBlock::ReleaseData() noexcept -> void
		{
			u32 count;
			while (strongCount.CompareExchangeWeak(count, 0) && count > 0)
				; // Intentionally left empty
		}

		template<typename T>
		auto DefaultRefCountedDeleter(MemRef<T>&& memref) noexcept -> void
		{
			static DefaultDeleter<T> deleter;
			deleter(Move(memref));
		}

		template <typename T, typename ControlBlock>
		RefCounted<T, ControlBlock>::RefCounted() noexcept
		{
		}

		template <typename T, typename ControlBlock>
		RefCounted<T, ControlBlock>::RefCounted(nullptr_t) noexcept
		{
		}

		template <typename T, typename ControlBlock>
		RefCounted<T, ControlBlock>::RefCounted(MemRef<T>&& memref) noexcept
			: m_data(Move(memref))
			, m_deleter(&DefaultRefCountedDeleter)
		{
			m_control = m_data.GetAlloc()->template Allocate<ControlBlock>();
			new (m_control.Ptr()) ControlBlock{};
			AcquireAndIncRef();
		}

		template <typename T, typename ControlBlock>
		RefCounted<T, ControlBlock>::RefCounted(MemRef<T>&& memref, Delegate<void(MemRef<T>&)> deleter) noexcept
			: m_data(Move(memref))
			, m_deleter(deleter)
		{
			m_control = m_data.GetAlloc()->template Allocate<ControlBlock>();
			new (m_control.Ptr()) ControlBlock{};
			AcquireAndIncRef();
		}

		template <typename T, typename ControlBlock>
		template <typename U>
		RefCounted<T, ControlBlock>::RefCounted(RefCounted<U, ControlBlock>&& rc) noexcept
		{
			DecRefAndRelease();
			m_data = Move(rc.m_data);
			m_control = Move(rc.m_control);
			m_deleter = Move(rc.m_deleter);
		}

		template <typename T, typename ControlBlock>
		RefCounted<T, ControlBlock>::RefCounted(const RefCounted& rc) noexcept
		{
			DecRefAndRelease();
			m_data = rc.m_data;
			m_control = rc.m_control;
			m_deleter = rc.m_deleter;
			AcquireAndIncRef();
		}

		template <typename T, typename ControlBlock>
		RefCounted<T, ControlBlock>::RefCounted(RefCounted&& rc) noexcept
		{
			DecRefAndRelease();
			m_data = Move(rc.m_data);
			m_control = Move(rc.m_control);
			m_deleter = Move(rc.m_deleter);
		}

		template <typename T, typename ControlBlock>
		RefCounted<T, ControlBlock>::~RefCounted() noexcept
		{
			DecRefAndRelease();
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::operator=(const RefCounted& rc) noexcept -> RefCounted&
		{
			DecRefAndRelease();
			m_data = rc.m_data;
			m_control = rc.m_control;
			m_deleter = rc.m_deleter;
			AcquireAndIncRef();
			return *this;
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::operator=(RefCounted&& rc) noexcept -> RefCounted&
		{
			DecRefAndRelease();
			m_data = Move(rc.m_data);
			m_control = Move(rc.m_control);
			m_deleter = Move(rc.m_deleter);
			return *this;
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::Release() noexcept -> MemRef<T>
		{
			m_control->ReleaseData();
			return Move(m_data);
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::Reset(MemRef<T>&& ref) noexcept -> void
		{
			DecRefAndRelease();
			m_data = Move(ref);
			m_control = m_data.GetAlloc()->template Allocate<ControlBlock>();
			AcquireAndIncRef();
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::Swap(RefCounted& other) noexcept -> void
		{
			Algo::Swap(m_data, other.m_data);
			Algo::Swap(m_control, other.m_control);
			Algo::Swap(m_deleter, other.m_deleter);
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::Get() const noexcept -> const T*
		{
			return *m_data.Ptr();
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::Get() noexcept -> T*
		{
			return *m_data.Ptr();
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::GetMemRef() const noexcept -> const MemRef<T>&
		{
			return m_data;
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::GetDeleter() const noexcept -> Delegate<void(MemRef<T>&&)>
		{
			return m_deleter;
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::UseCount() const noexcept -> u32
		{
			return m_control->strongCount;
		}

		template <typename T, typename ControlBlock>
		RefCounted<T, ControlBlock>::operator bool() const noexcept
		{
			return m_control && m_control->strongCount;
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::operator->() const noexcept -> const T*
		{
			return m_data.Ptr();
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::operator->() noexcept -> T*
		{
			return m_data.Ptr();
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::operator*() const noexcept -> const T&
		{
			return *m_data.Ptr();
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::operator*() noexcept -> T&
		{
			return *m_data.Ptr();
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::operator==(const RefCounted& other) const noexcept -> bool
		{
			return m_data == other.m_data;
		}

		template <typename T, typename ControlBlock>
		template <typename ... Args>
		auto RefCounted<T, ControlBlock>::Create(Args&&... args) noexcept -> RefCounted
		{
			return CreateWitAlloc(g_GlobalAlloc, Forward<Args>(args)...);
		}

		template <typename T, typename ControlBlock>
		template <typename ... Args>
		auto RefCounted<T, ControlBlock>::CreateWitAlloc(Alloc::IAllocator& alloc, Args&&... args) noexcept -> RefCounted
		{
			MemRef<T> memRef = alloc.Allocate<T>();
			new (memRef.Ptr()) T{ Forward<Args>(args)... };
			return RefCounted{ Move(memRef) };
		}

		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::AcquireAndIncRef() noexcept -> void
		{
			if (!m_control)
				return;

			m_control->Acquire();
			m_control->IncStrongRef();
		}
		
		template <typename T, typename ControlBlock>
		auto RefCounted<T, ControlBlock>::DecRefAndRelease() noexcept -> void
		{
			if (!m_control)
				return;

			if (m_control->DecStrongRef())
				m_deleter(Move(m_data));

			if (m_control->Release())
				m_control.Dealloc();
		}

		template <typename T, typename ControlBlock>
		Weak<T, ControlBlock>::Weak() noexcept
		{
		}

		template <typename T, typename ControlBlock>
		Weak<T, ControlBlock>::Weak(nullptr_t) noexcept
		{
		}

		template <typename T, typename ControlBlock>
		Weak<T, ControlBlock>::Weak(const RefCounted<T, ControlBlock>& rc) noexcept
			: m_data(rc.m_data)
			, m_control(rc.m_control)
		{
			AcquireAndIncRef();
		}

		template <typename T, typename ControlBlock>
		template <typename U>
		Weak<T, ControlBlock>::Weak(Weak<U, ControlBlock>&& weak) noexcept
			: m_data(Move(weak.m_data))
			, m_control(Move(weak.m_control))
		{
		}

		template <typename T, typename ControlBlock>
		Weak<T, ControlBlock>::Weak(const Weak& weak) noexcept
			: m_data(weak.m_data)
			, m_control(weak.m_control)
		{
			AcquireAndIncRef();
		}

		template <typename T, typename ControlBlock>
		Weak<T, ControlBlock>::Weak(Weak&& weak) noexcept
			: m_data(Move(weak.m_data))
			, m_control(Move(weak.m_control))
		{
		}

		template <typename T, typename ControlBlock>
		Weak<T, ControlBlock>::~Weak() noexcept
		{
			DecRefAndRelease();
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::operator=(const Weak& weak) noexcept -> Weak&
		{
			m_data = weak.m_data;
			m_control = weak.m_control;
			AcquireAndIncRef();
			return *this;
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::operator=(Weak&& weak) noexcept -> Weak&
		{
			m_data = Move(weak.m_data);
			m_control = Move(weak.m_control);
			return *this;
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::Swap(Weak& other) noexcept -> void
		{
			Algo::Swap(m_data, other.m_data);
			Algo::Swap(m_control, other.m_control);
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::Get() const noexcept -> const T*
		{
			ASSERT(IsValid(), "Calling Get() on an invalid Weak");
			return m_data.Ptr();
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::Get() noexcept -> T*
		{
			ASSERT(IsValid(), "Calling Get() on an invalid Weak");
			return m_data.Ptr();
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::GetMemRef() const noexcept -> const MemRef<T>&
		{
			ASSERT(IsValid(), "Calling GetMemRef() on an invalid Weak");
			return m_data;
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::UseCount() const noexcept -> u32
		{
			return m_control->strongCount;
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::IsValid() const noexcept -> bool
		{
			return m_control.IsValid();
		}

		template <typename T, typename ControlBlock>
		Weak<T, ControlBlock>::operator bool() const noexcept
		{
			return IsValid();
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::operator->() const noexcept -> const T*
		{
			ASSERT(IsValid(), "Calling operator->() on an invalid Weak");
			return m_data.Ptr();
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::operator->() noexcept -> T*
		{
			ASSERT(IsValid(), "Calling operator->() on an invalid Weak");
			return m_data.Ptr();
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::operator*() const noexcept -> const T&
		{
			ASSERT(IsValid(), "Calling operator*() on an invalid Weak");
			return *m_data.Ptr();
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::operator*() noexcept -> T&
		{
			ASSERT(IsValid(), "Calling operator*() on an invalid Weak");
			return *m_data.Ptr();
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::operator==(const Weak& other) const noexcept -> bool
		{
			return m_data == other.m_data;
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::AcquireAndIncRef() noexcept -> void
		{
			if (!m_control)
				return;

			m_control->Acquire();
			m_control->IncWeakRef();
		}

		template <typename T, typename ControlBlock>
		auto Weak<T, ControlBlock>::DecRefAndRelease() noexcept -> void
		{
			if (!m_control)
				return;

			m_control->DecWeakRef();
			if (m_control->Release())
				m_control.Dealloc();
		}
	}
}
