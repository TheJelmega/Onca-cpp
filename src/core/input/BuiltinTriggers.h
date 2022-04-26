#pragma once
#include "Common.h"
#include "IInput.h"
#include "core/memory/RefCounted.h"

namespace Core::Input
{
	class InputAction;

	/**
	 * Trigger when input is 'pressed'
	 */
	class CORE_API PressedTrigger final : public ITrigger
	{
	public:
		/**
		 * Create a pressed trigger
		 * \param[in] threshold Initial trigger threshold
		 */
		explicit PressedTrigger(f32 threshold = 0.5f);

		/*
		 * Tick the trigger with the associated input value
		 * \param[in] user User
		 * \param[in] value Input value
		 * \param[in] dt Delta time
		 */
		auto Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState override;
		/**
		 * Clone the trigger
		 * \return Cloned trigger
		 */
		auto Clone() const noexcept -> Unique<ITrigger> override;
	};

	/**
	 * Trigger when input is 'released'
	 */
	class CORE_API ReleasedTrigger final : public ITrigger
	{
	public:
		/**
		 * Create a released trigger
		 * \param[in] threshold Initial trigger threshold
		 */
		explicit ReleasedTrigger(f32 threshold = 0.5f);

		/*
		 * Tick the trigger with the associated input value
		 * \param[in] user User
		 * \param[in] value Input value
		 * \param[in] dt Delta time
		 */
		auto Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState override;
		/**
		 * Clone the trigger
		 * \return Cloned trigger
		 */
		auto Clone() const noexcept -> Unique<ITrigger> override;
	};

	/**
	 * Trigger when input is 'down'
	 */
	class CORE_API DownTrigger final : public ITrigger
	{
	public:
		/**
		 * Create a down trigger
		 * \param[in] threshold Initial trigger threshold
		 */
		explicit DownTrigger(f32 threshold = 0.5f);

		/*
		 * Tick the trigger with the associated input value
		 * \param[in] user User
		 * \param[in] value Input value
		 * \param[in] dt Delta time
		 */
		auto Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState override;
		/**
		 * Clone the trigger
		 * \return Cloned trigger
		 */
		auto Clone() const noexcept -> Unique<ITrigger> override;
	};

	/**
	 * Trigger after input has been held for a certain amount of time
	 */
	class CORE_API HoldTrigger final : public ITimedTrigger
	{
	public:
		/**
		 * Create a hold trigger
		 * \param[in] holdTime Time the trigger needs to be actuated to trigger
		 * \param[in] threshold Initial trigger threshold
		 */
		HoldTrigger(f32 holdTime = 1.f, f32 threshold = 0.5f);

		/*
		 * Tick the trigger with the associated input value
		 * \param[in] user User
		 * \param[in] curValue Input value
		 * \param[in] dt Delta time
		 */
		auto Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState override;
		/**
		 * Clone the trigger
		 * \return Cloned trigger
		 */
		auto Clone() const noexcept -> Unique<ITrigger> override;

		/**
		 * Set the time the trigger needs to be actuated to trigger
		 * \param[in] holdTime Time the trigger needs to be actuated to trigger
		 */
		void SetHoldTime(f32 holdTime) noexcept { m_holdTime = holdTime; }
		/**
		 * Get the time the trigger needs to be actuated to trigger
		 * \return Time the trigger needs to be actuated to trigger
		 */
		auto GetHoldTime() const noexcept -> f32 { return m_holdTime; }
		/**
		 * Set whether the trigger is a one shot
		 * \param[in] oneShot Whether the trigger is a one shot
		 */
		void SetIsOneShot(bool oneShot) noexcept { m_isOneShot = oneShot; }
		/**
		 * Check if the trigger is a one shot
		 * \return Whether the trigger is a one shot
		 */
		auto IsOneShot() const noexcept -> bool { return m_isOneShot; }

	private:
		f32  m_holdTime;      ///< Time trigger needs to be actuated to trigger
		bool m_isOneShot : 1; ///< Whether the trigger is a one shot
		bool m_triggered : 1; ///< Whether the trigger has already been triggered once
	};

	/**
	 * Trigger after input has been released after being held for a certain amount of time
	 */
	class CORE_API HoldReleaseTrigger final : public ITimedTrigger
	{
	public:
		/**
		 * Create a hold and release trigger
		 * \param[in] holdTime Time the trigger needs to be actuated to trigger
		 * \param[in] threshold Initial trigger threshold
		 */
		HoldReleaseTrigger(f32 holdTime = 1.f, f32 threshold = 0.5f);

		/*
		 * Tick the trigger with the associated input value
		 * \param[in] user User
		 * \param[in] curValue Input value
		 * \param[in] dt Delta time
		 */
		auto Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState override;
		/**
		 * Clone the trigger
		 * \return Cloned trigger
		 */
		auto Clone() const noexcept -> Unique<ITrigger> override;

		/**
		 * Set the time the trigger needs to be actuated before a release can trigger it
		 * \param[in] holdTime Time the trigger needs to be actuated before a release can trigger it
		 */
		void SetHoldTime(f32 holdTime) noexcept { m_holdTime = holdTime; }
		/**
		 * Get the time the trigger needs to be actuated before a release can trigger it
		 * \return Time the trigger needs to be actuated before a release can trigger it
		 */
		auto GetHoldTIme() const noexcept -> f32 { return m_holdTime; }

	private:
		f32 m_holdTime;  ///< Time trigger needs to be actuated to trigger
	};

	/**
	 * Trigger after input has been actuated and released in a certain timespan
	 */
	class CORE_API TapTrigger final : public ITimedTrigger
	{
	public:
		/**
		 * Create a hold and release trigger
		 * \param[in] releaseTime Max time between actuation and release
		 * \param[in] threshold Initial trigger threshold
		 */
		TapTrigger(f32 releaseTime = 1.f, f32 threshold = 0.5f);

		/*
		 * Tick the trigger with the associated input value
		 * \param[in] user User
		 * \param[in] curValue Input value
		 * \param[in] dt Delta time
		 */
		auto Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState override;
		/**
		 * Clone the trigger
		 * \return Cloned trigger
		 */
		auto Clone() const noexcept -> Unique<ITrigger> override;

		/**
		 * Set the max time between actuation and release
		 * \param[in] holdTime Max time between actuation and release
		 */
		void SetReleaseTime(f32 holdTime) noexcept { m_releaseTime = holdTime; }
		/**
		 * Get the max time between actuation and release
		 * \return Max time between actuation and release
		 */
		auto GetReleaseTIme() const noexcept -> f32 { return m_releaseTime; }

	private:
		f32 m_releaseTime;  ///< Time trigger needs to be actuated to trigger
	};

	/**
	 * Trigger after input has been actuated and released in a certain timespan
	 */
	class CORE_API PulseTrigger final : public ITimedTrigger
	{
	public:
		/**
		 * Create a hold and release trigger
		 * \param[in] interval Interval between pulses
		 * \param[in] threshold Initial trigger threshold
		 */
		PulseTrigger(f32 interval = 1.f, f32 threshold = 0.5f);

		/*
		 * Tick the trigger with the associated input value
		 * \param[in] user User
		 * \param[in] curValue Input value
		 * \param[in] dt Delta time
		 */
		auto Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState override;
		/**
		 * Clone the trigger
		 * \return Cloned trigger
		 */
		auto Clone() const noexcept -> Unique<ITrigger> override;

		/**
		 * Set whether to trigger on the initial actuation
		 * \param[in] triggerOnStart Whether to trigger on the initial actuation
		 */
		void SetTriggerOnStart(bool triggerOnStart) noexcept { m_triggerOnStart = triggerOnStart; }
		/**
		 * Check if the trigger is triggered on the initial actuation
		 * \return Whether the trigger is triggered on the initial actuation
		 */
		auto GetTriggerOnStart() const noexcept -> bool { return m_triggerOnStart; }

		/**
		 * Set the max time between actuation and release
		 * \param[in] holdTime Max time between actuation and release
		 */
		void SetInterval(f32 holdTime) noexcept { m_interval = holdTime; }
		/**
		 * Get the max time between actuation and release
		 * \return Max time between actuation and release
		 */
		auto GetInterval() const noexcept -> f32 { return m_interval; }

		/**
		 * Set the maximum number of pulses that can fire (0 means no limit)
		 * \param[in] maxPulses Maximum number of pulses that can fire
		 */
		void SetMaxPulses(u64 maxPulses) noexcept { m_maxPulses = maxPulses; }
		/**
		 * Get the maximum number of pulses that can fire (0 means no limit)
		 * \return Maximum number of pulses that can fire
		 */
		auto GetMaxPulses() const noexcept -> u64 { return m_maxPulses; }

	private:
		bool m_triggerOnStart; ///< Whether to trigger on the initial actuation of the trigger
		f32  m_interval;       ///< Time trigger needs to be actuated to trigger
		u64  m_maxPulses;      ///< Maximum number of allowed pulses
		u64  m_pulseIdx;       ///< Number of times the trigger has pulsed
	};

	/**
	 * Trigger that is always triggered
	 */
	class AlwaysTrigger : public ITrigger
	{
		/*
		 * Tick the trigger with the associated input value
		 * \param[in] user User
		 * \param[in] curValue Input value
		 * \param[in] dt Delta time
		 */
		auto Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState override { return TriggerState::Triggered; }
		/**
		 * Clone the trigger
		 * \return Cloned trigger
		 */
		auto Clone() const noexcept -> Unique<ITrigger> override;
	};

	/**
	 * A trigger which requires an input action to be triggered
	 */
	class CORE_API ChordedActionTrigger : public ITrigger
	{
	public:
		/**
		 * Create a chorded action
		 */
		ChordedActionTrigger();
		/**
		 * Create a chorded action with an input action
		 * \param[in] action Input action
		 */
		ChordedActionTrigger(const Weak<InputAction>& action);

		/*
		 * Tick the trigger with the associated input value
		 * \param[in] user User
		 * \param[in] curValue Input value
		 * \param[in] dt Delta time
		 */
		auto Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState override;
		/**
		 * Clone the trigger
		 * \return Cloned trigger
		 */
		auto Clone() const noexcept -> Unique<ITrigger> override;

		/**
		 * Get the trigger type
		 * \return Trigger type
		 */
		auto GetTriggerType() const noexcept -> TriggerType override { return TriggerType::Implicit; }

		/**
		 * Set the chorded action
		 * \param[in] action Input action
		 */
		void SetAction(const Weak<InputAction>& action) { m_action = action; }
		/**
		 * Get the chorded action
		 * \return Input action
		 */
		auto GetAction() const noexcept -> const Weak<InputAction>& { return m_action; }

	private:
		Weak<InputAction> m_action; // Action that needs to have been triggered
	};

	/**
	 * A trigger which blocks other triggered when an input action is triggered
	 */
	class CORE_API ChordedActionTriggerBlocker final : public ChordedActionTrigger
	{
	public:
		/**
		 * Create a chorded action blocker
		 */
		ChordedActionTriggerBlocker();
		/**
		 * Create a chorded action blocker with an input action
		 * \param[in] action Input action
		 */
		ChordedActionTriggerBlocker(const Weak<InputAction>& action);

		/**
		 * Get the trigger type
		 * \return Trigger type
		 */
		auto GetTriggerType() const noexcept -> TriggerType override { return TriggerType::Blocker; }
	};
}
