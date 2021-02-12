#include <stdlib.h>
#include "ast.h"
#include "factory.h"

ast_node* jade_create_node(jade_ast_kind kind) {
	ast_node* node = NULL;

	switch (kind) {
		case JADE_AST_KIND_PROGRAM: node = (ast_node*)malloc(sizeof(jade_program)); break;
		case JADE_AST_NODE_LIST: node = (ast_node*)malloc(sizeof(jade_node_list)); break;
		case JADE_AST_KIND_FUNCTION_DEFINITION: node = (ast_node*)malloc(sizeof(jade_function_definition)); break;
		case JADE_AST_KIND_VARIABLE_DEFINITION: node = (ast_node*)malloc(sizeof(jade_variable_definition)); break;
		case JADE_AST_KIND_IDENTIFIER: node = (ast_node*)malloc(sizeof(jade_identifier)); break;
		case JADE_AST_KIND_INTEGER: node = (ast_node*)malloc(sizeof(jade_integer)); break;
		case JADE_AST_KIND_FUNCTION_CALL: node = (ast_node*)malloc(sizeof(jade_function_call)); break;
		case JADE_AST_KIND_UNARY_EXPRESSION: node = (ast_node*)malloc(sizeof(jade_unary_expression)); break;
		case JADE_AST_KIND_BINARY_EXPRESSION: node = (ast_node*)malloc(sizeof(jade_binary_expression)); break;
		case JADE_AST_KIND_LABELED_EXPRESSION: node = (ast_node*)malloc(sizeof(jade_labeled_expression)); break;
		case JADE_AST_KIND_JUMP_EXPRESSION: node = (ast_node*)malloc(sizeof(jade_jump_expression)); break;
		case JADE_AST_KIND_VARIABLE_ASSIGNMENT: node = (ast_node*)malloc(sizeof(jade_variable_assignment)); break;
	}

	node->kind = kind;
	node->next = NULL;
	node->parent = NULL;
	return node;
}
