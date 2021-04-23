#include <stdio.h>
#include "visitor.h"

typedef struct printer_visitor {
	VISITORS(printer_visitor);
	FILE* file;
} printer_visitor;

static void visit_program(printer_visitor* visitor, jade_program* node) {
	visit(visitor, node->definition);
	fprintf(visitor->file, ",\n");
	visit(visitor, node->expression);
	fprintf(visitor->file, "\n");
}

static void visit_node_list(printer_visitor* visitor, jade_node_list* node) {
	ast_node* current = node->first;
	bool wrap = node->first != node->last;

	if (
		node->parent->kind == JADE_AST_KIND_FUNCTION_DEFINITION &&
		node == ((jade_function_definition*)node->parent)->parameters
	) {
		// do not wrap function definition parameters in parenthesis
		wrap = false;
	} else if (
		node->parent->kind == JADE_AST_KIND_FUNCTION_CALL &&
		node == ((jade_function_call*)node->parent)->arguments
	) {
		// do not wrap function call arguments in parenthesis
		wrap = false;
	}

	if (wrap)
		fprintf(visitor->file, "(");

	while (current) {
		visit(visitor, current);

		if (current->next)
			fprintf(visitor->file, ", ");

		current = current->next;
	}

	if (wrap)
		fprintf(visitor->file, ")");
}

static void visit_function_definition(printer_visitor* visitor, jade_function_definition* node) {
	visitor->visit_identifier(visitor, node->target);
	fprintf(visitor->file, "(");
	visitor->visit_node_list(visitor, node->parameters);
	fprintf(visitor->file, ") %s ", jade_token_kind_name(JADE_TOKEN_KIND_DEFINE));
	visit(visitor, node->expression);
}

static void visit_variable_definition(printer_visitor* visitor, jade_variable_definition* node) {
	visitor->visit_identifier(visitor, node->target);
	fprintf(visitor->file, " %s ", jade_token_kind_name(JADE_TOKEN_KIND_DEFINE));
	visit(visitor, node->expression);
}

static void visit_identifier(printer_visitor* visitor, jade_identifier* node) {
	fprintf(visitor->file, "%s", node->name);
}

static void visit_integer(printer_visitor* visitor, jade_integer* node) {
	fprintf(visitor->file, "%lld", node->value);
}

static void visit_function_call(printer_visitor* visitor, jade_function_call* node) {
	visitor->visit_identifier(visitor, node->target);
	fprintf(visitor->file, "(");
	visitor->visit_node_list(visitor, node->arguments);
	fprintf(visitor->file, ")");
}

static void visit_unary_expression(printer_visitor* visitor, jade_unary_expression* node) {
	fprintf(visitor->file, "%s", jade_token_kind_name(node->operator));
	visit(visitor, node->expression);
}

static void visit_binary_expression(printer_visitor* visitor, jade_binary_expression* node) {
	visit(visitor, node->left_expression);
	fprintf(visitor->file, " %s ", jade_token_kind_name(node->operator));
	visit(visitor, node->right_expression);
}

static void visit_labeled_expression(printer_visitor* visitor, jade_labeled_expression* node) {
	fprintf(visitor->file, "#(");
	visit(visitor, node->expression);
	fprintf(visitor->file, ", ");
	visit(visitor, node->true_expression);
	fprintf(visitor->file, ", ");
	visit(visitor, node->false_expression);
	fprintf(visitor->file, ")");
}

static void visit_jump_expression(printer_visitor* visitor, jade_jump_expression* node) {
	for (size_t i = 0; i <= node->loop_index; i++)
		fprintf(visitor->file, "#");

	if (node->is_break)
		fprintf(visitor->file, "!");
}

static void visit_variable_assignment(printer_visitor* visitor, jade_variable_assignment* node) {
	visitor->visit_identifier(visitor, node->target);
	fprintf(visitor->file, " %s ", jade_token_kind_name(JADE_TOKEN_KIND_ASSIGN));
	visit(visitor, node->expression);
}

void accept_printer_visitor(jade_program* node, FILE* file) {
	printer_visitor pv = {
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
		.file = file
	};

	visit_program(&pv, node);
}
