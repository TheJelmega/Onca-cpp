#pragma once
#include "core/MinInclude.h"
#include "core/containers/HashMap.h"
#include "core/memory/Unique.h"
#include "core/string/String.h"
#include "core/logging/LogCategory.h"

namespace Onca::FileSystem
{
	class Path;
}

namespace Onca
{
	DEFINE_LOG_CATEGORY(TOML);

	enum class TomlValueType
	{
		None,
		Table,
		Array,
		String,
		Integer,
		Float,
		Bool,

		// UNSUPPORTED
		Date,
		Time,
		DateTime
	};

	class CORE_API Toml
	{
		DISABLE_COPY(Toml);
	public:
		Toml() noexcept;
		Toml(TomlValueType type) noexcept;
		Toml(const String& val) noexcept;
		Toml(i64 val) noexcept;
		Toml(f64 val) noexcept;
		Toml(bool val) noexcept;
		
		Toml(Toml&&) noexcept = default;
		
		auto operator=(Toml&&) noexcept -> Toml& = default;

		explicit operator String() const noexcept;
		explicit operator i64() const noexcept;
		explicit operator f64() const noexcept;
		explicit operator bool() const noexcept;

		auto operator[](usize idx) const noexcept -> const Toml&;
		auto operator[](const String& name) const noexcept -> const Toml&;

		/**
		 * Set the node a string node with a given value
		 * \param[in] val Value
		 */
		void Set(const String& val) noexcept;
		/**
		 * Set the node an integer node with a given value
		 * \param[in] val Value
		 */
		void Set(i64 val) noexcept;
		/**
		 * Set the node a float node with a given value
		 * \param[in] val Value
		 */
		void Set(f64 val) noexcept;
		/**
		 * Set the node a bool node with a given value
		 * \param[in] val Value
		 */
		void Set(bool val) noexcept;

		/**
		 * Get or add a node to the table
		 * \param[in] name Name
		 * \param[in] type Node type if it has been added
		 * \return Pair with toml node and if it was newly added
		 */
		auto GetOrAddTableNode(const String& name, TomlValueType type = TomlValueType::None) noexcept -> Pair<Toml*, bool>;
		/**
		 * Add a node to an array
		 * \param[in] node Node
		 * \return Whether to node was able to be added, should only be false is the node is not an array
		 */
		auto AddToArray(Toml&& node) -> bool;

		/**
		 * Get the current type of the node
		 * \return Type
		 */
		auto GetType() const noexcept -> TomlValueType;

		// Direct node access
		//--------------------------------------------------------------------------------------------------------------------------------
		/**
		 * Get a string from the node, if the contained type is convertible to a string, convert it
		 * \return String
		 */
		auto GetString() const noexcept -> String;
		/**
		 * Get an integer from the node, if the contained type is convertible to a string, convert it
		 * \return Integer
		 */
		auto GetInt() const noexcept -> i64;
		/**
		 * Get a string from the node, if the contained type is convertible to a string, convert it
		 * \return Float
		 */
		auto GetFp() const noexcept -> f64;
		/**
		 * Get a bool from the node, if the contained type is convertible to a bool, convert it
		 * \return Bool
		 */
		auto GetBool() const noexcept -> bool;

		/**
		 * Get a string from the node, if the node is a different type, return a default value
		 * \param[in] val Default value
		 * \return String
		 */
		auto GetStringOr(const String& val) const noexcept -> const String&;
		/**
		 * Get an integer from the node, if the node is a different type, return a default value
		 * \param[in] val Default value
		 * \return Integer
		 */
		auto GetIntOr(i64 val) const noexcept -> i64;
		/**
		 * Get a float from the node, if the node is a different type, return a default value
		 * \param[in] val Default value
		 * \return Float
		 */
		auto GetFpOr(f64 val) const noexcept -> f64;
		/**
		 * Get a bool from the node, if the node is a different type, return a default value
		 * \param[in] val Default value
		 * \return Bool
		 */
		auto GetBoolOr(bool val) const noexcept -> bool;

		/**
		 * Try to get a string from the node, if the node is a different type, return a NullOpt
		 * \return String or NullOpt
		 */
		auto TryGetString() const noexcept -> Optional<String>;
		/**
		 * Try to get an integer from the node, if the node is a different type, return a NullOpt
		 * \return Integer or NullOpt
		 */
		auto TryGetInt() const noexcept -> Optional<i64>;
		/**
		 * Try to get a float from the node, if the node is a different type, return a NullOpt
		 * \return Float or NullOpt
		 */
		auto TryGetFp() const noexcept -> Optional<f64>;
		/**
		 * Try to get a bool from the node, if the node is a different type, return a NullOpt
		 * \return Bool or NullOpt
		 */
		auto TryGetBool() const noexcept -> Optional<bool>;

		// Array sub-node access
		//--------------------------------------------------------------------------------------------------------------------------------
		/**
		 * Get a string from an array sub-node, if the contained type is convertible to a string, convert it
		 * \param[in] idx Index
		 * \return String
		 */
		auto GetString(usize idx) const noexcept -> String;
		/**
		 * Get an integer from an array sub-node, if the contained type is convertible to an integer, convert it
		 * \param[in] idx Index
		 * \return String
		 */
		auto GetIntAt(usize idx) const noexcept -> i64;
		/**
		 * Get a float from an array sub-node, if the contained type is convertible to a float, convert it
		 * \param[in] idx Index
		 * \return Float
		 */
		auto GetFpAt(usize idx) const noexcept -> f64;
		/**
		 * Get a bool from an array sub-node, if the contained type is convertible to a bool, convert it
		 * \param[in] idx Index
		 * \return Bool
		 */
		auto GetBoolAt(usize idx) const noexcept -> bool;

		/**
		 * Get a string from an array sub-node, if the node is a different type, return a default value
		 * \param[in] val Default value
		 * \param[in] idx Index
		 * \return String
		 */
		auto GetStringOr(usize idx, const String& val) const noexcept -> const String&;
		/**
		 * Get an integer an array sub-node, if the node is a different type, return a default value
		 * \param[in] val Default value
		 * \param[in] idx Index
		 * \return Integer
		 */
		auto GetIntOr(usize idx, i64 val) const noexcept -> i64;
		/**
		 * Get a float an array sub-node, if the node is a different type, return a default value
		 * \param[in] val Default value
		 * \param[in] idx Index
		 * \return Float
		 */
		auto GetFpOr(usize idx, f64 val) const noexcept -> f64;
		/**
		 * Get a bool an array sub-node, if the node is a different type, return a default value
		 * \param[in] val Default value
		 * \param[in] idx Index
		 * \return Bool
		 */
		auto GetBoolOr(usize idx, bool val) const noexcept -> bool;

		/**
		 * Try to get a string an array sub-node, if the node is a different type, return a NullOpt
		 * \param[in] idx Index
		 * \return String or NullOpt
		 */
		auto TryGetString(usize idx) const noexcept -> Optional<String>;
		/**
		 * Try to get an integer an array sub-node, if the node is a different type, return a NullOpt
		 * \param[in] idx Index
		 * \return Integer or NullOpt
		 */
		auto TryGetInt(usize idx) const noexcept -> Optional<i64>;
		/**
		 * Try to get a float an array sub-node, if the node is a different type, return a NullOpt
		 * \param[in] idx Index
		 * \return Float or NullOpt
		 */
		auto TryGetFp(usize idx) const noexcept -> Optional<f64>;
		/**
		 * Try to get a bool an array sub-node, if the node is a different type, return a NullOpt
		 * \param[in] idx Index
		 * \return Bool or NullOpt
		 */
		auto TryGetBool(usize idx) const noexcept -> Optional<bool>;

		// Table sub-node access
		//--------------------------------------------------------------------------------------------------------------------------------
		/**
		 * Get a string from a table sub-node, if the contained type is convertible to a string, convert it
		 * \param[in] name Sub-node name
		 * \return String
		 */
		auto GetString(const String& name) const noexcept -> String;
		/**
		 * Get an integer from a table sub-node, if the contained type is convertible to an integer, convert it
		 * \param[in] name Sub-node name
		 * \return String
		 */
		auto GetIntAt(const String& name) const noexcept -> i64;
		/**
		 * Get a float from a table sub-node, if the contained type is convertible to a float, convert it
		 * \param[in] name Sub-node name
		 * \return Float
		 */
		auto GetFpAt(const String& name) const noexcept -> f64;
		/**
		 * Get a bool from a table sub-node, if the contained type is convertible to a bool, convert it
		 * \param[in] name Sub-node name
		 * \return Bool
		 */
		auto GetBoolAt(const String& name) const noexcept -> bool;

		/**
		 * Get a string from a table sub-node, if the node is a different type, return a default value
		 * \param[in] name Sub-node name
		 * \param[in] val Default value
		 * \return String
		 */
		auto GetStringOr(const String& name, const String& val) const noexcept -> const String&;
		/**
		 * Get an integer a table sub-node, if the node is a different type, return a default value
		 * \param[in] name Sub-node name
		 * \param[in] val Default value
		 * \return Integer
		 */
		auto GetIntOr(const String& name, i64 val) const noexcept -> i64;
		/**
		 * Get a float a table sub-node, if the node is a different type, return a default value
		 * \param[in] name Sub-node name
		 * \param[in] val Default value
		 * \return Float
		 */
		auto GetFpOr(const String& name, f64 val) const noexcept -> f64;
		/**
		 * Get a bool a table sub-node, if the node is a different type, return a default value
		 * \param[in] name Sub-node name
		 * \param[in] val Default value
		 * \return Bool
		 */
		auto GetBoolOr(const String& name, bool val) const noexcept -> bool;

		/**
		 * Try to get a string a table sub-node, if the node is a different type, return a NullOpt
		 * \param[in] name Sub-node name
		 * \return String or NullOpt
		 */
		auto TryGetString(const String& name) const noexcept -> Optional<String>;
		/**
		 * Try to get a string a table sub-node, if the node is a different type, return a NullOpt
		 * \param[in] name Sub-node name
		 * \return String or NullOpt
		 */
		auto TryGetInt(const String& name) const noexcept -> Optional<i64>;
		/**
		 * Try to get an integer a table sub-node, if the node is a different type, return a NullOpt
		 * \param[in] name Sub-node name
		 * \return Integer or NullOpt
		 */
		auto TryGetFp(const String& name) const noexcept -> Optional<f64>;
		/**
		 * Try to get a bool a table sub-node, if the node is a different type, return a NullOpt
		 * \param[in] name Sub-node name
		 * \return Bool or NullOpt
		 */
		auto TryGetBool(const String& name) const noexcept -> Optional<bool>;

		/**
		 * Get the sub-node at a given index
		 * \param[in] idx Index
		 * \return Sub-node
		 */
		auto GetSubNode(usize idx) const noexcept -> const Toml&;
		/**
		 * Get the sub-node at a given name
		 * \param[in] name Sub-node name
		 * \return Sub-node
		 */
		auto GetSubNode(const String& name) const noexcept -> const Toml&;

		/**
		 * Try to get the sub-node at a given index, nullptr is invalid
		 * \param[in] idx Index
		 * \return Sub-node
		 */
		auto TryGetSubNode(usize idx) const noexcept -> const Toml*;
		/**
		 * Try to get the sub-node at a given name, nullptr is invalid
		 * \param[in] name Sub-node name
		 * \return Sub-node
		 */
		auto TryGetSubNode(const String& name) const noexcept -> const Toml*;

		/**
		 * Get the array size, 0 if it is not an array
		 * \return Array size
		 */
		auto ArraySize() const noexcept -> usize;
		/**
		 * Check if the table contains a key, false if it is not a table
		 * \param[in] name Name
		 * \return Whether the table contains the key
		 */
		auto ContainsKey(const String& name) const noexcept -> bool;


		/**
		 * Parse toml from a file
		 * \param[in] path Path to toml file
		 * \return Parsed toml
		 */
		static auto ParseFromFile(const FileSystem::Path& path) noexcept -> Toml;
		/**
		 * Parse toml from a string
		 * \param[in] content String with toml file content
		 * \return Parsed toml
		 */
		static auto ParseFromString(const String& content) noexcept -> Toml;

	private:

		// TODO: Figure out why using `Toml` instead of `Unique<Toml>` makes `Toml` not `Movable<>`
		Variant<std::monostate, HashMap<String, Unique<Toml>>, DynArray<Unique<Toml>>, String, i64, f64, bool> m_value;
	};

	namespace Detail
	{
		/**
		 * Toml parser
		 */
		class TomlParser
		{
		public:
			/**
			 * Create a toml parser
			 */
			TomlParser() noexcept;

			/**
			 * Parse toml from a string
			 * \param[in] content String with toml file content
			 * \return Parsed toml
			 */
			auto ParseFromString(const String& content) noexcept -> Toml;

		private:
			// TODO: Move generic parser functions to utility file
			/**
			 * Skip whitespace in the string
			 */
			void SkipWhitespaceAndComments() noexcept;
			auto GetEscapedCharacter() noexcept -> UCodepoint;
			auto ExtractSingleLineString() noexcept -> String;
			auto ExtractMultiLineString() noexcept -> String;
			auto ExtractValueString() noexcept -> String;
			auto ExtractIdentifier() noexcept -> String;
			auto ExtractDottedIdentifiers() noexcept -> DynArray<String>;
			void ExtractAndInsertKeyValuePair( Toml& curNode) noexcept;
			auto ExtractValue() noexcept -> Toml;
			auto ExtractBasicValue() noexcept -> Variant<i64, f64, bool>;
			auto ExtractArray() noexcept -> Toml;
			auto ExtractInlineTable() noexcept -> Toml;

			String m_content;
			usize  m_idx;
		};
	}
}
