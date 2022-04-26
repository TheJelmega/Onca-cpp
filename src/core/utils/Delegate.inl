#pragma once
#if __RESHARPER__
#include "Delegate.h"
#endif

#include "Assert.h"
#include "Meta.h"

namespace Onca
{
	template <typename R, typename ... Args>
	Delegate<R(Args...)>::Delegate(void* pObj, StubPtr stub) noexcept
		: m_pObj(pObj)
		, m_pData(nullptr)
		, m_stub(stub)
	{
	}

	template <typename R, typename ... Args>
	template <typename C>
	Delegate<R(Args...)>::Delegate(DelPair<C> pair) noexcept
		: m_pData(nullptr)
		, m_stub(&PairStub<DelPair<C>>)
	{
		*reinterpret_cast<DelPair<C>*>(&m_pObj) = pair;
	}

	template <typename R, typename ... Args>
	Delegate<R(Args...)>::Delegate() noexcept
		: m_pObj(nullptr)
		, m_pData(nullptr)
		, m_stub(nullptr)
	{
	}

	template <typename R, typename ... Args>
	template <typename C>
	Delegate<R(Args...)>::Delegate(C* pObj, R(C::* pMethod)(Args...)) noexcept
		: Delegate(DelPair<C>{ pObj, pMethod })
	{
	}

	template <typename R, typename ... Args>
	template <typename C>
	Delegate<R(Args...)>::Delegate(C& obj, R(C::* pMethod)(Args...)) noexcept
		: Delegate(DelPair<C>{ &obj, pMethod })
	{
	}

	template <typename R, typename ... Args>
	Delegate<R(Args...)>::Delegate(R(* pFunc)(Args...)) noexcept
		: m_pObj(reinterpret_cast<void*>(pFunc))
		, m_pData(nullptr)
		, m_stub(&FunctorStub<R(*)(Args...)>)
	{
	}

	template <typename R, typename ... Args>
	template <Functor<R, Args...> F>
	Delegate<R(Args...)>::Delegate(F* pFunctor) noexcept
		: m_pObj(reinterpret_cast<void*>(pFunctor))
		, m_pData(nullptr)
		, m_stub(&FunctorStub<F*>)
	{
	}

	template <typename R, typename ... Args>
	template <Lambda<R, Args...> L>
	Delegate<R(Args...)>::Delegate(const L& lambda) noexcept
		: m_pObj(const_cast<void*>(reinterpret_cast<const void*>(&lambda)))
		, m_pData(nullptr)
		, m_stub(&LambdaStub<Decay<L>>)
	{
	}

	template <typename R, typename ... Args>
	Delegate<R(Args...)>::Delegate(std::nullptr_t) noexcept
		: Delegate()
	{
	}

	template <typename R, typename ... Args>
	auto Delegate<R(Args...)>::operator=(std::nullptr_t) noexcept -> Delegate&
	{
		m_pObj = nullptr;
		m_pData = nullptr;
		m_stub = nullptr;
		return *this;
	}

	template <typename R, typename ... Args>
	auto Delegate<R(Args...)>::operator=(R(* pFunc)(Args...)) noexcept -> Delegate&
	{
		m_pObj = pFunc;
		m_pData = nullptr;
		m_stub = &FunctorStub<R(*)(Args&&...)>;
		return *this;
	}

	template <typename R, typename ... Args>
	template <Functor<R, Args...> F>
	auto Delegate<R(Args...)>::operator=(F* pFunctor) noexcept -> Delegate&
	{
		m_pObj = reinterpret_cast<void*>(pFunctor);
		m_pData = nullptr;
		m_stub = &FunctorStub<F*>;
		return *this;
	}

	template <typename R, typename ... Args>
	template <Lambda<R, Args...> L>
	auto Delegate<R(Args...)>::operator=(const L& lambda) noexcept -> Delegate&
	{
		m_pObj = const_cast<void*>(reinterpret_cast<const void*>(&lambda));
		m_pData = nullptr;
		m_stub = &LambdaStub<Decay<L>>;
		return *this;
	}

	template <typename R, typename ... Args>
	auto Delegate<R(Args...)>::operator==(const Delegate& other) const noexcept -> bool
	{
		return m_pObj == other.m_pObj && m_pData == other.m_pData && m_stub == other.m_stub;
	}

	template <typename R, typename ... Args>
	auto Delegate<R(Args...)>::operator!=(const Delegate& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <typename R, typename ... Args>
	auto Delegate<R(Args...)>::operator==(std::nullptr_t) const noexcept -> bool
	{
		return !m_stub;
	}

	template <typename R, typename ... Args>
	Delegate<R(Args...)>::operator bool() const noexcept
	{
		return m_stub;
	}

	template <typename R, typename ... Args>
	auto Delegate<R(Args...)>::operator()(Args... args) noexcept -> R
	{
		return Invoke(Forward<Args>(args)...);
	}
	
	template <typename R, typename ... Args>
	auto Delegate<R(Args...)>::operator()(const Tuple<Args...>& tup) noexcept -> R
	{
		return Invoke(tup);
	}

	template <typename R, typename ... Args>
	void Delegate<R(Args ...)>::SetObject(void* pObj) noexcept
	{
		m_pObj = pObj;
	}

	template <typename R, typename ... Args>
	auto Delegate<R(Args...)>::HasObject() const noexcept -> bool
	{
		return m_pObj;
	}

	template <typename R, typename ... Args>
	auto Delegate<R(Args...)>::IsCalledOn(void* ptr) const noexcept -> bool
	{
		return m_pObj == ptr;
	}

	template <typename R, typename ... Args>
	auto Delegate<R(Args...)>::Invoke(Args&&... args) noexcept -> R
	{
		ASSERT(m_stub, "Cannot call a null delegate");
		if (m_pData)
			return m_stub(&m_pObj, Forward<Args>(args)...);
		return m_stub(m_pObj, Forward<Args>(args)...);
	}

	template <typename R, typename ... Args>
	auto Delegate<R(Args...)>::Invoke(const Tuple<Args...>& args) noexcept -> R
	{
		return Invoke(args, MakeIndexSequence<sizeof...(Args)>{});
	}

	template <typename R, typename ... Args>
	auto Delegate<R(Args...)>::TryInvoke(Args&&... args) noexcept -> R requires DefaultConstructible<R> || SameAs<R, void>
	{
		if (!CanInvoke())
			return R();
		return Invoke(Forward<Args>(args)...);
	}

	template <typename R, typename ... Args>
	auto Delegate<R(Args...)>::CanInvoke() const noexcept -> bool
	{
		return m_stub;
	}

	template <typename R, typename ... Args>
	auto Delegate<R(Args...)>::CanMemberInvoke() const noexcept -> bool
	{
		return m_pObj && m_stub;
	}

	template <typename R, typename ... Args>
	template <R(* Func)(Args...)>
	Delegate<R(Args...)> Delegate<R(Args...)>::From()
	{
		return { nullptr, &FuncStub<Func> };
	}

	template <typename R, typename ... Args>
	template <typename C, R(C::* Func)(Args...)>
	Delegate<R(Args...)> Delegate<R(Args...)>::From()
	{
		return { nullptr, &MethodStub<C, Func> };
	}

	template <typename R, typename ... Args>
	template <typename C, R(C::* Func)(Args...)>
	Delegate<R(Args...)> Delegate<R(Args...)>::From(C* pObj)
	{
		return { pObj, &MethodStub<C, Func> };
	}

	template <typename R, typename ... Args>
	template <typename C, R(C::* Func)(Args...)>
	Delegate<R(Args...)> Delegate<R(Args...)>::From(C& obj)
	{
		return { &obj, &MethodStub<C, Func> };
	}

	template <typename R, typename ... Args>
	Delegate<R(Args...)> Delegate<R(Args...)>::From(R(* pFunc)(Args...))
	{
		return { pFunc };
	}

	template <typename R, typename ... Args>
	template <typename C>
	Delegate<R(Args...)> Delegate<R(Args...)>::From(C* pObj, R(C::* pMethod)(Args...))
	{
		return { DelPair<C>{ pObj, pMethod } };
	}

	template <typename R, typename ... Args>
	template <Functor<R, Args...> F>
	Delegate<R(Args...)> Delegate<R(Args...)>::From(F* pFunctor)
	{
		return { pFunctor };
	}

	template <typename R, typename ... Args>
	template <Functor<R, Args...> F>
	Delegate<R(Args...)> Delegate<R(Args...)>::From(F& functor)
	{
		return { functor };
	}

	template <typename R, typename ... Args>
	template <Lambda L>
	Delegate<R(Args...)> Delegate<R(Args...)>::From(L& lambda)
	{
		return { lambda };
	}
	
	template <typename R, typename ... Args>
	template <R(* Func)(Args...)>
	auto Delegate<R(Args...)>::FuncStub(void*, Args&&... args) noexcept -> R
	{
		return Func(Forward<Args>(args)...);
	}

	template <typename R, typename ... Args>
	template <typename C, R(C::* Func)(Args...)>
	auto Delegate<R(Args...)>::MethodStub(void* pObj, Args&&... args) noexcept -> R
	{
		ASSERT(pObj, "No object assigned");
		return (static_cast<C*>(pObj)->*Func)(Forward<Args>(args)...);
	}

	template <typename R, typename ... Args>
	template <typename T>
	auto Delegate<R(Args...)>::PairStub(void* pObj, Args&&... args) noexcept -> R
	{
		ASSERT(pObj, "No object assigned");
		T* pair = static_cast<T*>(pObj);
		return ((pair->first)->*(pair->second))(Forward<Args>(args)...);
	}

	template <typename R, typename ... Args>
	template <typename F>
	auto Delegate<R(Args...)>::FunctorStub(void* pObj, Args&&... args) noexcept -> R
	{
		ASSERT(pObj, "No object assigned");
		return (*static_cast<F>(pObj))(Forward<Args>(args)...);
	}
	
	template <typename R, typename ... Args>
	template <typename L>
	auto Delegate<R(Args...)>::LambdaStub(void* pObj, Args&&... args) noexcept -> R
	{
		ASSERT(pObj, "No object assigned");
		return (*static_cast<L*>(pObj))(Forward<Args>(args)...);
	}
}
