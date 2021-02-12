#include "ast.h"

const char* ast_kind_names[] = {
	"program",
	"node_list",

	"function_definition",
	"variable_definition",

	"identifier",
	"integer",
	"function_call",
	"unary_expression",
	"binary_expression",
	"labeled_expression",
	"jump_expression",
	"variable_assignment"
};

const char* jade_ast_kind_name(jade_ast_kind kind) {
	return ast_kind_names[kind];
}
