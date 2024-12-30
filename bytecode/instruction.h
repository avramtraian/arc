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

class CallInstruction : public Instruction {
public:
    explicit CallInstruction(JumpAddress callee_address, u64 parameters_byte_count)
        : m_callee_address(callee_address)
        , m_parameters_byte_count(parameters_byte_count)
    {}

    virtual ~CallInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    JumpAddress m_callee_address;
    u64 m_parameters_byte_count;
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

class DecrementInstruction : public Instruction {
public:
    ALWAYS_INLINE explicit DecrementInstruction(Register dst_register)
        : m_dst_register(dst_register)
    {}

    virtual ~DecrementInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    Register m_dst_register;
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

class LoadFromStackInstruction : public Instruction {
public:
    ALWAYS_INLINE LoadFromStackInstruction(Register dst_register, u64 src_stack_offset)
        : m_dst_register(dst_register)
        , m_src_stack_offset(src_stack_offset)
    {}

    virtual ~LoadFromStackInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    Register m_dst_register;
    u64 m_src_stack_offset;
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

class PopInstruction : public Instruction {
public:
    ALWAYS_INLINE explicit PopInstruction(u64 pop_byte_count)
        : m_pop_byte_count(pop_byte_count)
    {}

    virtual ~PopInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    u64 m_pop_byte_count;
};

class PopRegisterInstruction : public Instruction {
public:
    PopRegisterInstruction() = default;
    virtual ~PopRegisterInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;
};

class PushInstruction : public Instruction {
public:
    ALWAYS_INLINE explicit PushInstruction(u64 push_byte_count)
        : m_push_byte_count(push_byte_count)
    {}

    virtual ~PushInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    u64 m_push_byte_count;
};

class PushImmediate8Instruction : public Instruction {
public:
    ALWAYS_INLINE explicit PushImmediate8Instruction(u64 immediate_value)
        : m_immediate_value(immediate_value)
    {}

    virtual ~PushImmediate8Instruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    u8 m_immediate_value;
};

class PushImmediate16Instruction : public Instruction {
public:
    ALWAYS_INLINE explicit PushImmediate16Instruction(u64 immediate_value)
        : m_immediate_value(immediate_value)
    {}

    virtual ~PushImmediate16Instruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    u16 m_immediate_value;
};

class PushImmediate32Instruction : public Instruction {
public:
    ALWAYS_INLINE explicit PushImmediate32Instruction(u64 immediate_value)
        : m_immediate_value(immediate_value)
    {}

    virtual ~PushImmediate32Instruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    u32 m_immediate_value;
};

class PushImmediate64Instruction : public Instruction {
public:
    ALWAYS_INLINE explicit PushImmediate64Instruction(u64 immediate_value)
        : m_immediate_value(immediate_value)
    {}

    virtual ~PushImmediate64Instruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    u64 m_immediate_value;
};

class PushRegisterInstruction : public Instruction {
public:
    ALWAYS_INLINE explicit PushRegisterInstruction(Register src_register)
        : m_src_register(src_register)
    {}

    virtual ~PushRegisterInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    Register m_src_register;
};

class ReturnInstruction : public Instruction {
public:
    ALWAYS_INLINE ReturnInstruction() {}
    virtual ~ReturnInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;
};

class StoreToStackInstruction : public Instruction {
public:
    ALWAYS_INLINE StoreToStackInstruction(u64 dst_stack_offset, Register src_register)
        : m_dst_stack_offset(dst_stack_offset)
        , m_src_register(src_register)
    {}

    virtual ~StoreToStackInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    u64 m_dst_stack_offset;
    Register m_src_register;
};

class Store8ToStackInstruction : public Instruction {
public:
    ALWAYS_INLINE Store8ToStackInstruction(u64 dst_stack_offset, Register src_register)
        : m_dst_stack_offset(dst_stack_offset)
        , m_src_register(src_register)
    {}

    virtual ~Store8ToStackInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    u64 m_dst_stack_offset;
    Register m_src_register;
};

class Store16ToStackInstruction : public Instruction {
public:
    ALWAYS_INLINE Store16ToStackInstruction(u64 dst_stack_offset, Register src_register)
        : m_dst_stack_offset(dst_stack_offset)
        , m_src_register(src_register)
    {}

    virtual ~Store16ToStackInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    u64 m_dst_stack_offset;
    Register m_src_register;
};

class Store32ToStackInstruction : public Instruction {
public:
    ALWAYS_INLINE Store32ToStackInstruction(u64 dst_stack_offset, Register src_register)
        : m_dst_stack_offset(dst_stack_offset)
        , m_src_register(src_register)
    {}

    virtual ~Store32ToStackInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    u64 m_dst_stack_offset;
    Register m_src_register;
};

class SubInstruction : public Instruction {
public:
    ALWAYS_INLINE SubInstruction(Register dst_register, Register lhs_register, Register rhs_register)
        : m_dst_register(dst_register)
        , m_lhs_register(lhs_register)
        , m_rhs_register(rhs_register)
    {}

    virtual ~SubInstruction() override = default;

    virtual void execute(runtime::Interpreter&) const override;
    virtual String to_string() const override;

private:
    Register m_dst_register;
    Register m_lhs_register;
    Register m_rhs_register;
};

}
