#pragma once
#include "Common.h"
#include "Key.h"
#include "core/containers/HashSet.h"

namespace Core::Input
{
	struct RebindState;
	class MappingContext;
	class InputAction;
	/**
	 * Context used to define how to map a binding
	 */
	class RebindContext
	{
	public:

		using OnCompleteCallback = Delegate<void(Weak<InputAction>, const Key&)>;
		using OnCancelCallback = Delegate<void()>;
		
	public:
		/**
		 * Create an invalid mapping context
		 */
		RebindContext();
		/**
		 * Create a mapping context
		 * \param[in] name Name of the binding to map
		 */
		RebindContext(const Weak<InputAction>& action, const String& name);

		/**
		 * Get the name of the binding that is currently being mapped
		 * \return Name of the binding being mapped
		 */
		auto GetPath() const noexcept -> const String& { return m_name; }
		/**
		 * Get the input action
		 * \return Input action
		 */
		auto GetAction() const noexcept -> Weak<InputAction> { return m_action; }

		/**
		 * Set the flags the key is allowed to have
		 * \param[in] flags Allowed flags
		 * \return Reference to the mapping context
		 */
		auto WithAllowedKeyFlags(KeyFlags flags) noexcept -> RebindContext&;
		/**
		 * Set the allowed value type
		 * \param[in] type Value type
		 * \return Reference to the mapping context
		 */
		auto WithValueType(ValueType type) noexcept -> RebindContext&;
		/**
		 * Add a key to ignore for the mapping
		 * \param[in] key Key to ignore
		 * \return Reference to the mapping context
		 */
		auto WithIgnoredKey(const Key& key) noexcept -> RebindContext&;
		/**
		 * Add a key that cancels the binding operation
		 * \param[in] key Key
		 * \return Reference to the mapping context
		 */
		auto WithCancelKey(const Key& key) noexcept -> RebindContext&;
		/**
		 * Set the timeout time
		 * \param[in] timeout Timeout time
		 * \return Reference to the mapping context
		 */
		auto WithTimeout(f32 timeout) noexcept -> RebindContext&;
		/**
		 * Set the threshold a key needs to pass to be accepted
		 * \param[in] threshold Threshold
		 * \return Reference to the mapping context
		 */
		auto WithThreshold(f32 threshold) noexcept -> RebindContext&;
		/**
		 * Set the allowed modifiers for this binding
		 * \param[in] modifier Modifier key
		 * \return Reference to the mapping context
		 */
		auto WithAllowedModifiers(const Key& modifier) noexcept -> RebindContext&;

		/**
		 * Add a section that the path needs to contain (needs to contain only 1 of the given sections)
		 * \param[in] section Section that path needs to contain
		 * \return Reference to the mapping
		 */
		auto WithKeyPathContaining(const String& section) noexcept -> RebindContext&;

		/**
		 * Set callback to be called on each assignment (once per action)
		 * \param[in] callback Callback
		 * \return Reference to the mapping context
		 */
		auto OnComplete(const OnCompleteCallback& callback) noexcept -> RebindContext&;
		/**
		 * Set callback to be called when binding is cancelled
		 * \param[in] callback Callback
		 * \return Reference to the mapping context
		 */
		auto OnCancel(const OnCancelCallback& callback) noexcept -> RebindContext&;

		/**
		 * Check if the mapping context is valid
		 * \return Whether the mapping context is valid
		 */
		auto IsValid() const noexcept -> bool;
		/**
		 * Check if a key is a modifier for this rebind
		 * \param[in] key Key
		 * \return Whether the key is a modifier
		 */
		auto IsModifier(const Key& key) const noexcept -> bool;
		/**
		 * Check if the key is valid for the binding
		 * \param[in] key Key
		 * \param[in] value Current key value
		 * \return Whether the key is valid for the binding
		 */
		auto IsKeyValid(const Key& key, Value value) const noexcept -> bool;
		/**
		 * Check if a key cancels the binding
		 * \param[in] key Key
		 * \return Whether the key cancels the binding
		 */
		auto IsCancelKey(const Key& key) const noexcept -> bool;
		/**
		 * Check if the mapping is timed out
		 * \param[in] time Time since the beginning of the mapping
		 * \return Whether the mapping is timed out
		 */
		auto IsTimeout(f32 time) const noexcept -> bool;


		/**
		 * Invalidate the context
		 */
		void Invalidate() noexcept;

		/**
		 * Call the OnComplete callback
		 * \param[in] state Rebind state
		 */
		void Complete(RebindState& state);
		/**
		 * Call the OnComplete callback
		 * \param[in] state Rebind state
		 */
		void Cancel(RebindState& state);

	private:
		Weak<InputAction>  m_action;           ///< Action to bind
		String             m_name;             ///< Name of the binding to map
		KeyFlags           m_allowedFlags;     ///< Flags the given key is allowed to have
		ValueType          m_allowedValueType; ///< Allowed value type
		f32                m_timeout;          ///< Timeout time
		f32                m_threshold;        ///< Minimum threshold before key is accepted

		DynArray<String>   m_neededSections;   ///< Elements the path needs to contain (only 1)
		DynArray<Key>      m_allowedModifiers; ///< Allowed modifiers

		HashSet<Key>       m_ignoredKeys;      ///< Keys that are ignored for mapping the key
		HashSet<Key>       m_cancelKeys;       ///< Key that cancel the mapping

		OnCompleteCallback m_onComplete;       ///< On complete callback
		OnCancelCallback   m_onCancel;         ///< On cancel callback
	};

	/**
	 * Current state of rebinding
	 */
	struct RebindState
	{
		/**
		 * Clear the rebind state
		 */
		void Clear() noexcept;

		Weak<MappingContext> mapping;   ///< Mapping context
		Weak<InputAction>    action;    ///< Action
		String               path;      ///< Binding path
		Key                  key;       ///< Key
		DynArray<Key>        modifiers; ///< Current modifiers
		f32                  time;      ///< Time since the beginning of the rebind
	};
}
