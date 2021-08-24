#pragma once
#include <atomic>
#include <optional>
#include <stdint.h>
#include <tuple>

using i8    = int8_t;
using i16   = int16_t;
using i32   = int32_t;
using i64   = int64_t;
using u8    = uint8_t;
using u16   = uint16_t;
using u32   = uint32_t;
using u64   = uint64_t;
using f32   = float;
using f64   = double;
using isize = ptrdiff_t;
using usize = uintptr_t;

template<typename T>
using InitializerList = std::initializer_list<T>;

// TODO: Custom version to allow references (no implicit rebind)
template<typename T>
using Optional = std::optional<T>;
inline constexpr std::nullopt_t NullOpt{ std::nullopt_t::_Tag{} };

template<typename ...T>
using Tuple = std::tuple<T...>;
template<typename T, usize N>
using Array = std::array<T, N>;