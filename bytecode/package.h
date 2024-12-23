/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <bytecode/instruction.h>
#include <core/containers/own_ptr.h>
#include <core/containers/vector.h>

namespace arc::bytecode {

class Package {
    ARC_MAKE_NONCOPYABLE(Package);
    ARC_MAKE_NONMOVABLE(Package);

public:
    Package();

    template<typename InstructionType, typename... Args>
    void emit_instruction(Args&&... args)
    {
        // Create the instruction and push it to the instruction list.
        m_instructions.push_back(create_own<InstructionType>(forward<Args>(args)...));
    }

    bool instruction_pointer_is_valid(usize instruction_pointer) const;
    const Instruction& fetch_instruction(usize instruction_pointer) const;

private:
    Vector<OwnPtr<bytecode::Instruction>> m_instructions;
};

}
