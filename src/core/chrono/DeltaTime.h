#pragma once
#include "core/MinInclude.h"

namespace Core::Chrono
{
	/**
	 * Delta time with dilation
	 */
	class DeltaTime
	{
	public:
		/**
		 * Create a 0 delta time with a dilation of 1
		 */
		DeltaTime() noexcept;
		/**
		 * Create a delta time
		 * \param[in] time Time
		 * \param[in] dilation Dilation
		 */
		DeltaTime(f32 time, f32 dilation = 1.f);

		/**
		 * Set the time
		 * \param[in] time Time
		 */
		void SetTime(f32 time) noexcept;
		/**
		 * Get the time with optional dilation applied (dilation applied by default)
		 * \param[in] useDilation Whether to use dilation
		 * \return Time with possible dilation
		 */
		auto GetTime(bool useDilation = true) const noexcept -> f32;
		/**
		 * Set the time dilation
		 * \param[in] dilation Time dilation
		 */
		void SetDilation(f32 dilation) noexcept;
		/**
		 * Get the time dilation
		 * \return Time dilation
		 */
		auto GetDilation() const noexcept -> f32;

		operator f32() noexcept;

	private:
		f32 m_time;     ///< Time
		f32 m_dilation; ///< Time dilation
	};
}

#include "DeltaTime.inl"