#pragma once

#include "ast.h"
#include "token.h"
#include "scanner.h"

typedef struct jade_parser {
	jade_scanner* scanner;
	jade_program* ast;
	jade_token token;
} jade_parser;

void jade_parser_init(jade_parser* parser, jade_scanner* scanner);
void jade_parser_destroy(jade_parser* parser);
jade_program* jade_parser_parse(jade_parser* parser);
