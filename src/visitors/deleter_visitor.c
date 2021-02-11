#include <stdlib.h>
#include "visitor.h"

static void visit_program(jade_visitor* visitor, jade_program* node) {
	visitor->visit_global_definition_list(visitor, node->definitions);
	visitor->visit_expression_list(visitor, node->expressions);
	free(node);
}

static void visit_global_definition_list(jade_visitor* visitor, jade_global_definition_list* node) {
	ast_node* current = node->first;

	while (current) {
		ast_node* next = current->next;
		visit(visitor, current);
		free(current);
		current = next;
	}

	free(node);
}

static void visit_function_definition(jade_visitor* visitor, jade_function_definition* node) {
	visitor->visit_identifier(visitor, node->target);
	visitor->visit_expression_list(visitor, node->parameters);
	visit(visitor, node->expression);
	free(node);
}

static void visit_variable_definition(jade_visitor* visitor, jade_variable_definition* node) {
	visitor->visit_identifier(visitor, node->target);
	visit(visitor, node->expression);
	free(node);
}

static void visit_expression_list(jade_visitor* visitor, jade_expression_list* node) {
	ast_node* current = node->first;

	while (current) {
		ast_node* next = current->next;
		visit(visitor, current);
		free(current);
		current = next;
	}

	free(node);
}

static void visit_identifier(jade_visitor* visitor, jade_identifier* node) {
	free(node);
}

static void visit_integer(jade_visitor* visitor, jade_integer* node) {
	free(node);
}

static void visit_function_call(jade_visitor* visitor, jade_function_call* node) {
	visitor->visit_identifier(visitor, node->target);
	visitor->visit_expression_list(visitor, node->arguments);
	free(node);
}

static void visit_unary_expression(jade_visitor* visitor, jade_unary_expression* node) {
	visit(visitor, node->expression);
	free(node);
}

static void visit_binary_expression(jade_visitor* visitor, jade_binary_expression* node) {
	visit(visitor, node->left_expression);
	visit(visitor, node->right_expression);
	free(node);
}

static void visit_labeled_expression(jade_visitor* visitor, jade_labeled_expression* node) {
	visit(visitor, node->expression);
	visit(visitor, node->true_expression);
	visit(visitor, node->false_expression);
	free(node);
}

static void visit_jump_expression(jade_visitor* visitor, jade_jump_expression* node) {
	free(node);
}

static void visit_variable_assignment(jade_visitor* visitor, jade_variable_assignment* node) {
	visitor->visit_identifier(visitor, node->target);
	visit(visitor, node->expression);
	free(node);
}

void accept_deleter_visitor(jade_program* node) {
	jade_visitor dv = {
		.visit_program = visit_program,
		.visit_global_definition_list = visit_global_definition_list,
		.visit_function_definition = visit_function_definition,
		.visit_variable_definition = visit_variable_definition,
		.visit_expression_list = visit_expression_list,
		.visit_identifier = visit_identifier,
		.visit_integer = visit_integer,
		.visit_function_call = visit_function_call,
		.visit_unary_expression = visit_unary_expression,
		.visit_binary_expression = visit_binary_expression,
		.visit_labeled_expression = visit_labeled_expression,
		.visit_jump_expression = visit_jump_expression,
		.visit_variable_assignment = visit_variable_assignment
	};

	visit_program(&dv, node);
}
