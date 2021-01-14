#include <stddef.h>
#include "parser.h"

void jade_parser_init(jade_parser* parser, jade_scanner* scanner) {
	parser->scanner = scanner;
	parser->ast = NULL;
}

void jade_parser_destroy(jade_parser* parser) {
	parser->scanner = NULL;

	// free all nodes in ast
	parser->ast = NULL;
}

jade_program* jade_parser_parse(jade_scanner* scanner) {
	// TODO: implement
	return NULL;
}
