/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <bytecode/forward.h>
#include <core/containers/string.h>

namespace Arc::Bytecode {

class Disassembler {
    ARC_MAKE_NONCOPYABLE(Disassembler);
    ARC_MAKE_NONMOVABLE(Disassembler);

public:
    Disassembler(const Package&);

    String instructions_as_string() const;

private:
    const Package& m_package;
};

}
