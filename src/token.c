#include "token.h"

const char* token_kind_names[] = {
	"eof",
	"identifier", "integer",
	"(", ")",
	",",
	"=", ":=",
	"#",
	"+", "-", "*", "/", "%",
	"~", "|", "&", "^", "<<", ">>",
	"==", "!=", "<", "<=", ">", ">=",
	"!", "||", "&&",
};

const char* jade_token_kind_name(jade_token_kind kind) {
	return token_kind_names[kind];
}

int jade_is_unary_operator(jade_token_kind kind) {
	return (
		kind == JADE_TOKEN_KIND_PLUS ||
		kind == JADE_TOKEN_KIND_MINUS ||
		kind == JADE_TOKEN_KIND_BNOT ||
		kind == JADE_TOKEN_KIND_NOT
	);
}

int jade_is_binary_operator(jade_token_kind kind) {
	return (
		kind == JADE_TOKEN_KIND_PLUS ||
		kind == JADE_TOKEN_KIND_MINUS ||
		kind == JADE_TOKEN_KIND_TIMES ||
		kind == JADE_TOKEN_KIND_DIVIDE ||
		kind == JADE_TOKEN_KIND_MODULO ||
		kind == JADE_TOKEN_KIND_BNOT ||
		kind == JADE_TOKEN_KIND_BOR ||
		kind == JADE_TOKEN_KIND_BAND ||
		kind == JADE_TOKEN_KIND_BXOR ||
		kind == JADE_TOKEN_KIND_SHL ||
		kind == JADE_TOKEN_KIND_SHR ||
		kind == JADE_TOKEN_KIND_EQ ||
		kind == JADE_TOKEN_KIND_NEQ ||
		kind == JADE_TOKEN_KIND_LT ||
		kind == JADE_TOKEN_KIND_LE ||
		kind == JADE_TOKEN_KIND_GT ||
		kind == JADE_TOKEN_KIND_GE ||
		kind == JADE_TOKEN_KIND_NOT ||
		kind == JADE_TOKEN_KIND_OR ||
		kind == JADE_TOKEN_KIND_AND
	);
}
