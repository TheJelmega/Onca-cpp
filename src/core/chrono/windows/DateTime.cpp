#include "../DateTime.h"
#if PLATFORM_WINDOWS
#include "core/platform/Platform.h"

namespace Onca::Chrono
{
	auto DateTime::Now() noexcept -> DateTime
	{
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);

		DateTime dt;
		dt.year        = u16(sysTime.wYear);
		dt.month       = u8 (sysTime.wMonth);
		dt.day         = u8 (sysTime.wDay);
		dt.hour        = u8 (sysTime.wHour);
		dt.minute      = u8 (sysTime.wMinute);
		dt.second      = u8 (sysTime.wSecond);
		dt.millisecond = u16(sysTime.wMilliseconds);
		return dt;
	}
}

#endif