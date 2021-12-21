#pragma once
#include "core/MinInclude.h"
#include "core/string/Include.h"

namespace Core::Chrono
{
	struct CORE_API DateTime
	{

		u16 year;
		u8 month;
		u8 day;
		u8 hour;
		u8 minute;
		u8 second;
		u16 millisecond;

		/**
		 * Get the default formatted string
		 * \return String
		 */
		auto ToString() const noexcept -> String;
		/**
		 * Format the DateTime based on the following options:
		 * The format for the consists of identifier and separators
		 * If the option is left empty, the default formatting will be used
		 *
		 * identifiers:
		 * - "y"   : year, from 0 to 99
		 * - "yy"  : year, from 0 to 99. always write 2 digit
		 * - "yyyy": full year, minimum of 4 digits
		 * - "M"   : month
		 * - "MM"  : month, always write 2 digits
		 * - "d"   : Day of the month
		 * - "dd"  : Day of the month, always write 2 digits
		 * - "h"   : hour, using a 12h clock
		 * - "hh"  : hour, using a 12h clock, always write 2 digits
		 * - "H"   : hour, using a 24h clock
		 * - "HH"  : hour, using a 24h clock, always write 2 digits
		 * - "m"   : minute
		 * - "mm"  : minute, always write 2 digits
		 * - "s"   : second
		 * - "ss"  : second. always write 2 digits
		 * - "u"   : millisecond
		 * - "uuu" : millisecond. always write 3 digits
		 * - "t"   : first character of AM/PM designator
		 * - "tt"  : AM/PM designator
		 *
		 * separators: any other character or escaped character that can be found in an identifier
		 *
		 *
		 * \param[in] options Format options
		 * \return Formatted string
		 */
		auto ToFormat(const String& options) const noexcept -> String;

		static auto Now() noexcept -> DateTime;

	};
}
