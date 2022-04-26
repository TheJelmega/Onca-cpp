#pragma once
#if __RESHARPER__
#include "DeltaTime.h"
#endif

namespace Onca::Chrono
{
	inline DeltaTime::DeltaTime() noexcept
		: m_time(0.f)
		, m_dilation(1.f)
	{
	}

	inline DeltaTime::DeltaTime(f32 time, f32 dilation)
		: m_time(time)
		, m_dilation(dilation)
	{
	}

	inline void DeltaTime::SetTime(f32 time) noexcept
	{
		m_time = time;
	}

	inline auto DeltaTime::GetTime(bool useDilation) const noexcept -> f32
	{
		return useDilation ? m_time * m_dilation : m_time;
	}

	inline void DeltaTime::SetDilation(f32 dilation) noexcept
	{
		m_dilation = dilation;
	}

	inline auto DeltaTime::GetDilation() const noexcept -> f32
	{
		return m_dilation;
	}

	inline DeltaTime::operator f32() noexcept
	{
		return m_time * m_dilation;
	}
}
