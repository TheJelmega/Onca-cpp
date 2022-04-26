#include "Rebind.h"

namespace Core::Input
{
	RebindContext::RebindContext()
		: m_timeout(0.f)
		, m_threshold(0.5f)
	{
	}

	RebindContext::RebindContext(const Weak<InputAction>& action, const String& name)
		: m_action(action)
		, m_name(name)
		, m_allowedFlags(KeyFlag::All)
		, m_timeout(0.f)
		, m_threshold(.5f)
	{
	}

	auto RebindContext::WithAllowedKeyFlags(KeyFlags flags) noexcept -> RebindContext&
	{
		m_allowedFlags = flags;
		return *this;
	}

	auto RebindContext::WithValueType(ValueType type) noexcept -> RebindContext&
	{
		m_allowedValueType = type;
		return *this;
	}

	auto RebindContext::WithIgnoredKey(const Key& key) noexcept -> RebindContext&
	{
		m_ignoredKeys.Insert(key);
		return *this;
	}

	auto RebindContext::WithCancelKey(const Key& key) noexcept -> RebindContext&
	{
		m_cancelKeys.Insert(key);
		return *this;
	}

	auto RebindContext::WithTimeout(f32 timeout) noexcept -> RebindContext&
	{
		m_timeout = timeout;
		return *this;
	}

	auto RebindContext::WithThreshold(f32 threshold) noexcept -> RebindContext&
	{
		m_threshold = threshold;
		return *this;
	}

	auto RebindContext::WithAllowedModifiers(const Key& modifier) noexcept -> RebindContext&
	{
		m_allowedModifiers.AddUnique(modifier);
		return *this;
	}

	auto RebindContext::WithKeyPathContaining(const String& section) noexcept -> RebindContext&
	{
		m_neededSections.Add(section);
		return *this;
	}

	auto RebindContext::OnComplete(const OnCompleteCallback& callback) noexcept -> RebindContext&
	{
		m_onComplete = callback;
		return *this;
	}

	auto RebindContext::OnCancel(const OnCancelCallback& callback) noexcept -> RebindContext&
	{
		m_onCancel = callback;
		return *this;
	}

	auto RebindContext::IsValid() const noexcept -> bool
	{
		return !m_name.IsEmpty();
	}

	auto RebindContext::IsModifier(const Key& key) const noexcept -> bool
	{
		return m_allowedModifiers.Contains(key);
	}

	auto RebindContext::IsKeyValid(const Key& key, Value value) const noexcept -> bool
	{
		// We can't have a key that has a flag that isn't allowed
		if (key.details->flags & ~m_allowedFlags)
			return false;
		if (key.GetInputValueType() != m_allowedValueType)
			return false;
		if (m_ignoredKeys.Contains(key))
			return false;

		bool foundSection = m_neededSections.IsEmpty() ||
			m_neededSections.ContainsIf([&key](const String& section) -> bool
			{
				return key.path.Contains(section);
			});
		if (!foundSection)
			return false;

		return value.value.LenSq() > m_threshold * m_threshold;
	}

	auto RebindContext::IsCancelKey(const Key& key) const noexcept -> bool
	{
		return m_cancelKeys.Contains(key);
	}

	auto RebindContext::IsTimeout(f32 time) const noexcept -> bool
	{
		return m_timeout == 0.f || time < m_timeout;
	}

	void RebindContext::Invalidate() noexcept
	{
		m_name.Clear(true);
		m_allowedFlags = KeyFlag::All;
		m_ignoredKeys.Clear(true);
		m_cancelKeys.Clear(true);
	}

	void RebindContext::Complete(RebindState& state)
	{
		if (m_onComplete)
			m_onComplete(state.action, state.key);
		state.Clear();
		Invalidate();
	}

	void RebindContext::Cancel(RebindState& state)
	{
		if (m_onCancel)
			m_onCancel();
		state.Clear();
		Invalidate();
	}

	void RebindState::Clear() noexcept
	{
		mapping = nullptr;
		action = nullptr;
		path.Clear(true);
		key = Key{};
		modifiers.Clear(true);
	}
}
