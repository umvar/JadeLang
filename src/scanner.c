#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

static char peek_char(jade_scanner* scanner);
static char get_char(jade_scanner* scanner);
static char next_char(jade_scanner* scanner);
static void syntax_error(jade_scanner* scanner, size_t line, size_t column);
static void skip_line(jade_scanner* scanner);
static void skip_whitespace(jade_scanner* scanner);
static void skip_identifier(jade_scanner* scanner);
static void skip_integer(jade_scanner* scanner);

void jade_scanner_init(jade_scanner* scanner, const char* path) {
	FILE* file = fopen(path, "r");
	char* source;
	size_t size;

	if (!file) {
		perror("fopen() failed");
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file);

	if (size < 0) {
		perror("ftell() failed");
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_SET);

	source = malloc(size + 1);
	fread(source, 1, size, file);
	source[size] = 0;
	fclose(file);

	scanner->path = path;
	scanner->source = source;
	scanner->position = 0;
	scanner->size = size;
	scanner->line = 1;
	scanner->column = 1;
}

void jade_scanner_destroy(jade_scanner* scanner) {
	free((void*)scanner->source);
}

jade_token jade_scan(jade_scanner* scanner) {
	char ch = peek_char(scanner);

	jade_token token = {
		.position = scanner->position,
		.size = 0,
		.line = scanner->line,
		.column = scanner->column
	};

	switch (ch) {
		case EOF: token.kind = JADE_TOKEN_KIND_EOF; break;
		case ';': skip_line(scanner); return jade_scan(scanner);
		case '(': token.kind = JADE_TOKEN_KIND_LPAREN; get_char(scanner); break;
		case ')': token.kind = JADE_TOKEN_KIND_RPAREN; get_char(scanner); break;
		case ',': token.kind = JADE_TOKEN_KIND_DELIMITER; get_char(scanner); break;
		case '#': token.kind = JADE_TOKEN_KIND_HASH; get_char(scanner); break;
		case '+': token.kind = JADE_TOKEN_KIND_PLUS; get_char(scanner); break;
		case '-': token.kind = JADE_TOKEN_KIND_MINUS; get_char(scanner); break;
		case '*': token.kind = JADE_TOKEN_KIND_TIMES; get_char(scanner); break;
		case '/': token.kind = JADE_TOKEN_KIND_DIVIDE; get_char(scanner); break;
		case '%': token.kind = JADE_TOKEN_KIND_MODULO; get_char(scanner); break;
		case '~': token.kind = JADE_TOKEN_KIND_BNOT; get_char(scanner); break;
		case '^': token.kind = JADE_TOKEN_KIND_BXOR; get_char(scanner); break;
		case '=':
			if (next_char(scanner) == '=') { token.kind = JADE_TOKEN_KIND_EQ; get_char(scanner); }
			else { token.kind = JADE_TOKEN_KIND_ASSIGN; }
			break;
		case ':':
			if (next_char(scanner) == '=') { token.kind = JADE_TOKEN_KIND_DEFINE; get_char(scanner); }
			else { syntax_error(scanner, token.line, token.column); }
			break;
		case '|':
			if (next_char(scanner) == '|') { token.kind = JADE_TOKEN_KIND_OR; get_char(scanner); }
			else { token.kind = JADE_TOKEN_KIND_BOR; }
			break;
		case '&':
			if (next_char(scanner) == '&') { token.kind = JADE_TOKEN_KIND_AND; get_char(scanner); }
			else { token.kind = JADE_TOKEN_KIND_BAND; }
			break;
		case '<': {
			switch (next_char(scanner)) {
				case '=': token.kind = JADE_TOKEN_KIND_LE; get_char(scanner); break;
				case '<': token.kind = JADE_TOKEN_KIND_SHL; get_char(scanner); break;
				default: token.kind = JADE_TOKEN_KIND_LT; break;
			}
			break;
		}
		case '>': {
			switch (next_char(scanner)) {
				case '=': token.kind = JADE_TOKEN_KIND_GE; get_char(scanner); break;
				case '>': token.kind = JADE_TOKEN_KIND_SHR; get_char(scanner); break;
				default: token.kind = JADE_TOKEN_KIND_GT; break;
			}
			break;
		}
		case '!':
			if (next_char(scanner) == '=') { token.kind = JADE_TOKEN_KIND_NEQ; get_char(scanner); }
			else { token.kind = JADE_TOKEN_KIND_NOT; }
			break;
		default:
			if (isalpha(ch) || ch == '_') { token.kind = JADE_TOKEN_KIND_IDENTIFIER; skip_identifier(scanner); }
			else if (isdigit(ch)) { token.kind = JADE_TOKEN_KIND_INTEGER; skip_integer(scanner); }
			else if (isspace(ch)) { skip_whitespace(scanner); return jade_scan(scanner); }
			else { syntax_error(scanner, token.line, token.column); }
			break;
	}

	token.size = scanner->position - token.position;
	return token;
}

void jade_lexeme(jade_scanner* scanner, const jade_token* token, char* buffer) {
	memcpy(buffer, scanner->source + token->position, token->size);
	buffer[token->size] = 0;
}

char peek_char(jade_scanner* scanner) {
	if (scanner->position < scanner->size) {
		return scanner->source[scanner->position];
	} else {
		return EOF;
	}
}

char get_char(jade_scanner* scanner) {
	if (scanner->position < scanner->size) {
		char ch = scanner->source[scanner->position];
		scanner->position += 1;

		if (ch == '\n') {
			scanner->line += 1;
			scanner->column = 1;
		} else {
			scanner->column += 1;
		}

		return ch;
	} else {
		return EOF;
	}
}

char next_char(jade_scanner* scanner) {
	get_char(scanner);
	return peek_char(scanner);
}

void syntax_error(jade_scanner* scanner, size_t line, size_t column) {
	// TODO: diagnostics
	fprintf(stderr, "%s:%ld:%ld: syntax error\n", scanner->path, line, column);
	exit(EXIT_FAILURE);
}

void skip_line(jade_scanner* scanner) {
	while (peek_char(scanner) != EOF && peek_char(scanner) != '\n')
		get_char(scanner);

	get_char(scanner);
}

void skip_whitespace(jade_scanner* scanner) {
	while (isspace(peek_char(scanner)))
		get_char(scanner);
}

void skip_identifier(jade_scanner* scanner) {
	while (isalnum(peek_char(scanner)) || peek_char(scanner) == '_')
		get_char(scanner);
}

void skip_integer(jade_scanner* scanner) {
	while (isdigit(peek_char(scanner)))
		get_char(scanner);
}
