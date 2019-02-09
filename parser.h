#ifndef NUSH_PARSER_H
#define NUSH_PARSER_H

#include "ast.h"
#include "svec.h"

nush_ast* parse(svec* tokens);

#endif