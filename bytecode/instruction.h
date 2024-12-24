/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <bytecode/jump_address.h>
#include <bytecode/register.h>
#include <core/containers/string.h>
#include <runtime/forward.h>

namespace arc::bytecode {

class Instruction {
    ARC_MAKE_NONCOPYABLE(Instruction);
    ARC_MAKE_NONMOVABLE(Instruction);

public:
    Instruction() = default;
    virtual ~Instruction() = default;

    virtual void execute(runtime::Interpreter&) const = 0;
    virtual String to_string() const = 0;
};

class AddInstruction : public Instruction {
public:
    ALWAYS_INLINE AddInstruction(Register dst_register, Register lhs_register, Register rhs_register)
        : m_dst_register(dst_register)
        , m_lhs_register(lhs_register)
        , m_rhs_register(rhs_register)
    {}

    virtual ~AddInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    Register m_dst_register;
    Register m_lhs_register;
    Register m_rhs_register;
};

class CompareGreaterInstruction : public Instruction {
public:
    ALWAYS_INLINE CompareGreaterInstruction(Register dst_register, Register lhs_register, Register rhs_register)
        : m_dst_register(dst_register)
        , m_lhs_register(lhs_register)
        , m_rhs_register(rhs_register)
    {}

    virtual ~CompareGreaterInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    Register m_dst_register;
    Register m_lhs_register;
    Register m_rhs_register;
};

class IncrementInstruction : public Instruction {
public:
    ALWAYS_INLINE explicit IncrementInstruction(Register dst_register)
        : m_dst_register(dst_register)
    {}

    virtual ~IncrementInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    Register m_dst_register;
};

class JumpInstruction : public Instruction {
public:
    ALWAYS_INLINE explicit JumpInstruction(JumpAddress jump_address)
        : m_jump_address(jump_address)
    {}

    virtual ~JumpInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    JumpAddress m_jump_address;
};

class JumpIfInstruction : public Instruction {
public:
    ALWAYS_INLINE JumpIfInstruction(Register condition_register, JumpAddress jump_address)
        : m_condition_register(condition_register)
        , m_jump_address(jump_address)
    {}

    virtual ~JumpIfInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    Register m_condition_register;
    JumpAddress m_jump_address;
};

class LoadImmediate8Instruction : public Instruction {
public:
    ALWAYS_INLINE LoadImmediate8Instruction(Register dst_register, u8 immediate_value)
        : m_dst_register(dst_register)
        , m_immediate_value(immediate_value)
    {}

    virtual ~LoadImmediate8Instruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    Register m_dst_register;
    u8 m_immediate_value;
};

}
