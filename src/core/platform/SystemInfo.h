#pragma once
#include "core/MinInclude.h"
#include "core/containers/DynArray.h"
#include "core/logging/LogCategory.h"

namespace LogCategories
{
	DEFINE_LOG_CATEGORY(SYSINFO);
}

namespace Core
{
	class CORE_API SystemInfo
	{
	public:
		
		/**
		 * The processor is split into groups of 64-cores with u64 core masks
		 */
		struct ProcessorGroup
		{
			u64 mask      = 0; ///< Mask of active cores
			u32 groupIdx  = 0; ///< Index of the group
			u8  coreCount = 0; ///< Number of cores in the group
		};

		/**
		 * Efficiency class o a core
		 */
		enum class EfficiencyClass : u8
		{
			Performance, ///< Performance optimized core
			Efficiency   ///< Efficiency optimized core
		};

		/**
		 * Info about each physical core
		 */
		struct CoreInfo
		{
			bool            supportSMT = false;                        ///< Whether the core support simultaneous multi threading
			EfficiencyClass efficiency = EfficiencyClass::Performance; ///< Efficiency class of a core
			u8              numThreads = 0;                            ///< Number of threads
			u64             mask       = 0;                            ///< Thread mask for core
			usize           groupIdx   = 0;                            ///< Processor group index
		};

		/**
		 * Cache level
		 */
		enum class CacheLevel : u8
		{
			L1   , ///< Level 1 cache
			L2   , ///< Level 2 cache
			L3   , ///< Level 3 cache
			Count, ///< Number of cache levels
		};

		/**
		 * Cache kind
		 */
		enum class CacheKind : u8
		{
			Unified    , ///< Unified cache
			Instruction, ///< Instruction cache
			Data       , ///< Data cache
			Trace      , ///< Trace cache
		};

		/**
		 * Info about cache
		 */
		struct CacheInfo
		{
			CacheLevel level                    = CacheLevel::L1;     ///< Cache level
			CacheKind  kind                     = CacheKind::Unified; ///< Cache kind
			u8         associativity            = false;              ///< Cache associativity, u8(-1) is fully associative
			u16        lineSize                 = 0;                  ///< Cache line size
			u32        cacheSize                = 0;                  ///< Cache size
			DynArray<Pair<usize, u64>> coreInfo;                      ///< Stores group id an associated mask
		};

		struct NUMANodeInfo
		{
			u32                        nodeId   = 0; ///< NUMA node id
			DynArray<Pair<usize, u64>> coreInfo;     ///< Stores group id an associated mask
		};

		struct CPUSetInfo
		{
			u32 id;              ///< CPU set id
			u32 group;           ///< Group
			u32 core;            ///< Group relative core index
			u32 physicalCore;    ///< Physical core index
			u32 lastLvlCacheIdx; ///< Index of the last level cache shared with other cores
		};

		struct ProcessorInfo
		{
			ProcessorInfo() noexcept = default;

			u32                                               numLogicalCores = 0;     ///< Number of logical cores
			u32                                               numaNodeId      = 0;     ///< Id of the numa node containing this processor
			bool                                              isHybrid        = false; ///< Whether the processor is a hybrid processor
			DynArray<CoreInfo>                                cores;                   ///< Core info
			DynArray<ProcessorGroup>                          groups;                  ///< Group info
			DynArray<CPUSetInfo>                              cpuSets;                 ///< CPU set info
			DynArray<DynArray<CPUSetInfo>>                    cpuSetsSharedCache;      ///< CPU set info, separated by shared cache
			DynArray<usize>                                   physicalCoreIds;         ///< IDs of the physical cores
			Array<DynArray<CacheInfo>, u8(CacheLevel::Count)> caches;                  ///< Per level cache info
		};

		struct IdentifiableInfo
		{
			String dnsDomainName;                 ///< Domain name of the DNS
			String dnsFullyQualifiedName;         ///< Fully qualified name of the DNS
			String dnsHostName;                   ///< Host name of the DNS
			String netBIOS;                       ///< NetBIOS name
			String dnsPhysicalDomainName;         ///< Physical domain name of the DNS
			String dnsPhysicalFullyQualifiedName; ///< Physical fully qualified name of the DNS
			String dnsPhysicalHostName;           ///< Physical host name of the DNS
			String physicalNetBIOS;               ///< Physical NetBIOS name
		};

		struct ProcessorFeatures
		{
			bool isHybrid                   : 1 = false; ///< If the processor is a hybrid processor (performance and efficiency cores)
			bool hasCompareExchange         : 1 = false; ///< If the processor support compare exchange
			bool hasCompareExchange128      : 1 = false; ///< If the processor support 128-bit compare exchange
			bool hasCompare64Exchange128    : 1 = false; ///< If the processor support 64-bit compare 128-bit exchange

			// x86/64 only
			bool x86HasMMX                  : 1 = false; ///< If the processor support MMX
			bool x86HasSSE                  : 1 = false; ///< If the processor support SSE
			bool x86HasSSE2                 : 1 = false; ///< If the processor support SSE2
			bool x86HasSSE3                 : 1 = false; ///< If the processor support SSE3
			bool x86HasSSSE3                : 1 = false; ///< If the processor support SSSE3
			bool x86HasSSE4_1               : 1 = false; ///< If the processor support SSE4.1
			bool x86HasSSE4_2               : 1 = false; ///< If the processor support SSE4.2
			bool x86HasAVX                  : 1 = false; ///< If the processor support AVX
			bool x86HasAVX2                 : 1 = false; ///< If the processor support AVX2
			bool x86HasAVX512F              : 1 = false; ///< If the processor support AVX512F
			bool x86HasAVX512DQ             : 1 = false; ///< If the processor support AVX512DQ
			bool x86HasAVX512IFMA           : 1 = false; ///< If the processor support AVX512IFMA
			bool x86HasAVX512PF             : 1 = false; ///< If the processor support AVX512PF
			bool x86HasAVX512ER             : 1 = false; ///< If the processor support AVX512ER
			bool x86HasAVX512CD             : 1 = false; ///< If the processor support AVX512CD
			bool x86HasAVX512BW             : 1 = false; ///< If the processor support AVX512BW
			bool x86HasAVX512VL             : 1 = false; ///< If the processor support AVX512VL
			bool x86HasAVX512VBMI           : 1 = false; ///< If the processor support AVX512VBMI
			bool x86HasAVX512VBMI2          : 1 = false; ///< If the processor support AVX512VBMI2
			bool x86HasAVX512VNNI           : 1 = false; ///< If the processor support AVX512VNNI
			bool x86HasAVX512BITALG         : 1 = false; ///< If the processor support AVX512BITALG
			bool x86HasAVX512VPOPCNTDQ      : 1 = false; ///< If the processor support AVX512VPOPCNTDQ
			bool x86HasAVX5124VNNIW         : 1 = false; ///< If the processor support AVX5124VNNIW
			bool x86HasAVX5124FMAPS         : 1 = false; ///< If the processor support AVX5124FMAPS
			bool x86HasAVX512VP2INTERSECT   : 1 = false; ///< If the processor support AVX512VP2INTERSECT
			bool x86HasAVX512FP16           : 1 = false; ///< If the processor support AVX512FP16
			bool x86HasAVX512BF16           : 1 = false; ///< If the processor support AVX512FP16
			bool x86HasPOPCNT               : 1 = false; ///< If the processor support POPCNT
			bool x86HasBMI1                 : 1 = false; ///< If the processor support BMI1
			bool x86HasBMI2                 : 1 = false; ///< If the processor support BMI2
			bool x86HasAMXBF16              : 1 = false; ///< If the processor support AMXBF16
			bool x86HasAMXTILE              : 1 = false; ///< If the processor support AMXTILE
			bool x86HasAMXINT8              : 1 = false; ///< If the processor support AMXINT8

			// ARM only
			bool armHas64bitLoadStoreAtomic : 1 = false; ///< If the processor support 64-bit load/store atomic instructions
			bool armHasDivideInstruction    : 1 = false; ///< If the ARM processor support the divide instruction
			bool armHasExternalCache        : 1 = false; ///< If the ARM processor support the external cache available
			bool armHasFMACInstruction      : 1 = false; ///< If the ARM processor support the FMAC instruction
			bool armHasCFP32Registers       : 1 = false; ///< If the ARM processor has a VFP/Neon 32 x 64bit register back present

		};

		/**
		 * Supported CPU architectures
		 */
		enum class ProcessorArch
		{
			X86_64, ///< x86-64
			ARM64, ///< ARM64
			Unknown, ///< Unknown
		};

	public:

		/**
		 * Initialize and collect the system info
		 */
		auto Init() noexcept -> void;
		/**
		 * Cleanup the system info before allocator destruction
		 */
		auto Shutdown() noexcept -> void;

		/**
		 * Get the identifiable data
		 * \return Identifiable data
		 */
		auto GetIdentifiableData() const noexcept -> const IdentifiableInfo&;
		/**
		 * Get the number of processors
		 * \return Number of processors
		 */
		auto GetProcessorCount() const noexcept -> usize;
		/**
		 * Get the processor architecture
		 * \return Processor architecture
		 */
		auto GetProcessorArchitecture() const noexcept -> ProcessorArch;
		/**
		 * Get the page size
		 * \return Page size
		 */
		auto GetPageSize() const noexcept -> usize;
		/**
		 * Get the application memory bounds
		 * \return Application memory bounds
		 */
		auto GetApplicationMemoryBounds() const noexcept -> Pair<const void*, const void*>;
		/**
		 * Get the application memory size
		 * \return Application memory size
		 */
		auto GetApplicationMemorySize() const noexcept -> usize;
		/**
		 * Get the virtual memory allocator granularity
		 * \return Virtual memory allocator granularity
		 */
		auto GetVirtualAllocGranularity() const noexcept -> usize;
		/**
		 * Get the size of memory that is installed in the system
		 * \return Size of memory that is installed in the system
		 */
		auto GetInstalledMemory() const noexcept -> u64;
		/**
		 * Get the identifier of the processor manufacturer
		 * \return Identifier of the processor manufacturer
		 */
		auto GetManufacturer() const noexcept -> const String&;
		/**
		 * Get the number of NUMA nodes for the given processor
		 * \return Number of NUMA nodes
		 */
		auto GetNUMANodeCount() const noexcept -> u32;
		/**
		 * Get the number of NUMA nodes for the given processor
		 * \return Number of NUMA nodes
		 */
		auto GetNumaNodes() const noexcept -> const DynArray<NUMANodeInfo>&;
		/**
		 * Get the NUMA node id for a given processor
		 * \param[in] processor Processor index
		 * \return NUMA node id
		 */
		auto GetNumaNodeId(u32 processor) const noexcept -> u32;
		/**
		 * Get the number of physical cores
		 * \param[in] processor Processor index
		 * \return Number of physical cores
		 */
		auto GetPhysicalCoreCount(u32 processor = 0) const noexcept -> u32;
		/**
		 * Get the number of logical cores
		 * \param[in] processor Processor index
		 * \return Number of logical cores
		 */
		auto GetLogicalCoreCount(u32 processor = 0) const noexcept -> u32;
		/**
		 * Get the number of windows processor groups for the given processor
		 * \param[in] processor Processor index
		 * \return Number of windows processor groups
		 */
		auto GetProcessorGroupCount(u32 processor = 0) const noexcept -> u32;
		/**
		 * Get the core info for a specific core
		 * \param[in] core Core index
		 * \param[in] processor Processor index
		 * \return Core info
		 */
		auto GetCoreInfo(u32 core, u32 processor = 0) const noexcept -> const CoreInfo&;
		/**
		 * Get cache info for a given processor
		 * \param[in] processor Processor index
		 * \return Cache info
		 */
		auto GetCacheInfo(u32 processor) const noexcept -> DynArray<CacheInfo>;
		/**
		 * Get cache info for a given processor and cache level
		 * \param[in] level Cache level
		 * \param[in] processor Processor index
		 * \return Cache info
		 */
		auto GetCacheInfo(CacheLevel level, u32 processor = 0) const noexcept -> DynArray<CacheInfo>;
		/**
		 * Get the CPU set info for cores per last level cache
		 * \param[in] processor Processor index
		 * \return CPU set info for cores per last level cache
		 */
		auto GetCoresPerLastLevelCache(u32 processor = 0) const noexcept -> const DynArray<DynArray<CPUSetInfo>>&;

		/**
		 * Get the physical core id for a given processor and physical core
		 * \param[in] core Physical core index
		 * \param[in] processor Processor index
		 * \return Physical core ID
		 */
		auto GetPhysicalCoreId(u32 core, u32 processor = 0) const noexcept -> u32;
		/**
		 * Get the CPU set id for a given processor and core
		 * \param[in] core Core index
		 * \param[in] processor Processor index
		 * \return CPU set id
		 */
		auto GetCpuSetIdForCore(u32 core, u32 processor = 0) const noexcept -> u32;
		/**
		 * Get the first CPU set id for a given processor and physical core
		 * \param[in] core Core index
		 * \param[in] processor Processor index
		 * \return First CPU set id for the core
		 */
		auto GetFirstCpuSetIdForPhysicalCore(u32 core, u32 processor = 0) const noexcept -> u32;
		/**
		 * Get the CPU set ids for a given processor and physical core
		 * \param[in] core Core index
		 * \param[in] processor Processor index
		 * \return CPU set ids for the core
		 */
		auto GetCpuSetIdsForPhysicalCore(u32 core, u32 processor = 0) const noexcept -> DynArray<u32>;

		/**
		 * Convert a core index to a group index and a relative core index
		 * \param[in] core Core index
		 * \param[in] processor Processor index
		 * \return Group and core index
		 */
		auto CoreIndexToGroupRelative(u32 core, u32 processor = 0) const noexcept -> Pair<u32, u32>;
		/**
		 * Convert a group and a relative core index into a core index
		 * \param[in] group Group index
		 * \param[in] core Core index relative to the group
		 * \return Core index
		 */
		auto GroupRelativeToCoreIndex(usize group, u32 core) const noexcept -> u32;
		/**
		 * Convert a group and a relative core index into a core index, for a certain processor
		 * \param[in] group Group index
		 * \param[in] core Core index relative to the group
		 * \param[in] processor Processor index
		 * \return Core index
		 */
		auto GroupRelativeToCoreIndex(usize group, u32 core, u32 processor) const noexcept -> u32;
		/**
		 * Check if 2 cores are on tha same last level cache
		 * \param[in] core0 First core index
		 * \param[in] core1 Second core index
		 * \param[in] processor Processor index
		 * \return Whether the cores are on tha same last level cache
		 */
		auto AreCoresOnSameLastLevelCache(usize core0, usize core1, u32 processor = 0) const noexcept -> bool;

		/**
		 * Get the the processor's features
		 * \return Struct with processor features
		 */
		auto GetProcessorFeatures() const noexcept -> const ProcessorFeatures&;

		/**
		 * Log out system info
		 * \param[in] logIdentifiableInfo Whether to log identifiable info
		 */
		auto LogSystemInfo(bool logIdentifiableInfo) const noexcept -> void;
	private:
		DynArray<ProcessorInfo> m_processorInfo;                                    ///< Information about the processors
		DynArray<NUMANodeInfo>  m_numaNodes;                                        ///< NUMA nodes
		IdentifiableInfo        m_identifiableInfo;                                 ///< Identifiable info
		ProcessorFeatures       m_processorFeatures;                                ///< Processor features
		ProcessorArch           m_processorArchitecture   = ProcessorArch::Unknown; ///< CPU architecture (for all CPUs)
		usize                   m_pageSize                = 0;                      ///< Page size
		void*                   m_appMemoryStart          = 0;                      ///< Start of application memory range
		void*                   m_appMemoryEnd            = 0;                      ///< End of application memory range
		usize                   m_virtAllocGranulariy     = 0;                      ///< Granularity of system allocations
		u64                     m_installedMemory         = 0;                      ///< Amount of memory installed in the system
		String                  m_manufacturer;                                     ///< Manufacturer
	};														  

	CORE_API auto GetSystemInfo() noexcept -> SystemInfo&;
}

#define g_SystemInfo (Core::GetSystemInfo())