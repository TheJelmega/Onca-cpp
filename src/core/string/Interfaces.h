#pragma once
#include "String.h"

namespace Onca
{
	template<typename T>
	concept ToStringMethod =
		requires(const T& t)
	{
		{ t.ToString() } noexcept -> SameAs<String>;
	};

	template<typename T>
	concept Stringifyable =
		ToStringMethod<T> ||
		requires(const T& t)
	{
		{ ToString(t) } noexcept -> SameAs<String>;
	};

	template<typename T>
	concept ToDebugStringMethod =
		requires(const T& t)
	{
		{ t.ToDebugString() } noexcept -> SameAs<String>;
	};

	template<typename T>
	concept DebugStringifyable =
		ToDebugStringMethod<T> ||
		requires(const T& t)
	{
		{ ToDebugString(t) } noexcept -> SameAs<String>;
	};

	template<typename T>
	concept ToFormatableMethod =
		requires(const T& t, const String & options)
	{
		{ t.ToFormat(options) } noexcept -> SameAs<String>;
	};

	template<typename T>
	concept Formatable =
		ToFormatableMethod<T> ||
		requires(const T& t, const String & options)
	{
		{ ToFormat(t, options) } noexcept -> SameAs<String>;
	};

	template<typename T>
	concept Parsable =
		requires(const String & str)
	{
		{ Parse<T>(str) } noexcept -> SameAs<T>;
	};

	template<typename T>
	concept TryParsable =
		requires(const String & str)
	{
		{ TryParse<T>(str) } noexcept -> SameAs<Optional<T>>;
	};
}
