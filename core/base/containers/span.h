/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/base/assertions.h>
#include <core/base/types.h>

namespace arc {

template<typename T>
class Span {
public:
    template<typename Q>
    friend class Span;

    using Iterator = T*;
    using ConstIterator = const T*;

public:
    ALWAYS_INLINE Span()
        : m_elements(nullptr)
        , m_count(0)
    {}

    ALWAYS_INLINE Span(const Span& other)
        : m_elements(other.m_elements)
        , m_count(other.m_count)
    {}

    ALWAYS_INLINE Span(const Span<RemoveConst<T>>& other)
    requires (is_const<T>)
        : m_elements(other.m_elements)
        , m_count(other.m_count)
    {}

    ALWAYS_INLINE Span(Span&& other) noexcept
        : m_elements(other.m_elements)
        , m_count(other.m_count)
    {
        other.m_elements = nullptr;
        other.m_count = 0;
    }

    ALWAYS_INLINE Span(Span<RemoveConst<T>>&& other) noexcept
    requires (is_const<T>)
        : m_elements(other.m_elements)
        , m_count(other.m_count)
    {
        other.m_elements = nullptr;
        other.m_count = 0;
    }

    ALWAYS_INLINE Span(T* in_elements, usize in_count)
        : m_elements(in_elements)
        , m_count(in_count)
    {}

    ALWAYS_INLINE Span& operator=(const Span& other)
    {
        m_elements = other.m_elements;
        m_count = other.m_count;
        return *this;
    }

    ALWAYS_INLINE Span& operator=(const Span<RemoveConst<T>>& other)
    requires (is_const<T>)
    {
        m_elements = other.m_elements;
        m_count = other.m_count;
        return *this;
    }

    ALWAYS_INLINE Span& operator=(Span&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        m_elements = other.m_elements;
        m_count = other.m_count;
        other.m_elements = nullptr;
        other.m_count = 0;
        return *this;
    }

    ALWAYS_INLINE Span& operator=(Span<RemoveConst<T>>&& other) noexcept
    requires (is_const<T>)
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        m_elements = other.m_elements;
        m_count = other.m_count;
        other.m_elements = nullptr;
        other.m_count = 0;
        return *this;
    }

public:
    NODISCARD ALWAYS_INLINE T* elements() { return m_elements; }
    NODISCARD ALWAYS_INLINE const T* elements() const { return m_elements; }

    NODISCARD ALWAYS_INLINE usize count() const { return m_count; }
    NODISCARD ALWAYS_INLINE static constexpr usize element_size() { return sizeof(T); }
    NODISCARD ALWAYS_INLINE usize byte_count() const { return m_count * element_size(); }

    NODISCARD ALWAYS_INLINE bool is_empty() const { return m_count == 0; }
    NODISCARD ALWAYS_INLINE bool has_elements() const { return m_count > 0; }

public:
    NODISCARD ALWAYS_INLINE T& at(usize index)
    {
        ARC_ASSERT(index < m_count);
        return m_elements[index];
    }

    NODISCARD ALWAYS_INLINE const T& at(usize index) const
    {
        ARC_ASSERT(index < m_count);
        return m_elements[index];
    }

    NODISCARD ALWAYS_INLINE T& operator[](usize index) { return at(index); }
    NODISCARD ALWAYS_INLINE const T& operator[](usize index) const { return at(index); }

    NODISCARD ALWAYS_INLINE T& first()
    {
        ARC_ASSERT(has_elements());
        return m_elements[0];
    }

    NODISCARD ALWAYS_INLINE const T& first() const
    {
        ARC_ASSERT(has_elements());
        return m_elements[0];
    }

    NODISCARD ALWAYS_INLINE T& last()
    {
        ARC_ASSERT(has_elements());
        return m_elements[m_count - 1];
    }

    NODISCARD ALWAYS_INLINE const T& last() const
    {
        ARC_ASSERT(has_elements());
        return m_elements[m_count - 1];
    }

public:
    NODISCARD ALWAYS_INLINE Span slice(usize offset, usize in_count) const
    {
        ARC_ASSERT(offset + in_count <= m_count);

        Span sliced_span;
        sliced_span.m_elements = m_elements + offset;
        sliced_span.m_count = in_count;
        return sliced_span;
    }

    NODISCARD ALWAYS_INLINE Span slice(usize offset) const
    {
        ARC_ASSERT(offset <= m_count);

        Span sliced_span;
        sliced_span.m_elements = m_elements + offset;
        sliced_span.m_count = m_count - offset;
        return sliced_span;
    }

public:
    NODISCARD ALWAYS_INLINE Iterator begin() { return Iterator(m_elements); }
    NODISCARD ALWAYS_INLINE Iterator end() { return Iterator(m_elements + m_count); }

    NODISCARD ALWAYS_INLINE ConstIterator begin() const { return ConstIterator(m_elements); }
    NODISCARD ALWAYS_INLINE ConstIterator end() const { return ConstIterator(m_elements + m_count); }

private:
    T* m_elements;
    usize m_count;
};

using ReadonlyByteSpan = Span<ReadonlyByte>;
using WriteonlyByteSpan = Span<WriteonlyByte>;
using ReadWriteByteSpan = Span<ReadWriteByte>;

}
