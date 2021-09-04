#pragma once

#include "Config.h"
#include "Defines.h"
#include "Concepts.h"
#include "Endianess.h"
#include "Assert.h"
#include "Atomic.h"

#include "utils/Utils.h"
#include "utils/Pair.h"
#include "utils/Algo.h"

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
#include "containers/Deque.h"
#include "containers/List.h"
#include "containers/IntrusiveList.h"
#include "containers/DList.h"
#include "containers/IntrusiveDList.h"
#include "containers/HashMap.h"
#include "containers/HashSet.h"

namespace Core
{
	template<Movable K, Movable V, Hasher<K> H, Comparator<K> C>
	using HashMultiMap = HashMap<K, V, H, C, true>;

	template<Movable K, Hasher<K> H, Comparator<K> C>
	using HashMultiSet = HashSet<K, H, C, true>;
}

#include "math/MathUtils.h"