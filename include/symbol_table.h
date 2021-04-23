#pragma once

#include <stddef.h>
#include "ast.h"

#define SYMBOL_TABLE_SIZE 256

typedef struct symbol {
	struct symbol* next;
	struct symbol* parent;
	jade_identifier* identifier;
	ast_node* node;
} symbol;

typedef struct symbol_table {
	struct symbol_table* parent;
	struct symbol table[SYMBOL_TABLE_SIZE];
	size_t size;
} symbol_table;

void symbol_table_init(symbol_table* symbols);
void symbol_table_destroy(symbol_table* symbols);
void symbol_table_open_scope(symbol_table** symbols, symbol_table* scope);
void symbol_table_close_scope(symbol_table** symbols);
symbol* symbol_table_bind(symbol_table* symbols, jade_identifier* identifier, ast_node* node);
void symbol_table_unbind(symbol_table* symbols, jade_identifier* identifier);
symbol* symbol_table_find(symbol_table* symbols, jade_identifier* identifier);
symbol* symbol_table_find_recursive(symbol_table* symbols, jade_identifier* identifier);
