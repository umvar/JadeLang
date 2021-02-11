#include "token.h"

const char* token_kind_names[] = {
	"EOF",
	"Identifier",
	"Integer",
	"(", ")", ",",
	"=", ":=",
	"#", "+", "-", "*", "/", "%",
	"~", "|", "&", "^", "<<", ">>",
	"==", "!=", "<", "<=", ">", ">=", "!", "||", "&&",
};

const char* jade_token_kind_name(jade_token_kind kind) {
	return token_kind_names[kind];
}
