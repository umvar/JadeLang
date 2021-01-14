#pragma once

#include "ast.h"

struct jade_visitor;

#define VISITOR(type, visitor_type) void (*visit_##type)(struct visitor_type* visitor, jade_##type* node)
#define VISITORS(visitor_type) VISITOR(program, visitor_type); VISITOR(global_definition_list, visitor_type); VISITOR(function_definition, visitor_type); VISITOR(variable_definition, visitor_type); VISITOR(expression_list, visitor_type); VISITOR(identifier, visitor_type); VISITOR(integer, visitor_type); VISITOR(function_call, visitor_type); VISITOR(unary_expression, visitor_type); VISITOR(binary_expression, visitor_type); VISITOR(labeled_expression, visitor_type); VISITOR(jump_expression, visitor_type); VISITOR(variable_assignment, visitor_type)

typedef struct jade_visitor {
	VISITORS(jade_visitor);
} jade_visitor;

void visit(void* visitor, ast_node* node);
