#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "visitors/deleter_visitor.h"

static jade_token next_token(jade_parser* parser);
static void check_token(jade_parser* parser, jade_token_kind expected);
static void expect_token(jade_parser* parser, jade_token_kind expected);
static void syntax_error(jade_parser* parser, jade_token_kind expected, jade_token_kind got);

static jade_program* parse_program(jade_parser* parser, ast_node* parent);
static jade_global_definition_list* parse_global_definition_list(jade_parser* parser, ast_node* parent);
static void parse_global_definition(jade_parser* parser, jade_global_definition_list* global_definition_list, jade_identifier* target);
static jade_identifier* parse_identifier(jade_parser* parser, ast_node* parent);
static jade_function_definition* parse_function_definition(jade_parser* parser, ast_node* parent, jade_identifier* target);
static jade_variable_definition* parse_variable_definition(jade_parser* parser, ast_node* parent, jade_identifier* target);
static jade_expression_list* parse_expression_list(jade_parser* parser, ast_node* parent);
static void parse_expression(jade_parser* parser, jade_expression_list* expression_list);

void jade_parser_init(jade_parser* parser, jade_scanner* scanner) {
	parser->scanner = scanner;
	parser->ast = NULL;
}

void jade_parser_destroy(jade_parser* parser) {
	accept_deleter_visitor(parser->ast);
}

void jade_parser_parse(jade_parser* parser) {
	parser->ast = parse_program(parser, (ast_node*)parser->ast);
	printf("\nPARSED PROGRAM");
}




static jade_program* parse_program(jade_parser* parser, ast_node* parent) {
    jade_program* program = malloc(sizeof(jade_program)); 
    program->kind = JADE_AST_KIND_PROGRAM;
    program->parent = parent; 

    // (
    expect_token(parser, JADE_TOKEN_KIND_LPAREN);                   

    if (next_token(parser).kind != JADE_TOKEN_KIND_RPAREN) {
        program->definitions = parse_global_definition_list(parser, (ast_node*)program);
	}

	// )
	expect_token(parser, JADE_TOKEN_KIND_RPAREN);

    // ,(
    expect_token(parser, JADE_TOKEN_KIND_DELIMITER); 
    expect_token(parser, JADE_TOKEN_KIND_LPAREN); 

    if (next_token(parser).kind != JADE_TOKEN_KIND_RPAREN) {
        //program->expressions;
	}

	// )
	check_token(parser, JADE_TOKEN_KIND_RPAREN);

    // EOF
    expect_token(parser, JADE_TOKEN_KIND_EOF);

    return program;
}

static jade_global_definition_list* parse_global_definition_list(jade_parser* parser, ast_node* parent) {
	jade_global_definition_list* global_definition_list = malloc(sizeof(jade_global_definition_list));
	global_definition_list->kind = JADE_AST_KIND_GLOBAL_DEFINITION_LIST;
	global_definition_list->parent = parent;

	// f
	check_token(parser, JADE_TOKEN_KIND_IDENTIFIER);
	jade_identifier* target = parse_identifier(parser, NULL);
	parse_global_definition(parser, global_definition_list, target);
	
	/*
	while (next_token(parser).kind == JADE_TOKEN_KIND_DELIMITER) {
		// continue parsing global definitions 
	}

	check_token(parser, JADE_TOKEN_KIND_RPAREN); 
	*/

	return global_definition_list;
}

static void parse_global_definition(jade_parser* parser, jade_global_definition_list* global_definition_list, jade_identifier* target) {
	jade_token_kind kind = next_token(parser).kind;
	if (kind == JADE_TOKEN_KIND_LPAREN) {
		global_definition_list->first = (jade_node*)parse_function_definition(parser, (ast_node*)global_definition_list, target);
		global_definition_list->last = global_definition_list->first;
	} else if (kind == JADE_TOKEN_KIND_DEFINE) {
		//global_definition_list->first = (jade_node*)parse_variable_definition(parser, (ast_node*)global_definition_list, target);
	} else {
		syntax_error(parser, JADE_TOKEN_KIND_LPAREN, parser->token.kind);
		syntax_error(parser, JADE_TOKEN_KIND_DEFINE, parser->token.kind);
	}
}

static jade_function_definition* parse_function_definition(jade_parser* parser, ast_node* parent, jade_identifier* target) {
	jade_function_definition* function_definition = malloc(sizeof(jade_function_definition));
	function_definition->kind = JADE_AST_KIND_FUNCTION_DEFINITION;
	function_definition->parent = parent;
	function_definition->target = target;
	target->parent = (ast_node*)function_definition;

	function_definition->parameters = parse_expression_list(parser, (ast_node*)function_definition);

	return function_definition;
}

static jade_expression_list* parse_expression_list(jade_parser* parser, ast_node* parent) {
	jade_expression_list* expression_list = malloc(sizeof(jade_expression_list)); 
	expression_list->kind = JADE_AST_KIND_EXPRESSION_LIST;
	expression_list->parent = parent;

	while (parser->token.kind != JADE_TOKEN_KIND_RPAREN) {
		parse_expression(parser, expression_list);
		while (parser->token.kind == JADE_TOKEN_KIND_DELIMITER) {
			parse_expression(parser, expression_list);
		}
	}


	return expression_list;
}

static void parse_expression(jade_parser* parser, jade_expression_list* expression_list) {
	
	while (parser->token.kind != JADE_TOKEN_KIND_DELIMITER || parser->token.kind != JADE_TOKEN_KIND_RPAREN) {
		next_token(parser);
		if (parser->token.kind == JADE_TOKEN_KIND_IDENTIFIER) {
			
		}
	}
}


static jade_identifier* parse_identifier(jade_parser* parser, ast_node* parent) {
	jade_identifier* identifier = malloc(sizeof(jade_identifier));
	identifier->kind = JADE_AST_KIND_IDENTIFIER;
	identifier->parent = parent;
	identifier->name = malloc(parser->token.size + 1);
	jade_lexeme(parser->scanner, &parser->token, identifier->name);

	return identifier;
}








static jade_token next_token(jade_parser* parser) {
	char lexeme[64];
	jade_lexeme(parser->scanner, &parser->token, lexeme);
	printf("%s", lexeme);

    parser->token = jade_scan(parser->scanner);
    return parser->token; 
}

static void check_token(jade_parser* parser, jade_token_kind expected) {
	if (parser->token.kind != expected)
		syntax_error(parser, expected, parser->token.kind);
}

static void expect_token(jade_parser* parser, jade_token_kind expected) {
	next_token(parser);
	check_token(parser, expected);
}

static void syntax_error(jade_parser* parser, jade_token_kind expected, jade_token_kind got) {
    fprintf(
		stderr, 
		"%s:%ld:%ld: expected: %s got: %s\n", 
		parser->scanner->path, 
		parser->scanner->line, 
		parser->scanner->column,
		jade_token_kind_name(expected),
		jade_token_kind_name(got)
	);
}