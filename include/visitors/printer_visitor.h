#pragma once

#include <stdio.h>
#include "ast.h"

void accept_printer_visitor(jade_program* node, FILE* file);
