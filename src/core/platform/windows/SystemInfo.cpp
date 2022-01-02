#include "../SystemInfo.h"

#include "core/logging/Logger.h"

#if PLATFORM_WINDOWS
#include "core/platform/Platform.h"

namespace Core
{
	auto SystemInfo::Init() noexcept -> void
	{
		SYSTEM_INFO sysInfo;
		::GetNativeSystemInfo(&sysInfo);

		switch (sysInfo.wProcessorArchitecture)
		{
		case PROCESSOR_ARCHITECTURE_AMD64: m_processorArchitecture = ProcessorArch::X86_64;  break;
		case PROCESSOR_ARCHITECTURE_ARM64: m_processorArchitecture = ProcessorArch::ARM64;   break;
		default:                           m_processorArchitecture = ProcessorArch::Unknown; break;
		}

		m_pageSize            = sysInfo.dwPageSize;
		m_appMemoryStart      = sysInfo.lpMinimumApplicationAddress;
		m_appMemoryEnd        = sysInfo.lpMaximumApplicationAddress;
		m_virtAllocGranulariy = sysInfo.dwAllocationGranularity;

		DWORD len = 0;
		::GetLogicalProcessorInformationEx(RelationProcessorPackage, nullptr, &len);
		DWORD err = ::GetLastError();
		if (err != ERROR_INSUFFICIENT_BUFFER)
		{
			g_Logger.Error(LogCategories::SYSINFO, "Could not retrieve processor info");
			return;
		}

		DynArray<u8> data;
		data.Resize(len);
		bool res =::GetLogicalProcessorInformationEx(RelationProcessorPackage, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(data.Data()), &len);
		if (!res)
		{
			g_Logger.Error(LogCategories::SYSINFO, "Could not retrieve processor info");
			return;
		}

		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX pProcessorInfo = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(data.Data());
		m_processorInfo.Resize(len / pProcessorInfo->Size);
		for (usize i = 0; i < m_processorInfo.Size(); ++i)
		{
			m_processorInfo[i].groups.Resize(pProcessorInfo->Processor.GroupCount);
			for (usize j = 0; j < pProcessorInfo->Processor.GroupCount; ++j)
			{
				const GROUP_AFFINITY& affinity = pProcessorInfo->Processor.GroupMask[i];
				m_processorInfo[i].groups[j].groupIdx = affinity.Group;
				m_processorInfo[i].groups[j].coreCount = Intrin::BitScanMSB(affinity.Mask) + 1;
				m_processorInfo[i].groups[j].mask = affinity.Mask;

				m_processorInfo[i].numLogicalCores += m_processorInfo[i].groups[j].coreCount;
			}

			pProcessorInfo = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(reinterpret_cast<u8*>(pProcessorInfo) + pProcessorInfo->Size);
		}

		len = 0;
		::GetLogicalProcessorInformationEx(RelationProcessorCore, nullptr, &len);
		err = ::GetLastError();
		if (err != ERROR_INSUFFICIENT_BUFFER)
		{
			g_Logger.Error(LogCategories::SYSINFO, "Could not retrieve core info");
			return;
		}
		
		data.Resize(len);
		res = ::GetLogicalProcessorInformationEx(RelationProcessorCore, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(data.Data()), &len);
		if (!res)
		{
			g_Logger.Error(LogCategories::SYSINFO, "Could not retrieve core info");
			return;
		}

		pProcessorInfo = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(data.Data());

		bool hasCoresWithSMT = false;
		for (usize i = 0; i < len;)
		{
			if (pProcessorInfo->Processor.Flags & LTP_PC_SMT)
			{
				hasCoresWithSMT = true;
				break;
			}

			i += pProcessorInfo->Size;
			pProcessorInfo = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(data.Data() + i);
		}

		pProcessorInfo = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(data.Data());
		for (usize i = 0; i < len;)
		{
			CoreInfo core;
			core.supportSMT = pProcessorInfo->Processor.Flags & LTP_PC_SMT;
			core.mask = pProcessorInfo->Processor.GroupMask[0].Mask;
			core.numThreads = Intrin::PopCnt(core.mask);

			for (usize j = 0; j < m_processorInfo.Size(); ++j)
			{
				bool exit = false;
				for (const ProcessorGroup& group : m_processorInfo[j].groups)
				{
					if (group.groupIdx != pProcessorInfo->Processor.GroupMask[0].Group)
						continue;

					m_processorInfo[j].cores.Add(core);
					exit = true;
				}

				if (exit)
					break;
			}

			i += pProcessorInfo->Size;
			pProcessorInfo = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(data.Data() + i);
		}

		len = 0;
		::GetLogicalProcessorInformationEx(RelationCache, nullptr, &len);
		err = ::GetLastError();
		if (err != ERROR_INSUFFICIENT_BUFFER)
		{
			g_Logger.Error(LogCategories::SYSINFO, "Could not retrieve cache info");
			return;
		}
		
		data.Resize(len);
		res = ::GetLogicalProcessorInformationEx(RelationCache, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(data.Data()), &len);
		if (res)
		{
			pProcessorInfo = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(data.Data());

			for (usize i = 0; i < len;)
			{
				CacheInfo cache;
				cache.level = CacheLevel(pProcessorInfo->Cache.Level - 1);
				cache.associativity = pProcessorInfo->Cache.Associativity;
				cache.kind = CacheKind(pProcessorInfo->Cache.Type);
				cache.lineSize = pProcessorInfo->Cache.LineSize;
				cache.cacheSize = pProcessorInfo->Cache.CacheSize;

				for (usize j = 0; j < pProcessorInfo->Cache.GroupCount; ++j)
				{
					cache.coreInfo.EmplaceBack(pProcessorInfo->Cache.GroupMasks[j].Group, pProcessorInfo->Cache.GroupMasks[j].Mask);
				}

				for (usize j = 0; j < m_processorInfo.Size(); ++j)
				{
					bool exit = false;
					for (const ProcessorGroup& group : m_processorInfo[j].groups)
					{
						if (group.groupIdx != cache.coreInfo[0].first)
							continue;

						m_processorInfo[j].caches[pProcessorInfo->Cache.Level - 1].Add(cache);
						exit = true;
					}

					if (exit)
						break;
				}

				i += pProcessorInfo->Size;
				pProcessorInfo = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(data.Data() + i);
			}
		}
		else
		{
			g_Logger.Error(LogCategories::SYSINFO, "Could not retrieve cache info");
			return;
		}

		len = 0;
		::GetLogicalProcessorInformationEx(RelationNumaNode, nullptr, &len);
		err = ::GetLastError();
		if (err != ERROR_INSUFFICIENT_BUFFER)
		{
			g_Logger.Error(LogCategories::SYSINFO, "Could not retrieve NUMA info");
			return;
		}
		
		data.Resize(len);
		res = ::GetLogicalProcessorInformationEx(RelationNumaNode, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(data.Data()), &len);
		if (!res)
		{
			g_Logger.Error(LogCategories::SYSINFO, "Could not retrieve NUMA info");
			return;
		}

		pProcessorInfo = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(data.Data());
		for (usize i = 0; i < len;)
		{
			NUMANodeInfo numa;
			numa.nodeId = pProcessorInfo->NumaNode.NodeNumber;

			for (usize j = 0; j < pProcessorInfo->NumaNode.GroupCount; ++j)
			{
				numa.coreInfo.EmplaceBack(pProcessorInfo->NumaNode.GroupMasks[j].Group, pProcessorInfo->NumaNode.GroupMasks[j].Mask);
			}
			m_numaNodes.Add(numa);

			for (usize j = 0; j < m_processorInfo.Size(); ++j)
			{
				bool exit = false;
				for (const ProcessorGroup& group : m_processorInfo[j].groups)
				{
					if (group.groupIdx != pProcessorInfo->Processor.GroupMask[0].Group)
						continue;

					m_processorInfo[j].numaNodeId = numa.nodeId;
					exit = true;
				}

				if (exit)
					break;
			}

			i += pProcessorInfo->Size;
			pProcessorInfo = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(data.Data() + i);
		}

		HANDLE curProcessHandle = ::GetCurrentProcess();

		ULONG cpuSetLen = 0;
		::GetSystemCpuSetInformation(nullptr, 0, &cpuSetLen, curProcessHandle, 0);
		err = ::GetLastError();
		if (err != ERROR_INSUFFICIENT_BUFFER)
		{
			g_Logger.Error(LogCategories::SYSINFO, "Could not retrieve CPU set info");
			return;
		}

		data.Resize(cpuSetLen);
		res = ::GetSystemCpuSetInformation(reinterpret_cast<PSYSTEM_CPU_SET_INFORMATION>(data.Data()), cpuSetLen, &cpuSetLen, curProcessHandle, 0);
		if (!res)
		{
			g_Logger.Error(LogCategories::SYSINFO, "Could not retrieve CPU set info");
			return;
		}

		PSYSTEM_CPU_SET_INFORMATION pCpuSetInfo = reinterpret_cast<PSYSTEM_CPU_SET_INFORMATION>(data.Data());
		for (usize i = 0; i < cpuSetLen;)
		{
			CPUSetInfo cpuSet;
			cpuSet.id = pCpuSetInfo->CpuSet.Id;
			cpuSet.group = pCpuSetInfo->CpuSet.Group;
			cpuSet.core = pCpuSetInfo->CpuSet.LogicalProcessorIndex;
			cpuSet.physicalCore = pCpuSetInfo->CpuSet.CoreIndex;
			cpuSet.lastLvlCacheIdx = pCpuSetInfo->CpuSet.LastLevelCacheIndex;

			for (usize j = 0; j < m_processorInfo.Size(); ++j)
			{
				bool exit = false;
				for (const ProcessorGroup& group : m_processorInfo[j].groups)
				{
					if (group.groupIdx != pProcessorInfo->Processor.GroupMask[0].Group)
						continue;

					ProcessorInfo& info = m_processorInfo[j];
					info.cpuSets.Add(cpuSet);

					if (info.physicalCoreIds.IsEmpty() ||
						info.physicalCoreIds.Back() != cpuSet.physicalCore)
						info.physicalCoreIds.Add(cpuSet.physicalCore);

					bool found = false;
					for (DynArray<CPUSetInfo>& sharedCache : info.cpuSetsSharedCache)
					{
						if (sharedCache[0].lastLvlCacheIdx != cpuSet.lastLvlCacheIdx)
							continue;

						sharedCache.Add(cpuSet);
						found = true;
					}

					if (!found)
					{
						info.cpuSetsSharedCache.Add(DynArray<CPUSetInfo>{});
						info.cpuSetsSharedCache.Back().Add(cpuSet);
					}

					exit = true;
				}

				if (exit)
					break;
			}

			i += pCpuSetInfo->Size;
			pCpuSetInfo = reinterpret_cast<PSYSTEM_CPU_SET_INFORMATION>(data.Data() + i);
		}

		DynArray<char16_t> utf16;
		len = 0;
		res =::GetComputerNameExW(ComputerNameDnsDomain, nullptr, &len);
		if (!res && ::GetLastError() == ERROR_MORE_DATA)
		{
			utf16.Resize(len);
			res = ::GetComputerNameExW(ComputerNameDnsDomain, reinterpret_cast<LPWSTR>(utf16.Data()), &len);
			if (res)
				m_identifiableInfo.dnsDomainName.Assign(utf16.Data());
			else
				g_Logger.Info(LogCategories::SYSINFO, "Failed to get DNS domain name");
		}

		len = 0;
		res = ::GetComputerNameExW(ComputerNameDnsFullyQualified, nullptr, &len);
		if (!res && ::GetLastError() == ERROR_MORE_DATA)
		{
			utf16.Resize(len);
			res = ::GetComputerNameExW(ComputerNameDnsFullyQualified, reinterpret_cast<LPWSTR>(utf16.Data()), &len);
			if (res)
				m_identifiableInfo.dnsFullyQualifiedName.Assign(utf16.Data());
			else
				g_Logger.Info(LogCategories::SYSINFO, "Failed to get DNS fully qualified name");
		}

		len = 0;
		res = ::GetComputerNameExW(ComputerNameDnsHostname, nullptr, &len);
		if (!res && ::GetLastError() == ERROR_MORE_DATA)
		{
			utf16.Resize(len);
			res = ::GetComputerNameExW(ComputerNameDnsHostname, reinterpret_cast<LPWSTR>(utf16.Data()), &len);
			if (res)
				m_identifiableInfo.dnsFullyQualifiedName.Assign(utf16.Data());
			else
				g_Logger.Info(LogCategories::SYSINFO, "Failed to get DNS host name");
		}

		len = 0;
		res = ::GetComputerNameExW(ComputerNamePhysicalDnsDomain, nullptr, &len);
		if (!res && ::GetLastError() == ERROR_MORE_DATA)
		{
			utf16.Resize(len);
			res = ::GetComputerNameExW(ComputerNamePhysicalDnsDomain, reinterpret_cast<LPWSTR>(utf16.Data()), &len);
			if (res)
				m_identifiableInfo.dnsPhysicalDomainName.Assign(utf16.Data());
			else
				g_Logger.Info(LogCategories::SYSINFO, "Failed to get physical DNS domain name");
		}

		len = 0;
		res = ::GetComputerNameExW(ComputerNamePhysicalDnsFullyQualified, nullptr, &len);
		if (!res && ::GetLastError() == ERROR_MORE_DATA)
		{
			utf16.Resize(len);
			res = ::GetComputerNameExW(ComputerNamePhysicalDnsFullyQualified, reinterpret_cast<LPWSTR>(utf16.Data()), &len);
			if (res)
				m_identifiableInfo.dnsPhysicalFullyQualifiedName.Assign(utf16.Data());
			else
				g_Logger.Info(LogCategories::SYSINFO, "Failed to get physical DNS fully qualified name");
		}

		len = 0;
		res = ::GetComputerNameExW(ComputerNamePhysicalDnsHostname, nullptr, &len);
		if (!res && ::GetLastError() == ERROR_MORE_DATA)
		{
			utf16.Resize(len);
			res = ::GetComputerNameExW(ComputerNamePhysicalDnsHostname, reinterpret_cast<LPWSTR>(utf16.Data()), &len);
			if (res)
				m_identifiableInfo.dnsPhysicalHostName.Assign(utf16.Data());
			else
				g_Logger.Info(LogCategories::SYSINFO, "Failed to get physical DNS host name");
		}

		len = 0;
		res = ::GetComputerNameExW(ComputerNameNetBIOS, nullptr, &len);
		if (!res && ::GetLastError() == ERROR_MORE_DATA)
		{
			utf16.Resize(len);
			res = ::GetComputerNameExW(ComputerNameNetBIOS, reinterpret_cast<LPWSTR>(utf16.Data()), &len);
			if (res)
				m_identifiableInfo.netBIOS.Assign(utf16.Data());
			else
				g_Logger.Info(LogCategories::SYSINFO, "Failed to get NetBIOS name");
		}
		len = 0;
		res = ::GetComputerNameExW(ComputerNamePhysicalNetBIOS, nullptr, &len);
		if (!res && ::GetLastError() == ERROR_MORE_DATA)
		{
			utf16.Resize(len);
			res = ::GetComputerNameExW(ComputerNamePhysicalNetBIOS, reinterpret_cast<LPWSTR>(utf16.Data()), &len);
			if (res)
				m_identifiableInfo.physicalNetBIOS.Assign(utf16.Data());
			else
				g_Logger.Info(LogCategories::SYSINFO, "Failed to get physical NetBIOS name");
		}

		::GetPhysicallyInstalledSystemMemory(reinterpret_cast<PULONGLONG>(&m_installedMemory));
		m_installedMemory *= 1024;

		m_processorFeatures.hasCompareExchange      = ::IsProcessorFeaturePresent(PF_COMPARE_EXCHANGE_DOUBLE);
		m_processorFeatures.hasCompareExchange128   = ::IsProcessorFeaturePresent(PF_COMPARE_EXCHANGE128);
		m_processorFeatures.hasCompare64Exchange128 = ::IsProcessorFeaturePresent(PF_COMPARE64_EXCHANGE128);

		if (m_processorArchitecture == ProcessorArch::ARM64)
		{
			m_processorFeatures.armHas64bitLoadStoreAtomic = ::IsProcessorFeaturePresent(PF_ARM_64BIT_LOADSTORE_ATOMIC);
			m_processorFeatures.armHasDivideInstruction    = ::IsProcessorFeaturePresent(PF_ARM_DIVIDE_INSTRUCTION_AVAILABLE);
			m_processorFeatures.armHasExternalCache        = ::IsProcessorFeaturePresent(PF_ARM_EXTERNAL_CACHE_AVAILABLE);
			m_processorFeatures.armHasFMACInstruction      = ::IsProcessorFeaturePresent(PF_ARM_FMAC_INSTRUCTIONS_AVAILABLE);
			m_processorFeatures.armHasCFP32Registers       = ::IsProcessorFeaturePresent(PF_ARM_VFP_32_REGISTERS_AVAILABLE);
		}


		if (m_processorArchitecture == ProcessorArch::X86_64)
		{
			// CPUID, flags: https://en.wikipedia.org/wiki/CPUID
			i32 reg[4];
			// Query
#if COMPILER_MSVC
			__cpuid(reg, 1);
#endif

			m_processorFeatures.x86HasMMX    = reg[3] & BIT(23);
			m_processorFeatures.x86HasSSE    = reg[3] & BIT(25);
			m_processorFeatures.x86HasSSE2   = reg[3] & BIT(26);
			m_processorFeatures.x86HasSSE3   = reg[2] & BIT(0);
			m_processorFeatures.x86HasSSSE3  = reg[2] & BIT(9);
			m_processorFeatures.x86HasSSE4_1 = reg[2] & BIT(19);
			m_processorFeatures.x86HasSSE4_2 = reg[2] & BIT(20);
			m_processorFeatures.x86HasAVX    = reg[2] & BIT(28);
			m_processorFeatures.x86HasPOPCNT = reg[2] & BIT(23);

			// Query extended features
#if COMPILER_MSVC
			__cpuidex(reg, 7, 0);
#endif

			m_processorFeatures.isHybrid                 = reg[3] & BIT(15);

			m_processorFeatures.x86HasAVX2               = reg[1] & BIT(5);
			m_processorFeatures.x86HasBMI1               = reg[1] & BIT(3);
			m_processorFeatures.x86HasBMI2               = reg[1] & BIT(8);
			m_processorFeatures.x86HasAVX512F            = reg[1] & BIT(16);
			m_processorFeatures.x86HasAVX512DQ           = reg[1] & BIT(17);
			m_processorFeatures.x86HasAVX512IFMA         = reg[1] & BIT(21);
			m_processorFeatures.x86HasAVX512PF           = reg[1] & BIT(26);
			m_processorFeatures.x86HasAVX512ER           = reg[1] & BIT(27);
			m_processorFeatures.x86HasAVX512CD           = reg[1] & BIT(28);
			m_processorFeatures.x86HasAVX512BW           = reg[1] & BIT(30);
			m_processorFeatures.x86HasAVX512VL           = reg[1] & BIT(31);
			m_processorFeatures.x86HasAVX512VBMI         = reg[2] & BIT(1);
			m_processorFeatures.x86HasAVX512VBMI2        = reg[2] & BIT(6);
			m_processorFeatures.x86HasAVX512VNNI         = reg[2] & BIT(11);
			m_processorFeatures.x86HasAVX512BITALG       = reg[2] & BIT(12);
			m_processorFeatures.x86HasAVX512VPOPCNTDQ    = reg[2] & BIT(14);
			m_processorFeatures.x86HasAVX5124VNNIW       = reg[3] & BIT(2);
			m_processorFeatures.x86HasAVX5124FMAPS       = reg[3] & BIT(3);
			m_processorFeatures.x86HasAVX512VP2INTERSECT = reg[3] & BIT(8);
			m_processorFeatures.x86HasAVX512FP16         = reg[3] & BIT(23);

#if COMPILER_MSVC
			__cpuidex(reg, 7, 1);
#endif

			m_processorFeatures.x86HasAVX512BF16         = reg[0] & BIT(5);

			__cpuid(reg, 0);
			m_manufacturer = String{ reinterpret_cast<const char*>(&reg[1]), 4 } +
						     String{ reinterpret_cast<const char*>(&reg[3]), 4 } + 
						     String{ reinterpret_cast<const char*>(&reg[2]), 4 };

			// TODO: https://www.intel.com/content/www/us/en/developer/articles/guide/alder-lake-developer-guide.html#inpage-nav-1-5-2
#if 0
			if (m_processorFeatures.isHybrid)
			{
#if COMPILER_MSVC
				__cpuid(reg, 0x1A);
#endif

			}
#endif


		}

	}
}

#endif