#pragma once

#include "token.h"

typedef enum jade_ast_kind {
	JADE_AST_KIND_PROGRAM,

	JADE_AST_KIND_GLOBAL_DEFINITION_LIST,
	JADE_AST_KIND_FUNCTION_DEFINITION,
	JADE_AST_KIND_VARIABLE_DEFINITION,

	JADE_AST_KIND_EXPRESSION_LIST,
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

typedef struct ast_node {
	jade_ast_kind kind;
	struct ast_node* next;
	struct ast_node* parent;
} ast_node;

typedef struct jade_global_definition_list {
	jade_ast_kind kind;
	ast_node* parent;
	ast_node* first;
	ast_node* last;
} jade_global_definition_list;

typedef struct jade_expression_list {
	jade_ast_kind kind;
	ast_node* parent;
	ast_node* first;
	ast_node* last;
} jade_expression_list;

typedef struct jade_program {
	jade_ast_kind kind;
	ast_node* parent;
	jade_global_definition_list* definitions;
	jade_expression_list* expressions;
} jade_program;

typedef struct jade_identifier {
	jade_ast_kind kind;
	ast_node* parent;
	const char* name;
} jade_identifier;

typedef struct jade_integer {
	jade_ast_kind kind;
	ast_node* parent;
	long value;
} jade_integer;

typedef struct jade_function_call {
	jade_ast_kind kind;
	ast_node* parent;
	jade_identifier* target;
	jade_expression_list* arguments;
} jade_function_call;

typedef struct jade_unary_expression {
	jade_ast_kind kind;
	ast_node* parent;
	jade_token_kind operator;
	ast_node* expression;
} jade_unary_expression;

typedef struct jade_binary_expression {
	jade_ast_kind kind;
	ast_node* parent;
	jade_token_kind operator;
	ast_node* left_expression;
	ast_node* right_expression;
} jade_binary_expression;

typedef struct jade_labeled_expression {
	jade_ast_kind kind;
	ast_node* parent;
	ast_node* expression;
	ast_node* true_expression;
	ast_node* false_expression;
} jade_labeled_expression;

typedef struct jade_jump_expression {
	jade_ast_kind kind;
	ast_node* parent;
	int loop_index;
	int is_break;
} jade_jump_expression;

typedef struct jade_variable_definition {
	jade_ast_kind kind;
	ast_node* parent;
	jade_identifier* target;
	ast_node* expression;
} jade_variable_definition;

typedef struct jade_variable_assignment {
	jade_ast_kind kind;
	ast_node* parent;
	jade_identifier* target;
	ast_node* expression;
} jade_variable_assignment;

typedef struct jade_function_definition {
	jade_ast_kind kind;
	ast_node* parent;
	jade_identifier* target;
	jade_expression_list* parameters;
	ast_node* expression;
} jade_function_definition;

const char* jade_ast_kind_name(jade_ast_kind kind);
