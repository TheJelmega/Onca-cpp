#pragma once
#include "core/Config.h"

#if PLATFORM_WINDOWS
#	include "windows/Win.h"
#else
#	error No implementation for platform
#endif