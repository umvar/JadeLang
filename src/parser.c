#include <stddef.h>
#include "parser.h"
#include "visitors/deleter_visitor.h"

void jade_parser_init(jade_parser* parser, jade_scanner* scanner) {
	parser->scanner = scanner;
	parser->ast = NULL;
}

void jade_parser_destroy(jade_parser* parser) {
	accept_deleter_visitor(parser->ast);
}

jade_program* jade_parser_parse(jade_scanner* scanner) {
	// TODO: implement
	return NULL;
}
