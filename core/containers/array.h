/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/assertions.h>
#include <core/types.h>

namespace Arc::core {

template<typename T, usize C>
requires (C > 0)
class Array {
public:
    using Iterator = T*;
    using ConstIterator = const T*;

public:
    Array() = default;
    ~Array() = default;

public:
    NODISCARD ALWAYS_INLINE T* elements() { return m_elements; }
    NODISCARD ALWAYS_INLINE const T* elements() const { return m_elements; }

    NODISCARD ALWAYS_INLINE static constexpr usize count() { return C; }
    NODISCARD ALWAYS_INLINE static constexpr usize element_size() { return sizeof(T); }
    NODISCARD ALWAYS_INLINE static constexpr usize byte_count() { return C * sizeof(T); }

public:
    NODISCARD ALWAYS_INLINE T& at(usize index)
    {
        ARC_ASSERT(index < C);
        return m_elements[index];
    }

    NODISCARD ALWAYS_INLINE const T& at(usize index) const
    {
        ARC_ASSERT(index < C);
        return m_elements[index];
    }

    NODISCARD ALWAYS_INLINE T& operator[](usize index) { return at(index); }
    NODISCARD ALWAYS_INLINE const T& operator[](usize index) const { return at(index); }

    NODISCARD ALWAYS_INLINE T& first() { return m_elements[0]; }
    NODISCARD ALWAYS_INLINE const T& first() const { return m_elements[0]; }
    NODISCARD ALWAYS_INLINE T& last() { return m_elements[C - 1]; }
    NODISCARD ALWAYS_INLINE const T& last() const { return m_elements[C - 1]; }

public:
    NODISCARD ALWAYS_INLINE Iterator begin() { return Iterator(m_elements); }
    NODISCARD ALWAYS_INLINE Iterator end() { return Iterator(m_elements + C); }

    NODISCARD ALWAYS_INLINE ConstIterator begin() const { return ConstIterator(m_elements); }
    NODISCARD ALWAYS_INLINE ConstIterator end() const { return ConstIterator(m_elements + C); }

private:
    T m_elements[C];
};

}

namespace Arc {
using core::Array;
}
