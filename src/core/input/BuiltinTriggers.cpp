#include "BuiltinTriggers.h"

#include "User.h"

namespace Core::Input
{
	PressedTrigger::PressedTrigger(f32 threshold)
		: ITrigger(threshold)
	{
	}

	auto PressedTrigger::Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState
	{
		return !IsActuated(m_oldValue) && IsActuated(value) ? TriggerState::Triggered : TriggerState::None;
	}

	auto PressedTrigger::Clone() const noexcept -> Unique<ITrigger>
	{
		return Unique<PressedTrigger>::Create(*this);
	}

	ReleasedTrigger::ReleasedTrigger(f32 threshold)
		: ITrigger(threshold)
	{
	}

	auto ReleasedTrigger::Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState
	{
		if (IsActuated(value))
			return TriggerState::Ongoing;
		if (IsActuated(m_oldValue))
			return TriggerState::Triggered;
		return TriggerState::None;
	}

	auto ReleasedTrigger::Clone() const noexcept -> Unique<ITrigger>
	{
		return Unique<ReleasedTrigger>::Create(*this);
	}

	DownTrigger::DownTrigger(f32 threshold)
		: ITrigger(threshold)
	{
	}

	auto DownTrigger::Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState
	{
		return IsActuated(value) ? TriggerState::Triggered : TriggerState::None;
	}

	auto DownTrigger::Clone() const noexcept -> Unique<ITrigger>
	{
		return Unique<DownTrigger>::Create(*this);
	}

	HoldTrigger::HoldTrigger(f32 holdTime, f32 threshold)
		: ITimedTrigger(threshold)
		, m_holdTime(holdTime)
		, m_isOneShot(false)
		, m_triggered(false)
	{
	}

	TriggerState HoldTrigger::Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept
	{
		if (IsActuated(value))
		{
			if (m_time >= 0.f)
				m_time += dt.GetTime(m_useTimeDilation);

			if (m_time < m_holdTime)
				return TriggerState::Ongoing;

			if (!m_isOneShot)
				return TriggerState::Triggered;

			TriggerState state = m_triggered ? TriggerState::None : TriggerState::Triggered;
			m_triggered = true;
			return state;
		}

		m_time = 0.f;
		m_triggered = false;
		return TriggerState::None;
	}

	auto HoldTrigger::Clone() const noexcept -> Unique<ITrigger>
	{
		return Unique<HoldTrigger>::Create(*this);
	}

	HoldReleaseTrigger::HoldReleaseTrigger(f32 holdTime, f32 threshold)
		: ITimedTrigger(threshold)
		, m_holdTime(holdTime)
	{
	}

	TriggerState HoldReleaseTrigger::Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept
	{
		if (IsActuated(value))
		{
			m_time += dt.GetTime(m_useTimeDilation);
			return TriggerState::Ongoing;
		}

		if (m_time == 0.f)
			return TriggerState::None;

		m_time = 0.f;
		return TriggerState::Triggered;
	}

	auto HoldReleaseTrigger::Clone() const noexcept -> Unique<ITrigger>
	{
		return Unique<HoldReleaseTrigger>::Create(*this);
	}

	TapTrigger::TapTrigger(f32 releaseTime, f32 threshold)
		: ITimedTrigger(threshold)
		, m_releaseTime(releaseTime)
	{
	}

	TriggerState TapTrigger::Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept
	{
		if (IsActuated(value))
		{
			m_time += dt.GetTime(m_useTimeDilation);
			return TriggerState::Ongoing;
		}
		return IsActuated(m_oldValue) && m_time <= m_releaseTime ? TriggerState::Triggered : TriggerState::None;
	}

	auto TapTrigger::Clone() const noexcept -> Unique<ITrigger>
	{
		return Unique<TapTrigger>::Create(*this);
	}

	PulseTrigger::PulseTrigger(f32 interval, f32 threshold)
		: ITimedTrigger(threshold)
		, m_triggerOnStart(true)
		, m_interval(interval)
		, m_maxPulses(0)
		, m_pulseIdx(0)
	{
	}

	TriggerState PulseTrigger::Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept
	{
		if (IsActuated(value))
		{
			if (m_pulseIdx >= m_maxPulses)
				return TriggerState::None;

			bool pulse = m_triggerOnStart && !IsActuated(m_oldValue);

			m_time += dt.GetTime(m_useTimeDilation);
			if (m_time > m_interval)
			{
				pulse = true;
				m_time = Math::Mod(m_time, m_interval);
			}

			if (!pulse)
				return TriggerState::Ongoing;

			++m_pulseIdx;
			return TriggerState::Triggered;
		}
		
		m_time = 0.f;
		m_pulseIdx = 0;
		return TriggerState::None;
	}

	auto PulseTrigger::Clone() const noexcept -> Unique<ITrigger>
	{
		return Unique<PulseTrigger>::Create(*this);
	}

	auto AlwaysTrigger::Clone() const noexcept -> Unique<ITrigger>
	{
		return Unique<AlwaysTrigger>::Create(*this);
	}

	ChordedActionTrigger::ChordedActionTrigger()
		: ITrigger(0.5f)
	{
	}

	ChordedActionTrigger::ChordedActionTrigger(const Weak<InputAction>& action)
		: ITrigger(0.5f)
		, m_action(action)
	{
	}
	
	auto ChordedActionTrigger::Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState
	{
		return user.IsInputActionTriggered(m_action) ? TriggerState::Triggered : TriggerState::None;
	}

	auto ChordedActionTrigger::Clone() const noexcept -> Unique<ITrigger>
	{
		return Unique<ChordedActionTrigger>::Create(*this);
	}

	ChordedActionTriggerBlocker::ChordedActionTriggerBlocker()
		: ChordedActionTrigger()
	{
	}

	ChordedActionTriggerBlocker::ChordedActionTriggerBlocker(const Weak<InputAction>& action)
		: ChordedActionTrigger(action)
	{
	}
}
