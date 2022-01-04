#include "SystemInfo.h"

#include "core/logging/Logger.h"

namespace Core
{
	auto SystemInfo::Shutdown() noexcept -> void
	{
		m_processorInfo.Clear(true);
		m_numaNodes.Clear(true);
		m_identifiableInfo.dnsDomainName.Clear(true);
		m_identifiableInfo.dnsFullyQualifiedName.Clear(true);
		m_identifiableInfo.dnsHostName.Clear(true);
		m_identifiableInfo.dnsPhysicalDomainName.Clear(true);
		m_identifiableInfo.dnsPhysicalFullyQualifiedName.Clear(true);
		m_identifiableInfo.dnsPhysicalHostName.Clear(true);
		m_identifiableInfo.netBIOS.Clear(true);
		m_identifiableInfo.physicalNetBIOS.Clear(true);
		m_manufacturer.Clear(true);
	}

	auto SystemInfo::GetIdentifiableData() const noexcept -> const IdentifiableInfo&
	{
		return m_identifiableInfo;
	}

	auto SystemInfo::GetProcessorCount() const noexcept -> usize
	{
		return m_processorInfo.Size();
	}

	auto SystemInfo::GetProcessorArchitecture() const noexcept -> ProcessorArch
	{
		return m_processorArchitecture;
	}

	auto SystemInfo::GetPageSize() const noexcept -> usize
	{
		return m_pageSize;
	}

	auto SystemInfo::GetApplicationMemoryBounds() const noexcept -> Pair<const void*, const void*>
	{
		return { m_appMemoryStart, m_appMemoryEnd };
	}

	auto SystemInfo::GetApplicationMemorySize() const noexcept -> usize
	{
		return usize(m_appMemoryEnd) - usize(m_appMemoryStart);
	}

	auto SystemInfo::GetVirtualAllocGranularity() const noexcept -> usize
	{
		return m_virtAllocGranulariy;
	}

	auto SystemInfo::GetInstalledMemory() const noexcept -> u64
	{
		return m_installedMemory;
	}

	auto SystemInfo::GetManufacturer() const noexcept -> const String&
	{
		return m_manufacturer;
	}

	auto SystemInfo::GetNUMANodeCount() const noexcept -> u32
	{
		return u32(m_numaNodes.Size());
	}

	auto SystemInfo::GetNumaNodes() const noexcept -> const DynArray<NUMANodeInfo>&
	{
		return m_numaNodes;
	}

	auto SystemInfo::GetNumaNodeId(u32 processor) const noexcept -> u32
	{
		if (processor >= m_processorInfo.Size())
			return u32(-1);
		return m_processorInfo[processor].numaNodeId;
	}

	auto SystemInfo::GetPhysicalCoreCount(u32 processor) const noexcept -> u32
	{
		if (processor >= m_processorInfo.Size())
			return 0;
		return u32(m_processorInfo[processor].cores.Size());
	}

	auto SystemInfo::GetLogicalCoreCount(u32 processor) const noexcept -> u32
	{
		if (processor >= m_processorInfo.Size())
			return 0;
		return m_processorInfo[processor].numLogicalCores;
	}

	auto SystemInfo::GetProcessorGroupCount(u32 processor) const noexcept -> u32
	{
		if (processor >= m_processorInfo.Size())
			return 0;
		return u32(m_processorInfo[processor].groups.Size());
	}

	auto SystemInfo::GetCoreInfo(u32 core, u32 processor) const noexcept -> const CoreInfo&
	{
		static CoreInfo empty;
		if (processor >= m_processorInfo.Size())
			return empty;

		if (core >= m_processorInfo[processor].cores.Size())
			return empty;

		return m_processorInfo[processor].cores[core];
	}

	auto SystemInfo::GetCacheInfo(u32 processor) const noexcept -> DynArray<CacheInfo>
	{
		if (processor >= m_processorInfo.Size())
			return DynArray<CacheInfo>{};

		DynArray<CacheInfo> cacheInfos;
		for (usize i = 0; i < usize(CacheLevel::Count); ++i)
		{
			cacheInfos.Add(m_processorInfo[processor].caches[i]);
		}
		return cacheInfos;
	}

	auto SystemInfo::GetCacheInfo(CacheLevel level, u32 processor) const noexcept -> DynArray<CacheInfo>
	{
		ASSERT(u8(level) < u8(CacheLevel::Count), "Invalid cache level");
		if (processor >= m_processorInfo.Size())
			return DynArray<CacheInfo>{};

		return m_processorInfo[processor].caches[u8(level)];
	}

	auto SystemInfo::GetCoresPerLastLevelCache(u32 processor) const noexcept -> const DynArray<DynArray<CPUSetInfo>>&
	{
		static DynArray<DynArray<CPUSetInfo>> empty;
		if (processor >= m_processorInfo.Size())
			return empty;

		return m_processorInfo[processor].cpuSetsSharedCache;
	}

	auto SystemInfo::GetPhysicalCoreId(u32 core, u32 processor) const noexcept -> u32
	{
		if (processor >= m_processorInfo.Size())
			return u32(-1);

		if (core >= m_processorInfo[processor].physicalCoreIds.Size())
			return u32(-1);
		return u32(m_processorInfo[processor].physicalCoreIds[core]);
	}

	auto SystemInfo::GetCpuSetIdForCore(u32 core, u32 processor) const noexcept -> u32
	{
		if(processor >= m_processorInfo.Size())
			return u32(-1);

		if (core >= m_processorInfo[processor].cpuSets.Size())
			return u32(-1);

		return m_processorInfo[processor].cpuSets[core].id;
	}

	auto SystemInfo::GetFirstCpuSetIdForPhysicalCore(u32 core, u32 processor) const noexcept -> u32
	{
		if (processor >= m_processorInfo.Size())
			return u32(-1);

		if (core >= m_processorInfo[processor].physicalCoreIds.Size())
			return u32(-1);

		usize physicalCoreId = m_processorInfo[processor].physicalCoreIds[core];
		for (const CPUSetInfo& cpuSet : m_processorInfo[processor].cpuSets)
		{
			if (cpuSet.physicalCore == physicalCoreId)
				return cpuSet.id;
		}

		return u32(-1);
	}

	auto SystemInfo::GetCpuSetIdsForPhysicalCore(u32 core, u32 processor) const noexcept -> DynArray<u32>
	{
		if (processor >= m_processorInfo.Size())
			return DynArray<u32>{};

		if (core >= m_processorInfo[processor].physicalCoreIds.Size())
			return DynArray<u32>{};

		usize physicalCoreId = m_processorInfo[processor].physicalCoreIds[core];
		DynArray<u32> ids;
		for (const CPUSetInfo& cpuSet : m_processorInfo[processor].cpuSets)
		{
			if (cpuSet.physicalCore == physicalCoreId)
				ids.Add(cpuSet.id);
			else if (!ids.IsEmpty())
				return ids;
		}
		return ids;
	}

	auto SystemInfo::GetLogicalCoresForCpuSetIds(const DynArray<u32>& ids) const noexcept -> DynArray<DynArray<u32>>
	{
		DynArray<DynArray<u32>> arrs;
		arrs.Resize(m_processorInfo.Size(), DynArray<u32>{});

		usize curIdx = 0;
		for (usize i = 0; i < m_processorInfo.Size(); ++i)
		{
			for (const CPUSetInfo& set : m_processorInfo[i].cpuSets)
			{
				for (u32 id : ids)
				{
					if (id != set.id)
						continue;

					arrs[i].Add(GroupRelativeToCoreIndex(set.group, set.core));
					++curIdx;
					if (curIdx == ids.Size())
						return arrs;
					break;
				}
			}
		}
		return arrs;
	}

	auto SystemInfo::GetPhysicalCoresForCpuSetIds(const DynArray<u32>& ids) const noexcept -> DynArray<DynArray<u32>>
	{
		DynArray<DynArray<u32>> arrs;
		arrs.Resize(m_processorInfo.Size(), DynArray<u32>{});

		usize curIdx = 0;
		for (usize i = 0; i < m_processorInfo.Size(); ++i)
		{
			for (const CPUSetInfo& set : m_processorInfo[i].cpuSets)
			{
				for (u32 id : ids)
				{
					if (id != set.id)
						continue;

					arrs[i].Add(set.physicalCore);
					++curIdx;
					if (curIdx == ids.Size())
						return arrs;
					break;
				}
			}
		}
		return arrs;
	}

	auto SystemInfo::CoreIndexToGroupRelative(u32 core, u32 processor) const noexcept -> Pair<u32, u32>
	{
		if (processor >= m_processorInfo.Size())
			return { u32(-1), u32(-1) };

		const ProcessorInfo& info = m_processorInfo[processor];
		for (usize i = 0; i < info.groups.Size(); ++i)
		{
			if (core < info.groups[i].coreCount)
				return { info.groups[i].groupIdx, core };
			core -= info.groups[i].coreCount;
		}
		return { u32(-1), u32(-1) };
	}

	auto SystemInfo::GroupRelativeToCoreIndex(usize group, u32 core) const noexcept -> u32
	{
		u32 processor = u32(-1);
		for (u32 i = 0; i < m_processorInfo.Size(); ++i)
		{
			const ProcessorInfo& info = m_processorInfo[i];
			for (const ProcessorGroup& groupInfo : info.groups)
			{
				if (group == groupInfo.groupIdx)
				{
					processor = i;
					break;
				}
			}
			if (processor != u32(-1))
				break;
		}
		return GroupRelativeToCoreIndex(group, core, processor);
	}

	auto SystemInfo::GroupRelativeToCoreIndex(usize group, u32 core, u32 processor) const noexcept -> u32
	{
		if (processor >= m_processorInfo.Size())
			return u32(-1);

		const ProcessorInfo& info = m_processorInfo[processor];
		for (usize i = 0; i < info.groups.Size(); ++i)
		{
			if (info.groups[i].groupIdx == group)
				return core;
			core += info.groups[i].coreCount;
		}
		return u32(-1);
	}

	auto SystemInfo::AreCoresOnSameLastLevelCache(usize core0, usize core1, u32 processor) const noexcept -> bool
	{
		if (processor >= m_processorInfo.Size())
			return false;

		const ProcessorInfo& info = m_processorInfo[processor];
		if (core0 >= info.cpuSets.Size() || core1 >= info.cpuSets.Size())
			return false;

		return info.cpuSets[core0].lastLvlCacheIdx == info.cpuSets[core1].lastLvlCacheIdx;
	}

	auto SystemInfo::GetProcessorFeatures() const noexcept -> const ProcessorFeatures&
	{
		return m_processorFeatures;
	}

	auto SystemInfo::LogSystemInfo(bool logIdentifiableInfo) const noexcept -> void
	{
		g_Logger.Info(LogCategories::SYSINFO, "System info:"_s);

		if (logIdentifiableInfo)
		{
			g_Logger.Append("================ INDENTIFIABLE INFO START ================"_s);

			g_Logger.Append("DNS domain name:                   {}"_s, m_identifiableInfo.dnsDomainName);
			g_Logger.Append("DNS fully qualified name:          {}"_s, m_identifiableInfo.dnsFullyQualifiedName);
			g_Logger.Append("DNS host name:                     {}"_s, m_identifiableInfo.dnsHostName);

			g_Logger.Append("Physical DNS domain name:          {}"_s, m_identifiableInfo.dnsPhysicalDomainName);
			g_Logger.Append("Physical DNS fully qualified name: {}"_s, m_identifiableInfo.dnsPhysicalFullyQualifiedName);
			g_Logger.Append("Physical DNS host name:            {}"_s, m_identifiableInfo.dnsPhysicalHostName);

			g_Logger.Append("Physical NetBIOS name:             {}"_s, m_identifiableInfo.netBIOS);
			g_Logger.Append("NetBIOS name:                      {}"_s, m_identifiableInfo.physicalNetBIOS);

			g_Logger.Append("================ INDENTIFIABLE INFO END   ================"_s);
		}

		g_Logger.Append("Page size        : {}"_s, m_pageSize);
		g_Logger.Append("Memory start     : {:X}"_s, usize(m_appMemoryStart));
		g_Logger.Append("Memory end       : {:X}"_s, usize(m_appMemoryEnd));
		g_Logger.Append("Memory size      : {}"_s, usize(m_appMemoryEnd) - usize(m_appMemoryStart));
		g_Logger.Append("Alloc granularity: {}"_s, m_virtAllocGranulariy);
		g_Logger.Append("Installed memory : {}MiB"_s, m_installedMemory / MB(1));

		String cpuArch = m_processorArchitecture == ProcessorArch::X86_64 ? "x86-64"_s :
			m_processorArchitecture == ProcessorArch::ARM64 ? "ARM64"_s :
			"Unknown";
		g_Logger.Append("CPU architecture:  {}"_s, cpuArch);
		g_Logger.Append("CPU manufacturer:  {}"_s, m_manufacturer);


		g_Logger.Append("Number of NUMA nodes: {}"_s, m_numaNodes.Size());
		for (usize i = 0; i < m_numaNodes.Size(); ++i)
		{
			const NUMANodeInfo& numa = m_numaNodes[i];

			String line = Format("NUMA_node[{}]: Node id: {}"_s, i, numa.nodeId);
			for (Pair<usize, u64> coreInfo : numa.coreInfo)
			{
				line += Format(" [group={} mask={:X}]"_s, coreInfo.first, coreInfo.second);
			}
			g_Logger.Append(line);
		}

		g_Logger.Append("Number of processors: {}"_s, m_processorInfo.Size());
		for (usize i = 0; i < m_processorInfo.Size(); ++i)
		{
			const ProcessorInfo& info = m_processorInfo[i];

			g_Logger.Append("CPU[{}]: Physical cores: {}"_s, i, info.cores.Size());
			g_Logger.Append("CPU[{}]: Logical cores:  {}"_s, i, info.numLogicalCores);
			g_Logger.Append("CPU[{}]: HUMA node id:   {}"_s, i, info.numaNodeId);

			g_Logger.Append("CPU[{}]: Processor groups: {}"_s, i, info.groups.Size());
			for (usize j = 0; j < info.groups.Size(); ++j)
			{
				const ProcessorGroup& group = info.groups[j];
				g_Logger.Append("CPU[{}].Group[{}]: idx: {}, logical cores: {}, mask={:X}"_s, i, j, group.groupIdx, group.coreCount, group.mask);
			}

			//g_Logger.Append("CPU[{}]: ")
			for (usize j = 0; j < info.cores.Size(); ++j)
			{
				const CoreInfo& core = info.cores[j];

				String SMTinfo = core.supportSMT ? Format("SMT ({} thread{})"_s, core.numThreads, core.numThreads > 1 ? "s"_s : ""_s) : "No SMT"_s;

				g_Logger.Append("CPU[{}].Core[{}]{}: {}, {} [group={} mask={:X}]"_s, i, j, j < 10 ? " "_s : ""_s,
								core.efficiency == EfficiencyClass::Performance ? "performance"_s : "efficiency "_s,
								SMTinfo,
								core.groupIdx,
								core.mask);
			}

			for (usize j = 0; j < usize(CacheLevel::Count); ++j)
			{
				for (usize k = 0; k < info.caches[j].Size(); ++k)
				{
					const CacheInfo& cache = info.caches[j][k];
					String level = CacheLevel(j) == CacheLevel::L1 ? "L1"_s :
						CacheLevel(j) == CacheLevel::L2 ? "L2"_s :
						"L3"_s;

					String kind = cache.kind == CacheKind::Unified ? "unified    "_s :
						cache.kind == CacheKind::Instruction ? "instruction"_s :
						cache.kind == CacheKind::Data ? "data       "_s :
						"trace      "_s;

					String associativity = cache.associativity == u8(-1) ? "   fully associative"_s :
						Format("{,-3}-way associative"_s, cache.associativity);

					String line = Format("CPU[{}].Cache[{}][{}]{}:  {}{} line_size={,-3}B cache_size={,-9}B"_s, i, level, k,
										 k < 10 ? " "_s : ""_s,
										 kind,
										 associativity,
										 cache.lineSize,
										 cache.cacheSize);

					for (Pair<usize, u64> coreInfo : cache.coreInfo)
					{
						line += Format(" [group={} mask={:X}]"_s, coreInfo.first, coreInfo.second);
					}
					g_Logger.Append(line);
				}
			}

			for (usize j = 0; j < info.cpuSets.Size(); ++j)
			{
				const CPUSetInfo& cpuSet = info.cpuSets[j];
				g_Logger.Append("CPU[{}].CPUSet[{}]{}: id={} group={} logical_core={,2} physical_core={,2} last_level_cache_index={}",
								i, j, j < 10 ? " "_s : ""_s, cpuSet.id, cpuSet.group, cpuSet.core, cpuSet.physicalCore, cpuSet.lastLvlCacheIdx);
			}
		}

		String yes = "YES"_s;
		String no = "NO"_s;

		g_Logger.Append("Flags: ");
		g_Logger.Append("Is hybrid:                       {}"_s, m_processorFeatures.isHybrid           ? yes : no);
		g_Logger.Append("Compare exchange:                {}"_s, m_processorFeatures.hasCompareExchange ? yes : no);
		g_Logger.Append("128-bit ompare exchange:         {}"_s, m_processorFeatures.hasCompareExchange ? yes : no);
		g_Logger.Append("64-bit compare 128-bit exchange: {}"_s, m_processorFeatures.hasCompareExchange ? yes : no);

		if (m_processorArchitecture == ProcessorArch::X86_64)
		{
			g_Logger.Append("MMX:                             {}"_s, m_processorFeatures.x86HasMMX                ? yes : no);
			g_Logger.Append("SSE:                             {}"_s, m_processorFeatures.x86HasSSE                ? yes : no);
			g_Logger.Append("SSE2:                            {}"_s, m_processorFeatures.x86HasSSE2               ? yes : no);
			g_Logger.Append("SSE3:                            {}"_s, m_processorFeatures.x86HasSSE3               ? yes : no);
			g_Logger.Append("SSSE3:                           {}"_s, m_processorFeatures.x86HasSSSE3              ? yes : no);
			g_Logger.Append("SSE4.1:                          {}"_s, m_processorFeatures.x86HasSSE4_1             ? yes : no);
			g_Logger.Append("SSE4.2:                          {}"_s, m_processorFeatures.x86HasSSE4_2             ? yes : no);
			g_Logger.Append("AVX:                             {}"_s, m_processorFeatures.x86HasAVX                ? yes : no);
			g_Logger.Append("AVX2:                            {}"_s, m_processorFeatures.x86HasAVX2               ? yes : no);
			g_Logger.Append("AVX512F:                         {}"_s, m_processorFeatures.x86HasAVX512F            ? yes : no);
			g_Logger.Append("AVX512DQ:                        {}"_s, m_processorFeatures.x86HasAVX512DQ           ? yes : no);
			g_Logger.Append("AVX512IFMA:                      {}"_s, m_processorFeatures.x86HasAVX512IFMA         ? yes : no);
			g_Logger.Append("AVX512PF:                        {}"_s, m_processorFeatures.x86HasAVX512PF           ? yes : no);
			g_Logger.Append("AVX512ER:                        {}"_s, m_processorFeatures.x86HasAVX512ER           ? yes : no);
			g_Logger.Append("AVX512CD:                        {}"_s, m_processorFeatures.x86HasAVX512CD           ? yes : no);
			g_Logger.Append("AVX512BW:                        {}"_s, m_processorFeatures.x86HasAVX512BW           ? yes : no);
			g_Logger.Append("AVX512VL:                        {}"_s, m_processorFeatures.x86HasAVX512VL           ? yes : no);
			g_Logger.Append("AVX512VBMI:                      {}"_s, m_processorFeatures.x86HasAVX512VBMI         ? yes : no);
			g_Logger.Append("AVX512VBMI2:                     {}"_s, m_processorFeatures.x86HasAVX512VBMI2        ? yes : no);
			g_Logger.Append("AVX512VNNI:                      {}"_s, m_processorFeatures.x86HasAVX512VNNI         ? yes : no);
			g_Logger.Append("AVX512BITALG:                    {}"_s, m_processorFeatures.x86HasAVX512BITALG       ? yes : no);
			g_Logger.Append("AVX512VPOPCNTDQ:                 {}"_s, m_processorFeatures.x86HasAVX512VPOPCNTDQ    ? yes : no);
			g_Logger.Append("AVX512_4VNNIW:                   {}"_s, m_processorFeatures.x86HasAVX5124VNNIW       ? yes : no);
			g_Logger.Append("AVX512_4FMAPS:                   {}"_s, m_processorFeatures.x86HasAVX5124FMAPS       ? yes : no);
			g_Logger.Append("AVX512VPINTERSECT:               {}"_s, m_processorFeatures.x86HasAVX512VP2INTERSECT ? yes : no);
			g_Logger.Append("AVX512FP16:                      {}"_s, m_processorFeatures.x86HasAVX512FP16         ? yes : no);
			g_Logger.Append("AVX512BF16:                      {}"_s, m_processorFeatures.x86HasAVX512BF16         ? yes : no);
			g_Logger.Append("POPCNT:                          {}"_s, m_processorFeatures.x86HasPOPCNT             ? yes : no);
			g_Logger.Append("BMI1:                            {}"_s, m_processorFeatures.x86HasBMI1               ? yes : no);
			g_Logger.Append("BMI2:                            {}"_s, m_processorFeatures.x86HasBMI2               ? yes : no);
			g_Logger.Append("AMXBF16:                         {}"_s, m_processorFeatures.x86HasAMXBF16            ? yes : no);
			g_Logger.Append("AMXTILE:                         {}"_s, m_processorFeatures.x86HasAMXTILE            ? yes : no);
			g_Logger.Append("AMXINT8:                         {}"_s, m_processorFeatures.x86HasAMXINT8            ? yes : no);
		}
		else if (m_processorArchitecture == ProcessorArch::ARM64)
		{
			g_Logger.Append(" 64-bit load/store atomic:       {}"_s, m_processorFeatures.armHas64bitLoadStoreAtomic ? yes : no);
			g_Logger.Append("Divide instruction:              {}"_s, m_processorFeatures.armHasDivideInstruction    ? yes : no);
			g_Logger.Append("External cache:                  {}"_s, m_processorFeatures.armHasExternalCache        ? yes : no);
			g_Logger.Append("FMAC instruction:                {}"_s, m_processorFeatures.armHasFMACInstruction      ? yes : no);
			g_Logger.Append("CFP32 registers:                 {}"_s, m_processorFeatures.armHasCFP32Registers       ? yes : no);
		}
	}

	auto GetSystemInfo() noexcept -> SystemInfo&
	{
		static SystemInfo info;
		return info;
	}
}
