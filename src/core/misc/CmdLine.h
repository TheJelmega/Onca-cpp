#pragma once
#include "core/MinInclude.h"
#include "core/containers/DynArray.h"
#include "core/containers/HashMap.h"
#include "core/string/Include.h"

namespace Core
{
	namespace Detail::LogCategories
	{
		DEFINE_LOG_CATEGORY(CMDLINE);
	}

	/**
	 * Command line parser
	 * - Short names should start with a single '-'
	 * - Long names should start with a double '--'
	 *
	 * The following styles can be used to pass value to the cmdline parser
	 * - '-a': Set single flag
	 * - '-abc': Set multiple flags
	 * - '-v value': Short option
	 * - '-vvalue': Short option without space
	 * - '--val': Long flag
	 * - '--val value': Long option
	 * - '--val=value': Long option with equals
	 */
	class CmdLine
	{
	private:

		struct ValueBase
		{
			virtual auto Parse(const String& str) noexcept -> bool { return false; }
			virtual ~ValueBase() {}
		};

		template<TryParsable T>
		struct Value : public ValueBase
		{
			Value(T& val);

			auto Parse(const String& str) noexcept -> bool override;

			T* pVal;
		};

		template<TryParsable T>
		struct ValueCallback : public ValueBase
		{
			ValueCallback(Delegate<void(T&&)>& callback);

			auto Parse(const String& str) noexcept -> bool override;

			Delegate<void(T&&)> callback;
		};

		struct Flag
		{
			DynArray<String> names;
			String desc;
			bool isSet = false;
			Delegate<void()> callback;
		};

		struct Option
		{
			DynArray<String> names;
			String desc;
			Unique<ValueBase> value;
		};

		struct Positional
		{
			String desc;
			Unique<ValueBase> value;
		};


		static constexpr u32 MinHelpNameWidth = 32;

	public:
		/**
		 * Create a new command line parser
		 * \param[in] helpHeader Header for help command
		 */
		explicit CmdLine(const String& helpHeader = ""_s) noexcept;

		/**
		 * Add an option to the command line option
		 * \tparam T Type of the associated value
		 * \param[in] names Named for the option, separated by commas
		 * \param[in] value Associated value
		 * \param[in] desc Option description
		 */
		template<TryParsable T >
		auto AddOption(const String& names, T& value, const String& desc = ""_s) noexcept -> void;
		/**
		 * Add an option to the command line option
		 * \tparam T Type of the associated value
		 * \param[in] names Named for the option, separated by commas
		 * \param[in] callback Associated value
		 * \param[in] desc Option description
		 */
		template<TryParsable T >
		auto AddOptionCallback(const String& names, const Delegate<void(T&&)>& callback, const String& desc = ""_s) noexcept -> void;

		/**
		 * Add a positional argument
		 * \tparam T Type of the associated value
		 * \param[in] value Associated value
		 * \param[in] desc Description
		 */
		template<TryParsable T>
		auto AddPositional(T& value, const String& desc = ""_s) noexcept -> void;

		/**
		 * Add a flag to the command line options
		 * \param[in] names Names for the option, separated by commas
		 * \param[in] desc Description
		 */
		auto AddFlag(const String& names, const String& desc = ""_s) noexcept -> void;
		/**
		 * Add a flag callback to the command line options
		 * \param[in] names Names for the option, separated by commas
		 * \param[in] callback Delegate to call when the flag is encountered
		 * \param[in] desc Description
		 */
		auto AddFlagCallback(const String& names, const Delegate<void()>& callback, const String& desc = ""_s) noexcept -> void;

		/**
		 * Check if a flag is set
		 * \param[in] name Name of the flag
		 */
		auto IsFlagSet(const String& name) const noexcept -> bool;


		/**
		 * Print out help for the command line options
		 */
		auto PrintHelp() const noexcept -> void;

		/**
		 * Parse the command line for the application
		 * \param[in] argc 'argc' from the main function
		 * \param[in] argv 'argv' from the main function
		 */
		auto Parse(i32 argc, char* argv[]) noexcept -> void;

	private:
		String m_helpHeader;

		DynArray<Flag> m_flags;
		HashMap<String, u32> m_flagNames;

		DynArray<Option> m_options;
		HashMap<String, u32> m_optionNames;

		DynArray<Positional> m_positionals;

	};
}

#include "CmdLine.inl"