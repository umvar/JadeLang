#include <stddef.h>
#include <string.h>
#include "symbol_table.h"

void symbol_table_init(symbol_table* symbols) {
	symbols->parent = NULL;
	symbols->size = 0;

	for (size_t i = 0; i < SYMBOL_TABLE_SIZE; i++) {
		symbol* symbol = &symbols->table[i];
		symbol->next = NULL;
		symbol->parent = NULL;
		symbol->identifier = NULL;
		symbol->node = NULL;
	}
}

void symbol_table_destroy(symbol_table* symbols) {}

void symbol_table_open_scope(symbol_table** symbols, symbol_table* scope) {
	scope->parent = *symbols;
	*symbols = scope;
}

void symbol_table_close_scope(symbol_table** symbols) {
	*symbols = (*symbols)->parent;
}

symbol* symbol_table_bind(symbol_table* symbols, jade_identifier* identifier, ast_node* node) {
	size_t index = identifier->hash % SYMBOL_TABLE_SIZE;

	// precondition
	if (symbols->size >= SYMBOL_TABLE_SIZE)
		return NULL;

	while (symbols->table[index].identifier) {
		symbol* symbol = &symbols->table[index];

		if (symbol->identifier->hash == identifier->hash)
			if (strcmp(symbol->identifier->name, identifier->name) == 0)
				return NULL; // duplicate key

		index = (index + 31) % SYMBOL_TABLE_SIZE;
	}

	symbol* symbol = &symbols->table[index];
	symbol->next = NULL;
	symbol->parent = NULL;
	symbol->identifier = identifier;
	symbol->node = node;
	symbols->size += 1;
	return symbol;
}

void symbol_table_unbind(symbol_table* symbols, jade_identifier* identifier) {
	symbol* symbol = symbol_table_find(symbols, identifier);

	if (symbol) {
		symbol->next = NULL;
		symbol->parent = NULL;
		symbol->identifier = NULL;
		symbol->node = NULL;
		symbols->size -= 1;
	}
}

symbol* symbol_table_find(symbol_table* symbols, jade_identifier* identifier) {
	size_t index = identifier->hash % SYMBOL_TABLE_SIZE;

	while (symbols->table[index].identifier) {
		symbol* symbol = &symbols->table[index];

		if (symbol->identifier->hash == identifier->hash)
			if (strcmp(symbol->identifier->name, identifier->name) == 0)
				return symbol;

		index = (index + 31) % SYMBOL_TABLE_SIZE;
	}

	return NULL;
}

symbol* symbol_table_find_recursive(symbol_table* symbols, jade_identifier* identifier) {
	while (symbols) {
		symbol* symbol = symbol_table_find(symbols, identifier);

		if (symbol)
			return symbol;
		else
			symbols = symbols->parent;
	}

	return NULL;
}
