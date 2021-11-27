#pragma once
#include "core/MinInclude.h"
#include "core/utils/Utils.h"

namespace Core
{
	namespace Detail
	{
		template<typename T>
		concept EnumFlagHasAllVal
			= requires()
		{
			{ T::All };
		};

		template<typename T>
		constexpr bool DoesSupportEnumFlagOps = false;

		template<typename T>
		concept SupportsEnumFlagOps = DoesSupportEnumFlagOps<T>;
	}

	template<EnumType E>
	class Flags
	{
		using I = UnderlyingType<E>;
	public:
		constexpr Flags() noexcept;
		constexpr Flags(E val) noexcept;
		constexpr Flags(I val) noexcept;
		constexpr Flags(const InitializerList<E>& enums) noexcept;

		constexpr Flags(const Flags&) noexcept = default;
		constexpr Flags(Flags&&) noexcept = default;

		constexpr auto operator!() const noexcept -> bool;

		constexpr auto operator~() const noexcept -> Flags;

		constexpr auto operator|(Flags other) const noexcept -> Flags;
		constexpr auto operator|(E other) const noexcept -> Flags;
		constexpr auto operator|(I mask) const noexcept -> Flags;

		constexpr auto operator&(Flags other) const noexcept -> Flags;
		constexpr auto operator&(E other) const noexcept -> Flags;
		constexpr auto operator&(I mask) const noexcept -> Flags;

		constexpr auto operator^(Flags other) const noexcept -> Flags;
		constexpr auto operator^(E other) const noexcept -> Flags;
		constexpr auto operator^(I mask) const noexcept -> Flags;

		constexpr auto operator=(const Flags& other) noexcept -> Flags&;
		constexpr auto operator=(Flags&& other) noexcept -> Flags&;

		constexpr auto operator|=(Flags other) noexcept -> Flags&;
		constexpr auto operator|=(E other) noexcept -> Flags&;
		constexpr auto operator|=(I mask) noexcept -> Flags&;

		constexpr auto operator&=(Flags other) noexcept -> Flags&;
		constexpr auto operator&=(E other) noexcept -> Flags&;
		constexpr auto operator&=(I mask) noexcept -> Flags&;

		constexpr auto operator^=(Flags other) noexcept -> Flags&;
		constexpr auto operator^=(E other) noexcept -> Flags&;
		constexpr auto operator^=(I mask) noexcept -> Flags&;

		constexpr auto operator==(Flags other) const noexcept -> bool;
		constexpr auto operator==(E other) const noexcept -> bool;

		constexpr auto operator!=(Flags other) const noexcept -> bool;
		constexpr auto operator!=(E other) const noexcept -> bool;

		constexpr operator E() const noexcept;
		constexpr operator I() const noexcept;

		constexpr auto Set(E val, bool set) noexcept -> void;
		constexpr auto IsSet(E val) const noexcept -> bool;
		constexpr auto IsSet(Flags flags) const noexcept -> bool;

		constexpr auto None() const noexcept -> bool;
		constexpr auto Any() const noexcept -> bool;
		constexpr auto All() const noexcept -> bool;

	private:
		I m_data;
	};

	template<typename E>
	struct Hash<Flags<E>>
	{
		constexpr auto operator()(const Flags<E>& flags) const noexcept -> u64;
	};

	template<Detail::SupportsEnumFlagOps E>
	constexpr auto operator|(E a, E b) noexcept -> Flags<E>
	{
		using I = UnderlyingType<E>;
		return { E(I(a) | i(b)) };
	}
	
}

#include "Flags.inl"

#define DEFINE_ENUM_FLAG_OPS(enum) \
namespace Core::Detail \
{ \
	template<> \
	constexpr bool DoesSupportEnumFlagOps<enum> = true; \
}

#define DEFINE_FLAGS(enum) \
	using enum##s = ::Core::Flags<enum>;



