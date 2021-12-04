#pragma once
#include "core/MinInclude.h"

namespace Core
{

	/**
	 * Result
	 * \tparam T Result type
	 * \tparam E Error type
	 * \nore Result and error type need to be different
	 */
	template<typename T, NotSameAs<T> E>
	class Result
	{
	public:
		/**
		 * Create a result with a valid value
		 * \param[in] val Value
		 */
		constexpr Result(T&& val) noexcept;
		/**
		 * Create a result with an error
		 * \param[in] err Error
		 */
		constexpr Result(E&& err) noexcept;

		/**
		 * Check if the result is a success
		 * \return Whether the result is a success
		 */
		constexpr auto Success() const noexcept -> bool;
		/**
		 * Check if the result is an error
		 * \return Whether the result is an error
		 */
		constexpr auto Failed() const noexcept -> bool;

		/**
		 * Get the value
		 * \return Value
		 * \note When calling this when the result is 'failed', the function is undefined behavior
		 */
		constexpr auto Value() const noexcept -> const T&;
		/**
		 * Get the error
		 * \return Error
		 * \note When calling this when the result is 'success', the function is undefined behavior
		 */
		constexpr auto Error() const noexcept -> const E&;

		/**
		 * Get the value if the result is 'success', otherwise return the default value
		 * \param[in] def Default value
		 * \return Value
		 */
		constexpr auto ValueOr(const T& def) const noexcept -> const T&;
		/**
		 * Get the value if the result is 'success', otherwise return the default value
		 * \param[in] def Default value
		 * \return Value
		 */
		constexpr auto ValueOr(T&& def) noexcept -> T;

		explicit operator bool() const noexcept;

	private:
		Variant<T, E> m_value;
	};
}

#include "Result.inl"