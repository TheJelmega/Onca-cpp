#pragma once
#include "core/MinInclude.h"
#include "core/containers/Array.h"

namespace Onca::Hashing
{
	namespace Detail
	{
		// https://wiki.osdev.org/CRC32
		/**
		 * Generate the Crc32 lookup table
		 * \param[in] polynomial Reverse polynomial
		 * \return Crc32 look-up table
		 */
		constexpr auto CreateCrc32Table(u32 polynomial) noexcept -> Array<u32, 256>
		{
			Array<u32, 256> arr;

			for (usize idx = 0; idx < 256; ++idx)
			{
				arr[idx] = u32(idx);
				for (usize i = 8; i; --i)
				{
					arr[idx] = (arr[idx] & 1) ? (arr[idx] >> 1) ^ polynomial : arr[idx] >> 1;
				}
			}

			return arr;
		}
		constexpr Array<u32, 256> Crc32Lut = CreateCrc32Table(0xEDB88320);
	}
	
	/**
	 * 32-bit cyclical redundancy check
	 */
	struct Crc32
	{
		constexpr auto operator()(const u8* pData, usize size) const noexcept -> u32;
	};

	
}

#include "CRC.inl"