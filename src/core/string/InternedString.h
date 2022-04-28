#pragma once
#include "core/MinInclude.h"
#include "StringId.h"
#include "core/containers/HashMap.h"

namespace Onca
{
	/**
	 * Interned string
	 */
	class CORE_API InternedString
	{
	public:
		/**
		 * Create an empty interned string
		 */
		InternedString() noexcept;
		/**
		 * Create an interned string from a string
		 * \param[in] string String
		 */
		InternedString(const String& string) noexcept;

		InternedString(const InternedString&) noexcept = default;
		InternedString(InternedString&&) noexcept = default;

		auto operator=(const String& string) noexcept -> InternedString&;
		auto operator=(const InternedString&) noexcept -> InternedString& = default;
		auto operator=(InternedString&&) noexcept -> InternedString& = default;

		auto operator==(const InternedString& other) const noexcept -> bool;
		auto operator==(const String& other) const noexcept -> bool;

		/**
		 * Get the string the interned string represents
		 * \return String
		 */
		auto Get() const noexcept -> const String&;
		/**
		 * Get the string the interned string represents
		 * \return String
		 */
		auto ToString() const noexcept -> String;
		/**
		 * Get the string id of the interned string
		 * \return String id
		 */
		auto Id() const noexcept -> StringId;
		/**
		 * Check if the interned string refers to an empty string
		 * \return Whether the interned string refers to an empty string
		 */
		auto IsEmpty() const noexcept -> bool;


		operator const String&() const noexcept;

	private:
		StringId m_id; ///< String id
	};

	CORE_API auto operator==(const String& str, const InternedString& interned) noexcept -> bool;

	template<>
	struct CORE_API Hash<InternedString>
	{
		auto operator()(const InternedString& t) const noexcept -> u64
		{
			return t.Id();
		}
	};

	class CORE_API InternedStringManager
	{
	public:
		/**
		 * Create an uninitialized interned string manager
		 */
		InternedStringManager() noexcept;
		
		/**
		 * Shut down the manager
		 */
		void Shutdown() noexcept;

		/**
		 * Add a string to the manager
		 * \param[in] str String
		 * \return String id of the string
		 */
		auto AddString(const String& str) noexcept -> StringId;
		/**
		 * Get the interned string from its id
		 * \param[in] id String id
		 * \return String
		 */
		auto GetString(StringId id) const noexcept -> const String&;

	private:
		HashMap<StringId, String> m_strings; ///< Interned strings
	};

	CORE_API auto GetInternedStringManager() noexcept -> InternedStringManager&;
}
