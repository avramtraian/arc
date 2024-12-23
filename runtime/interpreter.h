/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <bytecode/forward.h>
#include <runtime/virtual_machine.h>

namespace arc::runtime {

class Interpreter {
    ARC_MAKE_NONCOPYABLE(Interpreter);
    ARC_MAKE_NONMOVABLE(Interpreter);

public:
    Interpreter(VirtualMachine&, const bytecode::Package&);

    void execute();

    NODISCARD ALWAYS_INLINE VirtualMachine& vm() { return m_virtual_machine; }
    NODISCARD ALWAYS_INLINE const VirtualMachine& vm() const { return m_virtual_machine; }

private:
    VirtualMachine& m_virtual_machine;
    const bytecode::Package& m_package;
    usize m_instruction_pointer;
};

}
