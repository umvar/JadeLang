#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "factory.h"
#include "parser.h"
#include "scanner.h"
#include "symbol_table.h"
#include "util.h"
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

void test_symbol_table() {
	symbol_table symbols_;
	symbol_table* symbols = &symbols_;

	jade_identifier* id_a = (jade_identifier*)jade_create_node(JADE_AST_KIND_IDENTIFIER);
	id_a->name = "a";
	id_a->hash = compute_string_hash(id_a->name);

	jade_identifier* id_a_2 = (jade_identifier*)jade_create_node(JADE_AST_KIND_IDENTIFIER);
	id_a_2->name = "a";
	id_a_2->hash = compute_string_hash(id_a_2->name);

	jade_identifier* id_b = (jade_identifier*)jade_create_node(JADE_AST_KIND_IDENTIFIER);
	id_b->name = "b";
	id_b->hash = compute_string_hash(id_b->name);

	symbol_table_init(symbols);

	printf("%p\n", (void*)symbol_table_bind(symbols, id_a, NULL));
	printf("%p\n", (void*)symbol_table_bind(symbols, id_b, NULL));
	printf("%p\n", (void*)symbol_table_bind(symbols, id_b, NULL));

	symbol_table inner;
	symbol_table_init(&inner);
	symbol_table_open_scope(&symbols, &inner);

	printf("%p\n", (void*)symbol_table_bind(symbols, id_a, NULL));
	printf("%p\n", (void*)symbol_table_bind(symbols, id_b, NULL));
	printf("%p\n", (void*)symbol_table_bind(symbols, id_b, NULL));

	symbol_table_close_scope(&symbols);
	symbol_table_destroy(&inner);
	symbol_table_destroy(symbols);
	free(id_a);
	free(id_a_2);
	free(id_b);
}

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "USAGE: %s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	test_scanner(argv[1]);
	test_parser(argv[1]);
	test_symbol_table();

	return 0;
}
