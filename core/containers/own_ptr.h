/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/assertions.h>
#include <core/types.h>

namespace arc {

template<typename T>
class OwnPtr {
public:
    template<typename Q>
    friend class OwnPtr;

    template<typename Q>
    friend OwnPtr<Q> adopt_own(Q*);
    template<typename Q, typename... Args>
    friend OwnPtr<Q> create_own(Args&&...);

    ARC_MAKE_NONCOPYABLE(OwnPtr);

public:
    ALWAYS_INLINE OwnPtr()
        : m_instance(nullptr)
    {}

    ALWAYS_INLINE ~OwnPtr() { release(); }

    ALWAYS_INLINE OwnPtr(OwnPtr&& other) noexcept
        : m_instance(other.m_instance)
    {
        other.m_instance = nullptr;
    }

    ALWAYS_INLINE OwnPtr& operator=(OwnPtr&& other) noexcept
    {
        // Handle the self-assignment case.
        if (this == &other)
            return *this;

        release();

        m_instance = other.m_instance;
        other.m_instance = nullptr;

        return *this;
    }

public:
    template<typename Q>
    requires (!std::is_same_v<T, Q> && std::is_base_of_v<T, Q>)
    ALWAYS_INLINE OwnPtr(OwnPtr<Q>&& other) noexcept
        : m_instance(other.m_instance)
    {
        other.m_instance = nullptr;
    }

    template<typename Q>
    requires (!std::is_same_v<T, Q> && std::is_base_of_v<T, Q>)
    ALWAYS_INLINE OwnPtr& operator=(OwnPtr<Q>&& other) noexcept
    {
        // Handle the self-assignment case.
        if ((void*)(this) == (void*)(&other))
            return *this;

        release();

        m_instance = other.m_instance;
        other.m_instance = nullptr;

        return *this;
    }

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

    NODISCARD ALWAYS_INLINE T* get_non_const() const
    {
        ARC_ASSERT(is_valid());
        return m_instance;
    }

    NODISCARD ALWAYS_INLINE T& dereference() { return *get(); }
    NODISCARD ALWAYS_INLINE const T& dereference() const { return *get(); }
    NODISCARD ALWAYS_INLINE T& dereference_non_const() const { return *get_non_const(); }

    NODISCARD ALWAYS_INLINE T* operator->() { return get(); }
    NODISCARD ALWAYS_INLINE const T* operator->() const { return get(); }

    NODISCARD ALWAYS_INLINE T& operator*() { return *get(); }
    NODISCARD ALWAYS_INLINE const T& operator*() const { return *get(); }

public:
    ALWAYS_INLINE void release()
    {
        if (m_instance) {
            T* instance = m_instance;
            m_instance = nullptr;
            delete instance;
        }
    }

    template<typename Q>
    NODISCARD ALWAYS_INLINE OwnPtr<Q> as()
    {
        OwnPtr<Q> casted;
        casted.m_instance = m_instance;
        m_instance = nullptr;
        return casted;
    }

private:
    T* m_instance;
};

template<typename T>
NODISCARD ALWAYS_INLINE OwnPtr<T> adopt_own(T* instance)
{
    OwnPtr<T> own_ptr;
    own_ptr.m_instance = instance;
    return own_ptr;
}

template<typename T, typename... Args>
NODISCARD ALWAYS_INLINE OwnPtr<T> create_own(Args&&... args)
{
    OwnPtr<T> own_ptr;
    own_ptr.m_instance = new T(forward<Args>(args)...);
    return own_ptr;
}

}
