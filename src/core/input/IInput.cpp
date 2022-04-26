#include "IInput.h"

#include "InputManager.h"
#include "User.h"

namespace Core::Input
{
	auto ITimedTrigger::Tick(User& user, Value value, Chrono::DeltaTime dt) noexcept -> TriggerState
	{
		if (IsActuated(value))
		{
			m_time += dt.GetTime(m_useTimeDilation);
			return TriggerState::Ongoing;
		}

		m_time = 0.f;
		return TriggerState::None;
	}
}
