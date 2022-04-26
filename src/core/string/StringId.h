#pragma once
#include "core/MinInclude.h"
#include "String.h"
#include "ConstString.h"

namespace Onca
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
		 * Create a string id from a c-string
		 * \param[in] str C-string
		 */
		explicit constexpr StringId(const char* str) noexcept;
		/**
		 * Create a string id from a c-string
		 * \param[in] str C-string
		 * \param[in] len String length
		 */
		constexpr StringId(const char* str, usize len) noexcept;
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
		/**
		 * Reimplementation of FNV1A_64, because casting from char to u8 is not allowed at compile time
		 * \param[in] str C
		 * \param[in] len Lenght of the string
		 * \return Hash
		 */
		constexpr auto Hash(const char* str, usize len) noexcept -> u64;

		u64 m_id;
	};
}

namespace Literals
{
	constexpr auto operator""_sid(const char* cstr, usize size) noexcept -> Onca::StringId;
}
