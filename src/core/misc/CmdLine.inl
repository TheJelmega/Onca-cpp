#pragma once
#if __RESHARPER__
#include "CmdLine.h"
#endif

#include "core/logging/Logger.h"

namespace Core
{
	template <TryParsable T>
	CmdLine::Value<T>::Value(T& val)
		: pVal(&val)
	{
	}

	template <TryParsable T>
	bool CmdLine::Value<T>::Parse(const String& str) noexcept
	{
		if (Optional<T> res = TryParse<T>(str))
		{
			*pVal = Move(res.value());
			return true;
		}
		return false;
	}

	template <TryParsable T>
	CmdLine::ValueCallback<T>::ValueCallback(Delegate<void(T&&)>& callback)
		: callback(callback)
	{
	}

	template <TryParsable T>
	bool CmdLine::ValueCallback<T>::Parse(const String& str) noexcept
	{
		if (Optional<T> res = TryParse<T>(str))
		{
			callback(Move(res.value()));
			return true;
		} 
		return false;
	}

	inline CmdLine::CmdLine(const String& helpHeader) noexcept
		: m_helpHeader(helpHeader)
	{
	}

	template <TryParsable T>
	void CmdLine::AddOption(const String& names, T& value, const String& desc) noexcept
	{
		Option option;
		option.desc = desc;
		option.names = names.Split(',');
		option.value = Unique<Value<T>>::Create(value);

		u32 idx = u32(m_options.Size());
		for (const String& name : option.names)
		{
			m_optionNames.Emplace(name, idx);
		}
		m_options.Add(Move(option));

	}

	template <TryParsable T>
	void CmdLine::AddOptionCallback(const String& names, const Delegate<void(T&&)>& callback, const String& desc) noexcept
	{
		Option option;
		option.desc = desc;
		option.names = names.Split(',');
		option.value = Unique<ValueCallback<T>>::Create(callback);

		u32 idx = u32(m_options.Size());
		for (const String& name : option.names)
		{
			m_optionNames.Emplace(name, idx);
		}
		m_options.Add(Move(option));
	}

	template <TryParsable T>
	void CmdLine::AddPositional(T& value, const String& desc) noexcept
	{
		Positional positional;
		positional.desc = desc;
		positional.value = Unique<Value<T>>::Create(value);
		m_positionals.Add(Move(positional));
	}

	inline void CmdLine::AddFlag(const String& names, const String& desc) noexcept
	{
		Flag flag;
		flag.names = names.Split(',');
		flag.desc = desc;

		u32 idx = u32(m_flags.Size());
		for (const String& name : flag.names)
		{
			m_flagNames.Emplace(name, idx);
		}
		m_flags.Add(Move(flag));
	}

	inline void CmdLine::AddFlagCallback(const String& names, const Delegate<void()>& callback, const String& desc) noexcept
	{
		Flag flag;
		flag.names = names.Split(',');
		flag.desc = desc;
		flag.callback = callback;

		u32 idx = u32(m_flags.Size());
		for (const String& name : flag.names)
		{
			m_flagNames.Emplace(name, idx);
		}
		m_flags.Add(Move(flag));
	}

	inline auto CmdLine::IsFlagSet(const String& name) const noexcept -> bool
	{
		auto it = m_flagNames.Find(name);
		if (it == m_flagNames.End())
			return false;
		return m_flags[it->second].isSet;
	}

	inline void CmdLine::PrintHelp() const noexcept
	{
		g_Logger.Info(Detail::LogCategories::CMDLINE, m_helpHeader.IsEmpty() ? "Command line help"_s : m_helpHeader);
		g_Logger.Append("Options:"_s);

		for (const Flag& flag : m_flags)
		{
			String line;
			for (usize i = 0; i < flag.names.Size(); ++i)
			{
				if (i != 0)
					line += ", "_s;
				line += flag.names[i];
			}
			line += "    "_s;

			if (line.Length() < MinHelpNameWidth)
				line.PadRight(MinHelpNameWidth - line.Length());

			line += flag.desc;
			g_Logger.Append(line);
		}

		for (const Option& opt : m_options)
		{
			String line;
			for (usize i = 0; i < opt.names.Size(); ++i)
			{
				if (i != 0)
					line += ", "_s;
				line += opt.names[i] + " <val>"_s;
			}

			if (line.Length() < MinHelpNameWidth)
				line.PadRight(MinHelpNameWidth - line.Length());

			line += opt.desc;
			g_Logger.Append(line);
		}

		for (const Positional& pos : m_positionals)
		{
			String line = "<positional>";
			if (line.Length() < MinHelpNameWidth)
				line.PadRight(MinHelpNameWidth - line.Length());

			line += pos.desc;
			g_Logger.Append(line);
		}
	}

	inline void CmdLine::Parse(i32 argc, char* argv[]) noexcept
	{
		// Reuse string to minimize allocations
		String line;
		String tmp;
		usize positionalIdx = 0;

		for (usize i = 1; i < argc; ++i)
		{
			if (!argv[i])
				return;
			if (argv[i][1] == '\0')
				continue;

			line = argv[i];
			tmp.Clear();

			if (line[0] == '-')
			{
				if (line.Length() == 1)
				{
					g_Logger.Warning(LogCategories::CORE, "CmdLine: No option name available", line);
					continue;
				}

				if (line[1] == '-')
				{
					// long name
					line.Erase(0, 2);

					if (line == "help"_s)
					{
						PrintHelp();
						return;
					}

					g_Logger.Verbose(LogCategories::CORE, "CmdLine: Handling '{}'", line);

					usize equalsPos = line.Find('=');
					if (equalsPos != String::NPos)
					{
						// Should be an option
						tmp = line.SubString(equalsPos + 1);
						line.Erase(equalsPos);
					}

					auto it = m_flagNames.Find(line);
					if (it != m_flagNames.End())
					{
						Flag& flag = m_flags[it->second];
						flag.isSet = true;
						continue;
					}

					it = m_optionNames.Find(line);
					if (it != m_optionNames.End())
					{
						Option& opt = m_options[it->second];

						if (tmp.IsEmpty())
						{
							++i;
							opt.value->Parse(String{ argv[i] });
						}
						else
						{
							opt.value->Parse(tmp);
						}
					}
				}
				else
				{
					// short name
					line.Erase(0, 1);

					if (line.Contains('?'))
					{
						PrintHelp();
						return;
					}

					for (usize j = 0; j < line.Length(); ++j)
					{
						UCodepoint cp = line[j];

						tmp.Assign(cp, 1);
						auto it = m_flagNames.Find(tmp);
						if (it != m_flagNames.End())
						{
							Flag& flag = m_flags[it->second];
							flag.isSet = true;
							continue;
						}

						bool foundVal = false;
						it = m_optionNames.Find(tmp);
						if (it != m_optionNames.End())
						{
							Option& opt = m_options[it->second];

							if (j == line.Length() - 1)
								opt.value->Parse(String{ argv[i++] });
							else
								opt.value->Parse(line.SubString(j + 1));

							foundVal = true;
						}

						if (foundVal)
							break;
					}
				}
			}
			else if (positionalIdx < m_positionals.Size()) // positional
			{
				Positional& positional = m_positionals[positionalIdx++];      
				positional.value->Parse(line);
			}
		}
	}
}
