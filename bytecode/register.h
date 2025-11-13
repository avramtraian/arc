/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/containers/format.h>
#include <core/numeric_limits.h>

namespace Arc::Bytecode {

enum class Register : u8 {
    GPR0 = 0,
    GPR1,
    GPR2,
    GPR3,
    Count,
};

}

namespace Arc {
template<>
class Formatter<Bytecode::Register> {
public:
    ALWAYS_INLINE static void format(FormatStream& stream, const Bytecode::Register& value)
    {
        stream.push_codepoint('$');
        // clang-format off
        switch (value) {
            case Bytecode::Register::GPR0: stream.push_string("GPR0"sv); break;
            case Bytecode::Register::GPR1: stream.push_string("GPR1"sv); break;
            case Bytecode::Register::GPR2: stream.push_string("GPR2"sv); break;
            case Bytecode::Register::GPR3: stream.push_string("GPR3"sv); break;

            default: stream.push_codepoint('?');
        }
        // clang-format on
    }
};
}
