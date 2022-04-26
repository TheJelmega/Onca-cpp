#pragma once
#include "Common.h"
#include "core/chrono/DeltaTime.h"
#include "core/memory/Unique.h"

namespace Core::Input
{
	class User;
	class InputManager;
	
	class CORE_API ITrigger
	{
	public:
		/**
		 * Create an ITrigger with an actuation threshold
		 * \param[in] threshold Actuation threshold
		 */
		ITrigger(f32 threshold) : m_threshold(threshold) {}
		virtual ~ITrigger() {}

		/*
		 * Tick the trigger with the associated input value
		 * \param[in] value Input value
		 * \param[in] dt Delta time
		 */
		virtual auto Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState = 0;
		/**
		 * Clone the trigger
		 * \return Cloned trigger
		 */
		// TODO: do via reflection system
		virtual auto Clone() const noexcept -> Unique<ITrigger> = 0;
		/**
		 * Get the trigger type
		 * \return Trigger type
		 */
		virtual auto GetTriggerType() const noexcept -> TriggerType { return TriggerType::Explicit; }
		/**
		 * Set the threshold, where if above, the trigger will trigger
		 * \param[in] threshold Threshold
		 */
		void SetThreshold(f32 threshold) noexcept { m_threshold = threshold; }
		/**
		 * Get the threshold, where if above, the trigger will trigger
		 * \return Threshold
		 */
		auto GetThreshold() const noexcept -> f32 { return m_threshold; }

	protected:
		friend class InternalMapping;

		/**
		 * Check if a value is actuated (outside of threshold)
		 * \param[in] value Value
		 * \return Whether the value is actuated
		 */
		auto IsActuated(Value value) const noexcept -> bool { return value.value.LenSq() >= m_threshold * m_threshold; }

		Value m_oldValue;  ///< Old value
		f32   m_threshold; ///< Actuation threshold
	};

	class CORE_API ITimedTrigger : public ITrigger
	{
	public:
		ITimedTrigger(f32 threshold) : ITrigger(threshold), m_time(0.f), m_useTimeDilation(false) {}
		virtual ~ITimedTrigger() {}

		/*
		 * Tick the trigger with the associated input value
		 * \param[in] value Input value
		 * \param[in] dt Delta time
		 */
		auto Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState override;

		/**
		 * Set whether the trigger is affected by time dilation
		 * \param[in] affected Whether the trigger is affected by time dilation
		 */
		void SetAffectedByTimeDilation(bool affected) noexcept { m_useTimeDilation = affected; }
		/**
		 * Check fi the trigger is affected by time dilation
		 * \return Whether the trigger is affected by time dilation
		 */
		auto GetAffectedByTimeDilation() const noexcept -> bool { return m_useTimeDilation; }

	protected:
		f32  m_time;            ///< Time
		bool m_useTimeDilation; ///< Whether the trigger is affected by time dilation
	};

	class CORE_API IModifier
	{
	public:
		virtual ~IModifier() {}

		/**
		 * Modify the given value
		 * \param[in] value Value to modify
		 */
		virtual void Modify(Value& value) noexcept = 0;
		/**
		 * Clone the modifier
		 * \return Cloned modifier
		 */
		 // TODO: do via reflection system
		virtual auto Clone() const noexcept -> Unique<IModifier> = 0;
	};
}
