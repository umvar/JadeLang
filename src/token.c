#include "token.h"

const char* token_kind_names[] = {
	"EOF",
	"IDENTIFIER",
	"INTEGER",
	"LPAREN", "RPAREN",
	"DELIMITER",
	"ASSIGN", "DEFINE",
	"HASH",
	"PLUS", "MINUS", "TIMES", "DIVIDE", "MODULO",
	"BNOT", "BOR", "BAND", "BXOR", "SHL", "SHR",
	"EQ", "NEQ", "LT", "LE", "GT", "GE",
	"NOT", "OR", "AND"
};

const char* jade_token_kind_name(jade_token_kind kind) {
	return token_kind_names[kind];
}
