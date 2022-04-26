#pragma once
#include "Common.h"
#include "core/memory/RefCounted.h"

namespace Core::Input
{
	struct KeyDetails;
	/// Key path exists of multiple parts
	///	<Device>       <- Device that owns the key (Mouse,Keyboard,Gamepad,etc.)
	///	{Component}    <- Sub-component of a layout, depends on device, e.g. 'valveIndex{leftHand}' gets an input of the left controller of the valve index
	///	[Index]        <- Index of a sub-component, depends on sub-device, e.g. 'vive{tracker}[2]' gets an input on the 3rd vive tracker
	/// /
	/// Name           <- Actual key name
	///	or
	///	#(DisplayName) <- Display name that can be converted to a physical input
	///
	/// multiple names can appear at the end of a key, separated by a slash, this can for example be used to use the key's full axis or only part of it
	///
	///	Example: '<VR>{leftHand}/joystick/x'

	struct Key
	{
		Key() noexcept = default;
		Key(const String& path) noexcept;

		/**
		 * Check if the key is a button
		 * \return Whether the key is a button
		 */
		auto IsButton() const noexcept -> bool;
		/**
		 * Check if the key is an axis button
		 * \return Whether the key is an axis button
		 */
		auto IsAxisButton() const noexcept -> bool;
		/**
		 * Check if the key is a 1D axis
		 * \return Whether the key is a 1D axis
		 */
		auto IsAxis1D() const noexcept -> bool;
		/**
		 * Check if the key is a 2D axis
		 * \return Whether the key is a 2D axis
		 */
		auto IsAxis2D() const noexcept -> bool;
		/**
		 * Check if the key is a 3D axis
		 * \return Whether the key is a 3D axis
		 */
		auto IsAxis3D() const noexcept -> bool;

		/**
		 * Check if the key is a sensor
		 * \return Whether the key is a sensor
		 */
		auto IsSensor() const noexcept -> bool;
		/**
		 * Check if the key is a touch input
		 * \return Whether the key is a touch input
		 */
		auto IsTouch() const noexcept -> bool;
		/**
		 * Check if the key is a modifier
		 * \return Whether the key is a modifier
		 */
		auto IsModifier() const noexcept -> bool;

		/**
		 * Get the input value type for this key
		 * \return Input value type
		 */
		auto GetInputValueType() const noexcept -> ValueType;

		auto operator==(const Key& other) const noexcept -> bool { return path == other.path; }
		
		String           path;    ///< Path of the key // TODO: Use interned string
		Weak<KeyDetails> details; ///< Key information
	};

	enum class KeyFlag
	{
		None = 0, ///< No flags

		Button     = BIT(0), ///< Key is a button
		AxisButton = BIT(1), ///< Key is a axis button (part of an axis that is interpreted as a button, e.g.: gamepad joystick up)
		Axis1D     = BIT(2), ///< 1D axis
		Axis2D     = BIT(3), ///< 2D axis
		Axis3D     = BIT(4), ///< 3D axis

		Sensor     = BIT(5), ///< Key represents a sensor, e.g.: gyroscope
		Touch      = BIT(6), ///< Key represents a touch

		Modifier   = BIT(7), ///< Key is a modifier key (used for binding)

		All        = 0xFF,   ///< All flags
	};
	DEFINE_FLAGS(KeyFlag);

	struct KeyDetails
	{
		/**
		 * Create key details
		 * \param[in] flags Flags
		 * \param[in] shortDesc Short description
		 * \param[in] longDesc Long description, if empty, this is the same as the short description
		 */
		KeyDetails(KeyFlags flags, const String& shortDesc, const String& longDesc = String{})
			: flags(flags)
			, shortDesc(shortDesc)
			, longDesc(!longDesc.IsEmpty() ? longDesc : shortDesc)
			, devRefCnt(0)
		{}

		/**
		 * Check if the key is a button
		 * \return Whether the key is a button
		 */
		auto IsButton() const noexcept -> bool { return flags & KeyFlag::Button; }
		/**
		 * Check if the key is an axis button
		 * \return Whether the key is an axis button
		 */
		auto IsAxisButton() const noexcept -> bool { return flags & KeyFlag::AxisButton; }
		/**
		 * Check if the key is a 1D axis
		 * \return Whether the key is a 1D axis
		 */
		auto IsAxis1D() const noexcept -> bool { return flags & KeyFlag::Axis1D; }
		/**
		 * Check if the key is a 2D axis
		 * \return Whether the key is a 2D axis
		 */
		auto IsAxis2D() const noexcept -> bool { return flags & KeyFlag::Axis2D; }
		/**
		 * Check if the key is a 3D axis
		 * \return Whether the key is a 3D axis
		 */
		auto IsAxis3D() const noexcept -> bool { return flags & KeyFlag::Axis3D; }

		/**
		 * Check if the key is a sensor
		 * \return Whether the key is a sensor
		 */
		auto IsSensor() const noexcept -> bool { return flags & KeyFlag::Sensor; }
		/**
		 * Check if the key is a touch input
		 * \return Whether the key is a touch input
		 */
		auto IsTouch() const noexcept -> bool { return flags & KeyFlag::Touch; }
		/**
		 * Check if the key is a modifier
		 * \return Whether the key is a modifier
		 */
		auto IsModifier() const noexcept -> bool { return flags & KeyFlag::Modifier; }

		/**
		 * Get the input value type for this key
		 * \return Input value type
		 */
		auto GetInputValueType() const noexcept -> ValueType
		{
			if (flags & KeyFlag::Axis1D) return ValueType::Axis;
			if (flags & KeyFlag::Axis2D) return ValueType::Axis2D;
			if (flags & KeyFlag::Axis3D) return ValueType::Axis3D;
			return ValueType::Digital;
		}

		Key      key;       ///< Key instance
		KeyFlags flags;     ///< Key flags
		String   shortDesc; ///< Key short description (is also the key name)
		String   longDesc;  ///< Key long description
		usize    devRefCnt; ///< Device ref count
	};

}

namespace Core
{
	template<>
	struct Hash<Input::Key>
	{
		auto operator()(const Input::Key& key) const noexcept -> u64
		{
			static Hash<String> hasher;
			return hasher(key.path);
		}
	};
}

DEFINE_ENUM_FLAG_OPS(Core::Input::KeyFlag)
