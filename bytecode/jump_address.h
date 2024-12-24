/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/containers/format.h>
#include <core/types.h>

namespace arc::bytecode {

class JumpAddress {
public:
    explicit JumpAddress(u64 address)
        : m_address(address)
    {}

    JumpAddress(const JumpAddress&) = default;
    JumpAddress& operator=(const JumpAddress&) = default;

    NODISCARD ALWAYS_INLINE u64 address() const { return m_address; }

private:
    u64 m_address;
};

}

namespace arc {
template<>
class Formatter<bytecode::JumpAddress> {
public:
    ALWAYS_INLINE static void format(FormatStream& stream, const bytecode::JumpAddress& address)
    {
        stream.push_codepoint('@');
        stream.push_unsigned_integer(address.address());
    }
};
}
