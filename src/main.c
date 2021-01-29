#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "parser.h"

/*
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
*/

int test_parser(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "USAGE: jadec file\n");
		exit(EXIT_FAILURE);
	}

	jade_scanner scanner;
	jade_scanner_init(&scanner, argv[1]);
	//jade_scanner_destroy(&scanner);

	jade_parser parser;
	jade_parser_init(&parser, &scanner);
	jade_parser_parse(&parser);

	//printf("%s\n", parser.scanner->source);

	return EXIT_SUCCESS;
}


int main(int argc, char** argv) {
	//return test_scanner(argc, argv);
	return test_parser(argc, argv);
}
