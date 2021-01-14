#pragma once

#include "token.h"

typedef struct jade_scanner {
	const char* path;
	const char* source;
	long position;
	long size;
	long line;
	long column;
} jade_scanner;

void jade_scanner_init(jade_scanner* scanner, const char* path);
void jade_scanner_destroy(jade_scanner* scanner);
jade_token jade_scan(jade_scanner* scanner);
void jade_lexeme(jade_scanner* scanner, const jade_token* token, char* buffer);
