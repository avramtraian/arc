/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/base/assertions.h>
#include <core/base/types.h>

namespace arc {

template<typename T>
class OwnPtr {
    ARC_MAKE_NONCOPYABLE(OwnPtr);

    template<typename Q>
    friend class OwnPtr;

    template<typename Q>
    friend OwnPtr<Q> adopt_own(Q*);

public:
    ALWAYS_INLINE OwnPtr()
        : m_instance(nullptr)
    {}

    template<typename Q>
    requires (is_derived_from<Q, T>)
    ALWAYS_INLINE OwnPtr(OwnPtr<Q>&& other) noexcept
        : m_instance(other.m_instance)
    {
        other.m_instance = nullptr;
    }

    ALWAYS_INLINE OwnPtr(NullptrType)
        : m_instance(nullptr)
    {}

    template<typename Q>
    requires (is_derived_from<Q, T>)
    ALWAYS_INLINE OwnPtr& operator=(OwnPtr<Q>&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        release();
        m_instance = other.m_instance;
        other.m_instance = nullptr;
        return *this;
    }

    ALWAYS_INLINE OwnPtr& operator=(NullptrType)
    {
        release();
        return *this;
    }

    ALWAYS_INLINE ~OwnPtr() { release(); }

public:
    NODISCARD ALWAYS_INLINE bool is_valid() const { return m_instance; }

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
            delete m_instance;
            m_instance = nullptr;
        }
    }

    template<typename Q>
    NODISCARD ALWAYS_INLINE OwnPtr<Q> as()
    {
        Q* casted_instance = static_cast<Q*>(m_instance);
        m_instance = nullptr;
        return OwnPtr<Q>(casted_instance);
    }

private:
    ALWAYS_INLINE explicit OwnPtr(T* instance)
        : m_instance(instance)
    {}

private:
    T* m_instance;
};

template<typename T>
NODISCARD ALWAYS_INLINE OwnPtr<T> adopt_own(T* instance)
{
    return OwnPtr<T>(instance);
}

template<typename T, typename... Args>
NODISCARD ALWAYS_INLINE OwnPtr<T> create_own(Args&&... args)
{
    T* instance = new T(forward<Args>(args)...);
    return adopt_own<T>(instance);
}

}
