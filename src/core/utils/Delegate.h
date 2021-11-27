#pragma once
#include "Pair.h"
#include "core/MinInclude.h"

namespace Core
{
	template<typename T>
	class Delegate;

	/**
	 * Delegate
	 * \tparam R Return type
	 * \tparam Args Argument types
	 */
	template<typename R, typename... Args>
	class Delegate<R(Args...)>
	{
	private:
		using StubPtr = R(*)(void*, Args&&...);

		template<typename C>
		using DelPair = Pair<C*, R(C::*)(Args...)>;

		/**
		 * Create a Delegate
		 * \param[in] pObj Object pointer
		 * \param[in] stub Stub
		 */
		Delegate(void* pObj, StubPtr stub) noexcept;
		/**
		 * Create a delegate
		 * \tparam C Class/struct type
		 * \param[in] pair Delegate pair
		 */
		template<typename C>
		Delegate(DelPair<C> pair) noexcept;

	public:

		/**
		 * Create an empty delegate
		 */
		Delegate() noexcept;
		/**
		 * Create a delegate from an aggregate and a pointer to the method
		 * \tparam C Aggregate type
		 * \param[in] pObj Pointer to aggregate
		 * \param[in] pMethod Pointer to method
		 */
		template<typename C>
		Delegate(C* pObj, R(C::* pMethod)(Args...)) noexcept;
		/**
		 * Create a delegate from an aggregate and a pointer to the method
		 * \tparam C Aggregate type
		 * \param[in] obj reference to aggregate
		 * \param[in] pMethod Pointer to method
		 */
		template<typename C>
		Delegate(C& obj, R(C::* pMethod)(Args...)) noexcept;
		/**
		 * Create a delegate from a function pointer
		 * \param[in] pFunc Function pointer
		 */
		Delegate(R(*pFunc)(Args...)) noexcept;
		/**
		 * Create a delegate from a function
		 * \tparam F Functor type
		 * \param[in] pFunctor Pointer to functor
		 */
		template<Functor<R, Args...> F>
		Delegate(F* pFunctor) noexcept;
		/**
		 * Create a delegate from a lambda
		 * \tparam L Lambda type
		 * \param[in] lambda Lambda
		 */
		template<Lambda L>
		Delegate(L& lambda) noexcept;
		/**
		 * Create a delegate set to nullptr
		 */
		Delegate(std::nullptr_t) noexcept;


		auto operator==(const Delegate& other) const noexcept -> bool;
		auto operator!=(const Delegate& other) const noexcept -> bool;
		auto operator==(std::nullptr_t) const noexcept -> bool;

		explicit operator bool() const noexcept;

		auto operator()(Args&&... args) noexcept -> R;

		/**
		 * Set the object of the delegate
		 * \param[in] pObj Pointer to object
		 * \warning The user is responsible for passing the correct type to the function
		 */
		auto SetObject(void* pObj) noexcept -> void;
		/**
		 * Check if the delegate has an object assigned to it
		 * \return Whether the delegate has an object assigned to it
		 */
		auto HasObject() const noexcept -> bool;

		/**
		 * Invoke the delegate without any safety checks
		 * \param[in] args Arguments
		 * \return Return value
		 */
		auto Invoke(Args&&... args) noexcept -> R;
		/**
		 * Check if the delegate can be invoked
		 * \return Whether the delegate can be invoked
		 */
		auto CanInvoke() const noexcept -> bool;
		/**
		 * Check if the delegate can invoke a member function
		 * \return Whether the delegate can invoke a member function
		 */
		auto CanMemberInvoke() const noexcept -> bool;

		/**
		 * Create a delegate from a function
		 * \tparam Func Function
		 * \return Delegate
		 */
		template<R(*Func)(Args...)>
		static Delegate From();
		/**
		 * Create a delegate from a member function
		 * \tparam C Aggregate type
		 * \tparam Func Function
		 * \return Delegate
		 */
		template<typename C, R(C::*Func)(Args...)>
		static Delegate From();
		/**
		 * Create a delegate from a member function and an object
		 * \tparam C Aggregate type
		 * \tparam Func Function
		 * \param[in] pObj Pointer to aggregate object
		 * \return Delegate
		 */
		template<typename C, R(C::* Func)(Args...)>
		static Delegate From(C* pObj);
		/**
		 * Create a delegate from a member function and an object
		 * \tparam C Aggregate type
		 * \tparam Func Function
		 * \param[in] obj Aggregate object
		 * \return Delegate
		 */
		template<typename C, R(C::* Func)(Args...)>
		static Delegate From(C& obj);
		/**
		 * Create a delegate from a runtime function pointer
		 * \param[in] pFunc Function pointer
		 * \return Delegate
		 */
		static Delegate From(R(*pFunc)(Args...));
		/**
		 * Create a delegate from a runtime member pointer and an object
		 * \tparam C Class type
		 * \param[in] pObj Pointer to aggregate
		 * \param[in] pMethod Method pointer
		 * \return Delegate
		 */
		template<typename C>
		static Delegate From(C* pObj, R(C::* pMethod)(Args...));
		/**
		 * Create a delegate from a functor
		 * \tparam F Functor type
		 * \param[in] pFunctor Pointer to functor
		 * \return Delegate
		 */
		template<Functor<R, Args...> F>
		static Delegate From(F* pFunctor);
		/**
		 * Create a delegate from a functor
		 * \tparam F Functor type
		 * \param[in] functor Reference to functor
		 * \return Delegate
		 */
		template<Functor<R, Args...> F>
		static Delegate From(F& functor);
		/**
		 * Create a delegate from a lambda
		 * \tparam L Lambda type
		 * \param[in] lambda Reference to lambda
		 * \return Delegate
		 */
		template<Lambda L>
		static Delegate From(L& lambda);

	private:

		/**
		 * Stub to call free functions
		 * \tparam Func Function type
		 * \param[in] args Arguments
		 * \return Return value
		 */
		template<R(*Func)(Args...)>
		static auto FuncStub(void*, Args&&... args) noexcept -> R;
		/**
		 * Stub to call a method
		 * \tparam C Class/struct type
		 * \tparam Func Function type
		 * \param[in] pObj Class object
		 * \param[in] args Arguments
		 * \return Return value
		 */
		template<typename C, R(C::*Func)(Args...)>
		static auto MethodStub(void* pObj, Args&&... args) noexcept -> R;
		/**
		 * Stub to call a delegate pair
		 * \tparam T Pair type
		 * \param[in] pObj Class object
		 * \param[in] args Arguments
		 * \return Return value
		 */
		template<typename T>
		static auto PairStub(void* pObj, Args&&... args) noexcept -> R;
		/**
		 * Stub to call a functor
		 * \tparam F Function type
		 * \param[in] pObj Class object
		 * \param[in] args Arguments
		 * \return Return value
		 */
		template<typename F>
		static auto FunctorStub(void* pObj, Args&&... args) noexcept -> R;
		/**
		 * Stub to call a lambda
		 * \tparam L Lambda type
		 * \param[in] pObj Class object
		 * \param[in] args Arguments
		 * \return Return value
		 */
		template<typename L>
		static auto LambdaStub(void* pObj, Args&&... args) noexcept -> R;

		void*   m_pObj;  ///< Pointer to object
		void*   m_pData; ///< Pointer to additional data
		StubPtr m_stub;  ///< Pointer to stub
	};

}

#include "Delegate.inl"