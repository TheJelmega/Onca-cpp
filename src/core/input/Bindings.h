#pragma once
#include "Common.h"
#include "Key.h"
#include "core/memory/Unique.h"

namespace Onca::Input
{
	class InputManager;
	class User;


	/**
	 * Winning key if both positive and negative are actuated
	 */
	enum class WinningKey : u8
	{
		Positive, ///< Positive key should win
		Negative, ///< Negative key should win
		Neither , ///< Neither value should win (sum of keys)
	};

	/**
	 * How should multi axis bindings interpret the values
	 */
	enum class MultiAxisType : u8
	{
		DigitalNormalized, ///< Keys are handled as digital values (0 or 1), resulting vector is normalized
		Digital          , ///< Keys are handled as digital values (0 or 1)
		Analog           , ///< Keys are handled as analog values
	};

	class Binding
	{
	public:
		virtual ~Binding() noexcept {}

		/**
		 * Get the raw value from the binding
		 * \param[in] pManager Input manager
		 * \param[in] user User
		 * \return Value
		 */
		virtual auto GetValue(InputManager* pManager, User& user) const noexcept -> Value = 0;
		/**
		 * Bind a key to the binding
		 * \param[in] path Path to binding point
		 * \param[in] key Key
		 */
		virtual void BindKey(const String& path, const Key& key) noexcept = 0;
		/**
		 * Get the keys that are used by the binding
		 * \return Keys
		 */
		virtual auto GetKeys() const noexcept -> DynArray<Key> = 0;
		/**
		 * Clone the binding
		 * \return Binding
		 */
		virtual auto Clone() const noexcept -> Unique<Binding> = 0;
	};

	class KeyBinding final : public Binding
	{
	public:
		KeyBinding(const Key& key) noexcept;

		/**
		 * Get the raw value from the binding
		 * \param[in] pManager Input manager
		 * \param[in] user User
		 * \return Value
		 */
		auto GetValue(InputManager* pManager, User& user) const noexcept -> Value override;
		/**
		 * Bind a key to the binding
		 * \param[in] path Path to binding point
		 * \param[in] key Key
		 */
		void BindKey(const String& path, const Key& key) noexcept override;
		/**
		 * Get the keys that are used by the binding
		 * \return Keys
		 */
		auto GetKeys() const noexcept -> DynArray<Key> override;
		/**
		 * Clone the binding
		 * \return Binding
		 */
		auto Clone() const noexcept -> Unique<Binding> override;

	private:
		Key m_key; ///< Key
	};

	class KeyAxisBinding final : public Binding
	{
	public:
		KeyAxisBinding(const Key& neg, const Key& pos, WinningKey winning = WinningKey::Neither) noexcept;

		/**
		 * Get the raw value from the binding
		 * \param[in] pManager Input manager
		 * \param[in] user User
		 * \return Value
		 */
		auto GetValue(InputManager* pManager, User& user) const noexcept -> Value override;
		/**
		 * Bind a key to the binding
		 * \param[in] path Path to binding point
		 * \param[in] key Key
		 */
		void BindKey(const String& path, const Key& key) noexcept override;
		/**
		 * Get the keys that are used by the binding
		 * \return Keys
		 */
		auto GetKeys() const noexcept -> DynArray<Key> override;
		/**
		 * Clone the binding
		 * \return Binding
		 */
		auto Clone() const noexcept -> Unique<Binding> override;

	private:
		Key        m_neg;     ///< Negative key
		Key        m_pos;     ///< Positive key
		WinningKey m_winning; ///< Winning key
	};

	class MultiKeyAxisBinding final : public Binding
	{
	public:
		MultiKeyAxisBinding(ValueType valType, MultiAxisType type = MultiAxisType::DigitalNormalized) noexcept;
		MultiKeyAxisBinding(const Key& up, const Key& down, const Key& left, const Key& right, MultiAxisType type = MultiAxisType::DigitalNormalized) noexcept;
		MultiKeyAxisBinding(const Key& up, const Key& down, const Key& left, const Key& right, const Key& forward, const Key& backward, MultiAxisType type = MultiAxisType::DigitalNormalized) noexcept;

		/**
		 * Get the raw value from the binding
		 * \param[in] pManager Input manager
		 * \param[in] user User
		 * \return Value
		 */
		auto GetValue(InputManager* pManager, User& user) const noexcept -> Value override;
		/**
		 * Bind a key to the binding
		 * \param[in] path Path to binding point
		 * \param[in] key Key
		 */
		void BindKey(const String& path, const Key& key) noexcept override;
		/**
		 * Get the keys that are used by the binding
		 * \return Keys
		 */
		auto GetKeys() const noexcept -> DynArray<Key> override;
		/**
		 * Clone the binding
		 * \return Binding
		 */
		auto Clone() const noexcept -> Unique<Binding> override;

	private:
		Key           m_up;       ///< Up/positive Y key
		Key           m_down;     ///< Down/negative Y key
		Key           m_left;     ///< Left/negative X key
		Key           m_right;    ///< Right/positive X key
		Key           m_forward;  ///< Forward/positive Z key
		Key           m_backward; ///< Backward/negative Z key
		MultiAxisType m_type;     ///< Multi-axis type
		ValueType     m_valType;  ///< Value type
	};

	class CompositeAxisBinding final : public Binding
	{
	public:
		CompositeAxisBinding(Unique<Binding>&& axisX, Unique<Binding> axisY);
		CompositeAxisBinding(Unique<Binding>&& axisX, Unique<Binding> axisY, Unique<Binding>&& axisZ);

		CompositeAxisBinding(const CompositeAxisBinding& binding) noexcept;
		CompositeAxisBinding(CompositeAxisBinding&&) noexcept = default;

		/**
		 * Get the raw value from the binding
		 * \param[in] pManager Input manager
		 * \param[in] user User
		 * \return Value
		 */
		auto GetValue(InputManager* pManager, User& user) const noexcept -> Value override;
		/**
		 * Bind a key to the binding
		 * \param[in] path Path to binding point
		 * \param[in] key Key
		 */
		void BindKey(const String& path, const Key& key) noexcept override;
		/**
		 * Get the keys that are used by the binding
		 * \return Keys
		 */
		auto GetKeys() const noexcept -> DynArray<Key> override;
		/**
		 * Clone the binding
		 * \return Binding
		 */
		auto Clone() const noexcept -> Unique<Binding> override;

	private:
		Unique<Binding> m_x; ///< X axis
		Unique<Binding> m_y; ///< Y axis
		Unique<Binding> m_z; ///< Z axis
	};
}
