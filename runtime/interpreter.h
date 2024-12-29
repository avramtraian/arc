/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <bytecode/forward.h>
#include <bytecode/jump_address.h>
#include <core/containers/optional.h>
#include <runtime/virtual_machine.h>

namespace arc::runtime {

class Interpreter {
    ARC_MAKE_NONCOPYABLE(Interpreter);
    ARC_MAKE_NONMOVABLE(Interpreter);

public:
    Interpreter(VirtualMachine&, const bytecode::Package&);

    void set_entry_point(u64 entry_point_instruction_offset);
    void execute();

    NODISCARD ALWAYS_INLINE VirtualMachine& vm() { return m_virtual_machine; }
    NODISCARD ALWAYS_INLINE const VirtualMachine& vm() const { return m_virtual_machine; }

    void jump(bytecode::JumpAddress jump_address);

private:
    void fetch_and_execute();

private:
    VirtualMachine& m_virtual_machine;
    const bytecode::Package& m_package;
    usize m_instruction_pointer;
    Optional<bytecode::JumpAddress> m_jump_address;
};

}
