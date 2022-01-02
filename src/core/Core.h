#pragma once

// MinInclude
#include "Config.h"
#include "Defines.h"
#include "Concepts.h"

#include "Assert.h"

#include "utils/Meta.h"
#include "utils/Endianess.h"
#include "utils/Atomic.h"
#include "utils/Tuple.h"
#include "utils/Pair.h"
#include "utils/Utils.h"
#include "utils/Algo.h"
#include "utils/Flags.h"
#include "utils/Delegate.h"
#include "utils/Result.h"

#include "hash/Hash.h"

#include "intrin/Pack.h"
#include "intrin/BitIntrin.h"

#include "memory/MemUtils.h"
#include "memory/MemRef.h"
#include "memory/Unique.h"
#include "memory/RefCounted.h"

#include "platform/SystemInfo.h"
#include "threading/Threading.h"

#include "allocator/IAllocator.h"
#include "allocator/primitives/Mallocator.h"
#include "allocator/primitives/LinearAllocator.h"
#include "allocator/primitives/StackAllocator.h"
#include "allocator/primitives/PoolAllocator.h"
#include "allocator/primitives/BitmapAllocator.h"
#include "allocator/primitives/BuddyAllocator.h"
#include "allocator/primitives/FreeListAllocator.h"
#include "allocator/composable/ExpandableArena.h"
#include "allocator/composable/FallbackArena.h"
#include "allocator/composable/SegregatorArena.h"

#include "containers/Containers.h"

#include "string/Include.h"

#include "math/Math.h"

#include "filesystem/FileSystem.h"
#include "platform/Console.h"
#include "logging/Logger.h"
#include "misc/CmdLine.h"