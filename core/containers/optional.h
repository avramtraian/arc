/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/assertions.h>
#include <core/types.h>

namespace Arc {

template<typename T>
class Optional {
public:
    ALWAYS_INLINE Optional()
        : m_value_buffer {}
        , m_has_value(false)
    {}

    ALWAYS_INLINE Optional(const Optional& other)
        : m_has_value(other.m_has_value)
    {
        if (m_has_value)
            new (unchecked_value()) T(*other.unchecked_value());
    }

    ALWAYS_INLINE Optional(Optional&& other) noexcept
        : m_has_value(other.m_has_value)
    {
        if (m_has_value) {
            T& other_value = *other.unchecked_value();
            new (unchecked_value()) T(move(other_value));
            other_value.~T();
            other.m_has_value = false;
        }
    }

    ALWAYS_INLINE Optional(const T& value)
        : m_has_value(true)
    {
        new (unchecked_value()) T(value);
    }

    ALWAYS_INLINE Optional(T&& value)
        : m_has_value(true)
    {
        new (unchecked_value()) T(move(value));
    }

    ALWAYS_INLINE Optional& operator=(const Optional& other)
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        clear();
        m_has_value = other.m_has_value;
        if (m_has_value)
            new (unchecked_value()) T(*other.unchecked_value());

        return *this;
    }

    ALWAYS_INLINE Optional& operator=(Optional&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        clear();
        m_has_value = other.m_has_value;
        if (m_has_value) {
            T& other_value = *other.unchecked_value();
            new (unchecked_value()) T(move(other_value));
            other_value.~T();
            other.m_has_value = false;
        }

        return *this;
    }

    ALWAYS_INLINE Optional& operator=(const T& value)
    {
        clear();
        m_has_value = true;
        new (unchecked_value()) T(value);
        return *this;
    }

    ALWAYS_INLINE Optional& operator=(T&& value)
    {
        clear();
        m_has_value = true;
        new (unchecked_value()) T(move(value));
        return *this;
    }

public:
    NODISCARD ALWAYS_INLINE bool has_value() const { return m_has_value; }

    NODISCARD ALWAYS_INLINE T* operator->()
    {
        ARC_ASSERT(has_value());
        return unchecked_value();
    }

    NODISCARD ALWAYS_INLINE const T* operator->() const
    {
        ARC_ASSERT(has_value());
        return unchecked_value();
    }

    NODISCARD ALWAYS_INLINE T& value()
    {
        ARC_ASSERT(has_value());
        return *unchecked_value();
    }

    NODISCARD ALWAYS_INLINE const T& value() const
    {
        ARC_ASSERT(has_value());
        return *unchecked_value();
    }

    NODISCARD ALWAYS_INLINE T& operator*() { return value(); }
    NODISCARD ALWAYS_INLINE const T& operator*() const { return value(); }

    NODISCARD ALWAYS_INLINE T value_or(const T& fallback_value) const
    {
        if (m_has_value)
            return *unchecked_value();
        return fallback_value;
    }

public:
    ALWAYS_INLINE void clear()
    {
        if (m_has_value) {
            unchecked_value()->~T();
            m_has_value = false;
        }
    }

private:
    NODISCARD ALWAYS_INLINE T* unchecked_value() { return reinterpret_cast<T*>(m_value_buffer); }
    NODISCARD ALWAYS_INLINE const T* unchecked_value() const { return reinterpret_cast<const T*>(m_value_buffer); }

private:
    alignas(T) u8 m_value_buffer[sizeof(T)];
    bool m_has_value;
};

template<typename T>
class Optional<T&> {
public:
    ALWAYS_INLINE Optional()
        : m_value(nullptr)
    {}

    ALWAYS_INLINE Optional(const Optional& other)
        : m_value(other.m_value)
    {}

    ALWAYS_INLINE Optional(Optional&& other) noexcept
        : m_value(other.m_value)
    {
        other.m_value = nullptr;
    }

    ALWAYS_INLINE Optional(T& value)
        : m_value(&value)
    {}

    ALWAYS_INLINE Optional& operator=(const Optional& other)
    {
        m_value = other.m_value;
        return *this;
    }

    ALWAYS_INLINE Optional& operator=(Optional&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        m_value = other.m_value;
        other.m_value = nullptr;
        return *this;
    }

    ALWAYS_INLINE Optional& operator=(T& value)
    {
        m_value = &value;
        return *this;
    }

public:
    NODISCARD ALWAYS_INLINE bool has_value() const { return m_value != nullptr; }

    NODISCARD ALWAYS_INLINE T* operator->()
    {
        ARC_ASSERT(has_value());
        return m_value;
    }

    NODISCARD ALWAYS_INLINE const T* operator->() const
    {
        ARC_ASSERT(has_value());
        return m_value;
    }

    NODISCARD ALWAYS_INLINE T& value()
    {
        ARC_ASSERT(has_value());
        return *m_value;
    }

    NODISCARD ALWAYS_INLINE const T& value() const
    {
        ARC_ASSERT(has_value());
        return *m_value;
    }

    NODISCARD ALWAYS_INLINE T& operator*() { return value(); }
    NODISCARD ALWAYS_INLINE const T& operator*() const { return value(); }

public:
    ALWAYS_INLINE void clear() { m_value = nullptr; }

private:
    T* m_value;
};

}
