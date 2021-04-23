#pragma once

#include <stddef.h>

static size_t compute_string_hash(const char* lexeme) {
	size_t hash;

	for (hash = 0; *lexeme; lexeme++)
		hash = (hash * 31) + (*lexeme);

	return hash;
}
