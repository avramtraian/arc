/*
 * Copyright (c) 2024-2025 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/containers/optional.h>
#include <core/containers/string.h>

namespace Arc {

class InternalError {
    ARC_MAKE_NONCOPYABLE(InternalError);

public:
    InternalError() = default;
    InternalError(InternalError&&) noexcept = default;
    InternalError& operator=(InternalError&&) noexcept = delete;

    ALWAYS_INLINE InternalError(StringView filepath, u32 line_index)
        : m_filepath(filepath)
        , m_line_index(line_index)
    {}

    ALWAYS_INLINE InternalError(StringView filepath, u32 line_index, String error_message)
        : m_filepath(filepath)
        , m_line_index(line_index)
        , m_error_message(move(error_message))
    {}

public:
    NODISCARD ALWAYS_INLINE Optional<StringView> filepath() const { return m_filepath; }
    NODISCARD ALWAYS_INLINE Optional<u32> line_index() const { return m_line_index; }
    NODISCARD ALWAYS_INLINE Optional<String> error_message() const { return m_error_message; }

private:
    Optional<StringView> m_filepath;
    Optional<u32> m_line_index;
    Optional<String> m_error_message;
};

template<typename T, typename ErrorType = InternalError>
requires (!std::is_same_v<T, ErrorType>)
class NODISCARD ErrorOr {
    ARC_MAKE_NONCOPYABLE(ErrorOr);
    ARC_MAKE_NONMOVABLE(ErrorOr);

public:
    ALWAYS_INLINE ErrorOr(T value)
        : m_is_error(false)
        , m_value(move(value))
    {}

    ALWAYS_INLINE ErrorOr(ErrorType error)
        : m_is_error(true)
        , m_error(move(error))
    {}

    ALWAYS_INLINE ~ErrorOr()
    {
        if (m_is_error)
            m_error.~ErrorType();
        else
            m_value.~T();
    }

public:
    NODISCARD ALWAYS_INLINE bool is_error() const { return m_is_error; }

    NODISCARD ALWAYS_INLINE T release_value()
    {
        ARC_ASSERT_DEBUG(!is_error());
        return move(m_value);
    }

    NODISCARD ALWAYS_INLINE ErrorType release_error()
    {
        ARC_ASSERT_DEBUG(is_error());
        return move(m_error);
    }

private:
    bool m_is_error;
    union {
        T m_value;
        ErrorType m_error;
    };
};

template<typename ErrorType>
class NODISCARD ErrorOr<void, ErrorType> {
    ARC_MAKE_NONCOPYABLE(ErrorOr);
    ARC_MAKE_NONMOVABLE(ErrorOr);

public:
    ALWAYS_INLINE ErrorOr()
        : m_is_error(false)
        , m_error_storage {}
    {}

    ALWAYS_INLINE ErrorOr(ErrorType error)
        : m_is_error(true)
    {
        new (m_error_storage) ErrorType(move(error));
    }

    ALWAYS_INLINE ~ErrorOr()
    {
        if (m_is_error)
            reinterpret_cast<ErrorType*>(m_error_storage)->~ErrorType();
    }

public:
    NODISCARD ALWAYS_INLINE bool is_error() const { return m_is_error; }

    ALWAYS_INLINE void release_value()
    {
        ARC_ASSERT_DEBUG(!is_error());
        return;
    }

    NODISCARD ALWAYS_INLINE ErrorType release_error()
    {
        ARC_ASSERT_DEBUG(is_error());
        return move(*reinterpret_cast<ErrorType*>(m_error_storage));
    }

private:
    bool m_is_error;
    alignas(ErrorType) u8 m_error_storage[sizeof(ErrorType)];
};

#define ARC_INTERNAL_ERROR()                           ::Arc::InternalError(ARC_CONCATENATE(__FILE__, sv), __LINE__)
#define ARC_INTERNAL_ERROR_WITH_MESSAGE(error_message) ::Arc::InternalError(ARC_CONCATENATE(__FILE__, sv), __LINE__, error_message)

#define TRY(expression)                                                  \
    {                                                                    \
        auto ARC_CONCATENATE(_error_or, __LINE__) = expression;          \
        if (ARC_CONCATENATE(_error_or, __LINE__).is_error())             \
            return ARC_CONCATENATE(_error_or, __LINE__).release_error(); \
    }

#define TRY_ASSIGN(_lhs, _rhs)                                       \
    auto ARC_CONCATENATE(_error_or, __LINE__) = _rhs;                \
    if (ARC_CONCATENATE(_error_or, __LINE__).is_error())             \
        return ARC_CONCATENATE(_error_or, __LINE__).release_error(); \
    _lhs = ARC_CONCATENATE(_error_or, __LINE__).release_value();

#define TRY_RETURN(expression)                                           \
    {                                                                    \
        auto ARC_CONCATENATE(_error_or, __LINE__) = expression;          \
        if (ARC_CONCATENATE(_error_or, __LINE__).is_error())             \
            return ARC_CONCATENATE(_error_or, __LINE__).release_error(); \
        return ARC_CONCATENATE(_error_or, __LINE__).release_value();     \
    }
}
