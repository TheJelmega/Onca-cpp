#include "Bindings.h"

#include "InputManager.h"
#include "User.h"

namespace Core::Input
{
	KeyBinding::KeyBinding(const Key& key) noexcept
		: m_key(key)
	{
	}

	auto KeyBinding::GetValue(InputManager* pManager, User& user) const noexcept -> Value
	{
		return pManager->GetRawValue(user, m_key);
	}

	void KeyBinding::BindKey(const String& path, const Key& key) noexcept
	{
		// Path should be empty when we reach this stage of the binding, so ignore it
		m_key = key;
	}

	auto KeyBinding::GetKeys() const noexcept -> DynArray<Key>
	{
		return DynArray{ { m_key } };
	}

	auto KeyBinding::Clone() const noexcept -> Unique<Binding>
	{
		return Unique<KeyBinding>::Create(*this);
	}

	KeyAxisBinding::KeyAxisBinding(const Key& neg, const Key& pos, WinningKey winning) noexcept
		: m_neg(neg)
		, m_pos(pos)
		, m_winning(winning)
	{
	}

	auto KeyAxisBinding::GetValue(InputManager* pManager, User& user) const noexcept -> Value
	{
		Value neg = pManager->GetRawValue(user, m_neg);
		if (!neg.value.IsNearlyZero() && m_winning == WinningKey::Negative)
			return neg;

		Value pos = pManager->GetRawValue(user, m_pos);
		return m_winning == WinningKey::Positive ? pos : Value{ pos.value - neg.value, neg.type };
	}

	void KeyAxisBinding::BindKey(const String& path, const Key& key) noexcept
	{
		// Path should either be "neg" or "pos"
		if (path == "neg")
			m_neg = key;
		else
			m_pos = key;
	}

	auto KeyAxisBinding::GetKeys() const noexcept -> DynArray<Key>
	{
		return DynArray{ { m_neg, m_pos } };
	}

	auto KeyAxisBinding::Clone() const noexcept -> Unique<Binding>
	{
		return Unique<KeyAxisBinding>::Create(*this);
	}

	MultiKeyAxisBinding::MultiKeyAxisBinding(ValueType valType, MultiAxisType type) noexcept
		: m_type(type)
		, m_valType(valType)
	{
		ASSERT(valType == ValueType::Axis2D || valType == ValueType::Axis3D, "Invalid value type, must be 2D or 3D");
	}

	MultiKeyAxisBinding::MultiKeyAxisBinding(const Key& up, const Key& down, const Key& left, const Key& right, MultiAxisType type) noexcept
		: m_up(up)
		, m_down(down)
		, m_left(left)
		, m_right(right)
		, m_type(type)
		, m_valType(ValueType::Axis2D)
	{
	}

	MultiKeyAxisBinding::MultiKeyAxisBinding(const Key& up, const Key& down, const Key& left, const Key& right, const Key& forward, const Key& backward, MultiAxisType type) noexcept
		: m_up(up)
		, m_down(down)
		, m_left(left)
		, m_right(right)
		, m_forward(forward)
		, m_backward(backward)
		, m_type(type)
		, m_valType(ValueType::Axis3D)
	{
	}

	auto MultiKeyAxisBinding::GetValue(InputManager* pManager, User& user) const noexcept -> Value
	{
		if (m_forward.path.IsEmpty()) // 2D
		{
			Value negY = pManager->GetRawValue(user, m_down);
			Value posY = pManager->GetRawValue(user, m_up);
			Value negX = pManager->GetRawValue(user, m_left);
			Value posX = pManager->GetRawValue(user, m_right);

			switch (m_type)
			{
			case MultiAxisType::DigitalNormalized:
			case MultiAxisType::Digital:
			{
				f32 x = f32(!posX.value.IsNearlyZero()) - f32(!negX.value.IsNearlyZero());
				f32 y = f32(!posY.value.IsNearlyZero()) - f32(!negY.value.IsNearlyZero());

				f32v2 val{ x, y };
				if (m_type == MultiAxisType::DigitalNormalized)
					val.Normalize();
				return val;
			}
			case MultiAxisType::Analog:
			{
				return Value{ f32v2{ posX.value.x - negX.value.x, posY.value.x - negY.value.x } };
			}
			}
		}
		else // 3D
		{
			Value negY = pManager->GetRawValue(user, m_down);
			Value posY = pManager->GetRawValue(user, m_up);
			Value negX = pManager->GetRawValue(user, m_left);
			Value posX = pManager->GetRawValue(user, m_right);
			Value posZ = pManager->GetRawValue(user, m_forward);
			Value negZ = pManager->GetRawValue(user, m_backward);

			switch (m_type)
			{
			case MultiAxisType::DigitalNormalized:
			case MultiAxisType::Digital:
			{
				f32 x = f32(!posX.value.IsNearlyZero()) - f32(!negX.value.IsNearlyZero());
				f32 y = f32(!posY.value.IsNearlyZero()) - f32(!negY.value.IsNearlyZero());
				f32 z = f32(!posZ.value.IsNearlyZero()) - f32(!negZ.value.IsNearlyZero());

				f32v3 val{ x, y, z };
				if (m_type == MultiAxisType::DigitalNormalized)
					val.Normalize();
				return val;
			}
			case MultiAxisType::Analog:
			{
				return Value{ f32v3{ posX.value.x - negX.value.x, posY.value.x - negY.value.x, posZ.value.x - negZ.value.x } };
			}
			}
		}
		return Value{};
	}

	void MultiKeyAxisBinding::BindKey(const String& path, const Key& key) noexcept
	{
		StringId id{ path };
		switch (id)
		{
		case "up"_sid      :                                     m_up       = key; break;
		case "down"_sid    :                                     m_down     = key; break;
		case "left"_sid    :                                     m_left     = key; break;
		case "right"_sid   :                                     m_right    = key; break;
		case "forward"_sid : if (m_valType == ValueType::Axis3D) m_forward  = key; break;
		case "backward"_sid: if (m_valType == ValueType::Axis3D) m_backward = key; break;
		default: break;
		}
	}

	auto MultiKeyAxisBinding::GetKeys() const noexcept -> DynArray<Key>
	{
		if (m_valType == ValueType::Axis3D)
			return DynArray{ { m_up, m_down, m_left, m_right } };
		return DynArray{ { m_up, m_down, m_left, m_right, m_forward, m_backward } };
	}

	auto MultiKeyAxisBinding::Clone() const noexcept -> Unique<Binding>
	{
		return Unique<MultiKeyAxisBinding>::Create(*this);
	}

	CompositeAxisBinding::CompositeAxisBinding(Unique<Binding>&& axisX, Unique<Binding> axisY)
		: m_x(Move(axisX))
		, m_y(Move(axisY))
	{
	}

	CompositeAxisBinding::CompositeAxisBinding(Unique<Binding>&& axisX, Unique<Binding> axisY, Unique<Binding>&& axisZ)
		: m_x(Move(axisX))
		, m_y(Move(axisY))
		, m_z(Move(axisZ))
	{
	}

	CompositeAxisBinding::CompositeAxisBinding(const CompositeAxisBinding& binding) noexcept
		: m_x(binding.m_x->Clone())
		, m_y(binding.m_y->Clone())
		, m_z(binding.m_z->Clone())
	{
	}

	auto CompositeAxisBinding::GetValue(InputManager* pManager, User& user) const noexcept -> Value
	{
		Value x = m_x->GetValue(pManager, user);
		Value y = m_y->GetValue(pManager, user);

		if (!m_z)
			return f32v2{ x.value.x, y.value.x };

		Value z = m_z->GetValue(pManager, user);
		return f32v3{ x.value.x, y.value.x, z.value.x };
	}

	void CompositeAxisBinding::BindKey(const String& path, const Key& key) noexcept
	{
		// Path is expected to conform to "x/<other>", where 'x' can also be 'y' or 'z'
		if (path.Length() < 2 || path[1] != '/')
			return;

		switch (path[0])
		{
		case 'x': m_x->BindKey(path.SubString(2), key); break;
		case 'y': m_y->BindKey(path.SubString(2), key); break;
		case 'z': if (m_z) m_z->BindKey(path.SubString(2), key); break;
		default: break;
		}
	}

	auto CompositeAxisBinding::GetKeys() const noexcept -> DynArray<Key>
	{
		DynArray<Key> keys;
		keys.Add(m_x->GetKeys());
		keys.Add(m_y->GetKeys());
		if (m_z)
			keys.Add(m_z->GetKeys());
		return keys;
	}

	auto CompositeAxisBinding::Clone() const noexcept -> Unique<Binding>
	{
		return Unique<CompositeAxisBinding>::Create(*this);
	}
}
