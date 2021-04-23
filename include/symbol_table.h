#pragma once

#include <stddef.h>
#include "ast.h"

typedef struct symbol_table {
	struct symbol* table;
} symbol_table;

typedef struct symbol {
	struct symbol* previous;
	jade_identifier* name;
	ast_node* node;
} symbol;

void symbol_table_init(symbol_table* symbols);
void symbol_table_destroy(symbol_table* symbols);
symbol* symbol_table_bind(symbol_table* symbols, jade_identifier* name, ast_node* node);
void symbol_table_unbind(symbol_table* symbols, jade_identifier* name);
symbol* symbol_table_find(symbol_table* symbols, jade_identifier* name, size_t depth);
