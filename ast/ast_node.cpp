/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <ast/ast_node.h>
#include <core/containers/string_builder.h>

namespace arc::ast {

void ExecutionScope::dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const
{
    for (usize child_index = 0; child_index < m_children.count(); ++child_index) {
        builder.append_indentation(indentation_level);
        builder.append("({}) [{}]\n"sv, m_children[child_index]->class_name(), child_index);
        m_children[child_index]->dump_as_string(builder, indentation_level + indentation_count, indentation_count);
    }
}

//========================================================================================================================================//
//----------------------------------------------------------- EXPRESSION NODES -----------------------------------------------------------//
//========================================================================================================================================//

StringView unary_operation_to_string_view(UnaryOperation unary_operation)
{
    switch (unary_operation) {
#define _ARC_CASE(x)        \
    case UnaryOperation::x: \
        return #x##sv;

        ARC_ENUMERATE_UNARY_OPERATIONS(_ARC_CASE)
#undef _ARC_CASE

        default:
            ARC_ASSERT_NOT_REACHED;
    }
}

String unary_operation_to_string(UnaryOperation unary_operation)
{
    const StringView unary_operation_string_view = unary_operation_to_string_view(unary_operation);
    return String(unary_operation_string_view);
}

void UnaryExpression::dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const
{
    // Operation.
    builder.append_indentation(indentation_level);
    builder.append("Operation: {}\n"sv, unary_operation_to_string_view(m_unary_operation));
    // Operand.
    builder.append_indentation(indentation_level);
    builder.append("Operand: ({})\n"sv, m_expression->class_name());
    m_expression->dump_as_string(builder, indentation_level + indentation_count, indentation_count);
}

StringView binary_operation_to_string_view(BinaryOperation binary_operation)
{
    switch (binary_operation) {
#define _ARC_CASE(x)         \
    case BinaryOperation::x: \
        return #x##sv;

        ARC_ENUMERATE_BINARY_OPERATIONS(_ARC_CASE)
#undef _ARC_CASE

        default:
            ARC_ASSERT_NOT_REACHED;
    }
}

String binary_operation_to_string(BinaryOperation binary_operation)
{
    const StringView binary_operation_string_view = binary_operation_to_string_view(binary_operation);
    return String(binary_operation_string_view);
}

void BinaryExpression::dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const
{
    // Operation.
    builder.append_indentation(indentation_level);
    builder.append("Operation: {}\n"sv, binary_operation_to_string_view(m_binary_operation));
    // Left operand.
    builder.append_indentation(indentation_level);
    builder.append("Left operand: ({})\n"sv, m_left_expression->class_name());
    m_left_expression->dump_as_string(builder, indentation_level + indentation_count, indentation_count);
    // Right operand.
    builder.append_indentation(indentation_level);
    builder.append("Right operand: ({})\n"sv, m_right_expression->class_name());
    m_right_expression->dump_as_string(builder, indentation_level + indentation_count, indentation_count);
}

StringView literal_type_to_string_view(LiteralType literal_type)
{
    switch (literal_type) {
#define _ARC_CASE(x, n, t) \
    case LiteralType::x:   \
        return #x##sv;

        ARC_ENUMERATE_LITERAL_TYPES(_ARC_CASE)
#undef _ARC_CASE

        default:
            ARC_ASSERT_NOT_REACHED;
    }
}

String literal_type_to_string(LiteralType literal_type)
{
    const StringView literal_type_string_view = literal_type_to_string_view(literal_type);
    return String(literal_type_string_view);
}

void LiteralExpression::dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const
{
    // Type.
    builder.append_indentation(indentation_level);
    builder.append("Type:  {}\n"sv, literal_type_to_string_view(m_literal_type));
// Value.
#define _ARC_DUMP_LITERAL_VALUE(e, n, t)                \
    if (m_literal_type == LiteralType::e) {             \
        builder.append_indentation(indentation_level);  \
        builder.append("Value: {}\n"sv, m_literal_##n); \
    }

    ARC_ENUMERATE_LITERAL_TYPES(_ARC_DUMP_LITERAL_VALUE)
#undef _ARC_DUMP_LITERAL_VALUE
}

void IdentifierExpression::dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const
{
    builder.append_indentation(indentation_level);
    builder.append("Identifier name: {}\n"sv, m_identifier_name);
}

void AssignmentExpression::dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const
{
    // LHS.
    builder.append_indentation(indentation_level);
    builder.append("LHS: ({})\n"sv, m_left_expression->class_name());
    m_left_expression->dump_as_string(builder, indentation_level + indentation_count, indentation_count);
    // RHS.
    builder.append_indentation(indentation_level);
    builder.append("RHS: ({})\n"sv, m_right_expression->class_name());
    m_right_expression->dump_as_string(builder, indentation_level + indentation_count, indentation_count);
}

void MemberExpression::dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const
{}

void CallExpression::dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const
{}

//========================================================================================================================================//
//------------------------------------------------------------ STRUCTURE NODES -----------------------------------------------------------//
//========================================================================================================================================//

void WhileStructure::dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const
{
    builder.append_indentation(indentation_level);
    builder.append("({})\n"sv, m_body_execution_scope->class_name());
    m_body_execution_scope->dump_as_string(builder, indentation_level + indentation_count, indentation_count);
}

//========================================================================================================================================//
//------------------------------------------------------------ STATEMENT NODES -----------------------------------------------------------//
//========================================================================================================================================//

void ReturnStatement::dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const
{
    builder.append_indentation(indentation_level);
    if (m_return_value_expression.is_valid()) {
        builder.append("({})\n"sv, m_return_value_expression->class_name());
        m_return_value_expression->dump_as_string(builder, indentation_level + indentation_count, indentation_count);
    }
    else {
        builder.append("(void)\n"sv);
    }
}

//========================================================================================================================================//
//----------------------------------------------------------- DECLARATION NODES ----------------------------------------------------------//
//========================================================================================================================================//

void VariableDeclaration::dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const
{
    // Variable type.
    builder.append_indentation(indentation_level);
    builder.append("Type identifier name:    {}\n"sv, m_type_identifier_name);
    // Variable identifier name.
    builder.append_indentation(indentation_level);
    builder.append("Variable dentifier name: {}\n"sv, m_variable_identifier_name);
}

void FunctionDeclaration::dump_as_string(StringBuilder& builder, u32 indentation_level, u32 indentation_count) const
{
    // Return type.
    builder.append_indentation(indentation_level);
    builder.append("Return type:     {}\n"sv, m_return_type_identifier_name);

    // Identifier name.
    builder.append_indentation(indentation_level);
    builder.append("Identifier name: {}\n"sv, m_function_identifier_name);

    // Parameters.
    builder.append_indentation(indentation_level);
    builder.append("Parameters:      "sv);
    for (usize parameter_index = 0; parameter_index < m_parameters.count(); ++parameter_index) {
        builder.append("{} {}"sv, m_parameters[parameter_index].type_identifier_name,
                       m_parameters[parameter_index].variable_identifier_name);
        if (parameter_index != (m_parameters.count() - 1))
            builder.append(","sv);
    }
    builder.append_newline();

    // Body.
    builder.append_indentation(indentation_level);
    builder.append("Body: ({})\n"sv, m_body_execution_scope->class_name());
    m_body_execution_scope->dump_as_string(builder, indentation_level + indentation_count, indentation_count);
}

} // namespace arc::ast
