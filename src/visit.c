#include "visitor.h"

#define VISIT(type) ((jade_visitor*)visitor)->visit_##type(visitor, (jade_##type*)node)

void visit(void* visitor, ast_node* node) {
	switch (node->kind) {
		case JADE_AST_KIND_PROGRAM: VISIT(program); break;
		case JADE_AST_KIND_GLOBAL_DEFINITION_LIST: VISIT(global_definition_list); break;
		case JADE_AST_KIND_FUNCTION_DEFINITION: VISIT(function_definition); break;
		case JADE_AST_KIND_VARIABLE_DEFINITION: VISIT(variable_definition); break;
		case JADE_AST_KIND_EXPRESSION_LIST: VISIT(expression_list); break;
		case JADE_AST_KIND_IDENTIFIER: VISIT(identifier); break;
		case JADE_AST_KIND_INTEGER: VISIT(integer); break;
		case JADE_AST_KIND_FUNCTION_CALL: VISIT(function_call); break;
		case JADE_AST_KIND_UNARY_EXPRESSION: VISIT(unary_expression); break;
		case JADE_AST_KIND_BINARY_EXPRESSION: VISIT(binary_expression); break;
		case JADE_AST_KIND_LABELED_EXPRESSION: VISIT(labeled_expression); break;
		case JADE_AST_KIND_JUMP_EXPRESSION: VISIT(jump_expression); break;
		case JADE_AST_KIND_VARIABLE_ASSIGNMENT: VISIT(variable_assignment); break;
		default: break;
	}
}
