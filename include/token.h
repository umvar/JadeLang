#pragma once

typedef enum jade_token_kind {
	JADE_TOKEN_KIND_EOF,
	JADE_TOKEN_KIND_IDENTIFIER,
	JADE_TOKEN_KIND_INTEGER,

	JADE_TOKEN_KIND_LPAREN, // (
	JADE_TOKEN_KIND_RPAREN, // )
	JADE_TOKEN_KIND_DELIMITER, // ,

	JADE_TOKEN_KIND_ASSIGN, // =
	JADE_TOKEN_KIND_DEFINE, // :=

	JADE_TOKEN_KIND_HASH, // #
	JADE_TOKEN_KIND_PLUS, // +
	JADE_TOKEN_KIND_MINUS, // -
	JADE_TOKEN_KIND_TIMES, // *
	JADE_TOKEN_KIND_DIVIDE, // /
	JADE_TOKEN_KIND_MODULO, // %
	JADE_TOKEN_KIND_BNOT, // ~
	JADE_TOKEN_KIND_BOR, // |
	JADE_TOKEN_KIND_BAND, // &
	JADE_TOKEN_KIND_BXOR, // ^
	JADE_TOKEN_KIND_SHL, // <<
	JADE_TOKEN_KIND_SHR, // >>
	JADE_TOKEN_KIND_EQ, // ==
	JADE_TOKEN_KIND_NEQ, // !=
	JADE_TOKEN_KIND_LT, // <
	JADE_TOKEN_KIND_LE, // <=
	JADE_TOKEN_KIND_GT, // >
	JADE_TOKEN_KIND_GE, // >=
	JADE_TOKEN_KIND_NOT, // !
	JADE_TOKEN_KIND_OR, // ||
	JADE_TOKEN_KIND_AND, // &&
} jade_token_kind;

typedef struct jade_token {
	jade_token_kind kind;
	long position;
	long size;
	long line;
	long column;
} jade_token;

const char* jade_token_kind_name(jade_token_kind kind);
