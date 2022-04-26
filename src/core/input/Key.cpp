#include "Key.h"

namespace Onca::Input
{
	Key::Key(const String& path) noexcept
		: path(path)
	{
	}

	auto Key::IsButton() const noexcept -> bool
	{
		return details ? details->IsButton() : false;
	}

	auto Key::IsAxisButton() const noexcept -> bool
	{
		return details ? details->IsAxisButton() : false;
	}

	auto Key::IsAxis1D() const noexcept -> bool
	{
		return details ? details->IsAxis1D() : false;
	}

	auto Key::IsAxis2D() const noexcept -> bool
	{
		return details ? details->IsAxis2D() : false;
	}

	auto Key::IsAxis3D() const noexcept -> bool
	{
		return details ? details->IsAxis3D() : false;
	}

	auto Key::IsSensor() const noexcept -> bool
	{
		return details ? details->IsSensor() : false;
	}

	auto Key::IsTouch() const noexcept -> bool
	{
		return details ? details->IsTouch() : false;
	}

	auto Key::IsModifier() const noexcept -> bool
	{
		return details ? details->IsModifier() : false;
	}

	auto Key::GetInputValueType() const noexcept -> ValueType
	{
		return details ? details->GetInputValueType() : ValueType::Digital;
	}
}
