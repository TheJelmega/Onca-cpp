#pragma once

#if __RESHARPER__
#include "MathUtils.h"
#endif

namespace Onca::Math
{
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Min(const Intrin::Pack<T, Width>& a, const Intrin::Pack<T, Width>& b) noexcept -> Intrin::Pack<T, Width>
	{
		return a.Min(b);
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto Max(const Intrin::Pack<T, Width>& a, const Intrin::Pack<T, Width>& b) noexcept -> Intrin::Pack<T, Width>
	{
		return a.Max(b);
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto Clamp(const Intrin::Pack<T, Width>& val, const Intrin::Pack<T, Width>& min, const Intrin::Pack<T, Width>& max) noexcept -> Intrin::Pack<T, Width>
	{
		using Pack = Intrin::Pack<T, Width>;
		Pack minCmp = val.Compare<Intrin::ComparisonOp::Lt>(min);
		Pack maxCmp = val.Compare<Intrin::ComparisonOp::Gt>(max);
		return val.Blend(min, minCmp).Blend(max, maxCmp);
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto Lerp(const Intrin::Pack<T, Width>& a, const Intrin::Pack<T, Width>& b, const Intrin::Pack<T, Width>& i) noexcept -> Intrin::Pack<T, Width>
	{
		using Pack = Intrin::Pack<T, Width>;
		Pack one = Pack::Set(1);
		return a * (one - i) + b * i;
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto SmoothStep(const Intrin::Pack<T, Width>& min, const Intrin::Pack<T, Width>& max, const Intrin::Pack<T, Width>& i) noexcept -> Intrin::Pack<T, Width>
	{
		using Pack = Intrin::Pack<T, Width>;
		Pack zero = Pack::Zero();
		Pack one = Pack::Set(1);
		Pack two = Pack::Set(2);
		Pack three = Pack::Set(3);

		Pack tmp = Clamp((i - min) / (max - min), zero, one);
		return tmp * tmp * (three - two * tmp);
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto Step(const Intrin::Pack<T, Width>& edge, const Intrin::Pack<T, Width>& i) noexcept -> Intrin::Pack<T, Width>
	{
		using Pack = Intrin::Pack<T, Width>;
		Pack zero = Pack::Zero();
		Pack one = Pack::Set(1);

		Pack cmp = i.Compare<Intrin::ComparisonOp::Ge>(edge);
		return zero.Blend(one, cmp);
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto Abs(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>
	{
		return pack.Abs();
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto Ceil(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>
	{
		return pack.Ceil();
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto Floor(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>
	{
		return pack.Floor();
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto Trunc(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>
	{
		return pack.Trunc();
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto Round(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>
	{
		return pack.Trunc();
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto RoundEven(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>
	{
		return pack.RoundEven();
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto Sign(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>
	{
		return pack.Sign();
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto Fract(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>
	{
		return pack.Sub(pack.Trunc());
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto Mod(const Intrin::Pack<T, Width>& a, const Intrin::Pack<T, Width>& b) noexcept -> Intrin::Pack<T, Width>
	{
		// TODO: Is there a better SIMD implementation?
		using Pack = Intrin::Pack<T, Width>;
		Pack div = a.Div(b).Trunc();
		Pack mul = a * b;
		return a - mul;
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto ModF(const Intrin::Pack<T, Width>& pack) noexcept -> Pair<Intrin::Pack<T, Width>, Intrin::Pack<T, Width>>
	{
		using Pack = Intrin::Pack<T, Width>;
		Pack trunc = pack.Trunc();
		Pack fract = pack - trunc;
		return { trunc, pack };
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto Rcp(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>
	{
		return pack.Rcp();
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto Sqrt(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>
	{
		return pack.Sqrt();
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto RSqrt(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>
	{
		return pack.RSqrt();
	}

	template <Intrin::SimdBaseType T, usize Width>
	constexpr auto EpsilonCompare(const Intrin::Pack<T, Width>& a, const Intrin::Pack<T, Width>& b, T e) noexcept -> Intrin::Pack<T, Width>
	{
		using Pack = Intrin::Pack<T, Width>;
		Pack absDiff = a.Sub(b).Abs();
		Pack epsilon = Pack::Set(e);
		return absDiff.Compare<Intrin::ComparisonOp::Lt>(epsilon);
	}
}
