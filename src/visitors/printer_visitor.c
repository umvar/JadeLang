#include <stdio.h>
#include "visitor.h"

typedef struct printer_visitor {
	VISITORS(printer_visitor);
	long depth;
} printer_visitor;

static void write_depth(long depth) {
	for (long i = 0; i < depth; i++) {
		printf("  ");
	}
}

static void visit_program(printer_visitor* visitor, jade_program* node) {
	write_depth(visitor->depth);
	printf("Program\n");

	visitor->depth += 1;
	visitor->visit_global_definition_list(visitor, node->definitions);
	visitor->visit_expression_list(visitor, node->expressions);
	visitor->depth -= 1;
}

static void visit_global_definition_list(printer_visitor* visitor, jade_global_definition_list* node) {
	write_depth(visitor->depth);
	printf("Global Definition List\n");

	visitor->depth += 1;

	jade_node* current = node->first;

	while (current) {
		visit(visitor, current->expression);
		current = current->next;
	}

	visitor->depth -= 1;
}

static void visit_function_definition(printer_visitor* visitor, jade_function_definition* node) {
	write_depth(visitor->depth);
	printf("Function Definition\n");

	visitor->depth += 1;
	visitor->visit_identifier(visitor, node->target);
	visitor->visit_expression_list(visitor, node->parameters);
	visit(visitor, node->expression);
	visitor->depth -= 1;
}

static void visit_variable_definition(printer_visitor* visitor, jade_variable_definition* node) {
	write_depth(visitor->depth);
	printf("Variable Definition\n");

	visitor->depth += 1;
	visitor->visit_identifier(visitor, node->target);
	visit(visitor, node->expression);
	visitor->depth -= 1;
}

static void visit_expression_list(printer_visitor* visitor, jade_expression_list* node) {
	write_depth(visitor->depth);
	printf("Expression List\n");

	visitor->depth += 1;

	jade_node* current = node->first;

	while (current) {
		visit(visitor, current->expression);
		current = current->next;
	}

	visitor->depth -= 1;
}

static void visit_identifier(printer_visitor* visitor, jade_identifier* node) {
	write_depth(visitor->depth);
	printf("Identifier (%s)\n", node->name);
}

static void visit_integer(printer_visitor* visitor, jade_integer* node) {
	write_depth(visitor->depth);
	printf("Integer (%ld)\n", node->value);
}

static void visit_function_call(printer_visitor* visitor, jade_function_call* node) {
	write_depth(visitor->depth);
	printf("Function Call\n");

	visitor->depth += 1;
	visitor->visit_identifier(visitor, node->target);
	visitor->visit_expression_list(visitor, node->arguments);
	visitor->depth -= 1;
}

static void visit_unary_expression(printer_visitor* visitor, jade_unary_expression* node) {
	write_depth(visitor->depth);
	printf("Unary Expression (operator = %s)\n", jade_token_kind_name(node->operator));

	visitor->depth += 1;
	visit(visitor, node->expression);
	visitor->depth -= 1;
}

static void visit_binary_expression(printer_visitor* visitor, jade_binary_expression* node) {
	write_depth(visitor->depth);
	printf("Binary Expression (operator = %s)\n", jade_token_kind_name(node->operator));

	visitor->depth += 1;
	visit(visitor, node->left_expression);
	visit(visitor, node->right_expression);
	visitor->depth -= 1;
}

static void visit_labeled_expression(printer_visitor* visitor, jade_labeled_expression* node) {
	write_depth(visitor->depth);
	printf("Labeled Expression\n");

	visitor->depth += 1;
	visit(visitor, node->expression);
	visit(visitor, node->true_expression);
	visit(visitor, node->false_expression);
	visitor->depth -= 1;
}

static void visit_jump_expression(printer_visitor* visitor, jade_jump_expression* node) {
	write_depth(visitor->depth);
	printf("Jump Expression (loops: %d, break: %d)\n", node->loop_index, node->is_break);
}

static void visit_variable_assignment(printer_visitor* visitor, jade_variable_assignment* node) {
	write_depth(visitor->depth);
	printf("Variable Assignment\n");

	visitor->depth += 1;
	visitor->visit_identifier(visitor, node->target);
	visit(visitor, node->expression);
	visitor->depth -= 1;
}

void accept_printer_visitor(jade_program* node) {
	printer_visitor pv = {
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
		.visit_variable_assignment = visit_variable_assignment,
		.depth = 0
	};

	visit_program(&pv, node);
}
