#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "scanner.h"
#include "visitors/printer_visitor.h"
#include "visitors/ast_printer_visitor.h"

void test_scanner(const char* input) {
	jade_token token;
	jade_scanner scanner;
	jade_scanner_init(&scanner, input);
	printf("*** Scanner Output ***\n");

	while ((token = jade_scan(&scanner)).kind != JADE_TOKEN_KIND_EOF)
		printf("%s ", jade_token_kind_name(token.kind));

	printf("\n");
	jade_scanner_destroy(&scanner);
}

void test_parser(const char* input) {
	jade_scanner scanner;
	jade_parser parser;
	jade_scanner_init(&scanner, input);
	jade_parser_init(&parser, &scanner);

	jade_program* program = jade_parser_parse(&parser);
	printf("\n*** Printer Output ***\n");
	accept_printer_visitor(program, stdout);
	printf("\n*** AST Printer Output ***\n");
	accept_ast_printer_visitor(program, stdout);

	jade_parser_destroy(&parser);
	jade_scanner_destroy(&scanner);
}

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "USAGE: %s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	test_scanner(argv[1]);
	test_parser(argv[1]);

	return 0;
}
