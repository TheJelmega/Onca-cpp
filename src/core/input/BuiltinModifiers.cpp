#include "BuiltinModifiers.h"

namespace Core::Input
{
	NegateModifier::NegateModifier(bool x, bool y, bool z)
		: m_negX(x)
		, m_negY(y)
		, m_negZ(z)
	{
	}

	void NegateModifier::Modify(Value& value) noexcept
	{
		if (m_negX) value.value.x = -value.value.x;
		if (m_negY) value.value.y = -value.value.y;
		if (m_negZ) value.value.z = -value.value.z;
	}

	auto NegateModifier::Clone() const noexcept -> Unique<IModifier>
	{
		return Unique<NegateModifier>::Create(*this);
	}

	DeadzoneModifier::DeadzoneModifier()
		: m_lower(.2f)
		, m_upper(.8f)
		, m_type(DeadzoneType::Axial)
	{
	}

	void DeadzoneModifier::Modify(Value& value) noexcept
	{
		if (m_type == DeadzoneType::Axial)
		{
			f32 x = Math::Abs(value.value.x);
			if (x < m_lower)
				value.value.x = 0.f;
			else if (x > m_upper)
				value.value.x = value.value.x < 0.f ? - 1.f : 1.f;

			f32 y = Math::Abs(value.value.y);
			if (y < m_lower)
				value.value.y = 0.f;
			else if (y > m_upper)
				value.value.y = value.value.y < 0.f ? -1.f : 1.f;

			f32 z = Math::Abs(value.value.z);
			if (z < m_lower)
				value.value.z = 0.f;
			else if (z > m_upper)
				value.value.z = value.value.z < 0.f ? -1.f : 1.f;
		}
		else // m_type == DeadzoneType::Radial
		{
			f32 lenSq = value.value.LenSq();
			f32 lowerSq = m_lower * m_lower;
			f32 upperSq = m_upper * m_upper;
			if (lenSq < lowerSq)
				value.value = {};
			else if (lenSq > upperSq)
				value.value *= 1.f / Math::Sqrt(lenSq);
		}
	}

	auto DeadzoneModifier::Clone() const noexcept -> Unique<IModifier>
	{
		return Unique<DeadzoneModifier>::Create(*this);
	}

	ScalarModifier::ScalarModifier(const f32v3& scalar)
		: m_scalar(scalar)
	{
	}

	void ScalarModifier::Modify(Value& value) noexcept
	{
		value.value *= m_scalar;
	}

	auto ScalarModifier::Clone() const noexcept -> Unique<IModifier>
	{
		return Unique<ScalarModifier>::Create(*this);
	}

	SwizzleModifier::SwizzleModifier(SwizzleAxis x, SwizzleAxis y, SwizzleAxis z)
		: m_xAxis(x)
		, m_yAxis(y)
		, m_zAxis(z)
	{
	}

	void SwizzleModifier::Modify(Value& value) noexcept
	{
		value.value = value.value.Swizzle(u8(m_xAxis), u8(m_yAxis), u8(m_zAxis));
	}

	auto SwizzleModifier::Clone() const noexcept -> Unique<IModifier>
	{
		return Unique<SwizzleModifier>::Create(*this);
	}
}
