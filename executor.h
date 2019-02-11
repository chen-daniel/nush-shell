#ifndef NUSH_EXECUTOR_H
#define NUSH_EXECUTOR_H

#include "ast.h"

int execute(nush_ast *tokens, int pipe, int *fds, int bg);

#endif