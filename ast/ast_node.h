/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/assertions.h>
#include <core/containers/own_ptr.h>
#include <core/containers/string.h>
#include <core/containers/vector.h>

namespace arc::ast {

enum class ASTNodeType : u32 {
    Unknown = 0,
    Expression,
    ExecutionScope,
    DeclarationScope,
    WhileStructure,
    FunctionDeclaration,
    ReturnStatement,
    BreakInstruction,
    ContinueInstruction,
};

class ASTNode {
public:
    explicit ASTNode(ASTNodeType node_type)
        : m_node_type(node_type)
    {}

    virtual ~ASTNode() = default;

    virtual StringView class_name() const = 0;
    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const = 0;

public:
    NODISCARD ALWAYS_INLINE ASTNodeType node_type() const { return m_node_type; }

private:
    ASTNodeType m_node_type;
};

template<typename ASTNodeType, typename... Args>
NODISCARD ALWAYS_INLINE OwnPtr<ASTNodeType> create_node(Args&&... args)
{
    // TODO: Eventually allocate the nodes from a pool instead of directly from the heap.
    //       This is both a performance and fragmentation concern.
    return create_own<ASTNodeType>(forward<Args>(args)...);
}

enum class ASTExpressionType : u8 {
    Unknown = 0,
    Unary,
    Binary,
    Identifier,
    Literal,
    Call,
    Assignment,
    Member,
    Declaration,
};

class ASTExpression : public ASTNode {
public:
    explicit ASTExpression(ASTExpressionType expression_type)
        : ASTNode(ASTNodeType::Expression)
        , m_expression_type(expression_type)
    {}
    virtual ~ASTExpression() override = default;

public:
    NODISCARD ALWAYS_INLINE ASTExpressionType expression_type() const { return m_expression_type; }
    NODISCARD ALWAYS_INLINE bool is_unary() const { return m_expression_type == ASTExpressionType::Unary; }
    NODISCARD ALWAYS_INLINE bool is_binary() const { return m_expression_type == ASTExpressionType::Binary; }

private:
    ASTExpressionType m_expression_type;
};

enum class ASTUnaryOperation : u8 {
// clang-format off
#define ARC_ENUMERATE_UNARY_OPERATIONS(x)   \
    x(Unknown)                              \
    x(Negative)                             \
    x(Positive)                             \
    x(PreIncrement)                         \
    x(PostIncrement)                        \
    x(PreDecrement)                         \
    x(PostDecrement)                        \
    x(LogicalNOT)                           \
    x(BitwiseNOT)
// clang-format on

#define _ARC_ENUM_MEMBER(x) x,
    ARC_ENUMERATE_UNARY_OPERATIONS(_ARC_ENUM_MEMBER)
#undef _ARC_ENUM_MEMBER
};

StringView ast_unary_operation_to_string_view(ASTUnaryOperation unary_operation);
String ast_unary_operation_to_string(ASTUnaryOperation unary_operation);

class ASTUnaryExpression final : public ASTExpression {
public:
    ASTUnaryExpression(ASTUnaryOperation unary_operation, OwnPtr<ASTExpression> expression)
        : ASTExpression(ASTExpressionType::Unary)
        , m_unary_operation(unary_operation)
        , m_expression(move(expression))
    {}

    virtual ~ASTUnaryExpression() override = default;
    virtual StringView class_name() const override { return "ASTUnaryExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE ASTUnaryOperation unary_operation() const { return m_unary_operation; }

    NODISCARD ALWAYS_INLINE ASTExpression& expression() { return *m_expression; }
    NODISCARD ALWAYS_INLINE const ASTExpression& expression() const { return *m_expression; }

private:
    ASTUnaryOperation m_unary_operation;
    OwnPtr<ASTExpression> m_expression;
};

enum class ASTBinaryOperation : u8 {
// clang-format off
#define ARC_ENUMERATE_BINARY_OPERATIONS(x)  \
    x(Unknown)                              \
    x(Add)                                  \
    x(Subtract)                             \
    x(Multiply)                             \
    x(Divide)                               \
    x(LogicalAND)                           \
    x(LogicalOR)                            \
    x(LogicalXOR)                           \
    x(BitwiseAND)                           \
    x(BitwiseOR)                            \
    x(BitwiseXOR)                           \
    x(BitwiseLeftShift)                     \
    x(BitwiseRightShift)                    \
    x(CompareLess)                          \
    x(CompareLessOrEqual)                   \
    x(CompareGreater)                       \
    x(CompareGreaterOrEqual)
// clang-format on

#define _ARC_ENUM_MEMBER(x) x,
    ARC_ENUMERATE_BINARY_OPERATIONS(_ARC_ENUM_MEMBER)
#undef _ARC_ENUM_MEMBER
};

StringView ast_binary_operation_to_string_view(ASTBinaryOperation binary_operation);
String ast_binary_operation_to_string(ASTBinaryOperation binary_operation);

class ASTBinaryExpression final : public ASTExpression {
public:
    ASTBinaryExpression(ASTBinaryOperation binary_operation, OwnPtr<ASTExpression> left_expression, OwnPtr<ASTExpression> right_expression)
        : ASTExpression(ASTExpressionType::Binary)
        , m_binary_operation(binary_operation)
        , m_left_expression(move(left_expression))
        , m_right_expression(move(right_expression))
    {}

    virtual ~ASTBinaryExpression() override = default;
    virtual StringView class_name() const override { return "ASTBinaryExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE ASTBinaryOperation binary_operation() const { return m_binary_operation; }

    NODISCARD ALWAYS_INLINE ASTExpression& left_expression() { return *m_left_expression; }
    NODISCARD ALWAYS_INLINE const ASTExpression& left_expression() const { return *m_left_expression; }

    NODISCARD ALWAYS_INLINE ASTExpression& right_expression() { return *m_right_expression; }
    NODISCARD ALWAYS_INLINE const ASTExpression& right_expression() const { return *m_right_expression; }

private:
    ASTBinaryOperation m_binary_operation;
    OwnPtr<ASTExpression> m_left_expression;
    OwnPtr<ASTExpression> m_right_expression;
};

class ASTIdentifierExpression final : public ASTExpression {
public:
    explicit ASTIdentifierExpression(String identifier_name)
        : ASTExpression(ASTExpressionType::Identifier)
        , m_identifier_name(move(identifier_name))
    {}

    virtual ~ASTIdentifierExpression() override = default;
    virtual StringView class_name() const override { return "ASTIdentifierExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const String& identifier_name() const { return m_identifier_name; }

private:
    String m_identifier_name;
};

enum class ASTLiteralType : u8 {
// clang-format off
#define ARC_ENUMERATE_LITERAL_TYPES(x)                      \
    x(UnsignedInteger,      unsigned_integer,       u64)    \
    x(SignedInteger,        signed_integer,         s64)    \
    x(FloatingPointNumber,  floating_point_number,  f64)    \
    x(Character,            character,              char)   \
    x(String,               string,                 String)
    // clang-format on

    Unknown = 0,
#define _ARC_ENUM_MEMBER(e, n, t) e,
    ARC_ENUMERATE_LITERAL_TYPES(_ARC_ENUM_MEMBER)
#undef _ARC_ENUM_MEMBER
};

StringView ast_literal_type_to_string_view(ASTLiteralType literal_type);
String ast_literal_type_to_string(ASTLiteralType literal_type);

class ASTLiteralExpression final : public ASTExpression {
public:
    explicit ASTLiteralExpression(ASTLiteralType literal_type)
        : ASTExpression(ASTExpressionType::Literal)
        , m_literal_type(literal_type)
        , m_literal_unsigned_integer(0)
        , m_literal_signed_integer(0)
        , m_literal_floating_point_number(0.0)
        , m_literal_character('\0')
    {}

    virtual ~ASTLiteralExpression() override = default;
    virtual StringView class_name() const override { return "ASTLiteralExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE ASTLiteralType literal_expression_type() const { return m_literal_type; }

#define _ARC_DECLARE_LITERAL_FUNCTIONS(expression_type, snake_case_type, type)                                              \
    NODISCARD ALWAYS_INLINE bool is_##snake_case_type() const { return m_literal_type == ASTLiteralType::expression_type; } \
    NODISCARD ALWAYS_INLINE type& snake_case_type()                                                                         \
    {                                                                                                                       \
        ARC_ASSERT(is_##snake_case_type());                                                                                 \
        return m_literal_##snake_case_type;                                                                                 \
    }                                                                                                                       \
    ALWAYS_INLINE ASTLiteralExpression& set_##snake_case_type(type value)                                                   \
    {                                                                                                                       \
        ARC_ASSERT(is_##snake_case_type());                                                                                 \
        m_literal_##snake_case_type = move(value);                                                                          \
        return *this;                                                                                                       \
    }

    ARC_ENUMERATE_LITERAL_TYPES(_ARC_DECLARE_LITERAL_FUNCTIONS)
#undef _ARC_DECLARE_LITERAL_FUNCTIONS

public:
    ASTLiteralType m_literal_type;

    // NOTE: It seems wasteful to not store these primitive types inside a union. However, since the 'String' class dynamically allocates
    //       memory, we can't store it in the union as well - which would create an uncomfortable situation.
    // TODO: Think about packing these fields into a union. Maybe store the string literal contents into a read-only buffer of the package,
    //       and thus not potentially requiring a dynamic heap allocation.

    u64 m_literal_unsigned_integer;
    s64 m_literal_signed_integer;
    f64 m_literal_floating_point_number;
    char m_literal_character;
    String m_literal_string;
};

class ASTCallExpression final : public ASTExpression {
public:
    ASTCallExpression(OwnPtr<ASTExpression> callee_expression, Vector<OwnPtr<ASTExpression>> parameters = {})
        : ASTExpression(ASTExpressionType::Call)
        , m_callee_expression(move(callee_expression))
        , m_parameters(move(parameters))
    {}

    virtual ~ASTCallExpression() override = default;
    virtual StringView class_name() const override { return "ASTCallExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const OwnPtr<ASTExpression>& callee_expression() const { return m_callee_expression; }
    NODISCARD ALWAYS_INLINE const Vector<OwnPtr<ASTExpression>>& parameters() const { return m_parameters; }

    ALWAYS_INLINE ASTCallExpression& add_parameter(OwnPtr<ASTExpression> parameter)
    {
        m_parameters.push_back(move(parameter));
        return *this;
    }

private:
    OwnPtr<ASTExpression> m_callee_expression;
    Vector<OwnPtr<ASTExpression>> m_parameters;
};

class ASTAssignmentExpression final : public ASTExpression {
public:
    ASTAssignmentExpression(OwnPtr<ASTExpression> left_expression, OwnPtr<ASTExpression> right_expression)
        : ASTExpression(ASTExpressionType::Assignment)
        , m_left_expression(move(left_expression))
        , m_right_expression(move(right_expression))
    {}

    virtual ~ASTAssignmentExpression() override = default;
    virtual StringView class_name() const override { return "ASTAssignmentExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const OwnPtr<ASTExpression>& left_expression() const { return m_left_expression; }
    NODISCARD ALWAYS_INLINE const OwnPtr<ASTExpression>& right_expression() const { return m_right_expression; }

private:
    OwnPtr<ASTExpression> m_left_expression;
    OwnPtr<ASTExpression> m_right_expression;
};

class ASTMemberExpression final : public ASTExpression {
public:
    ASTMemberExpression(OwnPtr<ASTExpression> instance_expression, String member_identifier_name)
        : ASTExpression(ASTExpressionType::Member)
        , m_instance_expression(move(instance_expression))
        , m_member_identifier_name(move(member_identifier_name))
    {}

    virtual ~ASTMemberExpression() override = default;
    virtual StringView class_name() const override { return "ASTMemberExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const OwnPtr<ASTExpression>& instance_expression() const { return m_instance_expression; }
    NODISCARD ALWAYS_INLINE const String& member_identifier_name() const { return m_member_identifier_name; }

private:
    OwnPtr<ASTExpression> m_instance_expression;
    String m_member_identifier_name;
};

class ASTDeclarationExpression final : public ASTExpression {
public:
    ASTDeclarationExpression(String type_identifier_name, String variable_identifier_name)
        : ASTExpression(ASTExpressionType::Declaration)
        , m_type_identifier_name(move(type_identifier_name))
        , m_variable_identifier_name(move(variable_identifier_name))
    {}

    virtual ~ASTDeclarationExpression() override = default;
    virtual StringView class_name() const override { return "ASTDeclarationExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const String& type_identifier_name() const { return m_type_identifier_name; }
    NODISCARD ALWAYS_INLINE const String& variable_identifier_name() const { return m_variable_identifier_name; }

private:
    String m_type_identifier_name;
    String m_variable_identifier_name;
};

class ASTExecutionScope final : public ASTNode {
public:
    explicit ASTExecutionScope(Vector<OwnPtr<ASTNode>> children = {})
        : ASTNode(ASTNodeType::ExecutionScope)
        , m_children(move(children))
    {}

    virtual ~ASTExecutionScope() override = default;
    virtual StringView class_name() const override { return "ASTExecutionScope"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const Vector<OwnPtr<ASTNode>>& children() const { return m_children; }

    ALWAYS_INLINE ASTExecutionScope& add_child(OwnPtr<ASTNode> child)
    {
        m_children.push_back(move(child));
        return *this;
    }

private:
    Vector<OwnPtr<ASTNode>> m_children;
};

class ASTWhileStructure final : public ASTNode {
public:
    ASTWhileStructure(OwnPtr<ASTExpression> condition_expression, OwnPtr<ASTExecutionScope> body_execution_scope)
        : ASTNode(ASTNodeType::WhileStructure)
        , m_condition_expression(move(condition_expression))
        , m_body_execution_scope(move(body_execution_scope))
    {}

    virtual ~ASTWhileStructure() override = default;
    virtual StringView class_name() const override { return "ASTWhileStructure"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const OwnPtr<ASTExpression>& condition_expression() const { return m_condition_expression; }
    NODISCARD ALWAYS_INLINE const OwnPtr<ASTExecutionScope>& body_execution_scope() const { return m_body_execution_scope; }

private:
    OwnPtr<ASTExpression> m_condition_expression;
    OwnPtr<ASTExecutionScope> m_body_execution_scope;
};

class ASTFunctionDeclaration final : public ASTNode {
public:
    struct Parameter {
        String type_identifier_name;
        String variable_identifier_name;
    };

public:
    ASTFunctionDeclaration(String return_type_identifier_name, String function_identifier_name, Vector<Parameter> parameters,
                           OwnPtr<ASTExecutionScope> body_execution_scope)
        : ASTNode(ASTNodeType::FunctionDeclaration)
        , m_return_type_identifier_name(move(return_type_identifier_name))
        , m_function_identifier_name(move(function_identifier_name))
        , m_parameters(move(parameters))
        , m_body_execution_scope(move(body_execution_scope))
    {}

    virtual ~ASTFunctionDeclaration() override = default;
    virtual StringView class_name() const override { return "ASTFunctionDeclaration"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const String& return_type_identifier_name() const { return m_return_type_identifier_name; }
    NODISCARD ALWAYS_INLINE const String& function_identifier_name() const { return m_function_identifier_name; }
    NODISCARD ALWAYS_INLINE const Vector<Parameter>& parameters() const { return m_parameters; }
    NODISCARD ALWAYS_INLINE const OwnPtr<ASTExecutionScope>& body_execution_scope() const { return m_body_execution_scope; }

    ALWAYS_INLINE ASTFunctionDeclaration& add_parameter(String type_identifier_name, String variable_identifier_name)
    {
        Parameter parameter = {};
        parameter.type_identifier_name = move(type_identifier_name);
        parameter.variable_identifier_name = move(variable_identifier_name);
        m_parameters.push_back(move(parameter));
        return *this;
    }

private:
    String m_return_type_identifier_name;
    String m_function_identifier_name;
    Vector<Parameter> m_parameters;
    OwnPtr<ASTExecutionScope> m_body_execution_scope;
};

class ASTReturnStatement final : public ASTNode {
public:
    explicit ASTReturnStatement(OwnPtr<ASTExpression> return_value_expression)
        : ASTNode(ASTNodeType::ReturnStatement)
        , m_return_value_expression(move(return_value_expression))
    {}

    virtual ~ASTReturnStatement() override = default;
    virtual StringView class_name() const override { return "ASTReturnInstruction"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const OwnPtr<ASTExpression>& return_value_expression() const { return m_return_value_expression; }
    NODISCARD ALWAYS_INLINE bool is_void() const { return !m_return_value_expression.is_valid(); }

private:
    OwnPtr<ASTExpression> m_return_value_expression;
};

} // namespace arc::ast
