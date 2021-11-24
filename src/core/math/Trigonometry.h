#pragma once
#include "core/MinInclude.h"
#include "Concepts.h"
#include "Angle.h"

namespace Core::Math
{
	namespace Detail
	{
		template<Numeric T>
		constexpr auto CompTanHelper(T val) noexcept -> T;
		template<Numeric T>
		constexpr auto CompTan(T val) noexcept -> T;
		template<Numeric T>
		constexpr auto CompSin(T val) noexcept -> T;
		template<Numeric T>
		constexpr auto CompCos(T val) noexcept -> T;

		template<Numeric T>
		constexpr auto CompATanHelper (T val) noexcept -> T;
		template<Numeric T>
		constexpr auto CompATan(T val) noexcept -> T;
		template<Numeric T>
		constexpr auto CompASin(T val) noexcept -> T;
		template<Numeric T>
		constexpr auto CompACos(T val) noexcept -> T;
		template<Numeric T>
		constexpr auto CompATan2(T y, T x) noexcept -> T;
	}

	/**
	 * Calculate the sine of the angle
	 * \tparam T Type of the angle
	 * \tparam Kind Kind of the angle
	 * \param angle Angle
	 * \return Sine of the angle
	 * \note Constexpr version based on half-angle formula using tan
	 */
	template<Numeric T, AngleKind Kind>
	constexpr auto Sin(const Angle<T, Kind>& angle) noexcept -> T;
	/**
	 * Calculate the cosine of the angle
	 * \tparam T Type of the angle
	 * \tparam Kind Kind of the angle
	 * \param angle Angle
	 * \return Cosine of the angle
	 * \note Constexpr version based on half-angle formula using tan
	 */
	template<Numeric T, AngleKind Kind>
	constexpr auto Cos(const Angle<T, Kind>& angle) noexcept -> T;
	/**
	 * Calculate the tangent of the angle
	 * \tparam T Type of the angle
	 * \tparam Kind Kind of the angle
	 * \param angle Angle
	 * \return Tangent of the angle
	 * \note Constexpr version based on continuous fraction (taylor series is used for edge-case around 90 degrees)
	 */
	template<Numeric T, AngleKind Kind>
	constexpr auto Tan(const Angle<T, Kind>& angle) noexcept -> T;

	/**
	 * Calculate the arcsine of a value
	 * \tparam T Tpe of the value
	 * \param val Value
	 * \return Arcsine of the value
	 */
	template<Numeric T>
	constexpr auto ASin(T val) noexcept -> Angle<T, AngleKind::Radians>;
	/**
	 * Calculate the arccosine of a value
	 * \tparam T Tpe of the value
	 * \param val Value
	 * \return Arccosine of the value
	 */
	template<Numeric T>
	constexpr auto ACos(T val) noexcept -> Angle<T, AngleKind::Radians>;
	/**
	 * Calculate the arctangent of a value
	 * \tparam T Tpe of the value
	 * \param val Value
	 * \return Arctangent of the value
	 */
	template<Numeric T>
	constexpr auto ATan(T val) noexcept -> Angle<T, AngleKind::Radians>;

	/**
	 * Calculate the arctangent of a value
	 * \tparam T Tpe of the value
	 * \param y Value on Y-axis
	 * \param x Value on X-axis
	 * \return Arctangent of the value
	 */
	template<Numeric T>
	constexpr auto ATan2(T y, T x) noexcept -> Angle<T, AngleKind::Radians>;

	/**
	 * \brief Calculate the hyperbolic sine of the angle
	 * \tparam T Type of the angle
	 * \tparam Kind Kind of the angle
	 * \param val Angle
	 * \return Hyperbolic sine of the angle
	 */
	template<Numeric T, AngleKind Kind>
	constexpr auto Sinh(const Angle<T, Kind>& angle) noexcept -> T;
	/**
	 * \brief Calculate the hyperbolic cosine of the angle
	 * \tparam T Type of the angle
	 * \tparam Kind Kind of the angle
	 * \param val Angle
	 * \return Hyperbolic cosine of the angle
	 */
	template<Numeric T, AngleKind Kind>
	constexpr auto Cosh(const Angle<T, Kind>& angle) noexcept -> T;
	/**
	 * \brief Calculate the hyperbolic tangent of the angle
	 * \tparam T Type of the angle
	 * \tparam Kind Kind of the angle
	 * \param val Angle
	 * \return Hyperbolic tangent of the angle
	 */
	template<Numeric T, AngleKind Kind>
	constexpr auto Tanh(const Angle<T, Kind>& angle) noexcept -> T;
	/**
	 * \brief Calculate the hyperbolic arcsine of the angle
	 * \tparam T Type of the angle
	 * \param val Angle
	 * \return Hyperbolic arcsine of the angle
	 */
	template<Numeric T>
	constexpr auto ASinh(T val) noexcept -> Angle<T, AngleKind::Radians>;
	/**
	 * \brief Calculate the hyperbolic arccosine of the angle
	 * \tparam T Type of the angle
	 * \param val Angle
	 * \return Hyperbolic arccosine of the angle
	 */
	template<Numeric T>
	constexpr auto ACosh(T val) noexcept -> Angle<T, AngleKind::Radians>;
	/**
	 * \brief Calculate the hyperbolic arctangent of the angle
	 * \tparam T Type of the angle
	 * \param val Angle
	 * \return Hyperbolic arctangent of the angle
	 */
	template<Numeric T>
	constexpr auto ATanh(T val) noexcept -> Angle<T, AngleKind::Radians>;
}
