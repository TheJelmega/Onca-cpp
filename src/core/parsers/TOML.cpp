#include "TOML.h"

#include "core/logging/Logger.h"
#include "core/string/Parse.h"
#include "core/string/Stringify.h"

namespace Onca
{
	Toml::Toml() noexcept
	{
	}

	Toml::Toml(TomlValueType type) noexcept
	{
		switch (type)
		{
		case TomlValueType::Table:
			m_value = HashMap<String, Unique<Toml>>{};
			break;
		case TomlValueType::Array: 
			m_value = DynArray<Unique<Toml>>{};
			break;
		case TomlValueType::String: 
			m_value = String{};
			break;
		case TomlValueType::Integer:
			m_value = 0ll;
			break;
		case TomlValueType::Float:
			m_value = 0.0;
			break;
		case TomlValueType::Bool:
			m_value = false;
			break;
		case TomlValueType::Date: break;
		case TomlValueType::Time: break;
		case TomlValueType::DateTime: break;
		default: ;
		}
	}

	Toml::Toml(const String& val) noexcept
		: m_value(val)
	{
	}

	Toml::Toml(i64 val) noexcept
		: m_value(val)
	{
	}

	Toml::Toml(f64 val) noexcept
		: m_value(val)
	{
	}

	Toml::Toml(bool val) noexcept
		: m_value(val)
	{
	}

	Toml::operator String() const noexcept
	{
		return GetString();
	}

	Toml::operator i64() const noexcept
	{
		return GetInt();
	}

	Toml::operator f64() const noexcept
	{
		return GetFp();
	}

	Toml::operator bool() const noexcept
	{
		return GetBool();
	}

	auto Toml::operator[](usize idx) const noexcept -> const Toml&
	{
		return GetSubNode(idx);
	}

	auto Toml::operator[](const String& name) const noexcept -> const Toml&
	{
		return GetSubNode(name);
	}

	void Toml::Set(const String& val) noexcept
	{
		m_value = val;
	}

	void Toml::Set(i64 val) noexcept
	{
		m_value = val;
	}

	void Toml::Set(f64 val) noexcept
	{
		m_value = val;
	}

	void Toml::Set(bool val) noexcept
	{
		m_value = val;
	}

	auto Toml::GetOrAddTableNode(const String& name, TomlValueType type) noexcept -> Pair<Toml*, bool>
	{
		ASSERT(GetType() == TomlValueType::Table, "Node is not a table");
		HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		auto it = table.Find(name);

		bool added = false;
		if (it == table.End())
		{
			it = table.Insert(name, Unique<Toml>::Create(type)).first;
			added = true;
		}

		return { it->second.Get(), added };
	}

	auto Toml::AddToArray(Toml&& node) -> bool
	{
		if (GetType() != TomlValueType::Array)
			return false;

		DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		arr.Add(Unique<Toml>::Create(Move(node)));
		return true;
	}

	auto Toml::GetType() const noexcept -> TomlValueType
	{
		return TomlValueType(m_value.index());
	}

	auto Toml::GetString() const noexcept -> String
	{
		switch (TomlValueType(m_value.index()))
		{
		case TomlValueType::String: return std::get<String>(m_value);
		case TomlValueType::Integer: return ToString(std::get<i64>(m_value));
		case TomlValueType::Float: return ToString(std::get<f64>(m_value));
		case TomlValueType::Bool: return ToString(std::get<bool>(m_value));
		default:
			ASSERT(GetType() == TomlValueType::String, "Value cannot be converted to a string");
			return String{};
		}
	}

	auto Toml::GetInt() const noexcept -> i64
	{
		switch (TomlValueType(m_value.index()))
		{
		case TomlValueType::String: return TryParse<i64>(std::get<String>(m_value)).value_or(0);
		case TomlValueType::Integer: return std::get<i64>(m_value);
		case TomlValueType::Float: return i64(std::get<f64>(m_value));
		case TomlValueType::Bool: return i64(std::get<bool>(m_value));
		default: 
			ASSERT(GetType() == TomlValueType::String, "Value cannot be converted to an int");
			return 0;
		}
	}

	auto Toml::GetFp() const noexcept -> f64
	{
		switch (TomlValueType(m_value.index()))
		{
		case TomlValueType::String: return TryParse<f64>(std::get<String>(m_value)).value_or(0.0);
		case TomlValueType::Integer: return f64(std::get<i64>(m_value));
		case TomlValueType::Float: return std::get<f64>(m_value);
		case TomlValueType::Bool: return f64(std::get<bool>(m_value));
		default:
			ASSERT(GetType() == TomlValueType::String, "Value cannot be converted to a float");
			return 0;
		}
	}

	auto Toml::GetBool() const noexcept -> bool
	{
		switch (TomlValueType(m_value.index()))
		{
		case TomlValueType::String: return TryParse<bool>(std::get<String>(m_value)).value_or(false);
		case TomlValueType::Integer: return bool(std::get<i64>(m_value));
		case TomlValueType::Float: return bool(std::get<f64>(m_value));
		case TomlValueType::Bool: return std::get<bool>(m_value);
		default:
			ASSERT(GetType() == TomlValueType::String, "Value cannot be converted to a bool");
			return 0;
		}
	}

	auto Toml::GetStringOr(const String& val) const noexcept -> const String&
	{
		if (const String* pVal = std::get_if<String>(&m_value))
			return *pVal;
		return val;
	}

	auto Toml::GetIntOr(i64 val) const noexcept -> i64
	{
		if (const i64* pVal = std::get_if<i64>(&m_value))
			return *pVal;
		return val;
	}

	auto Toml::GetFpOr(f64 val) const noexcept -> f64
	{
		if (const f64* pVal = std::get_if<f64>(&m_value))
			return *pVal;
		return val;
	}

	auto Toml::GetBoolOr(bool val) const noexcept -> bool
	{
		if (const bool* pVal = std::get_if<bool>(&m_value))
			return *pVal;
		return val;
	}

	auto Toml::TryGetString() const noexcept -> Optional<String>
	{
		if (const String* pVal = std::get_if<String>(&m_value))
			return *pVal;
		return NullOpt;
	}

	auto Toml::TryGetInt() const noexcept -> Optional<i64>
	{
		if (const i64* pVal = std::get_if<i64>(&m_value))
			return *pVal;
		return NullOpt;
	}

	auto Toml::TryGetFp() const noexcept -> Optional<f64>
	{
		if (const f64* pVal = std::get_if<f64>(&m_value))
			return *pVal;
		return NullOpt;
	}

	auto Toml::TryGetBool() const noexcept -> Optional<bool>
	{
		if (const bool* pVal = std::get_if<bool>(&m_value))
			return *pVal;
		return NullOpt;
	}

	auto Toml::GetString(usize idx) const noexcept -> String
	{
		ASSERT(GetType() == TomlValueType::Array, "Node is not an array");
		const DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		ASSERT(idx < arr.Size(), "Index out of range");
		return arr[idx]->GetString();
	}

	auto Toml::GetIntAt(usize idx) const noexcept -> i64
	{
		ASSERT(GetType() == TomlValueType::Array, "Node is not an array");
		const DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		ASSERT(idx < arr.Size(), "Index out of range");
		return arr[idx]->GetInt();
	}

	auto Toml::GetFpAt(usize idx) const noexcept -> f64
	{
		ASSERT(GetType() == TomlValueType::Array, "Node is not an array");
		const DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		ASSERT(idx < arr.Size(), "Index out of range");
		return arr[idx]->GetFp();
	}

	auto Toml::GetBoolAt(usize idx) const noexcept -> bool
	{
		ASSERT(GetType() == TomlValueType::Array, "Node is not an array");
		const DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		ASSERT(idx < arr.Size(), "Index out of range");
		return arr[idx]->GetBool();
	}

	auto Toml::GetStringOr(usize idx, const String& val) const noexcept -> const String&
	{
		if (GetType() != TomlValueType::Array)
			return val;
		const DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		if (idx >= arr.Size())
			return val;
		return arr[idx]->GetStringOr(val);
	}

	auto Toml::GetIntOr(usize idx, i64 val) const noexcept -> i64
	{
		if (GetType() != TomlValueType::Array)
			return val;
		const DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		if (idx >= arr.Size())
			return val;
		return arr[idx]->GetIntOr(val);
	}

	auto Toml::GetFpOr(usize idx, f64 val) const noexcept -> f64
	{
		if (GetType() != TomlValueType::Array)
			return val;
		const DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		if (idx >= arr.Size())
			return val;
		return arr[idx]->GetFpOr(val);
	}

	auto Toml::GetBoolOr(usize idx, bool val) const noexcept -> bool
	{
		if (GetType() != TomlValueType::Array)
			return val;
		const DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		if (idx >= arr.Size())
			return val;
		return arr[idx]->GetBoolOr(val);
	}

	auto Toml::TryGetString(usize idx) const noexcept -> Optional<String>
	{
		if (GetType() != TomlValueType::Array)
			return NullOpt;
		const DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		if (idx >= arr.Size())
			return NullOpt;
		return arr[idx]->TryGetString();
	}

	auto Toml::TryGetInt(usize idx) const noexcept -> Optional<i64>
	{
		if (GetType() != TomlValueType::Array)
			return NullOpt;
		const DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		if (idx >= arr.Size())
			return NullOpt;
		return arr[idx]->TryGetInt();
	}

	auto Toml::TryGetFp(usize idx) const noexcept -> Optional<f64>
	{
		if (GetType() != TomlValueType::Array)
			return NullOpt;
		const DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		if (idx >= arr.Size())
			return NullOpt;
		return arr[idx]->TryGetFp();
	}

	auto Toml::TryGetBool(usize idx) const noexcept -> Optional<bool>
	{
		if (GetType() != TomlValueType::Array)
			return NullOpt;
		const DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		if (idx >= arr.Size())
			return NullOpt;
		return arr[idx]->TryGetBool();
	}

	auto Toml::GetString(const String& name) const noexcept -> String
	{
		ASSERT(GetType() == TomlValueType::Table, "Node is not a table");
		const HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		ASSERT(table.Contains(name), "Element does not exist");
		return table[name]->GetString();
	}

	auto Toml::GetIntAt(const String& name) const noexcept -> i64
	{
		ASSERT(GetType() == TomlValueType::Table, "Node is not a table");
		const HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		ASSERT(table.Contains(name), "Element does not exist");
		return table[name]->GetInt();
	}

	auto Toml::GetFpAt(const String& name) const noexcept -> f64
	{
		ASSERT(GetType() == TomlValueType::Table, "Node is not a table");
		const HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		ASSERT(table.Contains(name), "Element does not exist");
		return table[name]->GetFp();
	}

	auto Toml::GetBoolAt(const String& name) const noexcept -> bool
	{
		ASSERT(GetType() == TomlValueType::Table, "Node is not a table");
		const HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		ASSERT(table.Contains(name), "Element does not exist");
		return table[name]->GetBool();
	}

	auto Toml::GetStringOr(const String& name, const String& val) const noexcept -> const String&
	{
		if (GetType() != TomlValueType::Table)
			return val;
		const HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		auto it = table.Find(name);
		if (it == table.End())
			return val;
		return it->second->GetStringOr(val);
	}

	auto Toml::GetIntOr(const String& name, i64 val) const noexcept -> i64
	{
		if (GetType() != TomlValueType::Table)
			return val;
		const HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		auto it = table.Find(name);
		if (it == table.End())
			return val;
		return it->second->GetIntOr(val);
	}

	auto Toml::GetFpOr(const String& name, f64 val) const noexcept -> f64
	{
		if (GetType() != TomlValueType::Table)
			return val;
		const HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		auto it = table.Find(name);
		if (it == table.End())
			return val;
		return it->second->GetFpOr(val);
	}

	auto Toml::GetBoolOr(const String& name, bool val) const noexcept -> bool
	{
		if (GetType() != TomlValueType::Table)
			return val;
		const HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		auto it = table.Find(name);
		if (it == table.End())
			return val;
		return it->second->GetBoolOr(val);
	}

	auto Toml::TryGetString(const String& name) const noexcept -> Optional<String>
	{
		if (GetType() != TomlValueType::Table)
			return NullOpt;
		const HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		auto it = table.Find(name);
		if (it == table.End())
			return NullOpt;
		return it->second->TryGetString();
	}

	auto Toml::TryGetInt(const String& name) const noexcept -> Optional<i64>
	{
		if (GetType() != TomlValueType::Table)
			return NullOpt;
		const HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		auto it = table.Find(name);
		if (it == table.End())
			return NullOpt;
		return it->second->TryGetInt();
	}

	auto Toml::TryGetFp(const String& name) const noexcept -> Optional<f64>
	{
		if (GetType() != TomlValueType::Table)
			return NullOpt;
		const HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		auto it = table.Find(name);
		if (it == table.End())
			return NullOpt;
		return it->second->TryGetFp();
	}

	auto Toml::TryGetBool(const String& name) const noexcept -> Optional<bool>
	{
		if (GetType() != TomlValueType::Table)
			return NullOpt;
		const HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		auto it = table.Find(name);
		if (it == table.End())
			return NullOpt;
		return it->second->TryGetBool();
	}

	auto Toml::GetSubNode(usize idx) const noexcept -> const Toml&
	{
		ASSERT(GetType() == TomlValueType::Array, "Node is not an array");
		const DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		ASSERT(idx < arr.Size(), "Index out of range");
		return *arr[idx];
	}

	auto Toml::GetSubNode(const String& name) const noexcept -> const Toml&
	{
		ASSERT(GetType() == TomlValueType::Table, "Node is not a table");
		const HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		ASSERT(table.Contains(name), "Element does not exist");
		return *table[name];
	}

	auto Toml::TryGetSubNode(usize idx) const noexcept -> const Toml*
	{
		if (GetType() != TomlValueType::Array)
			return nullptr;
		const DynArray<Unique<Toml>>& arr = std::get<DynArray<Unique<Toml>>>(m_value);
		if (idx >= arr.Size())
			return nullptr;
		return arr[idx].Get();
	}

	auto Toml::TryGetSubNode(const String& name) const noexcept -> const Toml*
	{
		if (GetType() != TomlValueType::Table)
			return nullptr;
		const HashMap<String, Unique<Toml>>& table = std::get<HashMap<String, Unique<Toml>>>(m_value);
		auto it = table.Find(name);
		if (it == table.End())
			return nullptr;
		return it->second.Get();
	}

	auto Toml::ArraySize() const noexcept -> usize
	{
		if (GetType() != TomlValueType::Array)
			return 0;
		return std::get<DynArray<Unique<Toml>>>(m_value).Size();
	}

	auto Toml::ContainsKey(const String& name) const noexcept -> bool
	{
		if (GetType() != TomlValueType::Table)
			return 0;
		return std::get<HashMap<String, Unique<Toml>>>(m_value).Contains(name);
	}

	auto Toml::ParseFromFile(const FileSystem::Path& path) noexcept -> Toml
	{
		if (path.GetExtension() != "toml")
			g_Logger.Warning(TOML, "Parsing toml from file with other extension: {}", path.GetExtension());

		Result<FileSystem::File, SystemError> fileRes = FileSystem::File::Open(path);
		if (fileRes.Failed())
		{
			g_Logger.Error(TOML, "Failed to open toml file '{}': {}", path, fileRes.Error().info);
			return Toml{};
		}

		FileSystem::File file = fileRes.MoveValue();
		Result<String, SystemError> readRes = file.ReadString();
		file.Close();

		if (readRes.Failed())
		{
			g_Logger.Error(TOML, "Failed to read toml file '{}': {}", path, readRes.Error().info);
			return Toml{};
		}

		Detail::TomlParser parser;
		return parser.ParseFromString(readRes.Value());
	}

	auto Toml::ParseFromString(const String& content) noexcept -> Toml
	{
		Detail::TomlParser parser;
		return parser.ParseFromString(content);
	}

	namespace Detail
	{
		TomlParser::TomlParser() noexcept
			: m_idx(0)
		{
		}

		// TODO: Better error handling (location + recovery)
		auto TomlParser::ParseFromString(const String& content) noexcept -> Toml
		{
			m_idx = 0;
			m_content = content;

			Toml toml{ TomlValueType::Table };
			Toml* pCurNode = &toml;
			while (m_idx < content.Length())
			{
				SkipWhitespaceAndComments();

				// Start of table
				if (content[m_idx] == '[')
				{
					++m_idx;
					DynArray<String> idens = ExtractDottedIdentifiers();
					if (m_idx >= content.Length() || content[m_idx] != ']')
					{
						g_Logger.Error(TOML, "Table identifiers not ended by ']', terminating parsing");
						return toml;
					}
					++m_idx;

					pCurNode = &toml;
					for (usize i = 0; i < idens.Size(); ++i)
					{
						auto [pRef, added] = pCurNode->GetOrAddTableNode(idens[i], TomlValueType::Table);
						if (!added)
						{
							if (i + 1 == idens.Size())
							{
								String tableName = idens[0];
								for (usize j = 1; j < idens.Size(); ++j)
								{
									tableName += '.';
									tableName += idens[j];
								}
								g_Logger.Error(TOML, "Table '{}' was already created", tableName);
							}
							else if (pRef->GetType() != TomlValueType::Table)
							{
								String tableName = idens[0];
								for (usize j = 1; j <= i; ++j)
								{
									tableName += '.';
									tableName += idens[j];
								}
								g_Logger.Error(TOML, "Node '{}' cannot be used as part of a table", tableName);
							}
						}
						pCurNode = pRef;
					}
				}
				// Key
				else
				{
					ExtractAndInsertKeyValuePair(*pCurNode);
				}
			}
			return toml;
		}

		void TomlParser::SkipWhitespaceAndComments() noexcept
		{
			while (m_idx < m_content.Length() && (Unicode::IsWhitespace(m_content[m_idx]) || m_content[m_idx] == '#'))
			{
				if (m_content[m_idx] == '#')
				{
					usize eol = m_content.Find('\n', m_idx);
					m_idx = eol + 1;
				}
				else
				{
					++m_idx;
				}
			}
		}

		auto TomlParser::GetEscapedCharacter() noexcept -> UCodepoint
		{
			ASSERT(m_idx + 1 < m_content.Length(), "Index out of range");

			++m_idx;
			switch (m_content[m_idx])
			{
			case 'b':  ++m_idx; return '\b';
			case 't':  ++m_idx; return '\t';
			case 'n':  ++m_idx; return '\n';
			case 'f':  ++m_idx; return '\f';
			case 'r':  ++m_idx; return '\r';
			case '"':  ++m_idx; return '"';
			case '\\': ++m_idx; return '\\';
			case 'u':
			case 'U':
			{
				usize len = m_content[m_idx] == 'u' ? 4 : 8;
				++m_idx;
				if (m_idx + len > m_content.Length())
					return 0xFFFD;
				Optional<u32> opt = TryParse<u32>(m_content.SubString(m_idx, len));
				m_idx += len;
				return opt ? opt.value() : 0xFFFD;
			}
			default: return 0xFFFD;
			}
		}

		auto TomlParser::ExtractSingleLineString() noexcept -> String
		{
			String res;
			if (m_content[m_idx] == '"')
			{
				++m_idx;
				usize nextEscapeCode = m_content.Find('\'', m_idx);
				usize end = m_content.Find('"', m_idx);

				while (nextEscapeCode < end)
				{
					res.Add(m_content, m_idx, nextEscapeCode - m_idx);
					m_idx = nextEscapeCode;

					res.Add(GetEscapedCharacter());

					nextEscapeCode = m_content.Find('\'', m_idx);

					if (nextEscapeCode > end)
						end = m_content.Find('"', m_idx);
				}

				res.Add(m_content, m_idx, end - m_idx);
				m_idx = end + 1;
			}
			else if (m_content[m_idx] == '\'')
			{
				++m_idx;
				usize end = m_content.Find('\'', m_idx);
				res = m_content.SubString(m_idx, end - m_idx);
				m_idx = end + 1;
			}
			return res;
		}

		auto TomlParser::ExtractMultiLineString() noexcept -> String
		{
			String res;
			if (m_content[m_idx] == '"')
			{
				m_idx += 3;
				usize nextEscapeCode = m_content.Find('\'', m_idx);
				usize end = m_content.Find("\"\"\""_s, m_idx);

				while (nextEscapeCode < end)
				{
					res.Add(m_content, m_idx, nextEscapeCode - m_idx);
					m_idx = nextEscapeCode;

					res.Add(GetEscapedCharacter());

					nextEscapeCode = m_content.Find('\'', m_idx);

					if (nextEscapeCode > end)
						end = m_content.Find("\"\"\""_s, m_idx);
				}

				res.Add(m_content, m_idx, end - m_idx);
				m_idx = end + 1;
			}
			else if (m_content[m_idx] == '\'')
			{
				++m_idx;
				usize end = m_content.Find("'''"_s, m_idx);
				res = m_content.SubString(m_idx, end - m_idx);
				m_idx = end + 1;
			}
			return res;
		}

		auto TomlParser::ExtractValueString() noexcept -> String
		{
			if (m_content.StartsWith("\"\"\""_s) || m_content.StartsWith("'''"))
				return ExtractMultiLineString();
			else
				return ExtractSingleLineString();
		}

		auto TomlParser::ExtractIdentifier() noexcept -> String
		{
			String res;
			if (m_content[m_idx] == '"' || m_content[m_idx] == '\'')
			{
				res = ExtractSingleLineString();
			}
			else
			{
				usize special = m_content.FindFirstOf(".]=", m_idx);
				// TODO: All unicode whitespace
				usize whitespaceIdx = m_content.FindFirstOf(" \t\r\n", m_idx);

				usize end = Math::Min(special, whitespaceIdx);
				res = m_content.SubString(m_idx, end - m_idx);
				m_idx = end;
			}
			return res;
		}

		auto TomlParser::ExtractDottedIdentifiers() noexcept -> DynArray<String>
		{
			DynArray<String> idens;
			while (true)
			{
				SkipWhitespaceAndComments();
				idens.Add(ExtractIdentifier());
				SkipWhitespaceAndComments();
				if (m_idx < m_content.Length() && m_content[m_idx] == '.')
					++m_idx;
				else
					return idens;
			}
		}

		void TomlParser::ExtractAndInsertKeyValuePair(Toml& curNode) noexcept
		{
			DynArray<String> idens = ExtractDottedIdentifiers();
			if (m_idx >= m_content.Length() || m_content[m_idx] != '=')
			{
				g_Logger.Error(TOML, "Expected '='");
				return;
			}
			++m_idx;
			SkipWhitespaceAndComments();

			if (m_idx + 1 >= m_content.Length())
			{
				g_Logger.Error(TOML, "Unexpected end of file");
				return;
			}

			Toml* pCurNode = &curNode;
			for (usize i = 0; i < idens.Size() - 1; ++i)
			{
				auto [pRef, added] = pCurNode->GetOrAddTableNode(idens[i], TomlValueType::Table);
				if (!added)
				{
					String tableName = idens[0];
					for (usize j = 1; j <= i; ++j)
					{
						tableName += '.';
						tableName += idens[j];
					}
					g_Logger.Error(TOML, "Node '{}' cannot be used as part of a table", tableName);
				}
				pCurNode = pRef;
			}

			auto [ref, added] = pCurNode->GetOrAddTableNode(idens.Back(), TomlValueType::None);
			if (!added)
			{
				String tableName = idens[0];
				for (usize i = 1; i < idens.Size(); ++i)
				{
					tableName += '.';
					tableName += idens[i];
				}
				g_Logger.Error(TOML, "Node '{}' was already created", tableName);
			}

			*ref = ExtractValue();
		}

		auto TomlParser::ExtractValue() noexcept -> Toml
		{
			UCodepoint uc = m_content[m_idx];
			switch (uc)
			{
			case '"':
			case '\'':
			{
				return Toml{ ExtractValueString() };
			}
			case '[':
			{
				return ExtractArray();
			}
			case '{':
			{
				return ExtractInlineTable();
			}
			default:
			{
				constexpr TomlValueType varIdxToType[] =
				{
					TomlValueType::Integer,
					TomlValueType::Float,
					TomlValueType::Bool
				};

				Variant<i64, f64, bool> val = ExtractBasicValue();

				switch (varIdxToType[val.index()])
				{
				case TomlValueType::Integer:
					return Toml{ std::get<i64>(val) };
				case TomlValueType::Float:
					return Toml{ std::get<f64>(val) };
				case TomlValueType::Bool:
					return Toml{ std::get<bool>(val) };
				default:
					// UNREACHABLE
					return Toml{};
				}
			}
			}
		}

		auto TomlParser::ExtractBasicValue() noexcept -> Variant<i64, f64, bool>
		{
			String boolSubStr = m_content.SubString(m_idx, 5);
			if (boolSubStr == "false")
			{
				m_idx += 5;
				return false;
			}
			if (boolSubStr.StartsWith("true"))
			{
				m_idx += 4;
				return true;
			}

			// Currently only a number would be possible
			// TODO: All whitespace
			usize numEnd = m_content.FindFirstOf(" \t\r\n"_s, m_idx);
			String numberStr = m_content.SubString(m_idx, numEnd - m_idx);

			// Is it an integer of float
			// TODO: ContainsAny
			// . for fraction, e for exponential, and n/N for nan and inf
			usize fpSpecificIdx = numberStr.FindFirstOf(".enN");
			if (fpSpecificIdx != String::NPos)
			{
				Optional<f64> opt = TryParse<f64>(numberStr);
				if (!opt)
					g_Logger.Error(TOML, "Invalid float literal, setting default value");
				return opt.value_or(0.0);
			}

			Optional<i64> opt = TryParse<i64>(numberStr);
			if (!opt)
				g_Logger.Error(TOML, "Invalid float literal, setting default value");
			return opt.value_or(0);
		}

		auto TomlParser::ExtractArray() noexcept -> Toml
		{
			ASSERT(m_content[m_idx] == '[', "Invalid array parse");
			++m_idx;
			SkipWhitespaceAndComments();

			Toml arrNode{ TomlValueType::Array };
			if (m_idx >= m_content.Length() || m_content[m_idx] == ']')
				return arrNode;

			do
			{
				SkipWhitespaceAndComments();
				arrNode.AddToArray(ExtractValue());
				SkipWhitespaceAndComments();
			} while (m_idx < m_content.Length() && m_content[m_idx] == ',');

			if (m_idx >= m_content.Length() || m_content[m_idx] != ']')
			{
				g_Logger.Error(TOML, "Unexpeced end to the array");
				return arrNode;
			}

			++m_idx;
			return arrNode;
		}

		auto TomlParser::ExtractInlineTable() noexcept -> Toml
		{
			ASSERT(m_content[m_idx] == '{', "Invalid array parse");
			++m_idx;
			SkipWhitespaceAndComments();

			Toml table{ TomlValueType::Table };
			if (m_idx >= m_content.Length() || m_content[m_idx] == ']')
				return table;

			do
			{
				SkipWhitespaceAndComments();
				ExtractAndInsertKeyValuePair(table);
				SkipWhitespaceAndComments();
			} while (m_idx < m_content.Length() && m_content[m_idx] == ',');

			if (m_idx >= m_content.Length() || m_content[m_idx] != '}')
			{
				g_Logger.Error(TOML, "Unexpeced end to the array");
				return table;
			}

			++m_idx;
			return table;
		}
	}
}
