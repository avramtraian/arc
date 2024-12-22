/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/base/assertions.h>
#include <core/base/numeric_limits.h>
#include <core/base/types.h>

namespace arc {

class RefCounted {
    ARC_MAKE_NONCOPYABLE(RefCounted);
    ARC_MAKE_NONMOVABLE(RefCounted);

public:
    ALWAYS_INLINE RefCounted()
        : m_reference_count(0)
    {}

    ALWAYS_INLINE virtual ~RefCounted()
    {
        // NOTE: A reference counted object must be destroyed only when its reference count
        //       hits zero. Otherwise, uninitialized memory might be accessed later on.
        ARC_ASSERT(m_reference_count == 0);
    }

public:
    NODISCARD ALWAYS_INLINE u32 get_reference_count() const { return m_reference_count; }
    NODISCARD ALWAYS_INLINE bool reference_count_is_zero() const { return m_reference_count == 0; }

    ALWAYS_INLINE void increment_reference_count()
    {
        // Ensure that incrementing the reference count will not overflow.
        ARC_ASSERT_DEBUG(m_reference_count != NumericLimits<u32>::max());
        ++m_reference_count;
    }

    ALWAYS_INLINE void decrement_reference_count()
    {
        // Ensure that decrementing the reference count will not overflow.
        ARC_ASSERT_DEBUG(m_reference_count != 0);
        --m_reference_count;
    }

private:
    u32 m_reference_count;
};

template<typename T>
class RefPtr {
public:
    template<typename Q>
    friend class RefPtr;

    template<typename Q>
    friend RefPtr<Q> adopt_ref(Q*);

public:
    ALWAYS_INLINE RefPtr()
        : m_instance(nullptr)
    {}

    template<typename Q>
    requires (is_derived_from<Q, T>)
    ALWAYS_INLINE RefPtr(const RefPtr<Q>& other)
        : m_instance(other.m_instance)
    {
        if (m_instance)
            increment_reference_count();
    }

    template<typename Q>
    requires (is_derived_from<Q, T>)
    ALWAYS_INLINE RefPtr(RefPtr<Q>&& other) noexcept
        : m_instance(other.m_instance)
    {
        other.m_instance = nullptr;
    }

    ALWAYS_INLINE RefPtr(NullptrType)
        : m_instance(nullptr)
    {}

    template<typename Q>
    requires (is_derived_from<Q, T>)
    ALWAYS_INLINE RefPtr& operator=(const RefPtr<Q>& other)
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        release();
        m_instance = other.m_instance;
        if (m_instance)
            increment_reference_count();
        return *this;
    }

    template<typename Q>
    requires (is_derived_from<Q, T>)
    ALWAYS_INLINE RefPtr& operator=(RefPtr<Q>&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        release();
        m_instance = other.m_instance;
        other.m_instance = nullptr;
        return *this;
    }

    ALWAYS_INLINE RefPtr& operator=(NullptrType)
    {
        release();
        return *this;
    }

    ALWAYS_INLINE ~RefPtr() { release(); }

public:
    NODISCARD ALWAYS_INLINE bool is_valid() const { return m_instance != nullptr; }

    NODISCARD ALWAYS_INLINE T* get()
    {
        ARC_ASSERT(is_valid());
        return m_instance;
    }

    NODISCARD ALWAYS_INLINE const T* get() const
    {
        ARC_ASSERT(is_valid());
        return m_instance;
    }

    NODISCARD ALWAYS_INLINE T* operator->() { return get(); }
    NODISCARD ALWAYS_INLINE const T* operator->() const { return get(); }

    NODISCARD ALWAYS_INLINE T& dereference() { return *get(); }
    NODISCARD ALWAYS_INLINE const T& dereference() const { return *get(); }

    NODISCARD ALWAYS_INLINE T& operator*() { return dereference(); }
    NODISCARD ALWAYS_INLINE const T& operator*() const { return dereference(); }

public:
    ALWAYS_INLINE void release()
    {
        if (m_instance) {
            decrement_reference_count();
            if (reference_count_is_zero())
                delete m_instance;
            m_instance = nullptr;
        }
    }

    template<typename Q>
    NODISCARD ALWAYS_INLINE RefPtr<Q> as()
    {
        Q* casted_instance = static_cast<Q*>(m_instance);
        return RefPtr<Q>(casted_instance);
    }

    template<typename Q>
    NODISCARD ALWAYS_INLINE RefPtr<Q> as() const
    {
        Q* casted_instance = static_cast<Q*>(m_instance);
        return RefPtr<Q>(casted_instance);
    }

    NODISCARD ALWAYS_INLINE bool operator==(const RefPtr& other) noexcept
    {
        const bool reference_the_same_instance = m_instance == other.m_instance;
        return reference_the_same_instance;
    }

    NODISCARD ALWAYS_INLINE bool operator!=(const RefPtr& other) noexcept
    {
        const bool reference_the_same_instance = m_instance == other.m_instance;
        return !reference_the_same_instance;
    }

private:
    ALWAYS_INLINE explicit RefPtr(T* instance)
        : m_instance(instance)
    {
        if (m_instance)
            increment_reference_count();
    }

    ALWAYS_INLINE void increment_reference_count()
    {
        static_assert(is_derived_from<T, RefCounted>);
        RefCounted* ref_counted = static_cast<RefCounted*>(m_instance);
        ref_counted->increment_reference_count();
    }

    ALWAYS_INLINE void decrement_reference_count()
    {
        static_assert(is_derived_from<T, RefCounted>);
        RefCounted* ref_counted = static_cast<RefCounted*>(m_instance);
        ref_counted->decrement_reference_count();
    }

    NODISCARD ALWAYS_INLINE bool reference_count_is_zero()
    {
        static_assert(is_derived_from<T, RefCounted>);
        const RefCounted* ref_counted = static_cast<const RefCounted*>(m_instance);
        return ref_counted->reference_count_is_zero();
    }

private:
    T* m_instance;
};

template<typename T>
NODISCARD ALWAYS_INLINE RefPtr<T> adopt_ref(T* instance)
{
    return RefPtr<T>(instance);
}

template<typename T, typename... Args>
NODISCARD ALWAYS_INLINE RefPtr<T> create_ref(Args&&... args)
{
    T* instance = new T(forward<Args>(args)...);
    return adopt_ref<T>(instance);
}

}
