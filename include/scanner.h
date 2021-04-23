#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "token.h"

typedef struct jade_scanner {
	const char* path;
	const char* source;
	size_t position;
	size_t size;
	size_t line;
	size_t column;
} jade_scanner;

void jade_scanner_init(jade_scanner* scanner, const char* path);
void jade_scanner_destroy(jade_scanner* scanner);
jade_token jade_scan(jade_scanner* scanner);
void jade_lexeme(jade_scanner* scanner, const jade_token* token, char* buffer);
bool jade_is_unary_operator(jade_token_kind kind);
bool jade_is_binary_operator(jade_token_kind kind);
