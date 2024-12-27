/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/defines.h>

// Headers from the standard library.
#include <cstdarg>
#include <initializer_list>
#include <new>
#include <type_traits>

namespace arc {

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using s8 = signed char;
using s16 = signed short;
using s32 = signed int;
using s64 = signed long long;

using f32 = float;
using f64 = double;

using usize = unsigned long long;
using ssize = signed long long;
using uintptr = unsigned long long;

using ReadonlyByte = const u8;
using WriteonlyByte = u8;
using ReadWriteByte = u8;

using ReadonlyBytes = ReadonlyByte*;
using WriteonlyBytes = WriteonlyByte*;
using ReadWriteBytes = ReadWriteByte*;

using NullptrType = decltype(nullptr);

namespace impl {
// clang-format off
template<typename T> struct RemoveReference         { using Type = T; };
template<typename T> struct RemoveReference<T&>     { using Type = T; };
template<typename T> struct RemoveReference<T&&>    { using Type = T; };

template<typename T> struct RemoveConst             { using Type = T; };
template<typename T> struct RemoveConst<const T>    { using Type = T; };

template<typename T> struct RemovePointer           { using Type = T; };
template<typename T> struct RemovePointer<T*>       { using Type = T; };

template<typename T> struct IsReference         { static constexpr bool value = false; };
template<typename T> struct IsReference<T&>     { static constexpr bool value = true; };
template<typename T> struct IsReference<T&&>    { static constexpr bool value = true; };

template<typename T> struct IsConst             { static constexpr bool value = false; };
template<typename T> struct IsConst<const T>    { static constexpr bool value = true; };

template<typename T> struct IsUnsignedInteger       { static constexpr bool value = false; };
template<>           struct IsUnsignedInteger<u8>   { static constexpr bool value = true; };
template<>           struct IsUnsignedInteger<u16>  { static constexpr bool value = true; };
template<>           struct IsUnsignedInteger<u32>  { static constexpr bool value = true; };
template<>           struct IsUnsignedInteger<u64>  { static constexpr bool value = true; };

template<typename T> struct IsSignedInteger       { static constexpr bool value = false; };
template<>           struct IsSignedInteger<s8>   { static constexpr bool value = true; };
template<>           struct IsSignedInteger<s16>  { static constexpr bool value = true; };
template<>           struct IsSignedInteger<s32>  { static constexpr bool value = true; };
template<>           struct IsSignedInteger<s64>  { static constexpr bool value = true; };

template<typename T> struct IsFloatingPoint         { static constexpr bool value = false; };
template<>           struct IsFloatingPoint<f32>    { static constexpr bool value = true; };
template<>           struct IsFloatingPoint<f64>    { static constexpr bool value = true; };

template<typename TypeIfTrue, typename TypeIfFalse, bool condition> struct ConditionalType {};
template<typename TypeIfTrue, typename TypeIfFalse>                 struct ConditionalType<TypeIfTrue, TypeIfFalse, true>   { using Type = TypeIfTrue; };
template<typename TypeIfTrue, typename TypeIfFalse>                 struct ConditionalType<TypeIfTrue, TypeIfFalse, false>  { using Type = TypeIfFalse; };
// clang-format on
}

template<typename T>
using RemoveReference = typename impl::RemoveReference<T>::Type;

template<typename T>
using RemoveConst = typename impl::RemoveConst<T>::Type;

template<typename T>
using RemovePointer = typename impl::RemovePointer<T>::Type;

template<typename T>
constexpr bool is_reference = impl::IsReference<T>::value;

template<typename T>
constexpr bool is_const = impl::IsConst<T>::value;

template<typename T>
constexpr bool is_unsigned_integer = impl::IsUnsignedInteger<T>::value;
template<typename T>
constexpr bool is_signed_integer = impl::IsSignedInteger<T>::value;
template<typename T>
constexpr bool is_integer = is_unsigned_integer<T> || is_signed_integer<T>;

template<typename T>
constexpr bool is_floating_point = impl::IsFloatingPoint<T>::value;

template<typename DerivedType, typename BaseType>
constexpr bool is_derived_from = std::is_base_of_v<BaseType, DerivedType>;

template<typename T>
constexpr bool is_trivially_destructible = std::is_trivially_destructible_v<T>;

template<typename TypeIfTrue, typename TypeIfFalse, bool condition>
using ConditionalType = typename impl::ConditionalType<TypeIfTrue, TypeIfFalse, condition>::Type;

// The STL equivalent of the move function. Same signature and behaviour.
// https://en.cppreference.com/w/cpp/utility/move
template<typename T>
NODISCARD ALWAYS_INLINE constexpr RemoveReference<T>&& move(T&& instance) noexcept
{
    return static_cast<RemoveReference<T>&&>(instance);
}

// The STL equivalent of the forward function. Same signature and behaviour.
// https://en.cppreference.com/w/cpp/utility/forward
template<typename T>
NODISCARD ALWAYS_INLINE constexpr T&& forward(RemoveReference<T>& instance) noexcept
{
    return static_cast<T&&>(instance);
}

// The STL equivalent of the forward function. Same signature and behaviour.
// https://en.cppreference.com/w/cpp/utility/forward
template<typename T>
NODISCARD ALWAYS_INLINE constexpr T&& forward(RemoveReference<T>&& instance) noexcept
{
    return static_cast<T&&>(instance);
}

#define ARC_MAKE_NONCOPYABLE(type_name)   \
    type_name(const type_name&) = delete; \
    type_name& operator=(const type_name&) = delete

#define ARC_MAKE_NONMOVABLE(type_name)        \
    type_name(type_name&&) noexcept = delete; \
    type_name& operator=(type_name&&) noexcept = delete

}
