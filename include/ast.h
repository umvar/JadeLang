#pragma once

#include "token.h"

#define AST_NODE jade_ast_kind kind; struct ast_node* next; struct ast_node* parent

typedef enum jade_ast_kind {
	JADE_AST_KIND_PROGRAM,
	JADE_AST_NODE_LIST,

	JADE_AST_KIND_FUNCTION_DEFINITION,
	JADE_AST_KIND_VARIABLE_DEFINITION,

	JADE_AST_KIND_IDENTIFIER,
	JADE_AST_KIND_INTEGER,
	JADE_AST_KIND_FUNCTION_CALL,
	JADE_AST_KIND_UNARY_EXPRESSION,
	JADE_AST_KIND_BINARY_EXPRESSION,
	JADE_AST_KIND_LABELED_EXPRESSION,
	JADE_AST_KIND_JUMP_EXPRESSION,
	JADE_AST_KIND_VARIABLE_ASSIGNMENT
} jade_ast_kind;

struct ast_node;
typedef struct ast_node { AST_NODE; } ast_node;

typedef struct jade_node_list {
	AST_NODE;
	ast_node* first;
	ast_node* last;
} jade_node_list;

typedef struct jade_program {
	AST_NODE;
	ast_node* definition;
	ast_node* expression;
} jade_program;

typedef struct jade_identifier {
	AST_NODE;
	const char* name;
} jade_identifier;

typedef struct jade_integer {
	AST_NODE;
	long value;
} jade_integer;

typedef struct jade_function_call {
	AST_NODE;
	jade_identifier* target;
	jade_node_list* arguments; // list of expressions
} jade_function_call;

typedef struct jade_unary_expression {
	AST_NODE;
	jade_token_kind operator;
	ast_node* expression;
} jade_unary_expression;

typedef struct jade_binary_expression {
	AST_NODE;
	jade_token_kind operator;
	ast_node* left_expression;
	ast_node* right_expression;
} jade_binary_expression;

typedef struct jade_labeled_expression {
	AST_NODE;
	ast_node* expression;
	ast_node* true_expression;
	ast_node* false_expression;
} jade_labeled_expression;

typedef struct jade_jump_expression {
	AST_NODE;
	int loop_index;
	int is_break;
} jade_jump_expression;

typedef struct jade_variable_definition {
	AST_NODE;
	jade_identifier* target;
	ast_node* expression;
} jade_variable_definition;

typedef struct jade_variable_assignment {
	AST_NODE;
	jade_identifier* target;
	ast_node* expression;
} jade_variable_assignment;

typedef struct jade_function_definition {
	AST_NODE;
	jade_identifier* target;
	jade_node_list* parameters; // list of jade_identifier
	ast_node* expression;
} jade_function_definition;

const char* jade_ast_kind_name(jade_ast_kind kind);

#undef AST_NODE
