/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/assertions.h>
#include <core/containers/own_ptr.h>
#include <core/containers/string.h>
#include <core/containers/string_builder.h>
#include <core/containers/vector.h>

namespace Arc::AST {

enum class ASTNodeType : u32 {
    Unknown = 0,

    ExecutionScope,
    // DeclarationScope,

    Expression,

    // IfStructure,
    WhileStructure,
    // DoWhileStructure,
    // ForStructure,

    ReturnStatement,
    // ContinueStatement,
    // BreakStatement,

    Declaration,
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

class ExecutionScope final : public ASTNode {
public:
    explicit ExecutionScope()
        : ASTNode(ASTNodeType::ExecutionScope)
    {}

    virtual ~ExecutionScope() override = default;
    virtual StringView class_name() const override { return "ExecutionScope"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const Vector<OwnPtr<ASTNode>>& children() const { return m_children; }

    ALWAYS_INLINE ExecutionScope& add_child(OwnPtr<ASTNode> child)
    {
        m_children.push_back(move(child));
        return *this;
    }

private:
    Vector<OwnPtr<ASTNode>> m_children;
};

//========================================================================================================================================//
//----------------------------------------------------------- EXPRESSION NODES -----------------------------------------------------------//
//========================================================================================================================================//

enum class ExpressionType : u8 {
    Unknown = 0,
    Declaration,
    Unary,
    Binary,
    Literal,
    Identifier,
    Assignment,
    Member,
    Call,
};

class Expression : public ASTNode {
public:
    explicit Expression(ExpressionType expression_type)
        : ASTNode(ASTNodeType::Expression)
        , m_expression_type(expression_type)
    {}
    virtual ~Expression() override = default;

public:
    NODISCARD ALWAYS_INLINE ExpressionType expression_type() const { return m_expression_type; }
    NODISCARD ALWAYS_INLINE bool is_unary() const { return m_expression_type == ExpressionType::Unary; }
    NODISCARD ALWAYS_INLINE bool is_binary() const { return m_expression_type == ExpressionType::Binary; }

private:
    ExpressionType m_expression_type;
};

enum class DeclarationType : u8 {
    Variable,
    Function,
    Class,
};

class DeclarationExpression : public Expression {
public:
    explicit DeclarationExpression(DeclarationType declaration_type)
        : Expression(ExpressionType::Declaration)
        , m_declaration_type(declaration_type)
    {}

    virtual ~DeclarationExpression() override = default;

public:
    NODISCARD ALWAYS_INLINE DeclarationType declaration_type() const { return m_declaration_type; }

private:
    DeclarationType m_declaration_type;
};

enum class UnaryOperation : u8 {
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

StringView unary_operation_to_string_view(UnaryOperation unary_operation);
String unary_operation_to_string(UnaryOperation unary_operation);

class UnaryExpression final : public Expression {
public:
    UnaryExpression(UnaryOperation unary_operation, OwnPtr<Expression> expression)
        : Expression(ExpressionType::Unary)
        , m_unary_operation(unary_operation)
        , m_expression(move(expression))
    {}

    virtual ~UnaryExpression() override = default;
    virtual StringView class_name() const override { return "UnaryExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE UnaryOperation unary_operation() const { return m_unary_operation; }

    NODISCARD ALWAYS_INLINE Expression& expression() { return *m_expression; }
    NODISCARD ALWAYS_INLINE const Expression& expression() const { return *m_expression; }

private:
    UnaryOperation m_unary_operation;
    OwnPtr<Expression> m_expression;
};

enum class BinaryOperation : u8 {
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

StringView binary_operation_to_string_view(BinaryOperation binary_operation);
String binary_operation_to_string(BinaryOperation binary_operation);

class BinaryExpression final : public Expression {
public:
    BinaryExpression(BinaryOperation binary_operation, OwnPtr<Expression> left_expression, OwnPtr<Expression> right_expression)
        : Expression(ExpressionType::Binary)
        , m_binary_operation(binary_operation)
        , m_left_expression(move(left_expression))
        , m_right_expression(move(right_expression))
    {}

    virtual ~BinaryExpression() override = default;
    virtual StringView class_name() const override { return "BinaryExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE BinaryOperation binary_operation() const { return m_binary_operation; }

    NODISCARD ALWAYS_INLINE Expression& left_expression() { return *m_left_expression; }
    NODISCARD ALWAYS_INLINE const Expression& left_expression() const { return *m_left_expression; }

    NODISCARD ALWAYS_INLINE Expression& right_expression() { return *m_right_expression; }
    NODISCARD ALWAYS_INLINE const Expression& right_expression() const { return *m_right_expression; }

private:
    BinaryOperation m_binary_operation;
    OwnPtr<Expression> m_left_expression;
    OwnPtr<Expression> m_right_expression;
};

enum class LiteralType : u8 {
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

StringView literal_type_to_string_view(LiteralType literal_type);
String literal_type_to_string(LiteralType literal_type);

class LiteralExpression final : public Expression {
public:
    explicit LiteralExpression(LiteralType literal_type)
        : Expression(ExpressionType::Literal)
        , m_literal_type(literal_type)
        , m_literal_unsigned_integer(0)
        , m_literal_signed_integer(0)
        , m_literal_floating_point_number(0.0)
        , m_literal_character('\0')
    {}

    virtual ~LiteralExpression() override = default;
    virtual StringView class_name() const override { return "LiteralExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE LiteralType literal_type() const { return m_literal_type; }

#define _ARC_DECLARE_LITERAL_FUNCTIONS(expression_type, snake_case_type, type)                                           \
    NODISCARD ALWAYS_INLINE bool is_##snake_case_type() const { return m_literal_type == LiteralType::expression_type; } \
    NODISCARD ALWAYS_INLINE type& snake_case_type()                                                                      \
    {                                                                                                                    \
        ARC_ASSERT(is_##snake_case_type());                                                                              \
        return m_literal_##snake_case_type;                                                                              \
    }                                                                                                                    \
    ALWAYS_INLINE LiteralExpression& set_##snake_case_type(type value)                                                   \
    {                                                                                                                    \
        ARC_ASSERT(is_##snake_case_type());                                                                              \
        m_literal_##snake_case_type = move(value);                                                                       \
        return *this;                                                                                                    \
    }

    ARC_ENUMERATE_LITERAL_TYPES(_ARC_DECLARE_LITERAL_FUNCTIONS)
#undef _ARC_DECLARE_LITERAL_FUNCTIONS

public:
    LiteralType m_literal_type;

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

class IdentifierExpression final : public Expression {
public:
    explicit IdentifierExpression(String identifier_name)
        : Expression(ExpressionType::Identifier)
        , m_identifier_name(move(identifier_name))
    {}

    virtual ~IdentifierExpression() override = default;
    virtual StringView class_name() const override { return "IdentifierExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const String& identifier_name() const { return m_identifier_name; }

private:
    String m_identifier_name;
};

class AssignmentExpression final : public Expression {
public:
    AssignmentExpression(OwnPtr<Expression> left_expression, OwnPtr<Expression> right_expression)
        : Expression(ExpressionType::Assignment)
        , m_left_expression(move(left_expression))
        , m_right_expression(move(right_expression))
    {}

    virtual ~AssignmentExpression() override = default;
    virtual StringView class_name() const override { return "AssignmentExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const OwnPtr<Expression>& left_expression() const { return m_left_expression; }
    NODISCARD ALWAYS_INLINE const OwnPtr<Expression>& right_expression() const { return m_right_expression; }

private:
    OwnPtr<Expression> m_left_expression;
    OwnPtr<Expression> m_right_expression;
};

class MemberExpression final : public Expression {
public:
    MemberExpression(OwnPtr<Expression> instance_expression, String member_identifier_name)
        : Expression(ExpressionType::Member)
        , m_instance_expression(move(instance_expression))
        , m_member_identifier_name(move(member_identifier_name))
    {}

    virtual ~MemberExpression() override = default;
    virtual StringView class_name() const override { return "MemberExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const OwnPtr<Expression>& instance_expression() const { return m_instance_expression; }
    NODISCARD ALWAYS_INLINE const String& member_identifier_name() const { return m_member_identifier_name; }

private:
    OwnPtr<Expression> m_instance_expression;
    String m_member_identifier_name;
};

class CallExpression final : public Expression {
public:
    CallExpression(OwnPtr<Expression> callee_expression)
        : Expression(ExpressionType::Call)
        , m_callee_expression(move(callee_expression))
    {}

    virtual ~CallExpression() override = default;
    virtual StringView class_name() const override { return "CallExpression"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const OwnPtr<Expression>& callee_expression() const { return m_callee_expression; }
    NODISCARD ALWAYS_INLINE const Vector<OwnPtr<Expression>>& parameters() const { return m_parameters; }

    ALWAYS_INLINE CallExpression& add_parameter(OwnPtr<Expression> parameter)
    {
        m_parameters.push_back(move(parameter));
        return *this;
    }

private:
    OwnPtr<Expression> m_callee_expression;
    Vector<OwnPtr<Expression>> m_parameters;
};

//========================================================================================================================================//
//------------------------------------------------------------ STRUCTURE NODES -----------------------------------------------------------//
//========================================================================================================================================//

class WhileStructure final : public ASTNode {
public:
    WhileStructure(OwnPtr<Expression> condition_expression, OwnPtr<ExecutionScope> body_execution_scope)
        : ASTNode(ASTNodeType::WhileStructure)
        , m_condition_expression(move(condition_expression))
        , m_body_execution_scope(move(body_execution_scope))
    {}

    virtual ~WhileStructure() override = default;
    virtual StringView class_name() const override { return "WhileStructure"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const OwnPtr<Expression>& condition_expression() const { return m_condition_expression; }
    NODISCARD ALWAYS_INLINE const OwnPtr<ExecutionScope>& body_execution_scope() const { return m_body_execution_scope; }

private:
    OwnPtr<Expression> m_condition_expression;
    OwnPtr<ExecutionScope> m_body_execution_scope;
};

//========================================================================================================================================//
//------------------------------------------------------------ STATEMENT NODES -----------------------------------------------------------//
//========================================================================================================================================//

class ReturnStatement final : public ASTNode {
public:
    explicit ReturnStatement(OwnPtr<Expression> return_value_expression)
        : ASTNode(ASTNodeType::ReturnStatement)
        , m_return_value_expression(move(return_value_expression))
    {}

    virtual ~ReturnStatement() override = default;
    virtual StringView class_name() const override { return "ReturnStatement"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const OwnPtr<Expression>& return_value_expression() const { return m_return_value_expression; }
    NODISCARD ALWAYS_INLINE bool is_void() const { return !m_return_value_expression.is_valid(); }

private:
    OwnPtr<Expression> m_return_value_expression;
};

//========================================================================================================================================//
//----------------------------------------------------------- DECLARATION NODES ----------------------------------------------------------//
//========================================================================================================================================//

class VariableDeclaration final : public DeclarationExpression {
public:
    VariableDeclaration(String type_identifier_name, String variable_identifier_name)
        : DeclarationExpression(DeclarationType::Variable)
        , m_type_identifier_name(move(type_identifier_name))
        , m_variable_identifier_name(move(variable_identifier_name))
    {}

    virtual ~VariableDeclaration() override = default;
    virtual StringView class_name() const override { return "VariableDeclaration"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const String& type_identifier_name() const { return m_type_identifier_name; }
    NODISCARD ALWAYS_INLINE const String& variable_identifier_name() const { return m_variable_identifier_name; }

private:
    String m_type_identifier_name;
    String m_variable_identifier_name;
};

class FunctionDeclaration final : public DeclarationExpression {
public:
    struct Parameter {
        String type_identifier_name;
        String variable_identifier_name;
    };

public:
    FunctionDeclaration(String return_type_identifier_name, String function_identifier_name, Vector<Parameter> parameters,
                        OwnPtr<ExecutionScope> body_execution_scope)
        : DeclarationExpression(DeclarationType::Function)
        , m_return_type_identifier_name(move(return_type_identifier_name))
        , m_function_identifier_name(move(function_identifier_name))
        , m_parameters(move(parameters))
        , m_body_execution_scope(move(body_execution_scope))
    {}

    virtual ~FunctionDeclaration() override = default;
    virtual StringView class_name() const override { return "FunctionDeclaration"sv; }

    virtual void dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const override;

public:
    NODISCARD ALWAYS_INLINE const String& return_type_identifier_name() const { return m_return_type_identifier_name; }
    NODISCARD ALWAYS_INLINE const String& function_identifier_name() const { return m_function_identifier_name; }
    NODISCARD ALWAYS_INLINE const Vector<Parameter>& parameters() const { return m_parameters; }
    NODISCARD ALWAYS_INLINE const OwnPtr<ExecutionScope>& body_execution_scope() const { return m_body_execution_scope; }

    ALWAYS_INLINE FunctionDeclaration& add_parameter(String type_identifier_name, String variable_identifier_name)
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
    OwnPtr<ExecutionScope> m_body_execution_scope;
};

} // namespace Arc::AST
