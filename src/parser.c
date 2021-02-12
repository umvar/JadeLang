#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "factory.h"
#include "parser.h"
#include "scanner.h"
#include "visitors/deleter_visitor.h"

static void next_token(jade_parser* parser);
static void expect(jade_parser* parser, jade_token_kind expected);
static void syntax_error(jade_parser* parser, jade_token_kind expected);

static jade_program* parse_program(jade_parser* parser);
static jade_node_list* parse_node_list(jade_parser* parser, ast_node*(*parse)(jade_parser* parser));
static ast_node* parse_global_definition(jade_parser* parser);
static ast_node* parse_expression(jade_parser* parser);
static ast_node* parse_atom(jade_parser* parser);
static jade_identifier* parse_identifier(jade_parser* parser);
static jade_integer* parse_integer(jade_parser* parser);

void jade_parser_init(jade_parser* parser, jade_scanner* scanner) {
	parser->scanner = scanner;
	parser->ast = NULL;
	next_token(parser);
}

void jade_parser_destroy(jade_parser* parser) {
	accept_deleter_visitor(parser->ast);
}

jade_program* jade_parser_parse(jade_parser* parser) {
	if (parser->ast)
		accept_deleter_visitor(parser->ast);

	return parser->ast = parse_program(parser);
}

jade_program* parse_program(jade_parser* parser) {
	// program = global_definition ',' expression EOF;
	jade_program* program = (jade_program*)jade_create_node(JADE_AST_KIND_PROGRAM);
	program->definition = parse_global_definition(parser);
	program->definition->parent = (ast_node*)program;
	expect(parser, JADE_TOKEN_KIND_DELIMITER);
	program->expression = parse_expression(parser);
	program->expression->parent = (ast_node*)program;
	expect(parser, JADE_TOKEN_KIND_EOF);
	return program;
}

ast_node* parse_global_definition(jade_parser* parser) {
	if (parser->token.kind == JADE_TOKEN_KIND_LPAREN) {
		// global_definition = '(' [global_definition_list] ')';
		next_token(parser);
		jade_node_list* list = parse_node_list(parser, parse_global_definition);
		expect(parser, JADE_TOKEN_KIND_RPAREN);
		return (ast_node*)list;
	} else if (parser->token.kind == JADE_TOKEN_KIND_IDENTIFIER) {
		// global_definition = function_definition | variable_definition;
		jade_identifier* target = parse_identifier(parser);

		if (parser->token.kind == JADE_TOKEN_KIND_LPAREN) {
			// function_definition = identifier '(' [identifier_list] ')' ':=' expression;
			jade_function_definition* definition = (jade_function_definition*)jade_create_node(
				JADE_AST_KIND_FUNCTION_DEFINITION
			);
			definition->target = target;
			definition->target->parent = (ast_node*)definition;
			next_token(parser);
			definition->parameters = parse_node_list(parser, (ast_node*(*)(jade_parser*))parse_identifier);
			definition->parameters->parent = (ast_node*)definition;
			expect(parser, JADE_TOKEN_KIND_RPAREN);
			expect(parser, JADE_TOKEN_KIND_DEFINE);
			definition->expression = parse_expression(parser);
			definition->expression->parent = (ast_node*)definition;
			return (ast_node*)definition;
		} else if (parser->token.kind == JADE_TOKEN_KIND_DEFINE) {
			// variable_definition = identifier ':=' expression;
			jade_variable_definition* definition = (jade_variable_definition*)jade_create_node(
				JADE_AST_KIND_VARIABLE_DEFINITION
			);
			definition->target = target;
			definition->target->parent = (ast_node*)definition;
			next_token(parser);
			definition->expression = parse_expression(parser);
			definition->expression->parent = (ast_node*)definition;
			return (ast_node*)definition;
		} else {
			free(target);
			syntax_error(parser, JADE_TOKEN_KIND_DEFINE);
			next_token(parser);
			return NULL;
		}
	} else {
		syntax_error(parser, JADE_TOKEN_KIND_IDENTIFIER);
		next_token(parser);
		return NULL;
	}
}

jade_node_list* parse_node_list(jade_parser* parser, ast_node*(*parse)(jade_parser* parser)) {
	// node_list = NODE { ',' NODE };
	jade_node_list* list = (jade_node_list*)jade_create_node(JADE_AST_NODE_LIST);
	ast_node* node = parse(parser);

	if (node) {
		node->parent = (ast_node*)list;
		list->first = node;
		list->last = node;
	}

	while (parser->token.kind == JADE_TOKEN_KIND_DELIMITER) {
		next_token(parser);
		node = parse(parser);

		if (node) {
			node->parent = (ast_node*)list;
			list->last->next = node;
			list->last = node;
		}
	}

	return list;
}

ast_node* parse_expression(jade_parser* parser) {
	ast_node* atom = parse_atom(parser);

	if (jade_is_binary_operator(parser->token.kind)) {
		// binary_expression = expression binary_operator expression;
		ast_node* parent = atom;

		while (jade_is_binary_operator(parser->token.kind)) {
			jade_binary_expression* expression = (jade_binary_expression*)jade_create_node(
				JADE_AST_KIND_BINARY_EXPRESSION
			);
			expression->operator = parser->token.kind;
			expression->left_expression = (ast_node*)parent;
			expression->left_expression->parent = (ast_node*)expression;
			next_token(parser);
			expression->right_expression = parse_atom(parser);
			expression->right_expression->parent = (ast_node*)expression;
			parent = (ast_node*)expression;
		}

		return (ast_node*)parent;
	} else if (parser->token.kind == JADE_TOKEN_KIND_DEFINE) {
		// variable_definition = identifier ':=' expression;
		next_token(parser);
		if (atom->kind == JADE_AST_KIND_IDENTIFIER) {
			jade_variable_definition* expression = (jade_variable_definition*)jade_create_node(
				JADE_AST_KIND_VARIABLE_DEFINITION
			);
			expression->target = (jade_identifier*)atom;
			expression->target->parent = (ast_node*)expression;
			expression->expression = parse_expression(parser);
			expression->expression->parent = (ast_node*)expression;
			return (ast_node*)expression;
		} else {
			syntax_error(parser, JADE_TOKEN_KIND_EOF);
			return atom;
		}
	} else if (parser->token.kind == JADE_TOKEN_KIND_ASSIGN) {
		// variable_assignment = identifier '=' expression;
		next_token(parser);
		if (atom->kind == JADE_AST_KIND_IDENTIFIER) {
			jade_variable_assignment* expression = (jade_variable_assignment*)jade_create_node(
				JADE_AST_KIND_VARIABLE_ASSIGNMENT
			);
			expression->target = (jade_identifier*)atom;
			expression->target->parent = (ast_node*)expression;
			expression->expression = parse_expression(parser);
			expression->expression->parent = (ast_node*)expression;
			return (ast_node*)expression;
		} else {
			syntax_error(parser, JADE_TOKEN_KIND_EOF);
			return atom;
		}
	} else {
		return atom;
	}
}

ast_node* parse_atom(jade_parser* parser) {
	if (parser->token.kind == JADE_TOKEN_KIND_LPAREN) {
		next_token(parser);
		jade_node_list* expression = parse_node_list(parser, parse_expression);
		expect(parser, JADE_TOKEN_KIND_RPAREN);
		return (ast_node*)expression;
	} else if (parser->token.kind == JADE_TOKEN_KIND_IDENTIFIER) {
		// identifier, function_call
		jade_identifier* target = parse_identifier(parser);

		if (parser->token.kind == JADE_TOKEN_KIND_LPAREN) {
			// function_call = identifier '(' expression_list ')';
			jade_function_call* expression = (jade_function_call*)jade_create_node(
				JADE_AST_KIND_FUNCTION_CALL
			);
			expression->target = target;
			expression->target->parent = (ast_node*)expression;
			next_token(parser);
			expression->arguments = parse_node_list(parser, parse_expression);
			expression->arguments->parent = (ast_node*)expression;
			expect(parser, JADE_TOKEN_KIND_RPAREN);
			return (ast_node*)expression;
		} else {
			return (ast_node*)target;
		}
	} else if (parser->token.kind == JADE_TOKEN_KIND_INTEGER) {
		return (ast_node*)parse_integer(parser);
	} else if (jade_is_unary_operator(parser->token.kind)) {
		jade_unary_expression* expression = (jade_unary_expression*)jade_create_node(
			JADE_AST_KIND_UNARY_EXPRESSION
		);
		expression->operator = parser->token.kind;
		next_token(parser);
		expression->expression = parse_expression(parser);
		expression->expression->parent = (ast_node*)expression;
		return (ast_node*)expression;
	} else if (parser->token.kind == JADE_TOKEN_KIND_HASH) {
		// jump expression or labeled expression
		next_token(parser);

		if (parser->token.kind == JADE_TOKEN_KIND_LPAREN) {
			// labeled_expression = '#' '(' expression ',' expression ',' expression ')';
			jade_labeled_expression* expression = (jade_labeled_expression*)jade_create_node(
				JADE_AST_KIND_LABELED_EXPRESSION
			);
			next_token(parser);
			expression->expression = parse_expression(parser);
			expression->expression->parent = (ast_node*)expression;
			expect(parser, JADE_TOKEN_KIND_DELIMITER);
			expression->true_expression = parse_expression(parser);
			expression->true_expression->parent = (ast_node*)expression;
			expect(parser, JADE_TOKEN_KIND_DELIMITER);
			expression->false_expression = parse_expression(parser);
			expression->false_expression->parent = (ast_node*)expression;
			expect(parser, JADE_TOKEN_KIND_RPAREN);
			return (ast_node*)expression;
		} else {
			// jump_expression = "#+!?";
			jade_jump_expression* expression = (jade_jump_expression*)jade_create_node(
				JADE_AST_KIND_JUMP_EXPRESSION
			);
			expression->loop_index = 0;
			expression->is_break = 0;

			while (parser->token.kind == JADE_TOKEN_KIND_HASH) {
				expression->loop_index += 1;
				next_token(parser);
			}

			if (parser->token.kind == JADE_TOKEN_KIND_NOT) {
				expression->is_break = 1;
				next_token(parser);
			}

			return (ast_node*)expression;
		}
	} else {
		syntax_error(parser, JADE_TOKEN_KIND_EOF);
		return NULL;
	}
}

jade_identifier* parse_identifier(jade_parser* parser) {
	// identifier = "[a-zA-Z_]([a-zA-Z0-9_]+)?";
	// assert(parser->token.kind == JADE_TOKEN_KIND_IDENTIFIER);
	jade_identifier* identifier = (jade_identifier*)jade_create_node(JADE_AST_KIND_IDENTIFIER);
	char* lexeme = malloc(parser->token.size + 1);
	jade_lexeme(parser->scanner, &parser->token, lexeme);
	identifier->name = lexeme;
	next_token(parser);
	return identifier;
}

jade_integer* parse_integer(jade_parser* parser) {
	// integer = "[0-9]+";
	// assert(parser->token.kind == JADE_TOKEN_KIND_INTEGER);
	jade_integer* integer = (jade_integer*)jade_create_node(JADE_AST_KIND_INTEGER);
	char* lexeme = malloc(parser->token.size + 1);
	jade_lexeme(parser->scanner, &parser->token, lexeme);
	integer->value = strtol(lexeme, NULL, 10);
	free(lexeme);
	next_token(parser);
	return integer;
}

void next_token(jade_parser* parser) {
	parser->token = jade_scan(parser->scanner);
}

void expect(jade_parser* parser, jade_token_kind expected) {
	if (parser->token.kind != expected)
		syntax_error(parser, expected);
	else
		next_token(parser);
}

void syntax_error(jade_parser* parser, jade_token_kind expected) {
	fprintf(
		stderr,
		"%s:%ld:%ld: syntax error: expected '%s', got '%s'.\n",
		parser->scanner->path,
		parser->token.line,
		parser->token.column,
		jade_token_kind_name(expected),
		jade_token_kind_name(parser->token.kind)
	);
}
