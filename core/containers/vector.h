/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/assertions.h>
#include <core/types.h>

namespace arc {

template<typename T>
class Vector {
public:
    using Iterator = T*;
    using ConstIterator = const T*;

    static constexpr usize growth_factor_numerator = 3;
    static constexpr usize growth_factor_denominator = 2;
    static_assert(growth_factor_numerator > growth_factor_denominator);

public:
    ALWAYS_INLINE Vector()
        : m_elements(nullptr)
        , m_capacity(0)
        , m_count(0)
    {}

    ALWAYS_INLINE Vector(const Vector& other)
        : m_capacity(other.m_count)
        , m_count(other.m_count)
    {
        m_elements = allocate_memory(m_capacity);
        copy_elements(m_elements, other.m_elements, m_count);
    }

    ALWAYS_INLINE Vector(Vector&& other) noexcept
        : m_elements(other.m_elements)
        , m_capacity(other.m_capacity)
        , m_count(other.m_count)
    {
        other.m_elements = nullptr;
        other.m_capacity = 0;
        other.m_count = 0;
    }

    ALWAYS_INLINE ~Vector()
    {
        // This function will always release the memory buffer.
        clear_and_shrink();
    }

    ALWAYS_INLINE Vector& operator=(const Vector& other)
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        clear();
        ensure_capacity(other.m_count);

        m_count = other.m_count;
        copy_elements(m_elements, other.m_elements, m_count);

        return *this;
    }

    ALWAYS_INLINE Vector& operator=(Vector&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        clear_and_shrink();

        m_elements = other.m_elements;
        m_capacity = other.m_capacity;
        m_count = other.m_count;

        other.m_elements = nullptr;
        other.m_capacity = 0;
        other.m_count = 0;

        return *this;
    }

public:
    NODISCARD ALWAYS_INLINE T* elements() { return m_elements; }
    NODISCARD ALWAYS_INLINE const T* elements() const { return m_elements; }

    NODISCARD ALWAYS_INLINE usize capacity() const { return m_capacity; }
    NODISCARD ALWAYS_INLINE usize count() const { return m_count; }

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
    template<typename... Args>
    ALWAYS_INLINE void emplace_back(Args&&... args)
    {
        ensure_capacity(m_count + 1);
        new (m_elements + m_count) T(forward<Args>(args)...);
        ++m_count;
    }

    ALWAYS_INLINE void push_back(const T& element)
    {
        ensure_capacity(m_count + 1);
        new (m_elements + m_count) T(element);
        ++m_count;
    }

    ALWAYS_INLINE void push_back(T&& element)
    {
        ensure_capacity(m_count + 1);
        new (m_elements + m_count) T(move(element));
        ++m_count;
    }

public:
    ALWAYS_INLINE void clear()
    {
        for (usize index = 0; index < m_count; ++index) {
            m_elements[index].~T();
        }
        m_count = 0;
    }

    ALWAYS_INLINE void clear_and_shrink()
    {
        clear();
        free_memory(m_elements, m_capacity);
        m_elements = nullptr;
        m_capacity = 0;
    }

    ALWAYS_INLINE void shrink_to_fit()
    {
        if (m_count == m_capacity)
            return;

        T* new_elements = allocate_memory(m_count);
        move_elements(new_elements, m_elements, m_count);
        free_memory(m_elements, m_capacity);

        m_elements = new_elements;
        m_capacity = m_count;
    }

    ALWAYS_INLINE void ensure_capacity(usize required_capacity)
    {
        if (required_capacity <= m_capacity)
            return;

        usize new_capacity = (m_capacity * growth_factor_numerator) / growth_factor_denominator;
        if (new_capacity < required_capacity)
            new_capacity = required_capacity;

        T* new_elements = allocate_memory(new_capacity);
        move_elements(new_elements, m_elements, m_count);
        free_memory(m_elements, m_capacity);

        m_elements = new_elements;
        m_capacity = new_capacity;
    }

    ALWAYS_INLINE void set_count(usize new_count, const T& template_element)
    {
        ensure_capacity(new_count);

        for (usize index = new_count; index < m_count; ++index) {
            new (m_elements + index) T(template_element);
        }

        for (usize index = m_count; index < new_count; ++index) {
            m_elements[index].~T();
        }

        m_count = new_count;
    }

    ALWAYS_INLINE void set_count_defaulted(usize new_count)
    {
        ensure_capacity(new_count);

        for (usize index = new_count; index < m_count; ++index) {
            new (m_elements + index) T();
        }

        for (usize index = m_count; index < new_count; ++index) {
            m_elements[index].~T();
        }

        m_count = new_count;
    }

public:
    NODISCARD ALWAYS_INLINE Iterator begin() { return Iterator(m_elements); }
    NODISCARD ALWAYS_INLINE Iterator end() { return Iterator(m_elements + m_count); }

    NODISCARD ALWAYS_INLINE ConstIterator begin() const { return ConstIterator(m_elements); }
    NODISCARD ALWAYS_INLINE ConstIterator end() const { return ConstIterator(m_elements + m_count); }

private:
    NODISCARD ALWAYS_INLINE static T* allocate_memory(usize in_capacity)
    {
        const usize allocation_size = in_capacity * sizeof(T);
        return static_cast<T*>(::operator new(allocation_size));
    }

    ALWAYS_INLINE static void free_memory(T* in_elements, usize in_capacity)
    {
        MAYBE_UNUSED const usize allocation_size = in_capacity * sizeof(T);
        ::operator delete(in_elements);
    }

    ALWAYS_INLINE static void copy_elements(T* dst_elements, const T* src_elements, usize in_count)
    {
        for (usize index = 0; index < in_count; ++index) {
            new (dst_elements + index) T(src_elements[index]);
        }
    }

    ALWAYS_INLINE static void move_elements(T* dst_elements, T* src_elements, usize in_count)
    {
        for (usize index = 0; index < in_count; ++index) {
            new (dst_elements + index) T(move(src_elements[index]));
            src_elements[index].~T();
        }
    }

private:
    T* m_elements;
    usize m_capacity;
    usize m_count;
};

}
