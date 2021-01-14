#include <stdio.h>
#include <stdlib.h>
#include "visitors/printer_visitor.h"
#include "scanner.h"
#include "ast.h"

int test_scanner(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "USAGE: jadec file\n");
		exit(EXIT_FAILURE);
	}

	char lexeme[64];
	jade_token token;
	jade_scanner scanner;
	jade_scanner_init(&scanner, argv[1]);

	while ((token = jade_scan(&scanner)).kind != JADE_TOKEN_KIND_EOF) {
		jade_lexeme(&scanner, &token, lexeme);

		printf(
			"%s:%ld:%ld: %s (%s)\n",
			scanner.path,
			token.line,
			token.column,
			jade_token_kind_name(token.kind),
			lexeme
		);
	}

	jade_scanner_destroy(&scanner);
	return EXIT_SUCCESS;
}

int test_printer_visitor(int argc, char** argv) {
	jade_program program = { JADE_AST_KIND_PROGRAM, NULL };
	jade_global_definition_list definitions = { JADE_AST_KIND_GLOBAL_DEFINITION_LIST, (ast_node*)&program };
	jade_expression_list expressions = { JADE_AST_KIND_EXPRESSION_LIST, (ast_node*)&program };

	program.definitions = &definitions;
	program.expressions = &expressions;

	// f(x) = 3 + x
	jade_function_definition f = { JADE_AST_KIND_FUNCTION_DEFINITION, (ast_node*)&definitions, NULL, NULL, NULL };

	// f
	jade_identifier f_0 = { JADE_AST_KIND_IDENTIFIER, (ast_node*)&f, "f" };
	f.target = &f_0;

	// (x)
	jade_expression_list params = { JADE_AST_KIND_EXPRESSION_LIST, (ast_node*)&f, NULL, NULL };
	jade_identifier x_1 = { JADE_AST_KIND_IDENTIFIER, (ast_node*)&params, "x" };
	jade_node param0 = { NULL, (ast_node*)&x_1 };
	params.first = &param0;
	params.last = &param0;
	f.parameters = &params;

	// 3 + x
	jade_binary_expression bin_expr = { JADE_AST_KIND_BINARY_EXPRESSION, (ast_node*)&f, JADE_TOKEN_KIND_PLUS, NULL, NULL };
	jade_integer i_3 = { JADE_AST_KIND_INTEGER, (ast_node*)&bin_expr, 3 };
	jade_identifier x_0 = { JADE_AST_KIND_IDENTIFIER, (ast_node*)&bin_expr, "x" };
	bin_expr.left_expression = (ast_node*)&i_3;
	bin_expr.right_expression = (ast_node*)&x_0;
	f.expression = (ast_node*)&bin_expr;

	jade_node def_f = { NULL, (ast_node*)&f };

	definitions.first = &def_f;
	definitions.last = &def_f;

	expressions.first = NULL;
	expressions.last = NULL;

	accept_printer(&program);
	return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
	return test_printer_visitor(argc, argv);
}
