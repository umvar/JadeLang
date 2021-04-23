#include <stdio.h>
#include <stdlib.h>
#include "visitor.h"

typedef struct ast_printer_visitor {
	VISITORS(ast_printer_visitor);
	FILE* file;
	size_t depth;
} ast_printer_visitor;

static void print_indentation(ast_printer_visitor* visitor) {
	for (size_t i = 0; i < visitor->depth; i++)
		printf("\t");
}

static void visit_program(ast_printer_visitor* visitor, jade_program* node) {
	print_indentation(visitor);
	fprintf(visitor->file, "program\n");
	visitor->depth += 1;
	visit(visitor, node->definition);
	visit(visitor, node->expression);
	visitor->depth -= 1;
}

static void visit_node_list(ast_printer_visitor* visitor, jade_node_list* node) {
	print_indentation(visitor);
	fprintf(visitor->file, "node_list\n");
	ast_node* current = node->first;
	visitor->depth += 1;

	while (current) {
		visit(visitor, current);
		current = current->next;
	}

	visitor->depth -= 1;
}

static void visit_function_definition(ast_printer_visitor* visitor, jade_function_definition* node) {
	print_indentation(visitor);
	fprintf(visitor->file, "function_definition\n");
	visitor->depth += 1;
	visitor->visit_identifier(visitor, node->target);
	visitor->visit_node_list(visitor, node->parameters);
	visit(visitor, node->expression);
	visitor->depth -= 1;
}

static void visit_variable_definition(ast_printer_visitor* visitor, jade_variable_definition* node) {
	print_indentation(visitor);
	fprintf(visitor->file, "variable_definition\n");
	visitor->depth += 1;
	visitor->visit_identifier(visitor, node->target);
	visit(visitor, node->expression);
	visitor->depth -= 1;
}

static void visit_identifier(ast_printer_visitor* visitor, jade_identifier* node) {
	print_indentation(visitor);
	fprintf(visitor->file, "identifier (%s)\n", node->name);
}

static void visit_integer(ast_printer_visitor* visitor, jade_integer* node) {
	print_indentation(visitor);
	fprintf(visitor->file, "integer (%lld)\n", node->value);
}

static void visit_function_call(ast_printer_visitor* visitor, jade_function_call* node) {
	print_indentation(visitor);
	fprintf(visitor->file, "function_call\n");
	visitor->depth += 1;
	visitor->visit_identifier(visitor, node->target);
	visitor->visit_node_list(visitor, node->arguments);
	visitor->depth -= 1;
}

static void visit_unary_expression(ast_printer_visitor* visitor, jade_unary_expression* node) {
	print_indentation(visitor);
	fprintf(visitor->file, "unary_expression (%s)\n", jade_token_kind_name(node->operator));
	visitor->depth += 1;
	visit(visitor, node->expression);
	visitor->depth -= 1;
}

static void visit_binary_expression(ast_printer_visitor* visitor, jade_binary_expression* node) {
	print_indentation(visitor);
	fprintf(visitor->file, "binary_expression (%s)\n", jade_token_kind_name(node->operator));
	visitor->depth += 1;
	visit(visitor, node->left_expression);
	visit(visitor, node->right_expression);
	visitor->depth -= 1;
}

static void visit_labeled_expression(ast_printer_visitor* visitor, jade_labeled_expression* node) {
	print_indentation(visitor);
	fprintf(visitor->file, "labeled_expression\n");
	visitor->depth += 1;
	visit(visitor, node->expression);
	visit(visitor, node->true_expression);
	visit(visitor, node->false_expression);
	visitor->depth -= 1;
}

static void visit_jump_expression(ast_printer_visitor* visitor, jade_jump_expression* node) {
	print_indentation(visitor);
	fprintf(visitor->file, "jump_expression (index = %zu, break = %d)\n", node->loop_index, node->is_break);
}

static void visit_variable_assignment(ast_printer_visitor* visitor, jade_variable_assignment* node) {
	print_indentation(visitor);
	fprintf(visitor->file, "variable_assignment\n");
	visitor->depth += 1;
	visitor->visit_identifier(visitor, node->target);
	visit(visitor, node->expression);
	visitor->depth -= 1;
}

void accept_ast_printer_visitor(jade_program* node, FILE* file) {
	ast_printer_visitor pv = {
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
		.visit_variable_assignment = visit_variable_assignment,
		.file = file,
		.depth = 0
	};

	visit_program(&pv, node);
}
