#pragma once
#include "core/MinInclude.h"
#include "Concepts.h"
#include "FwdDecl.h"

namespace Onca::Math
{
	/**
	* Type representing angles, which also handles conversions
	* \tparam T Numeric type
	* \tparam Kind Angle kind
	*/
	template<Numeric T, AngleKind Kind>
	struct Angle
	{
		STATIC_ASSERT(Kind == AngleKind::Radians || Kind == AngleKind::Degrees, "Invalid angle kind");

		T val;

		/**
		* Create a zero angle
		*/
		constexpr Angle() noexcept;
		/**
		* Create an angle from a value
		* \tparam U Type of value
		* \param val Value of angle
		*/
		template<ConvertableTo<T> U>
		constexpr Angle(U val) noexcept;
		/**
		* Create an angle from another angle
		* \param angle Other angle
		*/
		constexpr Angle(const Angle& angle) noexcept;
		/**
		* Create an angle from another angle
		* \tparam U Type of the angle
		* \tparam A The kind of angle it is
		* \param angle Other angle
		*/
		template<Numeric U, AngleKind A>
		constexpr Angle(const Angle<U, A>& angle) noexcept;
		/**
		* Create an angle from another angle
		* \param angle Other angle
		*/
		constexpr Angle(Angle&& angle) noexcept;

		template<ConvertableTo<T> U>
		constexpr auto operator=(U val) noexcept -> Angle&;
		constexpr auto operator=(const Angle& other) noexcept -> Angle&;
		template<Numeric U, AngleKind A>
		constexpr auto operator=(const Angle<U, A>& other) noexcept -> Angle&;
		constexpr auto operator=(Angle&& other) noexcept -> Angle&;

		constexpr auto operator+() const noexcept -> Angle;
		constexpr auto operator-() const noexcept -> Angle;

		constexpr auto operator+(const Angle& other) const noexcept -> Angle;
		constexpr auto operator-(const Angle& other) const noexcept -> Angle;

		template<ConvertableTo<T> U>
		constexpr auto operator*(U other) const noexcept -> Angle;
		template<ConvertableTo<T> U>
		constexpr auto operator/(U other) const noexcept -> Angle;

		constexpr auto operator+=(const Angle& other) noexcept -> Angle&;
		constexpr auto operator-=(const Angle& other) noexcept -> Angle&;

		template<ConvertableTo<T> U>
		constexpr auto operator*=(U other) noexcept -> Angle&;
		template<ConvertableTo<T> U>
		constexpr auto operator/=(U other) noexcept -> Angle&;

		constexpr auto operator==(const Angle& other) const noexcept -> bool;
		constexpr auto operator!=(const Angle& other) const noexcept -> bool;

		/**
		 * Compare 2 angles using an epsilon
		 * \param[in] other Angle to compare with
		 * \param[in] e Epsilon
		 * \return Whether the angles are equal
		 */
		constexpr auto Compare(const Angle& other, T e = Consts::MathEpsilon<T>) const noexcept -> bool;

		/**
		* Limit the angle within the interval [-360; 360] degrees
		* \return Reference of this
		*/
		constexpr auto Mod() noexcept -> Angle&;

		/**
		 * Get a value that is bounded in the interval [-360; 360] degrees
		 * \return Bounded value
		 */
		constexpr auto GetBoundedVal() const noexcept -> T;

		/**
		* Get the angle in degrees
		* \return Angle in degrees
		*/
		constexpr auto ToDegrees() const noexcept -> Angle<T, AngleKind::Degrees>;
		/**
		* Get the angle in radians
		* \return Angle in radians
		*/
		constexpr auto ToRadians() const noexcept -> Angle<T, AngleKind::Radians>;
	};

	template<Numeric T, ConvertableTo<T> U, AngleKind Kind>
	constexpr auto operator*(U val, const Angle<T, Kind>& angle) noexcept -> Angle<T, Kind>;

	template<Numeric T, AngleKind Kind>
	struct EulerAngles
	{
		Angle<T, Kind> pitch;
		Angle<T, Kind> yaw;
		Angle<T, Kind> roll;
	};
}

inline namespace Literals
{
	constexpr auto operator""_deg(long double val) noexcept -> Onca::Math::Angle<f64, Onca::Math::AngleKind::Degrees>;
	constexpr auto operator""_deg(unsigned long long int val) noexcept -> Onca::Math::Angle<f64, Onca::Math::AngleKind::Degrees>;

	constexpr auto operator""_rad(long double val) noexcept -> Onca::Math::Angle<f64, Onca::Math::AngleKind::Radians>;
	constexpr auto operator""_rad(unsigned long long int val) noexcept -> Onca::Math::Angle<f64, Onca::Math::AngleKind::Radians>;
}
