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

#include "intrin/Pack.h"
#include "intrin/BitIntrin.h"

#include "memory/MemUtils.h"
#include "memory/MemRef.h"
#include "memory/Unique.h"

#include "threading/Sync.h"

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

#include "containers/Array.h"
#include "containers/DynArray.h"
#include "containers/InplaceDynArray.h"
#include "containers/Deque.h"
#include "containers/List.h"
#include "containers/IntrusiveList.h"
#include "containers/DList.h"
#include "containers/IntrusiveDList.h"
#include "containers/HashMap.h"
#include "containers/HashSet.h"
#include "containers/RedBlackTree.h"
#include "containers/SortedSet.h"

#include "string/StringUtils.h"
#include "string/String.h"
#include "string/ConstString.h"

#include "math/Math.h"