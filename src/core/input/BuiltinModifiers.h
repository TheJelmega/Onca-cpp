#pragma once
#include "Common.h"
#include "IInput.h"

namespace Onca::Input
{

	/**
	 * Modifier that can negate axes
	 */
	class NegateModifier final : public IModifier
	{
	public:
		/**
		 * Create a negate modifiers
		 * \param[in] x Whether to negate the X axis
		 * \param[in] y Whether to negate the Y axis
		 * \param[in] z Whether to negate the Z axis
		 */
		NegateModifier(bool x = true, bool y = true, bool z = true);

		/**
		 * Modify the given value
		 * \param[in] value Value to modify
		 */
		void Modify(Value& value) noexcept override;
		/**
		 * Clone the modifier
		 * \return Cloned modifier
		 */
		auto Clone() const noexcept -> Unique<IModifier> override;

		/**
		 * Set whether to negate the X axis
		 * \param[in] negate Whether to negate the X axis
		 */
		void SetNegateX(bool negate) noexcept { m_negX = negate; }
		/**
		 * Check if X is being negated
		 * \return Whether X is being negated
		 */
		auto IsXNegated() const noexcept -> bool { return m_negX; }
		/**
		 * Set whether to negate the X axis
		 * \param[in] negate Whether to negate the X axis
		 */
		void SetNegateY(bool negate) noexcept { m_negY = negate; }
		/**
		 * Check if X is being negated
		 * \return Whether X is being negated
		 */
		auto IsYNegated() const noexcept -> bool { return m_negY; }
		/**
		 * Set whether to negate the X axis
		 * \param[in] negate Whether to negate the X axis
		 */
		void SetNegateZ(bool negate) noexcept { m_negZ = negate; }
		/**
		 * Check if X is being negated
		 * \return Whether X is being negated
		 */
		auto IsZNegated() const noexcept -> bool { return m_negZ; }

	private:
		bool m_negX : 1; ///< Whether to negate the X component
		bool m_negY : 1; ///< Whether to negate the X component
		bool m_negZ : 1; ///< Whether to negate the X component
	};

	enum class DeadzoneType : u8
	{
		Axial , ///< Deadzone will be applied per axis
		Radial, ///< Deadzone will be applied to all axes simultaneously
	};

	/**
	 * Adds a deadzone to an input
	 */
	class DeadzoneModifier final : public IModifier
	{
	public:
		/**
		 * Create a deadzone modifier
		 */
		DeadzoneModifier();

		/**
		 * Modify the given value
		 * \param[in] value Value to modify
		 */
		void Modify(Value& value) noexcept override;
		/**
		 * Clone the modifier
		 * \return Cloned modifier
		 */
		auto Clone() const noexcept -> Unique<IModifier> override;

		/**
		 * Set the deadzone lower bound (values with a smaller absolute value will be ignored)
		 * \param[in] bound Lower bound
		 */
		void SetLowerBound(f32 bound) noexcept { m_lower = bound; }
		/**
		 * Get the lower bound
		 * \return Lower bound
		 */
		auto GetLowerBound() const noexcept -> f32 { return m_lower; }
		/**
		 * Set the deadzone upper bound (values with a larger absolute value will the clamped to 1)
		 * \param[in] bound Upper bound
		 */
		void SetUpperBound(f32 bound) noexcept { m_upper = bound; }
		/**
		 * Get the upper bound
		 * \return Upper bound
		 */
		auto GetUpperBound() const noexcept -> f32 { return m_upper; }
		/**
		 * Set the deadzone type
		 * \param[in] type Deadzone type
		 */
		void SetType(DeadzoneType type) noexcept { m_type = type; }
		/**
		 * Get the deadzone type
		 * \return Deadzone type
		 */
		auto GetType() const noexcept -> DeadzoneType { return m_type; }

	private:
		f32          m_lower; ///< Lower bound
		f32          m_upper; ///< Upper bound
		DeadzoneType m_type;  ///< Deadzone type
	};

	/**
	 * Modifier that scales axes by a scalar value
	 */
	class ScalarModifier final : public IModifier
	{
	public:
		/**
		 * Create a negate modifiers
		 * \param[in] scalar Scalar values
		 */
		ScalarModifier(const f32v3& scalar = f32v3{ 1.f });

		/**
		 * Modify the given value
		 * \param[in] value Value to modify
		 */
		void Modify(Value& value) noexcept override;
		/**
		 * Clone the modifier
		 * \return Cloned modifier
		 */
		auto Clone() const noexcept -> Unique<IModifier> override;

		/**
		 * Set the scalar values
		 * \param[in] scalar Scalar values
		 */
		void SetScalar(const f32v3& scalar) noexcept { m_scalar = scalar; }
		/**
		 * Get the scalar values
		 * \return Scalar values
		 */
		auto GetScalar() const noexcept -> const f32v3& { return m_scalar; }

	private:
		f32v3 m_scalar; ///< Scalar components
	};

	enum class SwizzleAxis : u8
	{
		X = f32v3::X,
		Y = f32v3::Y,
		Z = f32v3::Z,
	};

	/**
	 * Modifier that swizzles the input axes
	 */
	class SwizzleModifier final : public IModifier
	{
	public:
		/**
		 * Create a swizzle modifier
		 * \param[in] x X swizzle axis
		 * \param[in] y Y swizzle axis
		 * \param[in] z Z swixxle axis
		 */
		SwizzleModifier(SwizzleAxis x = SwizzleAxis::X, SwizzleAxis y = SwizzleAxis::Y, SwizzleAxis z = SwizzleAxis::Z);

		/**
		 * Modify the given value
		 * \param[in] value Value to modify
		 */
		void Modify(Value& value) noexcept override;
		/**
		 * Clone the modifier
		 * \return Cloned modifier
		 */
		auto Clone() const noexcept -> Unique<IModifier> override;

		/**
		 * Set the X swizzle axis (what axis X will be)
		 * \param[in] axis Swizzle axis
		 */
		void SetXSwizzle(SwizzleAxis axis) noexcept { m_xAxis = axis; }
		/**
		 * Get the X swizzle axis
		 * \return Swizzle axis
		 */
		auto GetXSwizzle() const noexcept -> SwizzleAxis { return m_xAxis; }
		/**
		 * Set the Y swizzle axis (what axis Y will be)
		 * \param[in] axis Swizzle axis
		 */
		void SetYSwizzle(SwizzleAxis axis) noexcept { m_yAxis = axis; }
		/**
		 * Get the Y swizzle axis
		 * \return Swizzle axis
		 */
		auto GetYSwizzle() const noexcept -> SwizzleAxis { return m_yAxis; }
		/**
		 * Set the Z swizzle axis (what axis Z will be)
		 * \param[in] axis Swizzle axis
		 */
		void SetZSwizzle(SwizzleAxis axis) noexcept { m_zAxis = axis; }
		/**
		 * Get the Z swizzle axis
		 * \return Swizzle axis
		 */
		auto GetZSwizzle() const noexcept -> SwizzleAxis { return m_zAxis; }

	private:
		SwizzleAxis m_xAxis;
		SwizzleAxis m_yAxis;
		SwizzleAxis m_zAxis;
	};
}
