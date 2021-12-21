#include "DateTime.h"

namespace Core::Chrono
{
	auto DateTime::ToString() const noexcept -> String
	{
		String res{ 22 };
		res.PadRight((year < 1000) + (year < 100) + (year < 10), '0');
		res += Core::ToString(year);

		res += '-';
		
		res.PadRight(month < 10, '0');
		res += Core::ToString(month);

		res += '-';

		res.PadRight(day < 10, '0');
		res += Core::ToString(day);

		res += ' ';

		res.PadRight(hour < 10, '0');
		res += Core::ToString(hour);

		res += ':';

		res.PadRight(minute < 10, '0');
		res += Core::ToString(minute);

		res += ':';

		res.PadRight(second < 10, '0');
		res += Core::ToString(second);

		res += '.';
		
		res.PadRight((millisecond < 100) + (millisecond < 10), '0');
		res += Core::ToString(millisecond);

		return res;
	}

	auto DateTime::ToFormat(const String& options) const noexcept -> String
	{
		if (options.IsEmpty())
			return ToString();

		String res{ options.Length() };
		for (usize i = 0, len = options.Length(); i < len; ++i)
		{
			switch (options[i])
			{
			case 'y':
			{
				if (i + 1 < len && options[i + 1] == 'y')
				{
					if (i + 3 < len && options[i + 2] == 'y' && options[i + 3] == 'y')
					{
						res.PadRight((year < 1000) + (year < 100) + (year < 10), '0');
						res += Core::ToString(year);
						i += 3;
					}
					else
					{
						u16 tmp = year % 100;
						res.PadRight((tmp < 10), '0');
						res += Core::ToString(tmp);
						++i;
					}
				}
				else
				{
					res += Core::ToString(year % 100);
				}
				break;
			}
			case 'M':
			{
				if (i + 1 < len && options[i + 1] == 'M')
				{
					res.PadRight((month < 10), '0');
					res += Core::ToString(month);
					++i;
				}
				else
				{
					res += Core::ToString(month);
				}
				break;
			}
			case 'd':
			{
				if (i + 1 < len && options[i + 1] == 'd')
				{
					res.PadRight((day < 10), '0');
					res += Core::ToString(day);
					++i;
				}
				else
				{
					res += Core::ToString(day);
				}
				break;
			}
			case 'h':
			{
				u8 tmp = hour > 12 ? hour - 12 : hour;
				if (i + 1 < len && options[i + 1] == 'h')
				{
					res.PadRight((tmp < 10), '0');
					res += Core::ToString(tmp);
					++i;
				}
				else
				{
					res += Core::ToString(tmp);
				}
				break;
			}
			case 'H':
			{
				if (i + 1 < len && options[i + 1] == 'H')
				{
					res.PadRight((hour < 10), '0');
					res += Core::ToString(hour);
					++i;
				}
				else
				{
					res += Core::ToString(hour);
				}
				break;
			}
			case 'm':
			{
				if (i + 1 < len && options[i + 1] == 'd')
				{
					res.PadRight((minute < 10), '0');
					res += Core::ToString(minute);
					++i;
				}
				else
				{
					res += Core::ToString(minute);
				}
				break;
			}
			case 's':
			{
				if (i + 1 < len && options[i + 1] == 'd')
				{
					res.PadRight((day < 10), '0');
					res += Core::ToString(day);
					++i;
				}
				else
				{
					res += Core::ToString(day);
				}
				break;
			}
			case 'u':
			{
				if (i + 1 < len && options[i + 1] == 'u' && options[i + 2] == 'u')
				{
					res.PadRight((millisecond < 100) + (millisecond < 10), '0');
					res += Core::ToString(millisecond);
					i += 2;
				}
				else
				{
					res += Core::ToString(millisecond);
				}
				break;
			}
			case 't':
			{
				if (i + 1 < len && options[i + 1] == 't')
				{
					res += hour < 12 ? "AM"_s : "PM"_s;
					++i;
				}
				else
				{
					res += hour < 12 ? "A"_s : "P"_s;
				}

				break;
			}
			case '\\':
			{
				if (i + 1 < len)
				{
					res += options[i + 1];
					++i;
				}
				break;
			}
			default:
			{
				res += options[i];
				break;
			}
			}
		}
		return res;
	}
}
