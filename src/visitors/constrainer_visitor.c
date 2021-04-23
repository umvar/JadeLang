#include "visitor.h"

typedef struct constrainer_visitor {
	VISITORS(constrainer_visitor);
} constrainer_visitor;

static void visit_program(constrainer_visitor* visitor, jade_program* node) {
	visit(visitor, node->definition);
	visit(visitor, node->expression);
}

static void visit_node_list(constrainer_visitor* visitor, jade_node_list* node) {
	ast_node* current = node->first;

	while (current) {
		ast_node* next = current->next;
		visit(visitor, current);
		current = next;
	}
}

static void visit_function_definition(constrainer_visitor* visitor, jade_function_definition* node) {
	visitor->visit_identifier(visitor, node->target);
	visitor->visit_node_list(visitor, node->parameters);
	visit(visitor, node->expression);
}

static void visit_variable_definition(constrainer_visitor* visitor, jade_variable_definition* node) {
	visitor->visit_identifier(visitor, node->target);
	visit(visitor, node->expression);
}

static void visit_identifier(constrainer_visitor* visitor, jade_identifier* node) {
}

static void visit_integer(constrainer_visitor* visitor, jade_integer* node) {
}

static void visit_function_call(constrainer_visitor* visitor, jade_function_call* node) {
	visitor->visit_identifier(visitor, node->target);
	visitor->visit_node_list(visitor, node->arguments);
}

static void visit_unary_expression(constrainer_visitor* visitor, jade_unary_expression* node) {
	visit(visitor, node->expression);
}

static void visit_binary_expression(constrainer_visitor* visitor, jade_binary_expression* node) {
	visit(visitor, node->left_expression);
	visit(visitor, node->right_expression);
}

static void visit_labeled_expression(constrainer_visitor* visitor, jade_labeled_expression* node) {
	visit(visitor, node->expression);
	visit(visitor, node->true_expression);
	visit(visitor, node->false_expression);
}

static void visit_jump_expression(constrainer_visitor* visitor, jade_jump_expression* node) {
}

static void visit_variable_assignment(constrainer_visitor* visitor, jade_variable_assignment* node) {
	visitor->visit_identifier(visitor, node->target);
	visit(visitor, node->expression);
}

void accept_constrainer_visitor(jade_program* node) {
	static constrainer_visitor cv = {
		.visit_program = visit_program,
		.visit_node_list = visit_node_list,
		.visit_function_definition = visit_function_definition,
		.visit_variable_definition = visit_variable_definition,
		.visit_identifier = visit_identifier,
		.visit_integer = visit_integer,
		.visit_function_call = visit_function_call,
		.visit_unary_expression = visit_unary_expression,
		.visit_binary_expression = visit_binary_expression,
		.visit_labeled_expression = visit_labeled_expression,
		.visit_jump_expression = visit_jump_expression,
		.visit_variable_assignment = visit_variable_assignment
	};

	visit_program(&cv, node);
}
