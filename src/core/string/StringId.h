#pragma once
#include "core/MinInclude.h"
#include "String.h"
#include "ConstString.h"

namespace Core
{
	class StringId
	{
	public:
		/**
		 * Create a null string id
		 */
		constexpr StringId() noexcept;
		/**
		 * Create a string id from a known id
		 * \param[in] id Id
		 */
		explicit constexpr StringId(u64 id) noexcept;
		/**
		 * Create a string id from a string
		 * \param[in] str String
		 */
		explicit StringId(const String& str) noexcept;
		/**
		 * Create a string id from a string
		 * \tparam Cap Const string capacity
		 * \param[in] str String
		 */
		template<usize Cap>
		explicit constexpr StringId(const ConstString<Cap>& str) noexcept;

		constexpr operator u64() const noexcept;

	private:
		u64 m_id;
	};
}