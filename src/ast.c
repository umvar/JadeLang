#include "ast.h"

const char* ast_kind_names[] = {
	"Program",
	"GlobalDefinitionList",
	"FunctionDefinition",
	"VariableDefinition",
	"ExpressionList",
	"Identifier",
	"Integer",
	"FunctionCall",
	"UnaryExpression",
	"BinaryExpression",
	"LabeledExpression",
	"JumpExpression",
	"VariableAssignment"
};

const char* jade_ast_kind_name(jade_ast_kind kind) {
	return ast_kind_names[kind];
}
